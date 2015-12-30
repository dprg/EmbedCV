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


#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>


#include "embedcv.h"


/*
 * Calculate radius distance of Hough transform edge line
 *
 * Inputs:    (x, y) is a point on the Hough line relative to the origin
 *            theta  is the angle of the Hough line to vote for
 *
 * Output:    radius is the length of the perpendicular from the origin to
 *            the Hough line (shortest possible distance)
 *
 * If the output radius is negative, then the Hough line through the point is
 * on the wrong side of the origin. This means that there should be no vote for
 * the line in a Hough transform bin.
 *
 */
int16_t HoughRadius(const int16_t x,
                    const int16_t y,
                    const size_t  theta)  /* theta of normal vector to edge */
{
  return (x * UintCos(theta) + y * UintSin(theta)) >> 16;
}


/*
 * Add the line votes from an image point to the Hough image
 *
 * Note the output Hough transform image must be 128 pixels wide but may be
 * of any height. All Hough lines too far away from the origin are discarded.
 *
 */
void HoughVoteLine(Image32_t    *img,
                   const int16_t x,
                   const int16_t y,
                   const int16_t dy,
                   const int16_t dx,
                   const size_t  neighborhood)
{
  const size_t  radiusLimit = img->height;
  size_t       *ptr         = img->data;
  int16_t       r;

  /* use gradient optimization to estimate lines to vote for */
  size_t theta = (ApproxAtan2(dy, dx) - neighborhood) % 128;

  UNROLL_LOOP( (neighborhood << 2) + 1,

      if ((r = HoughRadius(x, y, theta)) > 0)
      {
        r >>= 2;  /* Hough counting bins are 4 radius units high */
      }

      /* check for valid Hough line near enough to origin */
      if (r < radiusLimit)
      {
        ++ptr[ (r << 7) + theta ];
      }

      ++theta;
      theta %= 128;
  )
}

