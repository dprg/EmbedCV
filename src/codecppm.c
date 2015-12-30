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
 * PPM
 *
 */

void ReadPPMHead (size_t *width, size_t *height, size_t *components, FILE *s, Buffer_t *sb)
{
  const uint8_t PPM_BEGIN [] = { '2', '5', '5', '\n' };
  uint8_t markBuffer[sizeof(PPM_BEGIN) / sizeof(uint8_t) - 1];

  if (components)
  {
    *components = (GuessImageType(s) == ECV_PPM_GRAY) ? 1 : 3;
  }

  /* only figure out image dimensions if required */
  if (width || height)
  {
    /* 256 bytes should be adequate */
    READBUFFER( headBuffer, 256 )

    SeekMarker(markBuffer,
               sizeof(PPM_BEGIN) / sizeof(uint8_t),
               PPM_BEGIN,
               &headBuffer,
               s,
               sb);

    /* start looking from start of marker */
    const uint8_t *headBuf = headBuffer.head;
    const uint8_t *ptrBuf  = headBuffer.size
                             - sizeof(PPM_BEGIN) / sizeof(uint8_t)
                             - 1;  /* skip past 255\n */

    /* read backwards until first digit character */
    while ( (ptrBuf > headBuf) && ((*ptrBuf < '0') || (*ptrBuf > '9')) )
    {
      ptrBuf--;
    }

    /* read backwards until first non-digit character */
    while ( (ptrBuf > headBuf) && (*ptrBuf >= '0') && (*ptrBuf <= '9') )
    {
      ptrBuf--;
    }

    /* height */
    if (height)
    {
      *height = atoi(ptrBuf + 1);
    }

    /* read backwards until first digit character */
    while ( (ptrBuf > headBuf) && ((*ptrBuf < '0') || (*ptrBuf > '9')) )
    {
      ptrBuf--;
    }

    /* read backwards until first non-digit character */
    while ( (ptrBuf > headBuf) && (*ptrBuf >= '0') && (*ptrBuf <= '9') )
    {
      ptrBuf--;
    }

    /* width */
    if (width)
    {
      *width = atoi(ptrBuf + 1);
    }
  }
  else
  {
    /* just read past the header only */
    SeekMarker(markBuffer,
               sizeof(PPM_BEGIN) / sizeof(uint8_t),
               PPM_BEGIN,
               0,
               s,
               sb);
  }
}


/* 8 bit grayscale */
void ReadPPM8 (Image8_t *img, FILE *s, Buffer_t *sb)
{
  uint8_t *ptrImg = img->data;

  NORMAL_LOOP( img->width * img->height,

      *ptrImg++ = ReadByteFast(s, sb);
  )
}


/* 16 bit packed 565 RGB */
void ReadPPM565 (Image16_t *img, FILE *s, Buffer_t *sb)
{
  uint16_t *ptrImg = img->data;

  NORMAL_LOOP( img->width * img->height,

      *ptrImg++ = (ReadByteFast(s, sb) >> 3) |
                  (((uint16_t)ReadByteFast(s, sb) >> 2) << 5) |
                  (((uint16_t)ReadByteFast(s, sb) >> 3) << 11);
  )
}


/* 24 bit 888 RGB */
void ReadPPM888 (Image8_t *red, Image8_t *green, Image8_t *blue, FILE *s, Buffer_t *sb)
{
  uint8_t *ptrRed   = red->data;
  uint8_t *ptrGreen = green->data;
  uint8_t *ptrBlue  = blue->data;

  NORMAL_LOOP( red->width * red->height,

      *ptrRed++   = ReadByteFast(s, sb);
      *ptrGreen++ = ReadByteFast(s, sb);
      *ptrBlue++  = ReadByteFast(s, sb);
  )
}


/* 24 bit packed 8/16 Y/CbCr */
void ReadPPM816 (Image8_t *luma, Image16_t *chroma, FILE *s, Buffer_t *sb)
{
  uint8_t       *ptrLuma  = luma->data;
  PackedCbCr_t  *ptrCbCr  = (PackedCbCr_t *) chroma->data;

  NORMAL_LOOP( luma->width * luma->height,

      *ptrLuma++       = ReadByteFast(s, sb);
      ptrCbCr->data[0] = ReadByteFast(s, sb);
      ptrCbCr->data[1] = ReadByteFast(s, sb);
      ptrCbCr++;
  )
}


/*
 * Writing PPM
 *
 */


void WritePPMHead (FILE *s, const size_t width, const size_t height, const size_t components)
{
  switch (components)
  {
    case (1):  /* grayscale */
      fprintf(s, "P5\n%u %u\n255\n", width, height);
      break;

    case (3):  /* 24 bit RGB */
      fprintf(s, "P6\n%u %u\n255\n", width, height);
      break;
  }
}


/* 8 bit grayscale */
void WritePPM8 (FILE *s, const Image8_t *img)
{
  const uint8_t *ptrImg = img->data;

  NORMAL_LOOP( img->width * img->height,

      fprintf(s, "%c", *ptrImg++);
  )
}


/* 16 bit packed 565 RGB */
void WritePPM565 (FILE *s, const Image16_t *rgb)
{
  const uint16_t *ptrImg = rgb->data;

  NORMAL_LOOP( rgb->width * rgb->height,

      fprintf(s,
              "%c%c%c",
              (*ptrImg & 0x1f) << 3,
              ((*ptrImg >> 5) & 0x3f) << 2,
              ((*ptrImg >> 11) & 0x1f) << 3);
      ptrImg++;
  )
}


/* 24 bit 888 RGB */
void WritePPM888 (FILE *s, const Image8_t *red, const Image8_t *green, const Image8_t *blue)
{
  const uint8_t *ptrRed   = red->data;
  const uint8_t *ptrGreen = green->data;
  const uint8_t *ptrBlue  = blue->data;

  NORMAL_LOOP( red->width * red->height,

      fprintf(s, "%c%c%c", *ptrRed++, *ptrGreen++, *ptrBlue++);
  )
}


/* 24 bit packed 8/16 Y/CbCr */
void WritePPM816 (FILE *s, const Image8_t *luma, const Image16_t *chroma)
{
  const uint8_t       *ptrLuma  = luma->data;
  const PackedCbCr_t  *ptrCbCr  = (PackedCbCr_t *) chroma->data;

  NORMAL_LOOP( luma->width * luma->height,

      fprintf(s, "%c%c%c", *ptrLuma++, ptrCbCr->data[0], ptrCbCr->data[1]);
      ptrCbCr++;
  )
}

