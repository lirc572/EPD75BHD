#pragma once
#ifndef EPDGFXHPP
#define EPDGFXHPP

#include <cstdint>
#include "Image.hpp"
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
        void GFXSetRotation(std::uint16_t rotation);
        void GFXClear(GFXColor color);
        void GFXSetPixel(std::uint16_t x, std::uint16_t y, GFXColor color);
        void GFXClearRect(std::uint16_t x_start, std::uint16_t y_start, std::uint16_t x_end, std::uint16_t y_end, GFXColor color);
        void GFXDrawPoint(std::uint16_t x, std::uint16_t y, GFXColor color, std::uint8_t size);
        void GFXDrawImage(std::uint16_t x, std::uint16_t y, Image& img);
};
#endif