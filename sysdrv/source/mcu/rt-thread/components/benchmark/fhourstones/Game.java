// Fhourstones 3.1 Board Logic
// (http://www.cwi.nl/~tromp/c4/fhour.html)
//
// implementation of the well-known game
// usually played on a vertical board of 7 columns by 6 rows,
// where 2 players take turns in dropping counters in a column.
// the first player to get four of his counters
// in a horizontal, vertical or diagonal row, wins the game.
// if neither player has won after 42 moves, then the game is drawn.
//
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

class Game {
  static long color[];  // black and white bitboard
  static final int WIDTH = 7;
  static final int HEIGHT = 6;
// bitmask corresponds to board as follows in 7x6 case:
//  .  .  .  .  .  .  .  TOP
//  5 12 19 26 33 40 47
//  4 11 18 25 32 39 46
//  3 10 17 24 31 38 45
//  2  9 16 23 30 37 44
//  1  8 15 22 29 36 43
//  0  7 14 21 28 35 42  BOTTOM
  static final int H1 = HEIGHT+1;
  static final int H2 = HEIGHT+2;
  static final int SIZE = HEIGHT*WIDTH;
  static final int SIZE1 = H1*WIDTH;
  static final long ALL1 = (1L<<SIZE1)-1L; // assumes SIZE1 < 63
  static final int COL1 = (1<<H1)-1;
  static final long BOTTOM = ALL1 / COL1; // has bits i*H1 set
  static final long TOP = BOTTOM << HEIGHT;

  int moves[],nplies;
  byte height[]; // holds bit index of lowest free square
  
  public Game()
  {
    color = new long[2];
    height = new byte[WIDTH];
    moves = new int[SIZE];
    reset();
  }
  
  void reset()
  {
    nplies = 0;
    color[0] = color[1] = 0L;
    for (int i=0; i<WIDTH; i++)
      height[i] = (byte)(H1*i);
  }

  public long positioncode()
  {
    return color[nplies&1] + color[0] + color[1] + BOTTOM;
// color[0] + color[1] + BOTTOM forms bitmap of heights
// so that positioncode() is a complete board encoding
  }

  public String toString()
  {
    StringBuffer buf = new StringBuffer();

    for (int i=0; i<nplies; i++)
      buf.append(1+moves[i]);
    if (true) return buf.toString(); // remove to get board + info printed
    buf.append("\n");
    for (int w=0; w<WIDTH; w++)
      buf.append(" "+(w+1));
    buf.append("\n");
    for (int h=HEIGHT-1; h>=0; h--) {
      for (int w=h; w<SIZE1; w+=H1) {
        long mask = 1L<<w;
        buf.append((color[0]&mask)!= 0 ? " @" :
                   (color[1]&mask)!= 0 ? " 0" : " .");
      }
      buf.append("\n");
    }
    if (haswon(color[0]))
      buf.append("@ won\n");
    if (haswon(color[1]))
      buf.append("O won\n");
    return buf.toString();
  }

  // return whether columns col has room
  final boolean isplayable(int col)
  {
    return islegal(color[nplies&1] | (1L << height[col]));
  }

  // return whether newboard lacks overflowing column
  final boolean islegal(long newboard)
  {
    return (newboard & TOP) == 0;
  }

  // return whether newboard is legal and includes a win
  final boolean islegalhaswon(long newboard)
  {
    return islegal(newboard) && haswon(newboard);
  }

  // return whether newboard includes a win
  final boolean haswon(long newboard)
  {
    long diag1 = newboard & (newboard>>HEIGHT); // check diagonal \
    long hori = newboard & (newboard>>H1); // check horizontal -
    long diag2 = newboard & (newboard>>H2); // check diagonal /
    long vert = newboard & (newboard>>1); // check vertical |
    return ((diag1 & (diag1 >> 2*HEIGHT)) |
            (hori & (hori >> 2*H1)) |
            (diag2 & (diag2 >> 2*H2)) |
            (vert & (vert >> 2))) != 0;
  }

  void backmove()
  {
    int n;

    n = moves[--nplies];
    color[nplies&1] ^= 1L<<--height[n];
  }

  void makemove(int n) 
  {
    color[nplies&1] ^= 1L<<height[n]++;
    moves[nplies++] = n;
  }
}
