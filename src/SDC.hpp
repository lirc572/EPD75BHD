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
        if (!SD.begin(5))
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
            std::printf("SDC Initialized\n");
        }
        return SDC::instance;
    }
    fs::SDFS *fs;
    bool createDir(const char * path) {
        Serial.printf("Creating Dir: %s\n", path);
        return this->fs->mkdir(path);
    }
    bool removeDir(const char * path) {
        Serial.printf("Removing Dir: %s\n", path);
        return this->fs->rmdir(path);
    }
    String readFile(const char * path) {
        File file = this->fs->open(path);
        String res;
        if (!file) {
            return res;
        }
        while (file.available()) {
            res += file.readString();
        }
        file.close();
        return res;
    }
    bool writeFile(const char * path, String s) {
        File file = this->fs->open(path, FILE_WRITE);
        if (!file) {
            return false;
        }
        if (file.print(s)) {
            file.close();
            return true;
        }
        file.close();
        return false;
    }
    bool appendFile(const char * path, String s) {
        File file = this->fs->open(path, FILE_APPEND);
        if (!file) {
            return false;
        }
        if (file.print(s)) {
            file.close();
            return true;
        }
        file.close();
        return false;
    }
};

SDC* SDC::instance = NULL;

#endif