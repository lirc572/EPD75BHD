/*
 * Connect the SD card to the following pins:
 *
 * SD Card | ESP32
 *    D2       -
 *    D3       SS
 *    CMD      MOSI
 *    VSS      GND
 *    VDD      3.3V
 *    CLK      SCK
 *    VSS      GND
 *    D0       MISO
 *    D1       -
 */

#pragma once
#ifndef SDCHPP
#define SDCHPP
#include <cstdio>
#include <cstdint>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

// Singleton (not thread safe)
class SDC
{
private:
    static SDC* instance;
    class Deletor
    {
    public:
        ~Deletor() {
            if (SDC::instance != NULL) {
                delete SDC::instance;
            }
        }
    };
    static Deletor deletor;
    SDC()
    {
        if (!SD.begin())
        {
            std::printf("Card Mount Failed\n");
            return;
        }
        this->fs = &SD;
    }
public:
    static SDC* getInstance() {
        if (SDC::instance == NULL) {
            SDC::instance = new SDC();
        }
        return SDC::instance;
    }
    fs::SDFS *fs;
};

SDC* SDC::instance = NULL;

#endif