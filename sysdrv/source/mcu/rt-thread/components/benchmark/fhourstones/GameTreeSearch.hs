module GameTreeSearch(updateTT,lookupTT,Hash(Hash),GameTree(GTLoss,GTDraw,GTBranch),alphabeta,loss,lossdraw,draw,losswin,drawwin,win,newTT,TTable,statsTT,intlog,getPosed,ratio,locksize) where

import Data.Word
import Data.Int
import Data.Bits
import Data.List
import Data.Maybe
import Data.Array.IO
import Data.Array.MArray
import Data.Array.Unboxed
import Control.Monad
import Data.IORef
import Numeric
import CPUTime

data Hash = Hash !Int !Int deriving Show

type Hist = IOUArray Int Int
data GameTree l m = GTDraw | GTLoss | GTBranch Hash Hist [(m, GameTree l m)]

data TTable = TT {
  size :: Int,
  entries :: IOUArray Int Word64,
  posed :: IORef Word64
}

locksize = 26

newsize = 3+locksize
lockmask = (bit locksize) - 1 :: Int
newmask = (bit newsize) - 1 :: Int
newmask64 = fromIntegral newmask :: Word64
bigmask = complement newmask64

bigwork :: Word64 -> Int
bigwork x = fromIntegral (x `shiftR` (2*newsize))

biglock :: Word64 -> Int
biglock x = fromIntegral (x `shiftR` (3+newsize)) .&. lockmask

bigscore :: Word64 -> Int
bigscore x = (fromIntegral x `shiftR` newsize) .&. 7 -- .&. shld be redundant

newlock :: Word64 -> Int
newlock x = fromIntegral x .&. lockmask

newscore :: Word64 -> Int
newscore x = (fromIntegral x `shiftR` locksize) .&. 7

newTT :: Int -> IO TTable
newTT size = do
  entries <- newArray (0,size-1) 0 :: IO (IOUArray Int Word64)
  posed <- newIORef 0
  return $ TT size entries posed

statsTT (TT size entries posed) = do
  cnts <- newArray (loss,win) 0 :: IO (IOUArray Int Int)
  mapM_ (\i -> do
    he <- liftM fromIntegral $ readArray entries i
    when (biglock he /= 0) $ addArray 1 cnts (bigscore he)
    when (newlock he /= 0) $ addArray 1 cnts (newscore he)
   ) [0..size-1]
  scnts <- freeze cnts :: IO (Array Int Int)
  let total = sum $ elems scnts
  let myshows score = showFFloat (Just 3) $ ratio (scnts!score) total
  return $ (("- " ++) .myshows loss .
            (" < " ++) . myshows lossdraw . (" = " ++) . myshows draw .
            (" > " ++) . myshows drawwin . (" + " ++) . myshows win) ""

lookupTT :: TTable -> Hash -> IO (Maybe Int)
lookupTT (TT {entries=entries}) (Hash lock ttix) = do
  he <- readArray entries ttix
  return $ if lock == biglock he then Just (bigscore he) else
            if lock == newlock he then Just (newscore he) else Nothing

updateTT :: TTable -> Hash -> Int -> Int -> IO ()
updateTT tt@(TT size entries posed) hash@(Hash lock ttix) score work = do
  incIORef posed
  he <- readArray entries ttix
  writeArray entries ttix $! if lock == biglock he || work >= bigwork he then
    (((applock work lock `shiftL` 3) .|. fromIntegral score)
     `shiftL` newsize) .|. (he .&. newmask64)
   else (he .&. bigmask) .|. applock score lock where
  applock x l = fromIntegral ((x `shiftL` locksize) .|. l)


getPosed :: TTable -> IO Word64
getPosed (TT {posed = p}) = readIORef p

loss = 1 :: Int
lossdraw = 2 :: Int
draw = 3 :: Int
losswin = 3 :: Int
drawwin = 4 :: Int
win = 5 :: Int
exact = odd
nega = (6-)

alphabeta nodes tt = negamax where
 negamax window tree = do
  incIORef nodes
  case tree of
   GTLoss -> return loss
   GTDraw -> return draw
   GTBranch _ _ [(_,gt)] -> liftM nega $ negamax (nega window) gt
   GTBranch hash hist movelist -> do
    ttval <- lookupTT tt hash
    let (prune, window') = case ttval of
         Nothing -> (False, window)
         Just score -> (exact score || window == nega score, score)
    if prune then return window' else do
    let maxToFront t@(a@(h,_):t') = do
         hv <- readArray hist h
         if null t' then return (hv,t) else do
         (max, b:t'') <- maxToFront t'
         return $ if (hv >= max) then (hv,t) else (max,b:a:t'')
    let negaloop window s besth done todo =
         if null todo then return s else do
         (_, (hm,gtm):todo') <- maxToFront todo
         v <- liftM nega $ negamax (nega window) gtm
         if v <= window then (
           if v <= s then negaloop  window s besth
                     else negaloop drawwin v    hm) (hm:done) todo'
          else do
           let nworse = length done
           when (nworse > 0) (do
             mapM_ (addArray (-1) hist) done
             addArray nworse hist hm)
           return $ if v==draw && not (null todo') then drawwin else v
    poscnt <- getPosed tt
    s <- negaloop window' loss 0 [] movelist
    poscnt' <- getPosed tt
    let score = case ttval of
         Just olds | s == (nega olds) -> draw
         _ -> s
    updateTT tt hash score $ intlog $ poscnt' - poscnt
    return score

ratio x y = (fromIntegral x) / (fromIntegral y)
addArray d a i = do v <- readArray a i; writeArray a i $! v+d
intlog n = if n <= 1 then 0 else 1 + (intlog $ shiftR n 1)
incIORef r = do v <- readIORef r; writeIORef r $! (v+1)
