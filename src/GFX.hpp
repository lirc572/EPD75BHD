#pragma once
#ifndef EPDGFXHPP
#define EPDGFXHPP

#include <cstdint>
#include "Image.hpp"
#include "adafruitgfxfont.h"

/**
 * image color
**/
#define GFX_WHITE          0xFF
#define GFX_BLACK          0x00
#define GFX_RED            GFX_BLACK

class GFX {
    private:
        std::uint32_t WIDTHBYTE;
        std::uint32_t HEIGHTBYTE;
        std::uint16_t Imagesize;
        std::uint16_t ROTATION;
        const GFXfont* font;
        std::uint16_t cursorX;
        std::uint16_t cursorY;
        GFXColor fontColor;
        void GFXPutChar(char c);
        void GFXPutStr(char* str);
        void GFXClear(std::uint8_t* image, std::uint16_t color);
        void GFXClear(std::uint16_t color);
        void GFXSetPixel(std::uint8_t* image, std::uint16_t x, std::uint16_t y, std::uint16_t color);
        void GFXClearRect(std::uint8_t* image, std::uint16_t x_start, std::uint16_t y_start, std::uint16_t x_end, std::uint16_t y_end, std::uint16_t color);
    public:
        std::uint32_t WIDTH;
        std::uint32_t HEIGHT;
        std::uint8_t* BlackImage;
        std::uint8_t* RYImage;
        GFX(std::uint32_t width, std::uint32_t height);
        ~GFX();
        void GFXSetFont(const GFXfont* font);
        void GFXSetColor(GFXColor color);
        void GFXSetCursor(std::uint16_t x, std::uint16_t y);
        void GFXPrintf(const char* fmt, ...);
        void GFXSetRotation(std::uint16_t rotation);
        void GFXClear(GFXColor color);
        void GFXSetPixel(std::uint16_t x, std::uint16_t y, GFXColor color);
        void GFXClearRect(std::uint16_t x_start, std::uint16_t y_start, std::uint16_t x_end, std::uint16_t y_end, GFXColor color);
        void GFXDrawPoint(std::uint16_t x, std::uint16_t y, GFXColor color, std::uint8_t size);
        void GFXDrawImage(std::uint16_t x, std::uint16_t y, Image& img);
};
#endif