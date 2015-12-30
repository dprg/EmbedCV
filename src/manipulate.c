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


#include <stdint.h>
#include <stddef.h>
#include <string.h>


#include "embedcv.h"


/*
 * Crop an 8 bit image from another image
 *
 * The output image is presumed to be smaller than the input image and fit
 * inside. If the specified column and row combined with the dimensions of
 * the two images do not allow this, then the memory copy will cause a
 * segmentation fault. There is no bounds checking.
 *
 */
void CropImage (Image8_t       *outImg,
                const Image8_t *inImg,
                const size_t    inColumn,
                const size_t    inRow)
{
  const size_t outWidth = outImg->width;
  const size_t  inWidth = inImg->width;

  const size_t cpylen = sizeof(uint8_t) * outWidth;

  const uint8_t *ptrInData  = inImg->data + inRow * inWidth + inColumn;
  uint8_t       *ptrOutData = outImg->data;

  UNROLL_LOOP( outImg->height,

      memcpy(ptrOutData, ptrInData, cpylen);
      ptrOutData += outWidth;
      ptrInData  += inWidth;
  )
}


/*
 * Crop a 16 bit image from another image
 *
 * The output image is presumed to be smaller than the input image and fit
 * inside. If the specified column and row combined with the dimensions of
 * the two images do not allow this, then the memory copy will cause a
 * segmentation fault. There is no bounds checking.
 *
 */
void CropImageW (Image16_t       *outImg,
                 const Image16_t *inImg,
                 const size_t     inColumn,
                 const size_t     inRow)
{
  const size_t outWidth = outImg->width;
  const size_t  inWidth = inImg->width;

  const size_t cpylen = sizeof(uint16_t) * outWidth;

  const uint16_t *ptrInData  = inImg->data + inRow * inWidth + inColumn;
  uint16_t       *ptrOutData = outImg->data;

  UNROLL_LOOP( outImg->height,

      memcpy(ptrOutData, ptrInData, cpylen);
      ptrOutData += outWidth;
      ptrInData  += inWidth;
  )
}


/*
 * Paste an 8 bit image into another image
 *
 * The smaller input image is pasted into a larger output image. There is no
 * bounds checking so if the input image does not fit inside the output image,
 * the memory copy will cause a segmentation fault.
 *
 */
void PasteImage (Image8_t       *outImg,
                 const Image8_t *inImg,
                 const size_t    outColumn,
                 const size_t    outRow)
{
  const size_t outWidth = outImg->width;
  const size_t  inWidth = inImg->width;

  const size_t cpylen = sizeof(uint8_t) * inWidth;

  const uint8_t *ptrInData  = inImg->data;
  uint8_t       *ptrOutData = outImg->data + outRow * outWidth + outColumn;

  UNROLL_LOOP( inImg->height,

      memcpy(ptrOutData, ptrInData, cpylen);
      ptrOutData += outWidth;
      ptrInData  += inWidth;
  )
}


/*
 * Downsample an 8 bit image (subsample)
 *
 * The input image is presumed to be dimensionally larger than the output
 * image. The smaller output dimensions should divide evenly into the larger
 * input dimensions.
 *
 */
void DownsampleImage (Image8_t       *outImg,
                      const Image8_t *inImg)
{
  const size_t outWidth  = outImg->width;
  const size_t outHeight = outImg->height;
  const size_t inWidth   = inImg->width;
  const size_t inHeight  = inImg->height;
  const size_t innerStep = inWidth / outWidth;
  const size_t outerStep = inWidth * (inHeight / outHeight - 1);

  const uint8_t *ptrIn   = inImg->data;
  uint8_t       *ptrOut  = outImg->data;

  NORMAL_LOOP( outHeight,

      UNROLL_LOOP( outWidth,

          *ptrOut++ = *ptrIn;
          ptrIn += innerStep;
      )

      ptrIn += outerStep;
  )
}


/*
 * Downsample a 16 bit image (subsample)
 *
 * The input image is presumed to be dimensionally larger than the output
 * image. The smaller output dimensions should divide evenly into the larger
 * input dimensions.
 *
 *
 */
void DownsampleImageW (Image16_t *outImg,
                       const Image16_t *inImg)
{
  const size_t outWidth  = outImg->width;
  const size_t outHeight = outImg->height;
  const size_t inWidth   = inImg->width;
  const size_t inHeight  = inImg->height;
  const size_t innerStep = inWidth / outWidth;
  const size_t outerStep = inWidth * (inHeight / outHeight - 1);

  const uint16_t *ptrIn  = inImg->data;
  uint16_t       *ptrOut = outImg->data;

  NORMAL_LOOP( outHeight,

      UNROLL_LOOP( outWidth,

          *ptrOut++ = *ptrIn;
          ptrIn += innerStep;
      )

      ptrIn += outerStep;
  )
}


/*
 * Upsample an 8 bit image (make it larger)
 *
 */
void UpsampleImage (Image8_t *outImg,
                    const Image8_t *inImg)
{
  const size_t outWidth  = outImg->width;
  const size_t outHeight = outImg->height;

  const size_t inWidth   = inImg->width;
  const size_t inHeight  = inImg->height;

  const size_t widthRatio        = outWidth / inWidth;
  const size_t heightRatioMinus1 = outHeight / inHeight - 1;

  const size_t cpylen = sizeof(uint8_t) * outWidth;

  const uint8_t *ptrLast;

  const uint8_t *ptrIn  = inImg->data;
  uint8_t       *ptrOut = outImg->data;

  size_t row, col, i;

  NORMAL_LOOP( inHeight,

      ptrLast = ptrOut;

      UNROLL_LOOP( inWidth,

          NORMAL_LOOP( widthRatio,

              *ptrOut++ = *ptrIn;
          )

          ptrIn++;
      )

      NORMAL_LOOP( heightRatioMinus1,

          memcpy(ptrOut, ptrLast, cpylen);
          ptrOut += outWidth;
      )
  )
}


/*
 * Flip an 8 bit image up / down
 *
 */
void FlipImage (Image8_t *outImg)
{
  const size_t outWidth = outImg->width;
  uint8_t rowbuf [outWidth];

  const size_t   cpylen     = sizeof(uint8_t) * outWidth;

  uint8_t       *ptrTop     = outImg->data;
  uint8_t       *ptrBottom  = outImg->data + (outImg->height - 1) * outWidth;

  UNROLL_LOOP( outImg->height >> 1,

      memcpy(rowbuf, ptrTop, cpylen);
      memcpy(ptrTop, ptrBottom, cpylen);
      memcpy(ptrBottom, rowbuf, cpylen);

      ptrTop    += outWidth;
      ptrBottom -= outWidth;
  )
}


/*
 * Flip an 16 bit image up / down
 *
 */
void FlipImageW (Image16_t *outImg)
{
  const size_t outWidth = outImg->width;
  uint16_t rowbuf [outWidth];

  const size_t   cpylen     = sizeof(uint16_t) * outWidth;

  uint16_t       *ptrTop     = outImg->data;
  uint16_t       *ptrBottom  = outImg->data + (outImg->height - 1) * outWidth;

  UNROLL_LOOP( outImg->height >> 1,

      memcpy(rowbuf, ptrTop, cpylen);
      memcpy(ptrTop, ptrBottom, cpylen);
      memcpy(ptrBottom, rowbuf, cpylen);

      ptrTop    += outWidth;
      ptrBottom -= outWidth;
  )
}


/*
 * Flop an 8 bit image left / right
 *
 */
void FlopImage (Image8_t *outImg)
{
  const size_t outHeight    = outImg->height;
  const size_t outWidth     = outImg->width;
  const size_t halfWidth    = outWidth >> 1;
  const size_t widthLessOne = outWidth - 1;

  uint8_t *ptrRow = outImg->data;

  uint8_t *ptrLeft, *ptrRight, tmp;

  NORMAL_LOOP( outHeight,

      ptrLeft   = ptrRow;
      ptrRight  = ptrRow + widthLessOne;

      UNROLL_LOOP( halfWidth,

          tmp         = *ptrLeft;
          *ptrLeft++  = *ptrRight;
          *ptrRight-- = tmp;
      )

      ptrRow += outWidth;
  )
}


/*
 * Flop an 16 bit image left / right
 *
 */
void FlopImageW (Image16_t *outImg)
{
  const size_t outHeight    = outImg->height;
  const size_t outWidth     = outImg->width;
  const size_t halfWidth    = outWidth >> 1;
  const size_t widthLessOne = outWidth - 1;

  uint16_t *ptrRow = outImg->data;

  uint16_t *ptrLeft, *ptrRight, tmp;

  NORMAL_LOOP( outHeight,

      ptrLeft   = ptrRow;
      ptrRight  = ptrRow + widthLessOne;

      UNROLL_LOOP( halfWidth,

          tmp         = *ptrLeft;
          *ptrLeft++  = *ptrRight;
          *ptrRight-- = tmp;
      )

      ptrRow += outWidth;
  )
}


/*
 * Convert a RGB image to YCbCr
 *
 */
void ConvertImageRGBtoYCbCr (Image8_t       *outYImg,
                             Image8_t       *outCbImg,
                             Image8_t       *outCrImg,
                             const Image8_t *inRedImg,
                             const Image8_t *inGreenImg,
                             const Image8_t *inBlueImg)
{
  const uint8_t *ptrRed   = inRedImg->data;
  const uint8_t *ptrGreen = inGreenImg->data;
  const uint8_t *ptrBlue  = inBlueImg->data;

  uint8_t       *ptrLuma  = outYImg->data;
  uint8_t       *ptrCb    = outCbImg->data;
  uint8_t       *ptrCr    = outCrImg->data;

  UNROLL_LOOP( outYImg->width * outYImg->height,

      YCbCrFromRGB(ptrLuma++,
                   ptrCb++,
                   ptrCr++,
                   *ptrRed++,
                   *ptrGreen++,
                   *ptrBlue++);
  )
}


/*
 * Convert a RGB image to YCbCr in packed 16 bit pixels
 *
 */
void ConvertImageRGBtoYCbCrPacked (Image8_t       *outYImg,
                                   Image16_t      *outCbCrImg,
                                   const Image8_t *inRedImg,
                                   const Image8_t *inGreenImg,
                                   const Image8_t *inBlueImg)
{
  const uint8_t *ptrRed   = inRedImg->data;
  const uint8_t *ptrGreen = inGreenImg->data;
  const uint8_t *ptrBlue  = inBlueImg->data;

  uint8_t       *ptrLuma  = outYImg->data;
  PackedCbCr_t  *ptrCbCr = (PackedCbCr_t *) outCbCrImg->data;

  UNROLL_LOOP( outYImg->width * outYImg->height,

      YCbCrFromRGB(ptrLuma++,
                   &ptrCbCr->data[0],
                   &ptrCbCr->data[1],
                   *ptrRed++,
                   *ptrGreen++,
                   *ptrBlue++);
      ptrCbCr++;
  )
}

