// Adafruit_GFX font structure

#ifndef _GFXFONT_H_
#define _GFXFONT_H_
#include <cstdint>

/// Font data stored PER GLYPH
typedef struct {
  std::uint16_t bitmapOffset; ///< Pointer into GFXfont->bitmap
  std::uint8_t width;         ///< Bitmap dimensions in pixels
  std::uint8_t height;        ///< Bitmap dimensions in pixels
  std::uint8_t xAdvance;      ///< Distance to advance cursor (x axis)
  std::int8_t xOffset;        ///< X dist from cursor pos to UL corner
  std::int8_t yOffset;        ///< Y dist from cursor pos to UL corner
} GFXglyph;

/// Data stored for FONT AS A WHOLE
typedef struct {
  std::uint8_t *bitmap;  ///< Glyph bitmaps, concatenated
  GFXglyph *glyph;       ///< Glyph array
  std::uint16_t first;   ///< ASCII extents (first char)
  std::uint16_t last;    ///< ASCII extents (last char)
  std::uint8_t yAdvance; ///< Newline distance (y axis)
} GFXfont;

#endif // _GFXFONT_H_