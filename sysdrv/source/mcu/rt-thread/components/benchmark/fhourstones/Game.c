// Fhourstones 3.0 Board Logic
// (http://www.cwi.nl/~tromp/c4/fhour.html)
//
// implementation of the well-known game
// usually played on a vertical board of 7 columns by 6 rows,
// where 2 players take turns in dropping counters in a column.
// the first player to get four of his counters
// in a horizontal, vertical or diagonal row, wins the game.
// if neither player has won after 42 moves, then the game is drawn.
//
// This software is copyright (c) 1996-2008 by
//      John Tromp
//      600 Route 25A
//      East Setauket
//      NY 11733
// E-mail: john.tromp@gmail.com
//
// This notice must not be removed.
// This software must not be sold for profit.
// You may redistribute if your distributees have the
// same rights and restrictions.

#ifdef _WIN32
	#include <windows.h>
	typedef INT64 int64;
	typedef UINT64 uint64;
#else
	#include <stdint.h>
	typedef uint64_t uint64;
	typedef int64_t int64;
#endif

#include <stdio.h>
#include <stdlib.h>

#define WIDTH 7
#define HEIGHT 6
// bitmask corresponds to board as follows in 7x6 case:
//  .  .  .  .  .  .  .  TOP
//  5 12 19 26 33 40 47
//  4 11 18 25 32 39 46
//  3 10 17 24 31 38 45
//  2  9 16 23 30 37 44
//  1  8 15 22 29 36 43
//  0  7 14 21 28 35 42  BOTTOM
#define H1 (HEIGHT+1)
#define H2 (HEIGHT+2)
#define SIZE (HEIGHT*WIDTH)
#define SIZE1 (H1*WIDTH)

#if (SIZE1<=64)
  typedef uint64 bitboard;
#else
  typedef __int128_t bitboard;
#endif

#define COL1 (((bitboard)1<<H1)-(bitboard)1)
#if (SIZE1 != 64)
#define ALL1 (((bitboard)1<<SIZE1)-(bitboard)1)
#define BOTTOM (ALL1 / COL1) // has bits i*H1 set
#elif (WIDTH==8 && HEIGHT==7) // one of exceptional cases
#define BOTTOM 0x0101010101010101LL
#else
# add definition for missing (weird) exceptional cases
#endif
#define TOP (BOTTOM << HEIGHT)

bitboard color[2];  // black and white bitboard
int moves[SIZE],nplies;
char height[WIDTH]; // holds bit index of lowest free square
  
void reset()
{
  int i;
  nplies = 0;
  color[0] = color[1] = 0;
  for (i=0; i<WIDTH; i++)
    height[i] = (char)(H1*i);
}

bitboard positioncode()
{
  return color[nplies&1] + color[0] + color[1] + BOTTOM;
// color[0] + color[1] + BOTTOM forms bitmap of heights
// so that positioncode() is a complete board encoding
}

void printMoves()
{
  int i;
 
  for (i=0; i<nplies; i++)
    printf("%d", 1+moves[i]);
}

// return whether newboard lacks overflowing column
int islegal(bitboard newboard)
{
  return (newboard & TOP) == 0;
}

// return whether columns col has room
int isplayable(int col)
{
  return islegal(color[nplies&1] | ((bitboard)1 << height[col]));
}

// return non-zero iff newboard includes a win
bitboard haswon(bitboard newboard)
{
  bitboard diag1 = newboard & (newboard>>HEIGHT);
  bitboard hori = newboard & (newboard>>H1);
  bitboard diag2 = newboard & (newboard>>H2);
  bitboard vert = newboard & (newboard>>1);
  return ((diag1 & (diag1 >> 2*HEIGHT)) |
          (hori & (hori >> 2*H1)) |
          (diag2 & (diag2 >> 2*H2)) |
          (vert & (vert >> 2)));
}

// return whether newboard is legal and includes a win
int islegalhaswon(bitboard newboard)
{
  return islegal(newboard) && haswon(newboard);
}

void backmove()
{
  int n;

  n = moves[--nplies];
  color[nplies&1] ^= (bitboard)1<<--height[n];
}

void makemove(int n) 
{
  color[nplies&1] ^= (bitboard)1<<height[n]++;
  moves[nplies++] = n;
}
