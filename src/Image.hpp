/**
 * When an Image is initialized, every pixel is red,
 * since both BlackImage and RYImage are 0 initialized.
**/

#pragma once
#ifndef IMAGEHPP
#define IMAGEHPP
#include <cstdint>
#include <cstdio>

enum class GFXColor {
    WHITE,
    RED,
    BLACK
};

class Image {
    public:
        std::uint8_t* BlackImage;
        std::uint8_t* RYImage;
        std::uint16_t width;
        std::uint16_t height;
        std::uint32_t imageSize;
        std::uint32_t widthByte;
        Image(std::uint16_t w, std::uint16_t h) : width(w), height(h) {
            this->widthByte = (w % 8 == 0) ? (w / 8 ) : (w / 8 + 1);
            this->imageSize =  this->widthByte * h;
            std::printf("w=%d, h=%d, wb=%d\n", w, h, widthByte);
            std::printf("In Image(), about to new images, size=%d\n", imageSize);
            this->BlackImage = new std::uint8_t[this->imageSize]();
            this->RYImage = new std::uint8_t[this->imageSize]();
        }
        ~Image() {
            delete[] this->BlackImage;
            delete[] this->RYImage;
        }
        GFXColor getRYPixel(std::uint16_t x, std::uint16_t y) {
            if (x >= width || y >= height) {
                return GFXColor::WHITE; // if out of range, return White
            }
            std::uint32_t addr, offset;
            addr = y * widthByte + x / 8;
            offset = 7 - x % 8;
            return (this->RYImage[addr] >> offset & 1) ? GFXColor::WHITE : GFXColor::RED;
        }
        GFXColor getBlackPixel(std::uint16_t x, std::uint16_t y) {
            if (x >= width || y >= height) {
                return GFXColor::WHITE; // if out of range, return White
            }
            std::uint32_t addr, offset;
            addr = y * widthByte + x / 8;
            offset = 7 - x % 8;
            return (this->BlackImage[addr] >> offset & 1) ? GFXColor::WHITE : GFXColor::BLACK;
        }
        GFXColor getPixel(std::uint16_t x, std::uint16_t y) {
            GFXColor color1 = this->getBlackPixel(x, y);
            GFXColor color2 = this->getRYPixel(x, y);
            if (color1 == GFXColor::WHITE && color2 == GFXColor::WHITE) {
                return GFXColor::WHITE;
            } else if (color2 == GFXColor::RED) {
                return GFXColor::RED;
            } else {
                return GFXColor::BLACK;
            }
        }
        void setRYPixel(std::uint16_t x, std::uint16_t y, GFXColor color) {
            if (x >= width || y >= height) {
                return;
            }
            std::uint32_t addr;
            addr = y * widthByte + x / 8;
            if (color == GFXColor::RED) {
                this->RYImage[addr] = this->RYImage[addr] & ~(0x80 >> (x % 8));
            } else {
                this->RYImage[addr] = this->RYImage[addr] | (0x80 >> (x % 8));
            }
        }
        void setRYPixel(std::uint32_t bit_addr, GFXColor color) {
            this->setRYPixel(bit_addr % this->width, bit_addr / this->width, color);
        }
        void setBlackPixel(std::uint16_t x, std::uint16_t y, GFXColor color) {
            if (x >= width || y >= height) {
                return;
            }
            std::uint32_t addr;
            addr = y * widthByte + x / 8;
            if (color == GFXColor::BLACK) {
                this->BlackImage[addr] = this->BlackImage[addr] & ~(0x80 >> (x % 8));
            } else {
                this->BlackImage[addr] = this->BlackImage[addr] | (0x80 >> (x % 8));
            }
        }
        void setBlackPixel(std::uint32_t bit_addr, GFXColor color) {
            this->setBlackPixel(bit_addr % this->width, bit_addr / this->width, color);
        }
        void setPixel(std::uint16_t x, std::uint16_t y, GFXColor color) {
            switch (color)
            {
            case GFXColor::WHITE:
                this->setBlackPixel(x, y, GFXColor::WHITE);
                this->setRYPixel(x, y, GFXColor::WHITE);
                break;
            case GFXColor::RED:
                this->setBlackPixel(x, y, GFXColor::WHITE);
                this->setRYPixel(x, y, GFXColor::RED);
                break;
            case GFXColor::BLACK:
                this->setBlackPixel(x, y, GFXColor::BLACK);
                this->setRYPixel(x, y, GFXColor::WHITE);
                break;
            default:
                break;
            }
        }
        void setPixel(std::uint32_t bit_addr, GFXColor color) {
            this->setPixel(bit_addr % this->width, bit_addr / this->width, color);
        }
};
#endif