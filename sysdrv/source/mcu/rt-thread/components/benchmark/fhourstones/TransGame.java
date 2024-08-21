// This software is copyright (c) 1996-2005 by
//      John Tromp
//      Insulindeweg 908
//      1095 DX Amsterdam
//      Netherlands
// E-mail: tromp@cwi.nl
//
// This notice must not be removed.
// This software must not be sold for profit.
// You may redistribute if your distributees have the
// same rights and restrictions.

import java.text.DecimalFormat;

class TransGame extends Game {
  static final int LOCKSIZE = 26;
  static final int SCORELOCKSIZE = 3+LOCKSIZE;
  static final int TRANSIZE = 8306069;
  // should be a prime no less than about 2^{SIZE1-LOCKSIZE}
  static final int SYMMREC = 10; // symmetry normalize first SYMMREC moves
  static final int UNKNOWN = 0;
  static final int LOSS = 1;
  static final int DRAWLOSS = 2;
  static final int DRAW = 3;
  static final int DRAWWIN = 4;
  static final int WIN = 5;
  static final int LOSSWIN = LOSS+WIN;

  static final int SCORELOCKMASK = (1<<SCORELOCKSIZE)-1;
  static final int LOCKMASK = (1<<LOCKSIZE)-1;
  protected int ht[]; // hash entries
  protected int htindex, lock;
  
  protected long posed; // counts transtore calls
  
  public TransGame()
  {
    super();
    ht = new int[2*TRANSIZE];
  }
  
  void emptyTT()
  {
    int i, h, work;

    for (i=0; i<2*TRANSIZE; i++)
      ht[i] = 0;
    posed = 0L;
  }
  
  void hash()
  {
    long htemp = positioncode();
    if (nplies < SYMMREC) { // try symmetry recognition by reversing columns
      long htemp2 = 0L;
      for (long htmp=htemp; htmp!=0; htmp>>=H1)
        htemp2 = htemp2<<H1 | (htmp & COL1);
      if (htemp2 < htemp)
        htemp = htemp2;
    }
    lock = (int)(htemp >> (SIZE1-26));
    htindex = 2*(int)(htemp % TRANSIZE);
  }
  
  int transpose()
  {
    hash();
    int he0 = ht[htindex];
    int he1 = ht[htindex+1];
    if ((he0 & LOCKMASK) == lock) // biglock 
      return he1 >>> SCORELOCKSIZE; // bigscore
    if ((he1 & LOCKMASK) == lock) // newlock
      return (he1 >> LOCKSIZE) & 7; // newscore
    return UNKNOWN;
  }

  void transtore(int x, int lock, int score, int work)
  {
    posed++;
    int he0 = ht[x];
    int he1 = ht[x+1];
    int biglock = he0 & LOCKMASK;
    if (biglock == lock || work >= (he0 >>> LOCKSIZE)) {
      ht[x] = (work << LOCKSIZE) | lock;
      ht[x+1] = (score << SCORELOCKSIZE) | (he1 & SCORELOCKMASK);
    } else {
      ht[x+1] = ((he1 >>> SCORELOCKSIZE) << 3 | score) << LOCKSIZE | lock;
    }
  }
  
  String htstat()      /* some statistics on hash table performance */
  {
    int total, i;
    StringBuffer buf = new StringBuffer();
    int typecnt[];                // bound type stats
    DecimalFormat df = new DecimalFormat("######.###");

    typecnt = new int[8];
    for (i=0; i<8; i++)
      typecnt[i] = 0;
    for (i=0; i<2*TRANSIZE; i+=2) {
      int he0 = ht[i];
      int he1 = ht[i+1];
      int biglock = he0 & LOCKMASK;
      int bigscore = he1 >>> SCORELOCKSIZE;
      int newlock = he1 & LOCKMASK;
      int newscore = (he1 >> LOCKSIZE) & 7;
      if (biglock != 0)
        typecnt[bigscore]++;
      if (newlock != 0)
        typecnt[newscore]++;
    }
    for (total=0,i=LOSS; i<=WIN; i++)
      total += typecnt[i];
    if (total > 0)
      buf.append("- "+df.format(typecnt[LOSS]/(double)total) +
       " < "+df.format(typecnt[DRAWLOSS]/(double)total) +
       " = "+df.format(typecnt[DRAW]/(double)total) +
       " > " + df.format(typecnt[DRAWWIN]/(double)total) +
       " + " + df.format(typecnt[WIN]/(double)total));
    return buf.toString();
  }
}
