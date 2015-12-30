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


#include "embedcv.h"


/*
 * Compute the histogram of pixel values in an image
 *
 * The output Histogram_t must the same number of bins as the maximum pixel
 * value in the image. So a general 8 bit image implies 256 bins. However,
 * if the image is quantized to a limited set of values, fewer bins may be ok.
 * For example, if the pixel values in an image take on the values 0, 1, 2, 3
 * only, then a Histogram_t with only 4 bins will work.
 *
 * To be safe, it is recommended to have 256 bins in the Histogram_t.
 *
 */
void ImageHistogram (Histogram_t    *outHistogram,
                     const Image8_t *inImg)
{
  /* density histogram */
  const size_t numPixels = outHistogram->numberCounts
                         = inImg->width * inImg->height;
  const uint8_t *ptrImg  = inImg->data;
  size_t        *ptrBins = outHistogram->bins;

  UNROLL_LOOP( numPixels,

    ptrBins[ *ptrImg++ ]++;
  )

  /* cumulative and partial expectation distributions */
  size_t *ptrSumBins  = outHistogram->sumBins;
  size_t *ptrMeanBins = outHistogram->meanBins;
  size_t accumSum     = 0;
  size_t accumMean    = 0;

  const size_t numBins = outHistogram->numberBins;
  size_t tmp, i;
  for (i = 0; i < numBins; ++i)
  {
    tmp       = *ptrBins++;
    accumSum  = *ptrSumBins++  = accumSum + tmp;
    accumMean = *ptrMeanBins++ = accumMean + i * tmp;
  }
}


/*
 * Compute the histogram of distances from a reference value to the pixel
 * values in an image
 *
 * This is identical to ImageHistogram() except for tabulating the differences
 * between a reference value and the pixel values. There must be as many bins
 * as the largest difference value encountered plus one. For example, for a
 * general 8 bit image, the pixel values range from 0 to 255. If the reference
 * value is 127, then the largest possible difference is 128. So there must
 * be at least 129 bins in the output Histogram_t.
 *
 * To be safe, it is recommended to have 256 bins in the Histogram_t.
 *
 */
void ImageHistogramDist (Histogram_t    *outHistogram,
                         const Image8_t *inImg,
                         const uint8_t   value)
{
  /* density histogram */
  const size_t numPixels = outHistogram->numberCounts
                         = inImg->width * inImg->height;
  const uint8_t *ptrImg  = inImg->data;
  size_t *ptrBins        = outHistogram->bins;

  UNROLL_LOOP( numPixels,

    ptrBins[ UINTDIFF( *ptrImg++, value ) ]++;
  )

  /* cumulative and partial expectation distributions */
  size_t *ptrSumBins  = outHistogram->sumBins;
  size_t *ptrMeanBins = outHistogram->meanBins;
  size_t accumSum     = 0;
  size_t accumMean    = 0;

  const size_t numBins = outHistogram->numberBins;
  size_t tmp, i;
  for (i = 0; i < numBins; ++i)
  {
    tmp       = *ptrBins++;
    accumSum  = *ptrSumBins++  = accumSum + tmp;
    accumMean = *ptrMeanBins++ = accumMean + i * tmp;
  }
}


/*
 * Compute the histogram of packed 16 bit CbCr pixel values in an image
 *
 * This is really computing two separate 8 bit histograms on the 16 bit packed
 * CbCr pixels. One 8 bit histogram corresponds to the Cb image. The other
 * is for the Cr image. So this function is functionally equivalent to using
 * ImageHistogram() on the Cb and Cr images individually.
 *
 * The Cb and Cr histograms should each have 256 bins. Undersized histograms
 * (acceptable for ImageHistogram() and ImageDiffHistogram() will not work as
 * the number of bins is hardcoded here.
 *
 */
void ImageHistogramCbCr (Histogram_t     *outCbHistogram,
                         Histogram_t     *outCrHistogram,
                         const Image16_t *inImg)
{
  /* density histogram */
  const size_t numPixels = outCbHistogram->numberCounts
                         = outCrHistogram->numberCounts
                         = inImg->width * inImg->height;
  const PackedCbCr_t *ptrImg = (PackedCbCr_t *)inImg->data;
  size_t *ptrCbBins      = outCbHistogram->bins;
  size_t *ptrCrBins      = outCrHistogram->bins;

  UNROLL_LOOP( numPixels,

    ptrCbBins[ ptrImg->data[0] ]++;
    ptrCrBins[ ptrImg->data[1] ]++;
    ptrImg++;
  )

  /* cumulative and partial expectation distributions */
  size_t *ptrCbSumBins  = outCbHistogram->sumBins;
  size_t *ptrCbMeanBins = outCbHistogram->meanBins;
  size_t *ptrCrSumBins  = outCrHistogram->sumBins;
  size_t *ptrCrMeanBins = outCrHistogram->meanBins;
  size_t accumCbSum     = 0;
  size_t accumCbMean    = 0;
  size_t accumCrSum     = 0;
  size_t accumCrMean    = 0;
  size_t tmp, i;
  for (i = 0; i < 256; ++i)  /* both Cb and Cr histograms have 256 bins */
  {
    tmp         = *ptrCbBins++;
    accumCbSum  = *ptrCbSumBins++  = accumCbSum + tmp;
    accumCbMean = *ptrCbMeanBins++ = accumCbMean + i * tmp;

    tmp         = *ptrCrBins++;
    accumCrSum  = *ptrCrSumBins++  = accumCrSum + tmp;
    accumCrMean = *ptrCrMeanBins++ = accumCrMean + i * tmp;
  }
}


/*
 * Compute the histogram of distances from a reference value to the pixel
 * values in an image - packed 16 bit CbCr pixel version
 *
 * The output histogram requires a minimum of 361 bins. Undersized histograms
 * (acceptable for ImageHistogram() and ImageDiffHistogram() will not work as
 * the number of bins is hardcoded here.
 *
 * This number arises from
 *
 * 360 = floor( sqrt( 255 * 255  +  255 * 255 ) )
 *
 * which is the largest 2 norm distance possible in CbCr space.
 *
 */
void ImageHistogramCbCrDist (Histogram_t     *outHistogram,
                             const Image16_t *inImg,
                             const uint16_t   value)
{
  /* density histogram */
  const size_t numPixels = outHistogram->numberCounts
                         = inImg->width * inImg->height;
  const uint16_t *ptrImg = inImg->data;
  size_t *ptrBins        = outHistogram->bins;

  UNROLL_LOOP( numPixels,

    ptrBins[ CBCR2DIST( *ptrImg++, value ) ]++;
  )

  /* cumulative and partial expectation distributions */
  size_t *ptrSumBins  = outHistogram->sumBins;
  size_t *ptrMeanBins = outHistogram->meanBins;
  size_t accumSum     = 0;
  size_t accumMean    = 0;

  size_t tmp, i;
  for (i = 0; i < 361; ++i)
  {
    tmp       = *ptrBins++;
    accumSum  = *ptrSumBins++  = accumSum + tmp;
    accumMean = *ptrMeanBins++ = accumMean + i * tmp;
  }
}


/*
 * Compute basic statistics of a histogram
 *
 * There are no special requirements for the histogram regarding the number of
 * bins. The histogram can be general, not necessarily derived from an image.
 *
 */
void HistogramStats (size_t *outMinIdx,
                     size_t *outMaxIdx,
                     size_t *outMeanIdx,
                     size_t *outVariance,
                     const Histogram_t *inHistogram)
{
  const size_t numBins   = inHistogram->numberBins;
  const size_t *ptrBins  = inHistogram->bins;
  const size_t *endBins  = ptrBins + numBins;

  size_t minValue, maxValue;
  minValue = maxValue = *ptrBins++;

  size_t minIdx        = 0;
  size_t maxIdx        = 0;
  size_t varianceAccum = 0;

  /* compute minimum, maximum and variance */
  size_t value, i;
  for (i = 1; i < numBins; ++i)
  {
    value = *ptrBins++;

    /* minimum */
    if (minValue > value)
    {
      minValue = value;
      minIdx = i;
    }

    /* maximum */
    if (maxValue < value)
    {
      maxValue = value;
      maxIdx = i;
    }

    /* variance - expected value of index squared */
    varianceAccum += value * i * i;
  }

  *outMinIdx   = minIdx;
  *outMaxIdx   = maxIdx;

  /* compute mean */
  const size_t numCounts = inHistogram->numberCounts;
  size_t meanIdx;
  *outMeanIdx  = meanIdx
               = inHistogram->meanBins[numBins - 1] / numCounts;

  *outVariance = varianceAccum / numCounts - meanIdx * meanIdx;
}


/*
 * Compute the median index value of the histogram
 *
 * This uses binary search.
 *
 */
size_t HistogramMedian (const Histogram_t *inHistogram)
{
  const size_t  halfCounts  = inHistogram->numberCounts >> 1;

  /* initial guess is middle of histogram */

  const size_t *lowerBound = inHistogram->sumBins; 
  const size_t *upperBound = inHistogram->sumBins + inHistogram->numberBins;
  const size_t *ptrSumBins = inHistogram->sumBins
                             + (inHistogram->numberBins >> 1);

  /*
   * when upper and lower bounds are equal or differ by one, the distance
   * between them divided by two is always zero so must terminate
   */
  while ( (upperBound - lowerBound) & ~0x1 )
  {
    if (*ptrSumBins < halfCounts)
    {
      /* need to search upper half */
      lowerBound = ptrSumBins;
      ptrSumBins += (upperBound - lowerBound) >> 1;
    }
    else if (*ptrSumBins > halfCounts)
    {
      /* need to search lower half */
      upperBound = ptrSumBins;
      ptrSumBins -= (upperBound - lowerBound) >> 1;
    }
    else
    {
      /* extreme luck, median value with exactly half number of counts */
      break;
    }
  }

  return ptrSumBins - inHistogram->sumBins;
}


/*
 * Equalize the histogram of an image
 *
 */
void EqualizeImage (Image8_t          *outImg,
                    const Histogram_t *inHistogram)
{
  size_t sumBins[inHistogram->numberBins];

  /* rescale distribution to an 8 bit value */
  const size_t *ptrHistSumBins = inHistogram->sumBins;
  const size_t  numCounts      = inHistogram->numberCounts;
  size_t       *ptrSumBins     = sumBins;

  UNROLL_LOOP( inHistogram->numberBins,

    *ptrSumBins++ = ((*ptrHistSumBins++ << 8) - 1) / numCounts;
  )

  /* equalize the output image */
  const size_t  numPixels = outImg->width * outImg->height;
  uint8_t      *ptrImg    = outImg->data;

  UNROLL_LOOP( numPixels,

    *ptrImg = sumBins[ *ptrImg ];
    ptrImg++;
  )
}


/*
 * Compute Otsu's image segmentation threshold
 *
 * Algorithm is from the book:
 *
 * Image Processing, The Fundamentals
 * by Maria Petrou and Panagiota Bosdogianni
 * Wiley, 1999
 *
 */
size_t OtsuThreshold (const Histogram_t *inHistogram)
{
  const size_t numBins      = inHistogram->numberBins;
  const size_t numCounts    = inHistogram->numberCounts;

  const size_t *ptrMeanBins = inHistogram->meanBins;
  const size_t mean         = ptrMeanBins[numBins - 1] / numCounts;

  const size_t *ptrSumBins  = inHistogram->sumBins;
  const size_t *headSumBins = ptrSumBins;
  const size_t *endSumBins  = ptrSumBins + numBins;

  size_t last = 0, curr, a, p;

  /* find threshold that maximizes interclass variance */
  while (ptrSumBins != endSumBins)
  {
    p    = *ptrSumBins++;

    if ( (p == 0) || (p == numCounts) )
    {
      continue;  /* avoid divide by zero */
    }

    a    = UINTDIFF( *ptrMeanBins++, mean * p );

    curr = (a * a) / (p * (numCounts - p));
    if (curr < last)
    {
      break;
    }
    last = curr;
  }

  return ptrSumBins - headSumBins;
}


/*
 * Add an interval of pixel values to an 8 bit image segmentation map
 *
 */
void SegmentMap (uint8_t       *outMap,
                 const uint8_t  center,
                 const size_t   threshold,
                 const uint8_t  value)
{
  const size_t lowerBound = (threshold > center)
                                ? 0 : center - threshold;
  const size_t upperBound = (threshold + center > 256)
                                ? 256 : threshold + center;

  const uint8_t *endMap = outMap + upperBound;
  uint8_t       *ptrMap = outMap + lowerBound;

  while (ptrMap != endMap)
  {
    *ptrMap++ = value;
  }
}


/*
 * Add a circle of pixel values to a 16 bit packed CbCr image segmentation map
 *
 */
void SegmentMapCbCr (uint8_t        *outMap,
                     const uint16_t  center,
                     const size_t    threshold,
                     const uint8_t   value)
{
  /* generate Cb and Cr segmentation maps for bounding box */
  uint8_t cbMap[256], crMap[256];
  const uint8_t centerCb = ((PackedCbCr_t)center).data[0];
  const uint8_t centerCr = ((PackedCbCr_t)center).data[1];
  SegmentMap(cbMap, centerCb, threshold, 1);
  SegmentMap(crMap, centerCr, threshold, 1);

  /* initial point of 0 */
  const size_t  thresholdSq = threshold * threshold;
  if (*cbMap && *crMap && (CbCrSSD(center, 0) <= thresholdSq))
  {
    *outMap = value;
  }

  /* all other points */
  uint8_t iCb, iCr;
  uint16_t i;
  for (i = 1; i != 0; ++i)
  {
    iCb = ((PackedCbCr_t)i).data[0];
    iCr = ((PackedCbCr_t)i).data[1];

    /* check if point is inside bounding box around threshold circle */
    if (cbMap[iCb] && crMap[iCr] && (CbCrSSD(center, i) <= thresholdSq))
    {
      outMap[i] = value;
    }
  }
}


/*
 * Compute image segmentation of an 8 bit image
 *
 */
void SegmentImage (Image8_t       *outImg,
                   const Image8_t *inImg,
                   const uint8_t  *inMap)
{
  const size_t   numPixels = outImg->width * outImg->height;
  uint8_t       *ptrOutImg = outImg->data;
  const uint8_t *ptrInImg  = inImg->data;

  UNROLL_LOOP( numPixels,

      *ptrOutImg++ = inMap[ *ptrInImg++ ];
  )
}


/*
 * Compute image segmentation of a (packed) 16 bit image
 *
 * This can used for other 16 bit packed colorspace representations besides
 * 8/8 CbCr, i.e. 5/6/5 RGB
 *
 */
void SegmentImageW (Image8_t        *outImg,
                    const Image16_t *inImg,
                    const uint8_t   *inMap)
{
  const size_t   numPixels = outImg->width * outImg->height;
  uint8_t       *ptrOutImg = outImg->data;
  const uint16_t *ptrInImg = inImg->data;

  UNROLL_LOOP( numPixels,

      *ptrOutImg++ = inMap[ *ptrInImg++ ];
  )
}


/*
 * Split individual image segments out from a single image after segmentation
 *
 * The pointer to a pointer that is the output is should be a sparsely
 * populated array of pointers. Images need to be allocated only for those
 * values (maximum of 256 for an 8 bit image) that can appear. This is known
 * as the input image has previously been segmented with a segmentation mapping
 * array.
 *
 */
void SplitImageSegmentation (Image8_t      **outImg,
                             const Image8_t *inImg)
{
  const size_t numPixels = inImg->width * inImg->height;

  const uint8_t *ptrIn = inImg->data;

  size_t i;

  for (i = 0; i < numPixels; ++i)
  {
    outImg[ *ptrIn++ ]->data[ i ] = 0x1;  /* marking value is arbitrary */
  }
}

