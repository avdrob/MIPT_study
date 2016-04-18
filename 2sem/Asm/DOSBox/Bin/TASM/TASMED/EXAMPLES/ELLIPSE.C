//***********************************
// ELLIPSE.C from TASMED examples.
// Copyright (C) 1997 by CodeMaster.
// Fidonet: 2:5002/39.
// E-mail: cm@liceum.secna.ru.
//***********************************

// To compile with BCC execute "bcc -c -3 ellipse.c"

extern void near hline (int x1, int y, int x2, char color);
extern void near hlpair (int x, int dx, int y, int dy, char color);

void near
fillellipse (int xc, int yc, int a, int b, char color)
{
  int x = a;
  int y = 0;
  long aa = (long) a * a;
  long bb = (long) b * b;
  long d = aa - (bb * (long) a) + (bb >> 2);
  long dx = (bb << 1) * (long) a;
  long dy = 0;

  hline(xc - x, yc, xc + x, color);
  while(dy < dx)
    {
      if(d > 0)
        {
          x--;
          dx -= bb << 1;
          d -= dx;
        }
      dy += aa << 1;
      d  += aa + dy;
      hlpair(xc, x, yc, ++y, color);
    }

  while(x >= 0)
    {
      if(d < 0)
        {
          dy += aa << 1;
          d  += aa + dy;
          hlpair(xc, x, yc, ++y, color);
        }
      x--;
      dx -= bb << 1;
      d += bb - dx;
    }
}
