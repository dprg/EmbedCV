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
#include <string.h>
#include <jpeglib.h>
#include <transupp.h>


#include "embedcv.h"


/*
 * JPEG
 *
 */

/* read a compressed JPEG into a memory buffer until start of image marker */
void BufferJPEG (Buffer_t *outb, FILE *s, Buffer_t *sb)
{
  /* read to the start marker of the next JPEG or end of stream */

  const uint8_t SOI_MARKER [] = { 0xff, 0xd8, 0xff };

  /* if output buffer is null, just seek to the SOI marker without recording */
  if (outb)
  {
    uint8_t *outData = outb->position = outb->head;

    *outData++ = 0xff;
    *outData++ = 0xd8;
    *outData++ = 0xff;

    outb->size = outData;
  }

  uint8_t markBuffer[sizeof(SOI_MARKER) / sizeof(uint8_t) - 1];

  SeekMarker(markBuffer,
             sizeof(SOI_MARKER) / sizeof(uint8_t), SOI_MARKER,
             outb,
             s, sb);
}


/* quickly crop a JPEG image along DCT blocks */
void CropJPEG (FILE *dstStream,  /* output */
               FILE *srcStream,  /* input */
               const size_t width,
               const size_t height,
               const size_t xoffset,
               const size_t yoffset)
{
  struct jpeg_decompress_struct srcinfo;
  struct jpeg_compress_struct   dstinfo;
  struct jpeg_error_mgr         jsrcerr, jdsterr;

  jvirt_barray_ptr *src_coef_arrays;
  jvirt_barray_ptr *dst_coef_arrays;

  srcinfo.err = jpeg_std_error(&jsrcerr);
  jpeg_create_decompress(&srcinfo);

  dstinfo.err = jpeg_std_error(&jdsterr);
  jpeg_create_compress(&dstinfo);

  /* specify lossless crop transformation */
  jpeg_transform_info transformoption;
  transformoption.transform       = JXFORM_NONE;
  transformoption.trim            = FALSE;
  transformoption.perfect         = FALSE;
  transformoption.force_grayscale = FALSE;
  transformoption.crop            = TRUE;
  transformoption.crop_width       = width;
  transformoption.crop_height      = height;
  transformoption.crop_xoffset     = xoffset;
  transformoption.crop_yoffset     = yoffset;
  transformoption.crop_width_set   = JCROP_POS;
  transformoption.crop_height_set  = JCROP_POS;
  transformoption.crop_xoffset_set = JCROP_POS;
  transformoption.crop_yoffset_set = JCROP_POS;

  srcinfo.err->trace_level       = 0;
  jsrcerr.trace_level            = jdsterr.trace_level;
  srcinfo.mem->max_memory_to_use = dstinfo.mem->max_memory_to_use;

  jpeg_stdio_src(&srcinfo, srcStream);
  jcopy_markers_setup(&srcinfo, JCOPYOPT_DEFAULT);
  jpeg_read_header(&srcinfo, TRUE);

  jtransform_request_workspace(&srcinfo, &transformoption);
  src_coef_arrays = jpeg_read_coefficients(&srcinfo);
  jpeg_copy_critical_parameters(&srcinfo, &dstinfo);
  dst_coef_arrays = jtransform_adjust_parameters(&srcinfo,
                                                 &dstinfo,
                                                 src_coef_arrays,
                                                 &transformoption);

  jpeg_stdio_dest(&dstinfo, dstStream);
  jpeg_write_coefficients(&dstinfo, dst_coef_arrays);

  jtransform_execute_transformation(&srcinfo,
                                    &dstinfo,
                                    src_coef_arrays,
                                    &transformoption);

  /* cleanup */
  jpeg_finish_compress(&dstinfo);
  jpeg_destroy_compress(&dstinfo);
  jpeg_finish_decompress(&srcinfo);
  jpeg_destroy_decompress(&srcinfo);
}


/* read JPEG header, return dimensions and number components per pixel */
void ReadJPEGHead (size_t *outwidth,
                   size_t *outheight,
                   size_t *outcomponents,
                   FILE *s,
                   struct jpeg_decompress_struct *cinfo,
                   struct jpeg_error_mgr         *jerr,
                   J_COLOR_SPACE                  force_colorspace,
                   const size_t                   force_scale)
{
  cinfo->err                  = jpeg_std_error(jerr);

  jpeg_create_decompress(cinfo);
  jpeg_stdio_src(cinfo, s);
  jpeg_read_header(cinfo, TRUE);

  cinfo->dither_mode          = JDITHER_NONE;
  cinfo->dct_method           = JDCT_IFAST;
  cinfo->do_fancy_upsampling  = FALSE;
  cinfo->two_pass_quantize    = FALSE;
  cinfo->quantize_colors      = FALSE;

  /* optionally force output colorspace */
  if (force_colorspace != JCS_UNKNOWN)
  {
    cinfo->out_color_space    = force_colorspace;
  }

  /* optionally rescale output dimensions */
  switch (force_scale)
  {
    case (2):
    case (4):
    case (8):
      cinfo->scale_num   = 1;
      cinfo->scale_denom = force_scale;
  }

  jpeg_start_decompress(cinfo);

  *outwidth      = cinfo->output_width;
  *outheight     = cinfo->output_height;
  *outcomponents = cinfo->num_components;
}


/* 8 bit grayscale */
void ReadJPEG8 (Image8_t *img, FILE *s, struct jpeg_decompress_struct *cinfo)
{
  JSAMPARRAY buffer;
  uint8_t *scanlines[1];
  buffer = scanlines;
  buffer[0] = img->data;

  UNROLL_LOOP( cinfo->output_height,

      jpeg_read_scanlines(cinfo, buffer, 1);
      buffer[0] += cinfo->output_width;
  )

  /* cleanup */
  jpeg_finish_decompress(cinfo);
  jpeg_destroy_decompress(cinfo);
}


/* 16 bit packed 565 RGB */
void ReadJPEG565 (Image16_t *img, FILE *s, struct jpeg_decompress_struct *cinfo)
{
  JSAMPARRAY buffer = (cinfo->mem->alloc_sarray) (
                          (j_common_ptr) cinfo,
                          JPOOL_IMAGE,
                          cinfo->output_width * cinfo->output_components,
                          1 );

  /* decompress JPEG and write to output stream */
  uint16_t *outData = img->data;
  uint8_t  *bufptr;

  UNROLL_LOOP( cinfo->output_height,

      jpeg_read_scanlines(cinfo, buffer, 1);
      bufptr = buffer[0];

      UNROLL_LOOP( cinfo->output_width,

          *outData    =  ((uint16_t)(*bufptr++) >> 3);
          *outData   |= (((uint16_t)(*bufptr++) >> 2) << 5);
          *outData++ |= (((uint16_t)(*bufptr++) >> 3) << 11);
      )
  )

  /* cleanup */
  jpeg_finish_decompress(cinfo);
  jpeg_destroy_decompress(cinfo);
}


/* 24 bit 888 RGB or YCbCr (depends on colorspace passed to ReadJPEGHead) */
void ReadJPEG888 (Image8_t *imga, Image8_t *imgb, Image8_t *imgc, FILE *s, struct jpeg_decompress_struct *cinfo)
{
  JSAMPARRAY buffer = (cinfo->mem->alloc_sarray) (
                          (j_common_ptr) cinfo,
                          JPOOL_IMAGE,
                          cinfo->output_width * cinfo->output_components,
                          1 );

  /* decompress JPEG and write to output stream */
  uint8_t *outDataA = imga->data,
          *outDataB = imgb->data,
          *outDataC = imgc->data,
          *bufptr;

  UNROLL_LOOP( cinfo->output_height,

      jpeg_read_scanlines(cinfo, buffer, 1);
      bufptr = buffer[0];

      UNROLL_LOOP( cinfo->output_width,

          *outDataA++ = *bufptr++;
          *outDataB++ = *bufptr++;
          *outDataC++ = *bufptr++;
      )
  )

  /* cleanup */
  jpeg_finish_decompress(cinfo);
  jpeg_destroy_decompress(cinfo);
}


/* 24 bit packed 8/16 Y/CbCr (JCS_YCbCr colorspace passed to ReadJPEGHead) */
void ReadJPEG816 (Image8_t *imga, Image16_t *imgbc, FILE *s, struct jpeg_decompress_struct *cinfo)
{
  JSAMPARRAY buffer = (cinfo->mem->alloc_sarray) (
                          (j_common_ptr) cinfo,
                          JPOOL_IMAGE,
                          cinfo->output_width * cinfo->output_components,
                          1 );

  /* decompress JPEG and write to output stream */
  uint8_t  *outDataA  = imga->data, *bufptr;
  uint16_t *outDataBC = imgbc->data;

  UNROLL_LOOP( cinfo->output_height,

      jpeg_read_scanlines(cinfo, buffer, 1);
      bufptr = buffer[0];

      UNROLL_LOOP( cinfo->output_width,

          *outDataA++   = *bufptr++;
          *outDataBC    = *bufptr++;
          *outDataBC++ |= *bufptr++ << 8;
      )
  )

  /* cleanup */
  jpeg_finish_decompress(cinfo);
  jpeg_destroy_decompress(cinfo);
}


/* write JPEG header */
void WriteJPEGHead (FILE *s,
                    struct jpeg_compress_struct *cinfo,
                    struct jpeg_error_mgr       *jerr,
                    const size_t                 width,
                    const size_t                 height,
                    const size_t                 components,
                    J_COLOR_SPACE                colorspace,
                    J_COLOR_SPACE                force_colorspace)
{
  cinfo->err                  = jpeg_std_error(jerr);

  jpeg_create_compress(cinfo);

  cinfo->in_color_space = JCS_RGB;
  jpeg_set_defaults(cinfo);

  cinfo->err->trace_level     = 0;
  cinfo->dct_method           = JDCT_IFAST;

  /* optionally force output colorspace */
  if (force_colorspace != JCS_UNKNOWN)
  {
    jpeg_set_colorspace(cinfo, force_colorspace);
  }

  cinfo->data_precision   = BITS_IN_JSAMPLE;  /* always 8 bits */
  cinfo->image_width      = (JDIMENSION) width;
  cinfo->image_height     = (JDIMENSION) height;
  cinfo->input_components = components;       /* either 1 or 3 */
  cinfo->in_color_space   = colorspace;       /* mono, RGB, YCbCr */

  jpeg_default_colorspace(cinfo);

  jpeg_stdio_dest(cinfo, s);
  jpeg_start_compress(cinfo, TRUE);
}


/* 8 bit grayscale */
void WriteJPEG8 (FILE *s, struct jpeg_compress_struct *cinfo, const Image8_t *img)
{
  JSAMPARRAY inData;
  uint8_t *scanlines[1];
  inData = scanlines;
  inData[0] = img->data;

  UNROLL_LOOP( cinfo->image_height,

      jpeg_write_scanlines(cinfo, inData, 1);
      inData[0] += cinfo->image_width;
  )

  /* cleanup */
  jpeg_finish_compress(cinfo);
  jpeg_destroy_compress(cinfo);
}


/* 16 bit packed 565 RGB */
void WriteJPEG565 (FILE *s, struct jpeg_compress_struct *cinfo, const Image16_t *img)
{
  JSAMPARRAY inData;
  uint8_t *scanlines[1];
  inData = scanlines;

  uint8_t *rowData = malloc(cinfo->image_width * 3);
  uint8_t *ptrData;
  inData[0] = rowData;

  const uint16_t *ptrImg = img->data;
  uint16_t        tmp;

  UNROLL_LOOP( cinfo->image_height,

      ptrData = rowData;

      /* convert 565 RGB to 888 RGB in row buffer */
      UNROLL_LOOP( cinfo->image_width,

          tmp = *ptrImg++;
          *ptrData++ = tmp & 0x1f;
          *ptrData++ = (tmp >> 5) & 0x3f;
          *ptrData++ = (tmp >> 11);
      )

      jpeg_write_scanlines(cinfo, inData, 1);
  )

  free(rowData);

  /* cleanup */
  jpeg_finish_compress(cinfo);
  jpeg_destroy_compress(cinfo);
}


/* 24 bit 888 */
void WriteJPEG888 (FILE *s, struct jpeg_compress_struct *cinfo, const Image8_t *imga, const Image8_t *imgb, const Image8_t *imgc)
{
  JSAMPARRAY inData;
  uint8_t *scanlines[1];
  inData = scanlines;

  uint8_t *rowData = malloc(cinfo->image_width * 3);
  uint8_t *ptrData;
  inData[0] = rowData;

  const uint8_t *ptrA = imga->data;
  const uint8_t *ptrB = imgb->data;
  const uint8_t *ptrC = imgc->data;

  UNROLL_LOOP( cinfo->image_height,

      ptrData = rowData;

      /* gather the three images into the row buffer */
      UNROLL_LOOP( cinfo->image_width,

          *ptrData++ = *ptrA++;
          *ptrData++ = *ptrB++;
          *ptrData++ = *ptrC++;
      )

      jpeg_write_scanlines(cinfo, inData, 1);
  )

  free(rowData);

  /* cleanup */
  jpeg_finish_compress(cinfo);
  jpeg_destroy_compress(cinfo);
}


/* 24 bit packed 8/16 */
void WriteJPEG816 (FILE *s, struct jpeg_compress_struct *cinfo, const Image8_t *imga, const Image16_t *imgbc)
{
  JSAMPARRAY inData;
  uint8_t *scanlines[1];
  inData = scanlines;

  uint8_t *rowData = malloc(cinfo->image_width * 3);
  uint8_t *ptrData;
  inData[0] = rowData;

  const uint8_t  *ptrA  = imga->data;
  const uint16_t *ptrBC = imgbc->data;
  uint16_t        tmp;

  UNROLL_LOOP( cinfo->image_height,

      ptrData = rowData;

      /* gather the images into the row buffer */
      UNROLL_LOOP( cinfo->image_width,

          *ptrData++ = *ptrA++;
          tmp        = *ptrBC++;
          *ptrData++ = tmp & 0xff;
          *ptrData++ = tmp >> 8;
      )

      jpeg_write_scanlines(cinfo, inData, 1);
  )

  free(rowData);

  /* cleanup */
  jpeg_finish_compress(cinfo);
  jpeg_destroy_compress(cinfo);
}

