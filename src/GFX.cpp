#include "GFX.hpp"
#include <cstdlib>
#include <cstdio>

GFX::GFX(std::uint32_t width, std::uint32_t height) : WIDTH(width), HEIGHT(height) {
    this->Imagesize  = ((this->WIDTH % 8 == 0) ? (this->WIDTH / 8 ) : (this->WIDTH / 8 + 1)) * this->HEIGHT;
    this->WIDTHBYTE  = (width % 8 == 0) ? (width / 8) : (width / 8 + 1);
    this->HEIGHTBYTE = height;
    this->BlackImage = new std::uint8_t[this->Imagesize];
    this->RYImage    = new std::uint8_t[this->Imagesize];
    this->ROTATION   = 0;
    this->GFXClear(0xFF);
    std::printf("Imagesize=%d\nWITHBYTE=%d\nHEIGHTBYTE=%d\n", Imagesize, WIDTHBYTE, HEIGHTBYTE);
}

GFX::~GFX() {
    delete[] this->BlackImage;
    delete[] this->RYImage;
}

void GFX::GFXSetRotation(std::uint16_t rotation) {
    this->ROTATION = rotation;
}

void GFX::GFXClear(std::uint8_t* image, std::uint16_t color) {
    std::uint16_t x, y;
    for (y = 0; y < this->HEIGHTBYTE; y++) {
        for (x = 0; x < this->WIDTHBYTE; x++) {
            std::uint32_t addr = x + y * this->WIDTHBYTE;
            image[addr] = color;
        }
    }
}

void GFX::GFXClear(std::uint16_t color) {
    this->GFXClear(this->BlackImage, color);
    this->GFXClear(this->RYImage, color);
}

void GFX::GFXClear(GFXColor color) {
    switch (color)
    {
    case GFXColor::BLACK:
        this->GFXClear(this->BlackImage, GFX_BLACK);
        this->GFXClear(this->RYImage, GFX_WHITE);
        break;
    case GFXColor::RED:
        this->GFXClear(this->BlackImage, GFX_WHITE);
        this->GFXClear(this->RYImage, GFX_RED);
        break;
    case GFXColor::WHITE:
        this->GFXClear(this->BlackImage, GFX_WHITE);
        this->GFXClear(this->RYImage, GFX_WHITE);
        break;
    default:
        break;
    }
}

void GFX::GFXSetPixel(std::uint8_t* image, std::uint16_t x, std::uint16_t y, std::uint16_t color) {
    std::uint16_t xx, yy;
    switch (this->ROTATION)
    {
    case 0:
        xx = x;
        yy = y;
        break;
    case 90:
        xx = this->WIDTH - y - 1;
        yy = x;
        break;
    case 180:
        xx = this->WIDTH - x - 1;
        yy = this->HEIGHT - y - 1;
        break;
    case 270:
        xx = y;
        yy = this->HEIGHT - x - 1;
        break;
    default:
        return;
    }
    if (xx >= this->WIDTH || yy >= this->HEIGHT) {
        //std::printf("SetPixel out of bound\n");
        return;
    }
    std::uint32_t addr = xx / 8 + yy * this->WIDTHBYTE;
    if (color == 0x00) {
        image[addr] = image[addr] & ~(0x80 >> (xx % 8));
    } else {
        image[addr] = image[addr] | (0x80 >> (xx % 8));
    }
}

void GFX::GFXClearRect(std::uint8_t* image, std::uint16_t x_start, std::uint16_t y_start, std::uint16_t x_end, std::uint16_t y_end, std::uint16_t color) {
    std::uint16_t x, y;
    for (y = y_start; y < y_end; y++) {
        for (x = x_start; x < x_end; x++) {
            this->GFXSetPixel(image, x, y, color);
        }
    }
}

void GFX::GFXSetPixel(std::uint16_t x, std::uint16_t y, GFXColor color) {
    switch (color)
    {
    case GFXColor::BLACK:
        this->GFXSetPixel(this->BlackImage, x, y, GFX_BLACK);
        this->GFXSetPixel(this->RYImage, x, y, GFX_WHITE); 
        break;
    case GFXColor::RED:
        this->GFXSetPixel(this->BlackImage, x, y, GFX_WHITE);
        this->GFXSetPixel(this->RYImage, x, y, GFX_RED); 
        break;
    case GFXColor::WHITE:
        this->GFXSetPixel(this->BlackImage, x, y, GFX_WHITE);
        this->GFXSetPixel(this->RYImage, x, y, GFX_WHITE);
        break;
    default:
        break;
    }
}

void GFX::GFXClearRect(std::uint16_t x_start, std::uint16_t y_start, std::uint16_t x_end, std::uint16_t y_end, GFXColor color) {
    switch (color)
    {
    case GFXColor::BLACK:
        this->GFXClearRect(this->BlackImage, x_start, y_start, x_end, y_end, GFX_BLACK);
        this->GFXClearRect(this->RYImage, x_start, y_start, x_end, y_end, GFX_WHITE);
        break;
    case GFXColor::RED:
        this->GFXClearRect(this->BlackImage, x_start, y_start, x_end, y_end, GFX_WHITE);
        this->GFXClearRect(this->RYImage, x_start, y_start, x_end, y_end, GFX_RED); 
        break;
    case GFXColor::WHITE:
        this->GFXClearRect(this->BlackImage, x_start, y_start, x_end, y_end, GFX_WHITE);
        this->GFXClearRect(this->RYImage, x_start, y_start, x_end, y_end, GFX_WHITE);
        break;
    default:
        break;
    }
}

void GFX::GFXDrawPoint(std::uint16_t x, std::uint16_t y, GFXColor color, std::uint8_t size) {
    uint_fast16_t xx, yy;
    for (xx = x; xx < (x + size); xx++) {
        for (yy = y; yy < (y + size); yy++) {
            this->GFXSetPixel(xx, yy, color);
        }
    }
}

void GFX::GFXDrawImage(std::uint16_t x, std::uint16_t y, Image& img) {
    //std::printf("in GFXDrawImage()\n");
    for (std::uint16_t yy = 0; yy < img.height; yy++) {
        for (std::uint16_t xx = 0; xx < img.width; xx++) {
            //std::printf("Setting pix(%d, %d) to %s\n", xx+x, yy+y, img.getPixel(xx,yy)==GFXColor::RED?"RED":img.getPixel(xx,yy)==GFXColor::BLACK?"BLACK":"WHITE");
            this->GFXSetPixel(x+xx, y+yy, img.getPixel(xx, yy));
        }
    }
    //std::printf("exiting GFXDrawImage()\n");
}