/*
 * EmbedCV - an embeddable computer vision library
 *
 * Copyright (C) 2006  Chris Jang
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 *
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301  USA
 *
 * Email the author: cjang@ix.netcom.com
 *
 */


#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#include "embedcv.h"


/*
 * Square root with Newton's method and lookup table
 *
 */
size_t UintSqrt(size_t value)
{
  /* square root lookup table for 0 to 1023 inclusive */
  static const uint8_t lut[] = {
0, 
1, 1, 1, 
2, 2, 2, 2, 2, 
3, 3, 3, 3, 3, 3, 3, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 
5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 
7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 
10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 
10, 
11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 
11, 11, 11, 
12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 
12, 12, 12, 12, 12, 
13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 
13, 13, 13, 13, 13, 13, 13, 
14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 
14, 14, 14, 14, 14, 14, 14, 14, 14, 
15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 
15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 
16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 
16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 
17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 
17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 
18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 
18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 
19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 
19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 
20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 
20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 
20, 
21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 
21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 
21, 21, 21, 
22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 
22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 
22, 22, 22, 22, 22, 
23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 
23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 
23, 23, 23, 23, 23, 23, 23, 
24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 
24, 24, 24, 24, 24, 24, 24, 24, 24, 
25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 
25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 
25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 
26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 
26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 
26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 
27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 
27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 
27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 
28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 
28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 
28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 
29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 
29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 
29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 
30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 
30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 
30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 
30, 
31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 
31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 
31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 
31, 31, 31
    };

  if (value < 1024)
  {
    return lut[value];
  }
  else
  {
    size_t last, current;

    /* initial guess is half of the squared value */
    current = value >> 1;

    do
    {
      last = current;

      /* next guess */
      current = ( last + value / last ) >> 1;
    }
    while ( last != current && UINTDIFF( last, current ) != 1 );

    return current;
  }
}


/*
 * Returns orientation of vector as array index from 0 to 127
 *
 * The orientation typically comes from the normal vector computed in the Sobel
 * edge images. A normal pointing to the right corresponds to index 0.
 *
 * Warning - The image coordinate system is left-handed! The abscissa runs from
 * left to right across the image width. The ordinate runs from top to bottom
 * over the image height. This is consistent with conventional pixel layout
 * in memory.
 *
 * This function is really computing a rough approximation to atan2(). Over
 * 0 to 45 degrees, a linear approximation is used.
 *
 */
size_t ApproxAtan2(int16_t dy, int16_t dx)
{
  if (dx > 0)
  {
    if (dy > 0)
    {
      /* first quadrant */

      if (dy <= dx)
      {
        /* first octant, 0 to 45 degrees is index 0 to 16 */
        return (dy << 4) / dx;
      }
      else
      {
        /* second octant, 45 to 90 degrees is index 16 to 32 */
        return 32 - (dx << 4) / dy;
      }
    }
    else if (dy < 0)
    {
      /* fourth quadrant */
      dy = -dy;

      if (dy <= dx)
      {
        /* eighth octant, 315 to 360 degrees is index 112 to 0 (128) */
        return (128 - (dy << 4) / dx) % 128;
      }
      else
      {
        /* seventh octant, 270 to 315 degrees is index 96 to 112 */
        return 96 + (dx << 4) / dy;
      }
    }
    else
    {
      /* horizontal */
      return 0;
    }
  }
  else if (dx < 0)
  {
    dx = -dx;

    if (dy > 0)
    {
      /* second quadrant */
      if (dy <= dx)
      {
        /* fourth octant */
        return 64 - (dy << 4) / dx;
      }
      else
      {
        /* third octant */
        return 32 + (dx << 4) / dy;
      }
    }
    else if (dy < 0)
    {
      /* third quadrant */
      dy = -dy;

      if (dy <= dx)
      {
        /* fifth octant */
        return 64 + (dy << 4) / dx;
      }
      else
      {
        /* sixth octant */
        return 96 - (dx << 4) / dy;
      }
    }
    else
    {
      /* horizontal */
      return 64;
    }
  }
  else
  {
    /* vertical */
    if (dy > 0)
    {
      return 32;
    }
    else if (dy < 0)
    {
      return 96;
    }
    else
    {
      return 0;
    }
  }
}


/*
 * Returns 65536*sin(theta) for angle from 0 to 360 (orientation from 0 to 127)
 *
 */
int32_t UintSin(size_t theta)
{
  static const int32_t lut[] = {
    0,  /* index 0 is angle 0 */
    3215,  /* index 1 is angle 2.8125 */
    6423,  /* index 2 is angle 5.625 */
    9616,  /* index 3 is angle 8.4375 */
    12785,  /* index 4 is angle 11.25 */
    15923,  /* index 5 is angle 14.0625 */
    19024,  /* index 6 is angle 16.875 */
    22078,  /* index 7 is angle 19.6875 */
    25079,  /* index 8 is angle 22.5 */
    28020,  /* index 9 is angle 25.3125 */
    30893,  /* index 10 is angle 28.125 */
    33692,  /* index 11 is angle 30.9375 */
    36409,  /* index 12 is angle 33.75 */
    39039,  /* index 13 is angle 36.5625 */
    41575,  /* index 14 is angle 39.375 */
    44011,  /* index 15 is angle 42.1875 */
    46340,  /* index 16 is angle 45 */
    48558,  /* index 17 is angle 47.8125 */
    50660,  /* index 18 is angle 50.625 */
    52639,  /* index 19 is angle 53.4375 */
    54491,  /* index 20 is angle 56.25 */
    56212,  /* index 21 is angle 59.0625 */
    57797,  /* index 22 is angle 61.875 */
    59243,  /* index 23 is angle 64.6875 */
    60547,  /* index 24 is angle 67.5 */
    61705,  /* index 25 is angle 70.3125 */
    62714,  /* index 26 is angle 73.125 */
    63571,  /* index 27 is angle 75.9375 */
    64276,  /* index 28 is angle 78.75 */
    64826,  /* index 29 is angle 81.5625 */
    65220,  /* index 30 is angle 84.375 */
    65457,  /* index 31 is angle 87.1875 */
    65536,  /* index 32 is angle 90 */
    65457,  /* index 33 is angle 92.8125 */
    65220,  /* index 34 is angle 95.625 */
    64826,  /* index 35 is angle 98.4375 */
    64276,  /* index 36 is angle 101.25 */
    63571,  /* index 37 is angle 104.062 */
    62714,  /* index 38 is angle 106.875 */
    61705,  /* index 39 is angle 109.688 */
    60547,  /* index 40 is angle 112.5 */
    59243,  /* index 41 is angle 115.312 */
    57797,  /* index 42 is angle 118.125 */
    56212,  /* index 43 is angle 120.938 */
    54491,  /* index 44 is angle 123.75 */
    52639,  /* index 45 is angle 126.562 */
    50660,  /* index 46 is angle 129.375 */
    48558,  /* index 47 is angle 132.188 */
    46340,  /* index 48 is angle 135 */
    44011,  /* index 49 is angle 137.812 */
    41575,  /* index 50 is angle 140.625 */
    39039,  /* index 51 is angle 143.438 */
    36409,  /* index 52 is angle 146.25 */
    33692,  /* index 53 is angle 149.062 */
    30893,  /* index 54 is angle 151.875 */
    28020,  /* index 55 is angle 154.688 */
    25079,  /* index 56 is angle 157.5 */
    22078,  /* index 57 is angle 160.312 */
    19024,  /* index 58 is angle 163.125 */
    15923,  /* index 59 is angle 165.938 */
    12785,  /* index 60 is angle 168.75 */
    9616,  /* index 61 is angle 171.562 */
    6423,  /* index 62 is angle 174.375 */
    3215,  /* index 63 is angle 177.188 */
    0,  /* index 64 is angle 180 */
    -3215,  /* index 65 is angle 182.812 */
    -6423,  /* index 66 is angle 185.625 */
    -9616,  /* index 67 is angle 188.438 */
    -12785,  /* index 68 is angle 191.25 */
    -15923,  /* index 69 is angle 194.062 */
    -19024,  /* index 70 is angle 196.875 */
    -22078,  /* index 71 is angle 199.688 */
    -25079,  /* index 72 is angle 202.5 */
    -28020,  /* index 73 is angle 205.312 */
    -30893,  /* index 74 is angle 208.125 */
    -33692,  /* index 75 is angle 210.938 */
    -36409,  /* index 76 is angle 213.75 */
    -39039,  /* index 77 is angle 216.562 */
    -41575,  /* index 78 is angle 219.375 */
    -44011,  /* index 79 is angle 222.188 */
    -46340,  /* index 80 is angle 225 */
    -48558,  /* index 81 is angle 227.812 */
    -50660,  /* index 82 is angle 230.625 */
    -52639,  /* index 83 is angle 233.438 */
    -54491,  /* index 84 is angle 236.25 */
    -56212,  /* index 85 is angle 239.062 */
    -57797,  /* index 86 is angle 241.875 */
    -59243,  /* index 87 is angle 244.687 */
    -60547,  /* index 88 is angle 247.5 */
    -61705,  /* index 89 is angle 250.313 */
    -62714,  /* index 90 is angle 253.125 */
    -63571,  /* index 91 is angle 255.938 */
    -64276,  /* index 92 is angle 258.75 */
    -64826,  /* index 93 is angle 261.562 */
    -65220,  /* index 94 is angle 264.375 */
    -65457,  /* index 95 is angle 267.188 */
    -65536,  /* index 96 is angle 270 */
    -65457,  /* index 97 is angle 272.812 */
    -65220,  /* index 98 is angle 275.625 */
    -64826,  /* index 99 is angle 278.438 */
    -64276,  /* index 100 is angle 281.25 */
    -63571,  /* index 101 is angle 284.062 */
    -62714,  /* index 102 is angle 286.875 */
    -61705,  /* index 103 is angle 289.688 */
    -60547,  /* index 104 is angle 292.5 */
    -59243,  /* index 105 is angle 295.312 */
    -57797,  /* index 106 is angle 298.125 */
    -56212,  /* index 107 is angle 300.938 */
    -54491,  /* index 108 is angle 303.75 */
    -52639,  /* index 109 is angle 306.562 */
    -50660,  /* index 110 is angle 309.375 */
    -48558,  /* index 111 is angle 312.188 */
    -46340,  /* index 112 is angle 315 */
    -44011,  /* index 113 is angle 317.812 */
    -41575,  /* index 114 is angle 320.625 */
    -39039,  /* index 115 is angle 323.438 */
    -36409,  /* index 116 is angle 326.25 */
    -33692,  /* index 117 is angle 329.062 */
    -30893,  /* index 118 is angle 331.875 */
    -28020,  /* index 119 is angle 334.688 */
    -25079,  /* index 120 is angle 337.5 */
    -22078,  /* index 121 is angle 340.312 */
    -19024,  /* index 122 is angle 343.125 */
    -15923,  /* index 123 is angle 345.938 */
    -12785,  /* index 124 is angle 348.75 */
    -9616,  /* index 125 is angle 351.562 */
    -6423,  /* index 126 is angle 354.375 */
    -3215  /* index 127 is angle 357.188 */
    };

  return lut[theta % 128];
}


/*
 * Returns 65536*cos(theta) for angle from 0 to 360 (orientation from 0 to 127)
 *
 */
int32_t UintCos(size_t theta)
{
  static const int32_t lut[] = {
    65536,  /* index 0 is angle 0 */
    65457,  /* index 1 is angle 2.8125 */
    65220,  /* index 2 is angle 5.625 */
    64826,  /* index 3 is angle 8.4375 */
    64276,  /* index 4 is angle 11.25 */
    63571,  /* index 5 is angle 14.0625 */
    62714,  /* index 6 is angle 16.875 */
    61705,  /* index 7 is angle 19.6875 */
    60547,  /* index 8 is angle 22.5 */
    59243,  /* index 9 is angle 25.3125 */
    57797,  /* index 10 is angle 28.125 */
    56212,  /* index 11 is angle 30.9375 */
    54491,  /* index 12 is angle 33.75 */
    52639,  /* index 13 is angle 36.5625 */
    50660,  /* index 14 is angle 39.375 */
    48558,  /* index 15 is angle 42.1875 */
    46340,  /* index 16 is angle 45 */
    44011,  /* index 17 is angle 47.8125 */
    41575,  /* index 18 is angle 50.625 */
    39039,  /* index 19 is angle 53.4375 */
    36409,  /* index 20 is angle 56.25 */
    33692,  /* index 21 is angle 59.0625 */
    30893,  /* index 22 is angle 61.875 */
    28020,  /* index 23 is angle 64.6875 */
    25079,  /* index 24 is angle 67.5 */
    22078,  /* index 25 is angle 70.3125 */
    19024,  /* index 26 is angle 73.125 */
    15923,  /* index 27 is angle 75.9375 */
    12785,  /* index 28 is angle 78.75 */
    9616,  /* index 29 is angle 81.5625 */
    6423,  /* index 30 is angle 84.375 */
    3215,  /* index 31 is angle 87.1875 */
    0,  /* index 32 is angle 90 */
    -3215,  /* index 33 is angle 92.8125 */
    -6423,  /* index 34 is angle 95.625 */
    -9616,  /* index 35 is angle 98.4375 */
    -12785,  /* index 36 is angle 101.25 */
    -15923,  /* index 37 is angle 104.062 */
    -19024,  /* index 38 is angle 106.875 */
    -22078,  /* index 39 is angle 109.688 */
    -25079,  /* index 40 is angle 112.5 */
    -28020,  /* index 41 is angle 115.312 */
    -30893,  /* index 42 is angle 118.125 */
    -33692,  /* index 43 is angle 120.938 */
    -36409,  /* index 44 is angle 123.75 */
    -39039,  /* index 45 is angle 126.562 */
    -41575,  /* index 46 is angle 129.375 */
    -44011,  /* index 47 is angle 132.188 */
    -46340,  /* index 48 is angle 135 */
    -48558,  /* index 49 is angle 137.812 */
    -50660,  /* index 50 is angle 140.625 */
    -52639,  /* index 51 is angle 143.438 */
    -54491,  /* index 52 is angle 146.25 */
    -56212,  /* index 53 is angle 149.062 */
    -57797,  /* index 54 is angle 151.875 */
    -59243,  /* index 55 is angle 154.688 */
    -60547,  /* index 56 is angle 157.5 */
    -61705,  /* index 57 is angle 160.312 */
    -62714,  /* index 58 is angle 163.125 */
    -63571,  /* index 59 is angle 165.938 */
    -64276,  /* index 60 is angle 168.75 */
    -64826,  /* index 61 is angle 171.562 */
    -65220,  /* index 62 is angle 174.375 */
    -65457,  /* index 63 is angle 177.188 */
    -65536,  /* index 64 is angle 180 */
    -65457,  /* index 65 is angle 182.812 */
    -65220,  /* index 66 is angle 185.625 */
    -64826,  /* index 67 is angle 188.438 */
    -64276,  /* index 68 is angle 191.25 */
    -63571,  /* index 69 is angle 194.062 */
    -62714,  /* index 70 is angle 196.875 */
    -61705,  /* index 71 is angle 199.688 */
    -60547,  /* index 72 is angle 202.5 */
    -59243,  /* index 73 is angle 205.312 */
    -57797,  /* index 74 is angle 208.125 */
    -56212,  /* index 75 is angle 210.938 */
    -54491,  /* index 76 is angle 213.75 */
    -52639,  /* index 77 is angle 216.562 */
    -50660,  /* index 78 is angle 219.375 */
    -48558,  /* index 79 is angle 222.188 */
    -46340,  /* index 80 is angle 225 */
    -44011,  /* index 81 is angle 227.812 */
    -41575,  /* index 82 is angle 230.625 */
    -39039,  /* index 83 is angle 233.438 */
    -36409,  /* index 84 is angle 236.25 */
    -33692,  /* index 85 is angle 239.062 */
    -30893,  /* index 86 is angle 241.875 */
    -28020,  /* index 87 is angle 244.687 */
    -25079,  /* index 88 is angle 247.5 */
    -22078,  /* index 89 is angle 250.313 */
    -19024,  /* index 90 is angle 253.125 */
    -15923,  /* index 91 is angle 255.938 */
    -12785,  /* index 92 is angle 258.75 */
    -9616,  /* index 93 is angle 261.562 */
    -6423,  /* index 94 is angle 264.375 */
    -3215,  /* index 95 is angle 267.188 */
    0,  /* index 96 is angle 270 */
    3215,  /* index 97 is angle 272.812 */
    6423,  /* index 98 is angle 275.625 */
    9616,  /* index 99 is angle 278.438 */
    12785,  /* index 100 is angle 281.25 */
    15923,  /* index 101 is angle 284.062 */
    19024,  /* index 102 is angle 286.875 */
    22078,  /* index 103 is angle 289.688 */
    25079,  /* index 104 is angle 292.5 */
    28020,  /* index 105 is angle 295.312 */
    30893,  /* index 106 is angle 298.125 */
    33692,  /* index 107 is angle 300.938 */
    36409,  /* index 108 is angle 303.75 */
    39039,  /* index 109 is angle 306.562 */
    41575,  /* index 110 is angle 309.375 */
    44011,  /* index 111 is angle 312.188 */
    46340,  /* index 112 is angle 315 */
    48558,  /* index 113 is angle 317.812 */
    50660,  /* index 114 is angle 320.625 */
    52639,  /* index 115 is angle 323.438 */
    54491,  /* index 116 is angle 326.25 */
    56212,  /* index 117 is angle 329.062 */
    57797,  /* index 118 is angle 331.875 */
    59243,  /* index 119 is angle 334.688 */
    60547,  /* index 120 is angle 337.5 */
    61705,  /* index 121 is angle 340.312 */
    62714,  /* index 122 is angle 343.125 */
    63571,  /* index 123 is angle 345.938 */
    64276,  /* index 124 is angle 348.75 */
    64826,  /* index 125 is angle 351.562 */
    65220,  /* index 126 is angle 354.375 */
    65457  /* index 127 is angle 357.188 */
    };

  return lut[theta % 128];
}


/*
 * Returns 65536*tan(theta) for angle from 0 to 360 (orientation from 0 to 127)
 *
 */
int32_t UintTan(size_t theta)
{
  static const int32_t lut[] = {
    0,  /* index 0 is angle 0 */
    3219,  /* index 1 is angle 2.8125 */
    6454,  /* index 2 is angle 5.625 */
    9721,  /* index 3 is angle 8.4375 */
    13035,  /* index 4 is angle 11.25 */
    16415,  /* index 5 is angle 14.0625 */
    19880,  /* index 6 is angle 16.875 */
    23449,  /* index 7 is angle 19.6875 */
    27145,  /* index 8 is angle 22.5 */
    30996,  /* index 9 is angle 25.3125 */
    35029,  /* index 10 is angle 28.125 */
    39280,  /* index 11 is angle 30.9375 */
    43789,  /* index 12 is angle 33.75 */
    48604,  /* index 13 is angle 36.5625 */
    53784,  /* index 14 is angle 39.375 */
    59398,  /* index 15 is angle 42.1875 */
    65535,  /* index 16 is angle 45 */
    72307,  /* index 17 is angle 47.8125 */
    79855,  /* index 18 is angle 50.625 */
    88365,  /* index 19 is angle 53.4375 */
    98081,  /* index 20 is angle 56.25 */
    109340,  /* index 21 is angle 59.0625 */
    122609,  /* index 22 is angle 61.875 */
    138564,  /* index 23 is angle 64.6875 */
    158217,  /* index 24 is angle 67.5 */
    183160,  /* index 25 is angle 70.3125 */
    216043,  /* index 26 is angle 73.125 */
    261634,  /* index 27 is angle 75.9375 */
    329471,  /* index 28 is angle 78.75 */
    441807,  /* index 29 is angle 81.5625 */
    665398,  /* index 30 is angle 84.375 */
    1334015,  /* index 31 is angle 87.1875 */
    0 /*UNDEFINED*/,  /* index 32 is angle 90 */
    -1334015,  /* index 33 is angle 92.8125 */
    -665398,  /* index 34 is angle 95.625 */
    -441807,  /* index 35 is angle 98.4375 */
    -329471,  /* index 36 is angle 101.25 */
    -261634,  /* index 37 is angle 104.062 */
    -216043,  /* index 38 is angle 106.875 */
    -183160,  /* index 39 is angle 109.688 */
    -158217,  /* index 40 is angle 112.5 */
    -138564,  /* index 41 is angle 115.312 */
    -122609,  /* index 42 is angle 118.125 */
    -109340,  /* index 43 is angle 120.938 */
    -98081,  /* index 44 is angle 123.75 */
    -88365,  /* index 45 is angle 126.562 */
    -79855,  /* index 46 is angle 129.375 */
    -72307,  /* index 47 is angle 132.188 */
    -65536,  /* index 48 is angle 135 */
    -59398,  /* index 49 is angle 137.812 */
    -53784,  /* index 50 is angle 140.625 */
    -48604,  /* index 51 is angle 143.438 */
    -43789,  /* index 52 is angle 146.25 */
    -39280,  /* index 53 is angle 149.062 */
    -35029,  /* index 54 is angle 151.875 */
    -30996,  /* index 55 is angle 154.688 */
    -27145,  /* index 56 is angle 157.5 */
    -23449,  /* index 57 is angle 160.312 */
    -19880,  /* index 58 is angle 163.125 */
    -16415,  /* index 59 is angle 165.938 */
    -13035,  /* index 60 is angle 168.75 */
    -9721,  /* index 61 is angle 171.562 */
    -6454,  /* index 62 is angle 174.375 */
    -3219,  /* index 63 is angle 177.188 */
    0,  /* index 64 is angle 180 */
    3219,  /* index 65 is angle 182.812 */
    6454,  /* index 66 is angle 185.625 */
    9721,  /* index 67 is angle 188.438 */
    13035,  /* index 68 is angle 191.25 */
    16415,  /* index 69 is angle 194.062 */
    19880,  /* index 70 is angle 196.875 */
    23449,  /* index 71 is angle 199.688 */
    27145,  /* index 72 is angle 202.5 */
    30996,  /* index 73 is angle 205.312 */
    35029,  /* index 74 is angle 208.125 */
    39280,  /* index 75 is angle 210.938 */
    43789,  /* index 76 is angle 213.75 */
    48604,  /* index 77 is angle 216.562 */
    53784,  /* index 78 is angle 219.375 */
    59398,  /* index 79 is angle 222.188 */
    65535,  /* index 80 is angle 225 */
    72307,  /* index 81 is angle 227.812 */
    79855,  /* index 82 is angle 230.625 */
    88365,  /* index 83 is angle 233.438 */
    98081,  /* index 84 is angle 236.25 */
    109340,  /* index 85 is angle 239.062 */
    122609,  /* index 86 is angle 241.875 */
    138564,  /* index 87 is angle 244.687 */
    158217,  /* index 88 is angle 247.5 */
    183160,  /* index 89 is angle 250.313 */
    216043,  /* index 90 is angle 253.125 */
    261634,  /* index 91 is angle 255.938 */
    329471,  /* index 92 is angle 258.75 */
    441807,  /* index 93 is angle 261.562 */
    665398,  /* index 94 is angle 264.375 */
    1334015,  /* index 95 is angle 267.188 */
    0 /*UNDEFINED*/,  /* index 96 is angle 270 */
    -1334015,  /* index 97 is angle 272.812 */
    -665398,  /* index 98 is angle 275.625 */
    -441807,  /* index 99 is angle 278.438 */
    -329471,  /* index 100 is angle 281.25 */
    -261634,  /* index 101 is angle 284.062 */
    -216043,  /* index 102 is angle 286.875 */
    -183160,  /* index 103 is angle 289.688 */
    -158217,  /* index 104 is angle 292.5 */
    -138564,  /* index 105 is angle 295.312 */
    -122609,  /* index 106 is angle 298.125 */
    -109340,  /* index 107 is angle 300.938 */
    -98081,  /* index 108 is angle 303.75 */
    -88365,  /* index 109 is angle 306.562 */
    -79855,  /* index 110 is angle 309.375 */
    -72307,  /* index 111 is angle 312.188 */
    -65536,  /* index 112 is angle 315 */
    -59398,  /* index 113 is angle 317.812 */
    -53784,  /* index 114 is angle 320.625 */
    -48604,  /* index 115 is angle 323.438 */
    -43789,  /* index 116 is angle 326.25 */
    -39280,  /* index 117 is angle 329.062 */
    -35029,  /* index 118 is angle 331.875 */
    -30996,  /* index 119 is angle 334.688 */
    -27145,  /* index 120 is angle 337.5 */
    -23449,  /* index 121 is angle 340.312 */
    -19880,  /* index 122 is angle 343.125 */
    -16415,  /* index 123 is angle 345.938 */
    -13035,  /* index 124 is angle 348.75 */
    -9721,  /* index 125 is angle 351.562 */
    -6454,  /* index 126 is angle 354.375 */
    -3219  /* index 127 is angle 357.188 */
    };

  return lut[theta % 128];
}


/*
 * Sum of squares distance between two CbCr points (Euclidean norm squared)
 *
 */
#ifndef USE_INLINE
size_t CbCrSSD(const uint16_t v1, const uint16_t v2)
{
  const uint16_t diff1 = UINTDIFF( ((PackedCbCr_t)v1).data[0],
                                   ((PackedCbCr_t)v2).data[0] );
  const uint16_t diff2 = UINTDIFF( ((PackedCbCr_t)v1).data[1],
                                   ((PackedCbCr_t)v2).data[1] );
  return diff1 * diff1 + diff2 * diff2;
}
#endif


/*
 * From: http://en.wikipedia.org/wiki/YCbCr
 *
JPEG-YCbCr (601) from "digital 8-bit R'G'B'  "
========================================================================
Y' =       + 0.299    * R'd + 0.587    * G'd + 0.114    * B'd
Cb = 128   - 0.168736 * R'd - 0.331264 * G'd + 0.5      * B'd
Cr = 128   + 0.5      * R'd - 0.418688 * G'd - 0.081312 * B'd
........................................................................
R'd, G'd, B'd   in {0, 1, 2, ..., 255}
Y', Cb, Cr      in {0, 1, 2, ..., 255}
 *
 */
#ifndef USE_INLINE
void YCbCrFromRGB (uint8_t *outY,
                   uint8_t *outCb,
                   uint8_t *outCr,
                   const uint8_t inRed,
                   const uint8_t inGreen,
                   const uint8_t inBlue)
{
  size_t tmpY, tmpCb, tmpCr;

  tmpY   = inRed * 299;
  tmpCb  = 128000000 - 168736 * inRed;
  tmpCr  = 128000000 + 500000 * inRed;

  tmpY  += 587 * inGreen;
  tmpCb -= 331264 * inGreen;
  tmpCr -= 418688 * inGreen;

  tmpY  += 114 * inBlue;
  tmpCb += 500000 * inBlue;
  tmpCr -= 81312 * inBlue;

  *outY  = tmpY / 1000;
  *outCb = tmpCb / 1000000;
  *outCr = tmpCr / 1000000;
}
#endif


/* guess image type from first few bytes in the stream */
EcvImageType GuessImageType (FILE *s)
{
  /* first two bytes indicate image format */
  uint8_t headbytes[2];
  if (fread(headbytes, sizeof(uint8_t), 2, s) == 2)
  {
    if ( (headbytes[0] == 0xff) && (headbytes[1] == 0xd8) )
    {
      /* need another byte for JPEG marker start */
      if (fread(headbytes, sizeof(uint8_t), 1, s) == 1)
      {
        if (headbytes[0] == 0xff)
        {
          /* JPEG format */
          return ECV_JPEG;
        }
      }
    }
    else if (headbytes[0] == 'P')
    {
      /* PPM formats P5 (grayscale) or P6 (RGB) */
      if (headbytes[1] == '5')
      {
        return ECV_PPM_GRAY;
      }
      else if (headbytes[1] == '6')
      {
        return ECV_PPM_RGB;
      }
    }
  }

  return ECV_UNKNOWN;
}


/*
 * Buffered reading of bytes from a stream (safe version)
 *
 */
#ifndef USE_INLINE
int ReadByte (uint8_t  *outValue,
              FILE     *inStream,
              Buffer_t *inoutBuffer)
{
  if ( (inoutBuffer->position == inoutBuffer->size) && (! feof(inStream)) )
  {
    inoutBuffer->position = inoutBuffer->head;
    inoutBuffer->size = inoutBuffer->head +
                        fread(inoutBuffer->head,
                              sizeof(uint8_t),
                              inoutBuffer->tail - inoutBuffer->head,
                              inStream);
  }

  if (inoutBuffer->position < inoutBuffer->size)
  {
    *outValue = *inoutBuffer->position++;
    return 1;
  }
  else
  {
    return 0;
  }
}
#endif


/*
 * Buffered reading of bytes from a stream (unsafe version)
 *
 */
#ifndef USE_INLINE
uint8_t ReadByteFast (FILE     *inStream,
                      Buffer_t *inoutBuffer)
{
  if ( (inoutBuffer->position == inoutBuffer->size) && (! feof(inStream)) )
  {
    inoutBuffer->position = inoutBuffer->head;
    inoutBuffer->size = inoutBuffer->head +
                        fread(inoutBuffer->head,
                              sizeof(uint8_t),
                              inoutBuffer->tail - inoutBuffer->head,
                              inStream);
  }

  if (inoutBuffer->position < inoutBuffer->size)
  {
    return *inoutBuffer->position++;
  }
  else
  {
    return 0;
  }
}
#endif


/*
 * Read from a stream until a marker string is encountered
 *
 */
void SeekMarker (uint8_t       *markBuffer,    /* one element smaller */
                 const size_t   markLength,
                 const uint8_t *markString,
                 Buffer_t      *outBuffer,     /* optional - may be null */
                 FILE          *stream,
                 Buffer_t      *streamBuffer)
{
  const uint8_t  tailMarkChar = *(markString + markLength - 1);

  const uint8_t *endMarkBuffer = markBuffer + markLength - 2;

  uint8_t       *ptrMarkBuffer,
                *nextMarkBuffer,
                *ptrSize;

  uint8_t value;  /* most recently read byte */

  /* previously read bytes go in the markBuffer */
  memset(markBuffer, 0, sizeof(uint8_t) * (markLength - 1));

  if (outBuffer)
  {
    ptrSize = (uint8_t *) outBuffer->size;  /* cast away constness */

    while (1)
    {
      if (! ReadByte(&value, stream, streamBuffer))
      {
        break;
      }

      /* expect output buffer is large enough to contain all bytes read */
      if (ptrSize != outBuffer->tail)
      {
        *ptrSize++ = value;
      }

      if ( (value == tailMarkChar) &&
           (! memcmp(markBuffer, markString, markLength - 1)) )
      {
        break;
      }

      ptrMarkBuffer  = markBuffer;
      nextMarkBuffer = markBuffer + 1;
      while (ptrMarkBuffer != endMarkBuffer)
      {
        *ptrMarkBuffer++ = *nextMarkBuffer++;
      }
      *ptrMarkBuffer = value;
    }

    outBuffer->size = ptrSize;
  }
  else
  {
    while (1)
    {
      if (! ReadByte(&value, stream, streamBuffer))
      {
        break;
      }

      if ( (value == tailMarkChar) &&
           (! memcmp(markBuffer, markString, markLength - 1)) )
      {
        break;
      }

      ptrMarkBuffer  = markBuffer;
      nextMarkBuffer = markBuffer + 1;
      while (ptrMarkBuffer != endMarkBuffer)
      {
        *ptrMarkBuffer++ = *nextMarkBuffer++;
      }
      *ptrMarkBuffer = value;
    }
  }
}

