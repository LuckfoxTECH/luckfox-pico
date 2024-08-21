module Connect4 where

import Data.Word
import Data.Int
import Data.Bits
import Data.List
import Data.Array.Unboxed

(width,height) = (7,6)
height' = height + 1
height'' = height + 2
size = width * height
bsize = width * height'

type BitBoard = Word64 -- bitmask corresponds to board as follows in 7x6 case:
--  .  .  .  .  .  .  .  TOP
--  5 12 19 26 33 40 47
--  4 11 18 25 32 39 46
--  3 10 17 24 31 38 45
--  2  9 16 23 30 37 44
--  1  8 15 22 29 36 43
--  0  7 14 21 28 35 42  BOTTOM

type Heights = UArray Int Int -- index of lowest free square in each column

data Game = Game !Int !BitBoard !BitBoard Heights

bottom = (bit bsize - 1) `div` (bit height' - 1) -- mask for bottom squares
-- equals 2^0 + 2^{height'} + 2^{2*height'} + .. + 2^{(width-1)*height'}
-- which is a geometric series evaluating to the above fraction
top = bottom `shiftL` height

showbb white black =
  header++'\n':unlines (map showrow [height-1,height-2..0])++wininfo where
  header    = ' ':unwords (map show [1..width])
  showrow h = ' ':intersperse ' ' (map cell [h,h+height'..bsize-1])
  cell i | testBit black i = '@'
         | testBit white i = 'O'
         | otherwise = '.'
  wininfo | isWon black = "@ won\n"
          | isWon white = "O won\n"
          | otherwise = ""

instance Show Game where
  show (Game n tm ntm _) = if even n then showbb tm ntm else showbb ntm tm

allmoves = [0..width-1]

isLegal bb = (bb .&. top) == 0

isLegalGame (Game _ _ ntm _) = isLegal ntm

legalmoves g = filter (isLegalGame . snd . move g) allmoves

isWon bb = any ((/= 0) . dir) [1,height,height',height''] where
  dir d = let t = bb .&. (bb `shiftR` d) in t .&. (t `shiftR` (2*d))

isWonGame (Game _ _ ntm _) = isWon ntm

isLegalWon bb = isLegal bb && isWon bb
isLegalWonGame (Game _ _ ntm _) = isLegalWon ntm

move (Game n tm ntm hs) i = (h,g) where
  h = hs!i
  g = Game (n+1) ntm (setBit tm h) (hs//[(i,h+1)])

encode :: Game -> Word64
encode (Game n tm ntm _) =
  if n >= symmrec then key else min key (revcols 0 key) where
    symmrec = 10 -- symmetry normalize first SYMMREC moves
    key = 2*tm + ntm + bottom
    revcols 0 a = a
    revcols k a = revcols (shiftR k height') (shiftL a height' .|. k.&.col1)
    col1 = bit height' - 1
    
isWinnable (Game _ tm _ hs) = any (isLegalWon . setBit  tm) (elems hs)

goodMoves g@(Game _ _ ntm hs) =
  let noThreatAbove h = not $ isLegalWon $ setBit ntm $ h+1 in
  case filter (isLegalWon . setBit ntm) (elems hs) of -- forced moves
    [] -> [hig | i <- allmoves, let hig@(hi,(Game _ _ ntmi _)) = move g i,
                                isLegal ntmi && noThreatAbove hi ]
    [hf] | noThreatAbove hf -> [move g (hf `div` height')]
    _ -> [] -- no good moves if multiple forced

newGame = Game 0 0 0 $ listArray (0,width-1) [0,height'..]

listGame = foldl (\i->snd.(move i)) newGame

quads = [quad | i <- [0..bsize-1], dir <- [1,height,height',height''],
                let max = i + 3*dir, let quad = [i,i+dir..max],
                max < bsize && all (not . testBit top) quad]

cntElems b a = accumArray (+) 0 b $ a `zip` (repeat 1) :: Array Int Int

moveEval = cntElems (0,bsize-1) $ concat quads
