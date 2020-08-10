#include "GFX.hpp"
#include <Arduino.h>
#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <pgmspace.h>
#include "Fonts/FreeSerif9pt7b.h"

GFX::GFX(std::uint32_t width, std::uint32_t height) : WIDTH(width), HEIGHT(height)
{
    this->Imagesize = ((this->WIDTH % 8 == 0) ? (this->WIDTH >> 3) : ((this->WIDTH >> 3) + 1)) * this->HEIGHT;
    this->WIDTHBYTE = (width % 8 == 0) ? (width >> 3) : ((width >> 3) + 1);
    this->HEIGHTBYTE = height;
    //this->BlackImage = new std::uint8_t[this->Imagesize];
    //this->RYImage = new std::uint8_t[this->Imagesize];
    this->ROTATION = 0;
    this->fontColor = GFXColor::BLACK;
    this->font = &FreeSerif9pt7b;
    this->cursorX = 0;
    this->cursorY = 0;
    //this->GFXClear(0xFF);
    //std::printf("Imagesize=%d\nWITHBYTE=%d\nHEIGHTBYTE=%d\n", Imagesize, WIDTHBYTE, HEIGHTBYTE);
}

void GFX::InitializeBuffer() {
    //std::printf("Initializing buffer~\n");
    if (this->BlackImage == NULL) {
        std::printf("Free heap: %d\n", ESP.getFreeHeap());
        std::printf("Gg to new BlackImage(58096B)\n");
        this->BlackImage = new std::uint8_t[this->Imagesize];
        std::printf("Newed\n");
        this->GFXClear(this->BlackImage, 0xFF);
        std::printf("Cleared\n");
    }
    if (this->RYImage == NULL) {
        std::printf("Free heap: %d\n", ESP.getFreeHeap());
        std::printf("Gg to new RYImage(58096B)\n");
        //this->RYImage = new std::uint8_t[this->Imagesize];
        this->RYImage = new std::uint8_t[this->Imagesize/4*3]; //wtf?? how can this even work?
        std::printf("Newed\n");
        std::printf("Free heap: %d\n", ESP.getFreeHeap());
        //std::uint8_t* ha = new std::uint8_t[this->Imagesize/4];
        //std::printf("Free heap: %d\n", ESP.getFreeHeap());
        //std::printf("(%d, %d)\n", &this->RYImage[this->Imagesize/4*3], ha);
        this->GFXClear(this->RYImage, 0xFF);
        std::printf("Cleared\n");
    }
    //std::printf("Done initializing buffer~\n");
}

void GFX::DeleteBuffer() {
    if (this->BlackImage != NULL) {
        delete[] this->BlackImage;
        this->BlackImage = NULL;
    }
    if (this->RYImage != NULL) {
        delete[] this->RYImage;
        this->RYImage = NULL;
    }
}

GFX::~GFX()
{
    this->DeleteBuffer();
}

void GFX::GFXSetRotation(std::uint16_t rotation)
{
    this->ROTATION = rotation;
}

void GFX::GFXClear(std::uint8_t *image, std::uint16_t color)
{
    std::uint16_t x, y;
    for (y = 0; y < this->HEIGHTBYTE; y++)
    {
        for (x = 0; x < this->WIDTHBYTE; x++)
        {
            std::uint32_t addr = x + y * this->WIDTHBYTE;
            image[addr] = color;
        }
    }
}

void GFX::GFXClear(std::uint16_t color)
{
    this->GFXClear(this->BlackImage, color);
    this->GFXClear(this->RYImage, color);
}

void GFX::GFXClear(GFXColor color)
{
    this->InitializeBuffer();
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

void GFX::GFXSetPixel(std::uint8_t *image, std::uint16_t x, std::uint16_t y, std::uint16_t color)
{
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
    if (xx >= this->WIDTH || yy >= this->HEIGHT)
    {
        //std::printf("SetPixel out of bound\n");
        return;
    }
    std::uint32_t addr = (xx >> 3) + yy * this->WIDTHBYTE;
    if (color == 0x00)
    {
        image[addr] = image[addr] & ~(0x80 >> (xx % 8));
    }
    else
    {
        image[addr] = image[addr] | (0x80 >> (xx % 8));
    }
}

void GFX::GFXClearRect(std::uint8_t *image, std::uint16_t x_start, std::uint16_t y_start, std::uint16_t x_end, std::uint16_t y_end, std::uint16_t color)
{
    std::uint16_t x, y;
    for (y = y_start; y <= y_end; y++)
    {
        for (x = x_start; x <= x_end; x++)
        {
            this->GFXSetPixel(image, x, y, color);
        }
    }
}

void GFX::GFXSetPixel(std::uint16_t x, std::uint16_t y, GFXColor color)
{
    this->InitializeBuffer();
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

void GFX::GFXClearRect(std::uint16_t x_start, std::uint16_t y_start, std::uint16_t x_end, std::uint16_t y_end, GFXColor color)
{
    this->InitializeBuffer();
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

void GFX::GFXDrawPoint(std::uint16_t x, std::uint16_t y, GFXColor color, std::uint8_t size)
{
    uint_fast16_t xx, yy;
    for (xx = x; xx < (x + size); xx++)
    {
        for (yy = y; yy < (y + size); yy++)
        {
            this->GFXSetPixel(xx, yy, color);
        }
    }
}

void GFX::GFXDrawImage(std::uint16_t x, std::uint16_t y, Image &img)
{
    //std::printf("in GFXDrawImage()\n");
    for (std::uint16_t yy = 0; yy < img.height; yy++)
    {
        for (std::uint16_t xx = 0; xx < img.width; xx++)
        {
            //std::printf("Setting pix(%d, %d) to %s\n", xx+x, yy+y, img.getPixel(xx,yy)==GFXColor::RED?"RED":img.getPixel(xx,yy)==GFXColor::BLACK?"BLACK":"WHITE");
            this->GFXSetPixel(x + xx, y + yy, img.getPixel(xx, yy));
        }
    }
    //std::printf("exiting GFXDrawImage()\n");
}

void GFX::GFXSetFont(const GFXfont *font)
{
    this->font = font;
}

void GFX::GFXSetColor(GFXColor color)
{
    this->fontColor = color;
}

void GFX::GFXSetCursor(std::uint16_t x, std::uint16_t y)
{
    this->cursorX = x;
    this->cursorY = y;
}

void GFX::GFXPutChar(char c)
{
    //std::printf("In GFXPutChar(%c)\n", c);
    if (c >= 0x20 && c <= 0x7E)
    {
        //print(c);
        GFXglyph *glyph = &(this->font->glyph[c - this->font->first]);
        std::uint8_t *bitmap = &(this->font->bitmap[glyph->bitmapOffset]);
        std::uint16_t bitIndex, byte;
        std::uint8_t bitMask;
        std::uint8_t x, y;
        for (x = 0; x < glyph->width; x++)
        {
            for (y = 0; y < glyph->height; y++)
            {
                bitIndex = x + glyph->width * y;
                byte = bitIndex >> 3;
                bitMask = 0x80 >> (bitIndex & 7);
                //std::printf(" x(%d), y(%d), bitIndex(%d), byte(%d), bitMask(%d), pix(%d)\n", x, y, bitIndex, byte, bitMask, (bitmap[byte]&bitMask)?1:0);
                //std::printf("  coord: (%d,%d)\n", this->cursorX + glyph->xOffset + x, this->cursorY + glyph->yOffset + y);
                if (bitmap[byte] & bitMask)
                {
                    this->GFXSetPixel(
                        this->cursorX + glyph->xOffset + x,
                        this->cursorY + glyph->yOffset + y,
                        this->fontColor);
                }
            }
        }
        this->cursorX += this->font->glyph[c - this->font->first].xAdvance;
    }
    else if (c == '\n')
    {
        this->cursorY += this->font->yAdvance;
        this->cursorX = 0;
    }
}

void GFX::GFXPutStr(char *str)
{
    //std::printf("in GFXPutStr(%s)", str);
    for (const char *p = str; *p != '\0'; p++)
    {
        //std::printf("char(%c)\n", *p);
        this->GFXPutChar(*p);
    }
}

void GFX::GFXPrintf(const char *fmt, ...)
{
    const char *p;
    std::va_list argp;
    int i;
    char *s;
    char fmtbuf[256];
    va_start(argp, fmt);
    for (p = fmt; *p != '\0'; p++)
    {
        if (*p != '%')
        {
            this->GFXPutChar(*p);
            continue;
        }
        switch (*++p)
        {
        case 'c':
            i = va_arg(argp, int);
            this->GFXPutChar(i);
            break;
        case 'd':
            i = va_arg(argp, int);
            s = __itoa(i, fmtbuf, 10);
            this->GFXPutStr(s);
            break;
        case 's':
            s = va_arg(argp, char *);
            this->GFXPutStr(s);
            break;
        case 'x':
            i = va_arg(argp, int);
            s = __itoa(i, fmtbuf, 16);
            this->GFXPutStr(s);
            break;
        case '%':
            this->GFXPutChar('%');
            break;
        }
    }
    va_end(argp);
}
