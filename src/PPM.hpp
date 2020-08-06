#pragma once
#ifndef PPMHPP
#define PPMHPP

#include <FS.h>
#include <cstdint>
#include <cstdio>
#include "Image.hpp"
#include "SDC.hpp"
/*
Todo:
- var for buffer addr
- bitwise operation to set/clear correct bit
*/

String ReadLine(fs::File &file)
{
    String line("");
    char c;
    while (file.available() && (c = char(file.read())) != '\n')
    {
        if (c > 31 && c < 127)
        {
            line.concat(c);
        }
    }
    return line;
}

/**
 * Read a Plain PPM file and convert to
 * a black image array and a red image array
 * If picture is too big, the ESP32's heap may be used up and crash
 * Return value:
 *   0: Success
 *   1: Failed to open file
 *   2. File type not supported (not P3)
 *   3. File type not supported (Maxval not 1)
 **/
std::uint8_t PPM2IMG(fs::FS &fs, const char *path, Image *&img)
{
    fs::File file = fs.open(path);
    if (!file)
    {
        std::printf("Error: Failed to open file\n");
        return 1;
    }
    std::uint32_t line_number = 0;
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    std::uint32_t area = 0;
    std::uint8_t r = 0, g = 0, b = 0;
    std::uint32_t bit_addr = 0;
    while (file.available())
    {
        String line = ReadLine(file);
        // std::printf("Line: %s\n", line.c_str());
        if (line[0] == '#')
        {
            std::printf("a comment\n");
            continue; // skip this line
        }
        line_number++;
        if (line_number == 1) // "P3"
        {
            //std::printf("Line1\n");
            if (line != "P3")
            {
                std::printf("Error: File type not supported (not P3)\n");
                std::printf("line = '%s'\n", line.c_str());
                return 2;
            }
            std::printf("Image format: P3\n");
        }
        else if (line_number == 2) // "<WIDTH> <HEIGHT>"
        {
            // std::printf("Line2\n");
            std::uint8_t which = 0;
            for (std::uint8_t i = 0; i < line.length(); i++)
            {
                if (!which)
                {
                    if (line[i] >= '0' && line[i] <= '9')
                    {
                        width = width * 10 + line[i] - '0';
                    }
                    else
                    {
                        which = 1;
                    }
                }
                else
                {
                    if (line[i] >= '0' && line[i] <= '9')
                    {
                        height = height * 10 + line[i] - '0';
                    }
                }
            }
            std::printf("Image width:%d, height:%d\n", width, height);
            area = width * height;
            img = new Image(width, height);
        }
        else if (line_number == 3) // "1"
        {
            //std::printf("Line3\n");
            if (line != "1")
            {
                std::printf("Error: File type not supported (Maxval not 1)\n");
                std::printf("line = '%s'\n", line.c_str());
                delete img;
                return 3;
            }
            std::printf("Image maxval: 1\n");
        }
        else // "r g b"
        {
            if (bit_addr >= area)
            {
                std::printf("Read enough pixels!\n");
                return 0; // done reading all pixels, ignore the rest of the file
            }
            //std::printf("Line%d\n", line_number);
            r = line[0] - '0';
            g = line[2] - '0';
            b = line[4] - '0';
            //std::printf("color: (%d,%d,%d)\n", r, g, b);
            if (r && g && b)
            { // White
                //std::printf("setPix(%d, WHITE)\n", bit_addr);
                img->setPixel(bit_addr++, GFXColor::WHITE);
            }
            else if (r && !g && !b)
            { // Red
                //std::printf("setPix(%d, RED)\n", bit_addr);
                img->setPixel(bit_addr++, GFXColor::RED);
            }
            else if (!r && !g && !b)
            { // Black
                //std::printf("setPix(%d, BLACK)\n", bit_addr);
                img->setPixel(bit_addr++, GFXColor::BLACK);
            }
        }
    }
    //std::printf("file.available() == false:<\n");
    return 0;
}

/**
 * Read a Plain PPM file and convert to
 * a black image array and a red image array
 * If picture is too big, the ESP32's heap may be used up and crash
 * Return value:
 *   0: Success
 *   1: Failed to open file
 *   2. File type not supported (not P3)
 *   3. File type not supported (Maxval not 1)
 **/
std::uint8_t DrawPPM(fs::FS &fs, const char *path, GFX &gfx, std::uint32_t x, std::uint32_t y)
{
    fs::File file = fs.open(path);
    if (!file)
    {
        std::printf("Error: Failed to open file\n");
        return 1;
    }
    std::uint32_t line_number = 0;
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    std::uint32_t area = 0;
    std::uint8_t r = 0, g = 0, b = 0;
    std::uint32_t bit_addr = 0;
    std::uint32_t x_addr = 0, y_addr = 0;
    while (file.available())
    {
        String line = ReadLine(file);
        // std::printf("Line: %s\n", line.c_str());
        if (line[0] == '#')
        {
            std::printf("a comment\n");
            continue; // skip this line
        }
        line_number++;
        if (line_number == 1) // "P3"
        {
            //std::printf("Line1\n");
            if (line != "P3")
            {
                std::printf("Error: File type not supported (not P3)\n");
                std::printf("line = '%s'\n", line.c_str());
                return 2;
            }
            std::printf("Image format: P3\n");
        }
        else if (line_number == 2) // "<WIDTH> <HEIGHT>"
        {
            // std::printf("Line2\n");
            std::uint8_t which = 0;
            for (std::uint8_t i = 0; i < line.length(); i++)
            {
                if (!which)
                {
                    if (line[i] >= '0' && line[i] <= '9')
                    {
                        width = width * 10 + line[i] - '0';
                    }
                    else
                    {
                        which = 1;
                    }
                }
                else
                {
                    if (line[i] >= '0' && line[i] <= '9')
                    {
                        height = height * 10 + line[i] - '0';
                    }
                }
            }
            std::printf("Image width:%d, height:%d\n", width, height);
            area = width * height;
        }
        else if (line_number == 3) // "1"
        {
            //std::printf("Line3\n");
            if (line != "1")
            {
                std::printf("Error: File type not supported (Maxval not 1)\n");
                std::printf("line = '%s'\n", line.c_str());
                return 3;
            }
            std::printf("Image maxval: 1\n");
        }
        else // "r g b"
        {
            if (bit_addr >= area)
            {
                std::printf("Read enough pixels!\n");
                return 0; // done reading all pixels, ignore the rest of the file
            }
            //std::printf("Line%d\n", line_number);
            r = line[0] - '0';
            g = line[2] - '0';
            b = line[4] - '0';
            x_addr = bit_addr % width;
            y_addr = bit_addr / width;
            //std::printf("color: (%d,%d,%d)\n", r, g, b);
            if (r && g && b)
            { // White
                //std::printf("setPix(%d,%d, WHITE)\n", x+x_addr, y+y_addr);
                gfx.GFXSetPixel(x + x_addr, y + y_addr, GFXColor::WHITE);
            }
            else if (r && !g && !b)
            { // Red
                //std::printf("setPix(%d,%d, RED)\n", x+x_addr, y+y_addr);
                gfx.GFXSetPixel(x + x_addr, y + y_addr, GFXColor::RED);
            }
            else if (!r && !g && !b)
            { // Black
                //std::printf("setPix(%d,%d, BLACK)\n", x+x_addr, y+y_addr);
                gfx.GFXSetPixel(x + x_addr, y + y_addr, GFXColor::BLACK);
            }
            bit_addr++;
        }
    }
    //std::printf("file.available() == false:<\n");
    return 0;
}

#endif