#pragma once
#ifndef EPD75BHDHPP
#define EPD75BHDHPP

#define DEBUG 1

#include <cstdint>
#include "GFX.hpp"

/**
 * Note: If both Black and RY bits of a pixel are set (=0),
 * it will be red/yellow and not black!
 **/
class EPD75BHD: public GFX {
    private:
        //static std::uint32_t WIDTH;
        //static std::uint32_t HEIGHT;
        std::uint8_t SCK_PIN;
        std::uint8_t MOSI_PIN;
        std::uint8_t CS_PIN;
        std::uint8_t RST_PIN;
        std::uint8_t DC_PIN;
        std::uint8_t BUSY_PIN;
        void SPI_WriteByte(std::uint8_t data);
        void SendCommand(std::uint8_t Reg);
        void SendData(std::uint8_t Data);
    public:
        EPD75BHD(
            std::uint8_t sck_pin,
            std::uint8_t mosi_pin,
            std::uint8_t cs_pin,
            std::uint8_t rst_pin,
            std::uint8_t dc_pin,
            std::uint8_t busy_pin
        );
        ~EPD75BHD();
        void WaitUntilNotBusy();
        void HW_Reset();
        void SW_Reset();
        void Init();
        void ClearRed();
        void ClearBlack();
        void Clear();
        void Display(const std::uint8_t *blackimage, const std::uint8_t *ryimage);
        void Display();
        void DeepSleep(void);
};

#endif
