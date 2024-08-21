// Java Fhourstones 3.1 Transposition table logic
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

#include "Game.c"

#define LOCKSIZE 26
#define TRANSIZE 8306069
// should be a prime no less than about 2^{SIZE1-LOCKSIZE}, e.g.
// 4194301,8306069,8388593,15999961,33554393,67108859,134217689,268435399

#define SYMMREC 10 // symmetry normalize first SYMMREC moves
#define UNKNOWN 0
#define LOSS 1
#define DRAWLOSS 2
#define DRAW 3
#define DRAWWIN 4
#define WIN 5
#define LOSSWIN 6

typedef struct {
#if (LOCKSIZE<=32)
  unsigned biglock:LOCKSIZE;
  unsigned bigwork:6;
  unsigned newlock:LOCKSIZE;
#else
  uint64 biglock:LOCKSIZE;
  unsigned bigwork:6;
  uint64 newlock:LOCKSIZE;
#endif
  unsigned newscore:3;
  unsigned bigscore:3;
} hashentry;

unsigned int htindex, lock;
hashentry *ht;
  
uint64 posed; // counts transtore calls
  
void trans_init()
{
  ht = (hashentry *)calloc(TRANSIZE, sizeof(hashentry));
  if (!ht) {
    printf("Failed to allocate %lu bytes\n", TRANSIZE*sizeof(hashentry));
    exit(0);
  }
}
  
void emptyTT()
{
  int i;

  for (i=0; i<TRANSIZE; i++) {
#if (LOCKSIZE<=32)
    ht[i].biglock = 0;
    ht[i].bigwork = 0;
    ht[i].newlock = 0;
#else
    ht[i].biglock = 0;
    ht[i].bigwork = 0;
    ht[i].newlock = 0;
#endif
    ht[i].newscore = 0;
    ht[i].bigscore = 0;
  }
  posed = 0;
}
  
void hash()
{
  bitboard htmp, htemp = positioncode();
  if (nplies < SYMMREC) { // try symmetry recognition by reversing columns
    bitboard htemp2 = 0;
    for (htmp=htemp; htmp!=0; htmp>>=H1)
      htemp2 = htemp2<<H1 | (htmp & COL1);
    if (htemp2 < htemp)
      htemp = htemp2;
  }
  lock = (unsigned int)(SIZE1>LOCKSIZE ? htemp >> (SIZE1-LOCKSIZE) : htemp);
  htindex = (unsigned int)(htemp % TRANSIZE);
}
  
int transpose()
{
  hashentry he;

  hash();
  he = ht[htindex];
  if (he.biglock == lock)
    return he.bigscore;
  if (he.newlock == lock)
    return he.newscore;
  return UNKNOWN;
}
  
void transtore(int x, unsigned int lock, int score, int work)
{
  hashentry he;

  posed++;
  he = ht[x];
  if (he.biglock == lock || work >= he.bigwork) {
    he.biglock = lock;
    he.bigscore = score;
    he.bigwork = work;
  } else {
    he.newlock = lock;
    he.newscore = score;
  }
  ht[x] = he;
}

void htstat()      /* some statistics on hash table performance */
{
  int total, i;
  int typecnt[8];                /* bound type stats */
  hashentry he;
 
  for (i=0; i<8; i++)
    typecnt[i] = 0;
  for (i=0; i<TRANSIZE; i++) {
    he = ht[i];
    if (he.biglock != 0)
      typecnt[he.bigscore]++;
    if (he.newlock != 0)
      typecnt[he.newscore]++;
  }
  for (total=0,i=LOSS; i<=WIN; i++)
    total += typecnt[i];
  if (total > 0) {
    printf("- %5.3f  < %5.3f  = %5.3f  > %5.3f  + %5.3f\n",
      typecnt[LOSS]/(double)total, typecnt[DRAWLOSS]/(double)total,
      typecnt[DRAW]/(double)total, typecnt[DRAWWIN]/(double)total,
      typecnt[WIN]/(double)total);
  }
}
