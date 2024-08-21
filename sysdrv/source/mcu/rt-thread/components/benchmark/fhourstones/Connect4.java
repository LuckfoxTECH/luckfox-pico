// Fhourstones 3.0 Board Logic
// Copyright 2000-2004 John Tromp

import java.io.*;

class Connect4 {
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
  
  public Connect4()
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
    return 2*color[0] + color[1] + BOTTOM;
// color[0] + color[1] + BOTTOM forms bitmap of heights
// so that positioncode() is a complete board encoding
  }

  public String toString()
  {
    StringBuffer buf = new StringBuffer();

    for (int i=0; i<nplies; i++)
      buf.append(1+moves[i]);
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
    long y = newboard & (newboard>>HEIGHT);
    if ((y & (y >> 2*HEIGHT)) != 0) // check diagonal \
      return true;
    y = newboard & (newboard>>H1);
    if ((y & (y >> 2*H1)) != 0) // check horizontal -
      return true;
    y = newboard & (newboard>>H2); // check diagonal /
    if ((y & (y >> 2*H2)) != 0)
      return true;
    y = newboard & (newboard>>1); // check vertical |
    return (y & (y >> 2)) != 0;
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

  public static void main(String argv[])
  {
    Connect4 c4;
    String line;
    int col=0, i, result;
    long nodes, msecs;

    c4 = new Connect4();
    c4.reset();
    BufferedReader dis = new BufferedReader(new InputStreamReader(System.in));

    for (;;) {
      System.out.println("position " + c4.positioncode() + " after moves " + c4 + "enter move(s):");
      try {
        line = dis.readLine();
      } catch (IOException e) {
        System.out.println(e);
        System.exit(0);
        return;
      }
      if (line == null)
        break;
      for (i=0; i < line.length(); i++) {
        col = line.charAt(i) - '1';
        if (col >= 0 && col < WIDTH && c4.isplayable(col))
          c4.makemove(col);
      }
    }
  }
}
