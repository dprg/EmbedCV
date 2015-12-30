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


#include <stdlib.h>
#include <string.h>


#include "embedcv.h"


/*
 * Integral image transform (as used by Viola and Jones)
 *
 */
void IntegralImage (Image32_t      *outImg,
                    const Image8_t *inImg)
{
  const size_t width = inImg->width;

  size_t        *ptrOut = outImg->data;
  const uint8_t  *ptrIn = inImg->data;

  size_t accum = 0;

  /* first row */
  UNROLL_LOOP( width,

      accum += *ptrIn++;
      *ptrOut++ = accum;
  )

  const size_t *ptrLast;

  /* subsequent rows */
  UNROLL_LOOP( inImg->height - 1,

      ptrLast = ptrOut - width;
      accum   = 0;

      UNROLL_LOOP( width,

          accum += *ptrIn++;
          *ptrOut++ = accum + *ptrLast++;
      )
  )
}


/*
 * Convert the integral image to an 8 bit image
 *
 */
void ConvertIntegralFeatureImage (Image8_t        *outImg,
                                  const Image32_t *inImg,
                                  const size_t     shift)
{
  const size_t colStep   = outImg->width / inImg->width;
  const size_t rowStep   = outImg->height / inImg->height;
  const size_t rowOffset = outImg->width * rowStep - inImg->width * colStep;

  const size_t *ptrIn = inImg->data;

  uint8_t *ptrOut = outImg->data
                        + ((outImg->width - inImg->width * colStep) >> 1)
                        + ((outImg->height - inImg->height * rowStep) >> 1)
                              * outImg->width;

  NORMAL_LOOP( inImg->height,

      UNROLL_LOOP( inImg->width,

          *ptrOut = (*ptrIn++) >> shift;

          ptrOut += colStep;
      )

      ptrOut += rowOffset;
  )
}


/*
 * Integral image feature of two boxes, one above the other
 *
 * The output image is smaller than the input image for two reasons. One is
 * that the dimensions of the box feature limits travel to the borders of the
 * input image. The box feature window can not move all the way over to the
 * edge without hanging over it. The other reason is the specified step size
 * as the box feature scans over the input image. The window skips positions
 * in the input image. So it consequently is more efficient to store the
 * output image in a packed representation.
 *
 */
void IntegralFeatureUpDown (Image32_t       *outImg,
                            size_t          *outMaxValue,
                            const Image32_t *inImg,
                            const size_t     boxWidth,
                            const size_t     boxHeight,
                            const size_t     colStep,
                            const size_t     rowStep)
{
  const size_t *ptrInUpperLeft   = inImg->data;
  const size_t *ptrInCenterLeft  = inImg->data + boxHeight * inImg->width;
  const size_t *ptrInLowerLeft   = inImg->data
                                       + ((boxHeight * inImg->width) << 1);
  const size_t *ptrInUpperRight  = ptrInUpperLeft + boxWidth;
  const size_t *ptrInCenterRight = ptrInCenterLeft + boxWidth;
  const size_t *ptrInLowerRight  = ptrInLowerLeft + boxWidth;

  const size_t numColSteps = (inImg->width - boxWidth) / colStep;
  const size_t rowOffset   = inImg->width * rowStep - numColSteps * colStep;

  size_t *ptrOut = outImg->data;

  size_t upperBoxSum, lowerBoxSum, diffValue, diffMax = 0;

  NORMAL_LOOP( (inImg->height - (boxHeight << 1)) / rowStep,

      UNROLL_LOOP( numColSteps,

          upperBoxSum = *ptrInUpperLeft + *ptrInCenterRight
                            - (*ptrInUpperRight + *ptrInCenterLeft);

          lowerBoxSum = *ptrInCenterLeft + *ptrInLowerRight
                            - (*ptrInCenterRight + *ptrInLowerLeft);

          *ptrOut++ = diffValue = UINTDIFF(upperBoxSum, lowerBoxSum);

          if (diffValue > diffMax)
          {
            diffMax = diffValue;
          }

          ptrInUpperLeft   += colStep;
          ptrInCenterLeft  += colStep;
          ptrInLowerLeft   += colStep;
          ptrInUpperRight  += colStep;
          ptrInCenterRight += colStep;
          ptrInLowerRight  += colStep;
      )

      ptrInUpperLeft   += rowOffset;
      ptrInCenterLeft  += rowOffset;
      ptrInLowerLeft   += rowOffset;
      ptrInUpperRight  += rowOffset;
      ptrInCenterRight += rowOffset;
      ptrInLowerRight  += rowOffset;
  )

  if (outMaxValue)
  {
    *outMaxValue = diffMax;
  }
}


/*
 * Integral image feature of two boxes, one left and the other right
 *
 * This is very much like the IntegralFeatureUpDown() function except that
 * the box window looks for differences horizontally instead of vertically.
 *
 */
void IntegralFeatureLeftRight (Image32_t       *outImg,
                               size_t          *outMaxValue,
                               const Image32_t *inImg,
                               const size_t     boxWidth,
                               const size_t     boxHeight,
                               const size_t     colStep,
                               const size_t     rowStep)
{
  const size_t *ptrInUpperLeft   = inImg->data;
  const size_t *ptrInUpperCenter = inImg->data + boxWidth;
  const size_t *ptrInUpperRight  = inImg->data + (boxWidth << 1);

  const size_t *ptrInLowerLeft   = inImg->data + (boxHeight * inImg->width);
  const size_t *ptrInLowerCenter = ptrInLowerLeft + boxWidth;
  const size_t *ptrInLowerRight  = ptrInLowerLeft + (boxWidth << 1);

  const size_t numColSteps = (inImg->width - (boxWidth << 1)) / colStep;
  const size_t rowOffset   = inImg->width * rowStep - numColSteps * colStep;

  size_t *ptrOut = outImg->data;

  size_t leftBoxSum, rightBoxSum, diffValue, diffMax = 0;

  NORMAL_LOOP( (inImg->height - boxHeight) / rowStep,

      UNROLL_LOOP( numColSteps,

          leftBoxSum  = *ptrInUpperLeft + *ptrInLowerCenter
                            - (*ptrInUpperCenter + *ptrInLowerLeft);

          rightBoxSum = *ptrInUpperCenter + *ptrInLowerRight
                            - (*ptrInUpperRight + *ptrInLowerCenter);

          *ptrOut++ = diffValue = UINTDIFF(leftBoxSum, rightBoxSum);

          if (diffValue > diffMax)
          {
            diffMax = diffValue;
          }

          ptrInUpperLeft   += colStep;
          ptrInUpperCenter += colStep;
          ptrInUpperRight  += colStep;
          ptrInLowerLeft   += colStep;
          ptrInLowerCenter += colStep;
          ptrInLowerRight  += colStep;
      )

      ptrInUpperLeft   += rowOffset;
      ptrInUpperCenter += rowOffset;
      ptrInUpperRight  += rowOffset;
      ptrInLowerLeft   += rowOffset;
      ptrInLowerCenter += rowOffset;
      ptrInLowerRight  += rowOffset;
  )

  if (outMaxValue)
  {
    *outMaxValue = diffMax;
  }
}


/*
 * Integral image feature of along the diagonal with four boxes
 *
 * Diagonal corners and edges tend to be detected. This complements the other
 * box feature detectors for LeftRight (vertical edges) and UpDown (horizontal
 * edges).
 *
 */
void IntegralFeatureDiagonal (Image32_t       *outImg,
                              size_t          *outMaxValue,
                              const Image32_t *inImg,
                              const size_t     boxWidth,
                              const size_t     boxHeight,
                              const size_t     colStep,
                              const size_t     rowStep)
{
  /* top row */
  const size_t *ptrIn00 = inImg->data;
  const size_t *ptrIn01 = ptrIn00 + boxWidth;
  const size_t *ptrIn02 = ptrIn01 + boxWidth;

  /* middle row */
  const size_t *ptrIn10 = ptrIn00 + boxHeight * inImg->width;
  const size_t *ptrIn11 = ptrIn10 + boxWidth;
  const size_t *ptrIn12 = ptrIn11 + boxWidth;

  /* bottom row */
  const size_t *ptrIn20 = ptrIn10 + boxHeight * inImg->width;
  const size_t *ptrIn21 = ptrIn20 + boxWidth;
  const size_t *ptrIn22 = ptrIn21 + boxWidth;

  const size_t numColSteps = (inImg->width - (boxWidth << 1)) / colStep;
  const size_t rowOffset   = inImg->width * rowStep - numColSteps * colStep;

  size_t *ptrOut = outImg->data;

  /* BLACK WHITE
   * WHITE BLACK
   */
  size_t totalBoxSum, whiteBoxSum, diffValue, diffMax = 0;

  NORMAL_LOOP( (inImg->height - (boxHeight << 1)) / rowStep,

      UNROLL_LOOP( numColSteps,

          totalBoxSum = *ptrIn00 + *ptrIn22 - *ptrIn02 - *ptrIn20;
          whiteBoxSum = *ptrIn01 + *ptrIn10 + *ptrIn12 + *ptrIn21
                            - *ptrIn02 - *ptrIn20 - (*ptrIn11 << 1);

          *ptrOut++ = diffValue
                    = UINTDIFF(whiteBoxSum, totalBoxSum - whiteBoxSum);

          if (diffValue > diffMax)
          {
            diffMax = diffValue;
          }

          ptrIn00 += colStep;
          ptrIn01 += colStep;
          ptrIn02 += colStep;
          ptrIn10 += colStep;
          ptrIn11 += colStep;
          ptrIn12 += colStep;
          ptrIn20 += colStep;
          ptrIn21 += colStep;
          ptrIn22 += colStep;
      )

      ptrIn00 += rowOffset;
      ptrIn01 += rowOffset;
      ptrIn02 += rowOffset;
      ptrIn10 += rowOffset;
      ptrIn11 += rowOffset;
      ptrIn12 += rowOffset;
      ptrIn20 += rowOffset;
      ptrIn21 += rowOffset;
      ptrIn22 += rowOffset;
  )

  if (outMaxValue)
  {
    *outMaxValue = diffMax;
  }
}

