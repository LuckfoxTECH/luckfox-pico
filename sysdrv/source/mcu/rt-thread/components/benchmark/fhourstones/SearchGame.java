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

import java.io.*;
import java.text.DecimalFormat;

public class SearchGame extends TransGame {
  static final int BOOKPLY = 0;  // full-width search up to this depth
  static final int REPORTPLY = 2;
  static int reportply = 2;
  int history[][] = new int[2][SIZE1];
  long nodes, msecs;
  
  void inithistory()
  {
    for (int side=0; side<2; side++)
      for (int i=0; i<(WIDTH+1)/2; i++)
        for (int h=0; h<H1/2; h++)
          history[side][H1*i+h] = history[side][H1*(WIDTH-1-i)+HEIGHT-1-h] =
          history[side][H1*i+HEIGHT-1-h] = history[side][H1*(WIDTH-1-i)+h] =
            4+Math.min(3,i) + Math.max(-1,Math.min(3,h)-Math.max(3-i,0))
            + Math.min(3,Math.min(i,h)) + Math.min(3,h);
  }

  int ab(int alpha, int beta)
  {
    nodes++;
    if (nplies == SIZE-1) // one move left
      return DRAW; // by assumption, player to move can't win
    int side, otherside;
    otherside = (side = nplies & 1) ^ 1;
    long other = color[otherside];
    int i,nav,av[] = new int[WIDTH];
    long newbrd;
    boolean winontop;
    for (i = nav = 0; i < WIDTH; i++) {
      newbrd = other | (1L << height[i]); // check opponent move
      if (!islegal(newbrd)) 
        continue;
      winontop = islegalhaswon(other | (2L << height[i]));
      if (haswon(newbrd)) { // immediate threat
        if (winontop) // can't stop double threat
          return LOSS;
        nav = 0; // forced move
        av[nav++] = i;
        while (++i < WIDTH)
          if (islegalhaswon(other | (1L << height[i])))
            return LOSS;
        break;
      }
      if (!winontop)
        av[nav++] = i;
    }
    if (nav == 0)
      return LOSS;
    if (nplies == SIZE-2) // two moves left
      return DRAW; // opponent has no win either
    int score;
    if (nav == 1) {
      makemove(av[0]);
      score = LOSSWIN-ab(LOSSWIN-beta,LOSSWIN-alpha);
      backmove();
      return score;
    }
    int ttscore = transpose();
    if (ttscore != UNKNOWN) {
      if (ttscore == DRAWLOSS) {
        if ((beta = DRAW) <= alpha)
          return ttscore;
      } else if (ttscore == DRAWWIN) {
        if ((alpha = DRAW) >= beta)
          return ttscore;
      } else return ttscore; // exact score
    }
    int hashindx = htindex;
    int hashlock = lock;
    long poscnt = posed;
    int besti=0,j,l,sc;
    int v,val;
    score = LOSS;
    for (i = 0; i < nav; i++) {
      val = history[side][height[av[l = i]]];
      for (j = i+1; j < nav; j++) {
        v = history[side][height[av[j]]];
        if (v > val) {
          val = v; l = j;
        }
      }
      for (j = av[l]; l>i; l--)
        av[l] = av[l-1];
      makemove(av[i] = j);
      sc = LOSSWIN-ab(LOSSWIN-beta,LOSSWIN-alpha);
      backmove();
      if (sc > score) {
        besti = i;
        if ((score=sc) > alpha && nplies >= BOOKPLY && (alpha=sc) >= beta) {
          if (score == DRAW && i < nav-1)
            score = DRAWWIN;
          if (besti > 0) {
            for (i = 0; i < besti; i++)
              history[side][height[av[i]]]--; // punish worse
            history[side][height[av[besti]]] += besti;
          }
          break;
        }
      }
    }
    if (score == LOSSWIN-ttscore) // combine < and >
      score = DRAW;
    poscnt = posed - poscnt;
    int work;
    for (work=0; (poscnt>>=1) != 0; work++) ; // work=log #positions stored
    transtore(hashindx, hashlock, score, work);
    if (nplies <= reportply) {
      System.out.println(toString() + "#-<=>+".charAt(score) + work);
    }
    return score;
  }

  int solve()
  {
    int i, side = nplies & 1, otherside = side ^ 1;
    reportply = nplies + REPORTPLY;
    nodes = 0L;
    msecs = 1L;
    if (haswon(color[otherside]))
      return LOSS;
    for (i = 0; i < WIDTH; i++)
      if (islegalhaswon(color[side] | (1L << height[i])))
        return WIN;
    inithistory();
    msecs = System.currentTimeMillis();
    int score = ab(LOSS, WIN);
    msecs = System.currentTimeMillis() + 1 - msecs; // prevent division by 0
    return score;
  }

  public static void main(String argv[])
  {
    System.out.println("Fhourstones 3.1 (Java)");
    System.out.println("Boardsize = " + WIDTH + "x" + HEIGHT);
    System.out.println("Using " + TRANSIZE + " transposition table entries.");
    SearchGame c4 = new SearchGame();
    BufferedReader dis = new BufferedReader(new InputStreamReader(System.in));
    DecimalFormat df = new DecimalFormat("######.###");
 
    for (;;) {
      String line=null;
      try {
        line = dis.readLine();
      } catch (IOException e) {
        System.out.println(e);
        System.exit(0);
      }
      if (line == null)
        break;
      c4.reset();
      for (int i=0; i < line.length(); i++)
        c4.makemove(line.charAt(i) - '1');
      System.out.println("\nSolving " + c4.nplies + "-ply position after "
        + c4.toString() + " . . .");

      c4.emptyTT();
      int result = c4.solve();
      long poscnt = c4.posed;
      int work;
      for (work=0; (poscnt>>=1) != 0; work++) ; //work = log #transpositions
      System.out.println("score = " + result + " (" +
         "#-<=>+".charAt(result) + ")  work = " + work);
      System.out.println("" + c4.nodes + " pos / " + c4.msecs +
        " msec = " + df.format((double)c4.nodes/c4.msecs) + " Kpos/sec");
      System.out.println(c4.htstat());
    }
  }
}
