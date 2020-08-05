#include <Arduino.h>
#include <cstdio>
#include "EPD75BHD.hpp"

/*
std::uint32_t EPD75BHD::WIDTH = 880;
std::uint32_t EPD75BHD::HEIGHT = 528;
*/

/**
 * Write one byte through SPI
 **/
void EPD75BHD::SPI_WriteByte(std::uint8_t data)
{
    //SPI.beginTransaction(spi_settings);
    digitalWrite(this->CS_PIN, LOW);

    for (int i = 0; i < 8; i++)
    {
        if ((data & 0x80) == 0)
            digitalWrite(this->MOSI_PIN, LOW);
        else
            digitalWrite(this->MOSI_PIN, HIGH);

        data <<= 1;
        digitalWrite(this->SCK_PIN, HIGH);
        digitalWrite(this->SCK_PIN, LOW);
    }

    //SPI.transfer(data);
    digitalWrite(this->CS_PIN, HIGH);
    //SPI.endTransaction();
}

/**
 * Send one command through SPI
 **/
void EPD75BHD::SendCommand(std::uint8_t Reg)
{
    digitalWrite(this->DC_PIN, LOW);
    digitalWrite(this->CS_PIN, LOW);
    this->SPI_WriteByte(Reg);
    digitalWrite(this->CS_PIN, HIGH);
}

/**
 * Send one byte of data through SPI
 **/
void EPD75BHD::SendData(std::uint8_t Data)
{
    digitalWrite(this->DC_PIN, HIGH);
    digitalWrite(this->CS_PIN, LOW);
    this->SPI_WriteByte(Data);
    digitalWrite(this->CS_PIN, HIGH);
}

/**
 * Construct a EPD75BHD object, and configure respective GPIO pins
 **/
EPD75BHD::EPD75BHD(
    std::uint8_t sck_pin,
    std::uint8_t mosi_pin,
    std::uint8_t cs_pin,
    std::uint8_t rst_pin,
    std::uint8_t dc_pin,
    std::uint8_t busy_pin) : GFX(880, 528), SCK_PIN(sck_pin), MOSI_PIN(mosi_pin), CS_PIN(cs_pin), RST_PIN(rst_pin), DC_PIN(dc_pin), BUSY_PIN(busy_pin)
{
    pinMode(this->BUSY_PIN, INPUT);
    pinMode(this->RST_PIN, OUTPUT);
    pinMode(this->DC_PIN, OUTPUT);
    pinMode(this->SCK_PIN, OUTPUT);
    pinMode(this->MOSI_PIN, OUTPUT);
    pinMode(this->CS_PIN, OUTPUT);
    digitalWrite(this->CS_PIN, HIGH);
    digitalWrite(this->SCK_PIN, LOW);

    //std::uint16_t Imagesize = ((this->WIDTH % 8 == 0) ? (this->WIDTH >> 3 ) : ((this->WIDTH >> 3) + 1)) * this->HEIGHT;
    //this->BlackImage       = (std::uint8_t*) malloc(Imagesize);
    //this->RYImage          = (std::uint8_t*) malloc(Imagesize);

#ifdef DEBUG
    Serial.begin(115200);
#endif
    // spi
    // SPI.setDataMode(SPI_MODE0);
    // SPI.setBitOrder(MSBFIRST);
    // SPI.setClockDivider(SPI_CLOCK_DIV4);
    // SPI.begin();
}

EPD75BHD::~EPD75BHD()
{
    //free(this->BlackImage);
    //free(this->RYImage);
}

/**
 * Block until the EPD is not busy
 **/
void EPD75BHD::WaitUntilNotBusy()
{
    std::uint8_t busy = digitalRead(this->BUSY_PIN);
#ifdef DEBUG
    std::printf("e-Paper busy\n");
#endif
    do
    {
        busy = digitalRead(this->BUSY_PIN);
    } while (busy);
#ifdef DEBUG
    std::printf("e-Paper busy released\n");
#endif
    delay(200);
}

/**
 * Hardware reset
 **/
void EPD75BHD::HW_Reset()
{
    digitalWrite(this->RST_PIN, LOW);
    delay(2);
    digitalWrite(this->RST_PIN, HIGH);
    delay(200);
}

/**
 * Software reset through a reset command
 **/
void EPD75BHD::SW_Reset()
{
    this->SendCommand(0x12);
}

/**
 * Initialize EPD
 **/
void EPD75BHD::Init()
{
    this->HW_Reset();
    this->SW_Reset();
    this->WaitUntilNotBusy();

    this->SendCommand(0x46); // Auto write RED RAM
    this->SendData(0xF7);
    this->WaitUntilNotBusy();

    this->SendCommand(0x47); // Auto write B/W RAM
    this->SendData(0xF7);
    this->WaitUntilNotBusy();

    this->SendCommand(0x0C); // Soft start setting (not found in datasheet)
    this->SendData(0xAE);
    this->SendData(0xC7);
    this->SendData(0xC3);
    this->SendData(0xC0);
    this->SendData(0x40);

    this->SendCommand(0x01); // Driver output control
    this->SendData(0xAF);    // Set MUX as 527?
    this->SendData(0x02);
    this->SendData(0x01);

    this->SendCommand(0x11); // Data entry mode
    this->SendData(0x01);

    this->SendCommand(0x44);
    this->SendData(0x00); // RAM x address start at 0
    this->SendData(0x00);
    this->SendData(0x6F); // RAM x address end at 36Fh -> 879
    this->SendData(0x03);
    this->SendCommand(0x45);
    this->SendData(0xAF); // RAM y address start at 20Fh;
    this->SendData(0x02);
    this->SendData(0x00); // RAM y address end at 00h;
    this->SendData(0x00);

    this->SendCommand(0x3C); // VBD
    this->SendData(0x01);    // LUT1, for white

    this->SendCommand(0x18);
    this->SendData(0X80);
    this->SendCommand(0x22);
    this->SendData(0XB1); //Load Temperature and waveform setting.
    this->SendCommand(0x20);
    this->WaitUntilNotBusy(); //waiting for the electronic paper IC to release the idle signal

    this->SendCommand(0x4E);
    this->SendData(0x00);
    this->SendData(0x00);
    this->SendCommand(0x4F);
    this->SendData(0xAF);
    this->SendData(0x02);
}

void EPD75BHD::ClearRed(void)
{
    std::uint32_t i, j, width, height;
    width = (this->WIDTH % 8 == 0) ? (this->WIDTH >> 3) : ((this->WIDTH >> 3) + 1);
    height = this->HEIGHT;
    this->WaitUntilNotBusy();
    this->SendCommand(0x4F);
    this->SendData(0xAf);
    this->SendData(0x02);
    this->SendCommand(0x26); //RED
    for (j = 0; j < height; j++)
    {
        for (i = 0; i < width; i++)
        {
            this->SendData(0X00);
        }
    }
    this->SendCommand(0x22);
    this->SendData(0xC7);
    this->SendCommand(0x20);
    delay(200);
    this->WaitUntilNotBusy();
    printf("clear EPD\n");
}

void EPD75BHD::ClearBlack(void)
{
    std::uint32_t i, j, width, height;
    width = (this->WIDTH % 8 == 0) ? (this->WIDTH >> 3) : ((this->WIDTH >> 3) + 1);
    height = this->HEIGHT;
    //this->WaitUntilNotBusy();
    this->SendCommand(0x4F);
    this->SendData(0xAf);
    this->SendData(0x02);
    this->SendCommand(0x24); //BLACK
    for (j = 0; j < height; j++)
    {
        for (i = 0; i < width; i++)
        {
            this->SendData(0XFF);
        }
    }
    this->WaitUntilNotBusy();
}

/**
 * Clear screen
 * Takes 20 secondes!
 **/
void EPD75BHD::Clear()
{
    std::uint32_t i, j, width, height;
    width = (this->WIDTH % 8 == 0) ? (this->WIDTH >> 3) : ((this->WIDTH >> 3) + 1);
    height = this->HEIGHT;
    //this->WaitUntilNotBusy();
    this->SendCommand(0x4F);
    this->SendData(0xAf);
    this->SendData(0x02);
    this->SendCommand(0x24); //BLACK
    for (j = 0; j < height; j++)
    {
        for (i = 0; i < width; i++)
        {
            this->SendData(0XFF);
        }
    }
    //this->WaitUntilNotBusy();
    delay(400);
    this->SendCommand(0x4F);
    this->SendData(0xAf);
    this->SendData(0x02);
    this->SendCommand(0x26); //RED
    for (j = 0; j < height; j++)
    {
        for (i = 0; i < width; i++)
        {
            this->SendData(0X00);
        }
    }
    this->SendCommand(0x22);
    this->SendData(0xC7);
    this->SendCommand(0x20);
    delay(200);
    this->WaitUntilNotBusy();
#ifdef DEBUG
    std::printf("clear EPD\n");
#endif
}

/**
 * Sends the image buffer in RAM to e-Paper and displays
 **/
void EPD75BHD::Display(const std::uint8_t *blackimage, const std::uint8_t *ryimage)
{
    std::uint32_t i, j, width, height;
    width = (this->WIDTH % 8 == 0) ? (this->WIDTH >> 3) : ((this->WIDTH >> 3) + 1);
    height = this->HEIGHT;

    this->SendCommand(0x4F);
    this->SendData(0xAf);
    this->SendData(0x02);
    this->SendCommand(0x24); //BLACK
    for (j = 0; j < height; j++)
    {
        for (i = 0; i < width; i++)
        {
            this->SendData(blackimage[i + j * width]);
        }
    }
    // this->WaitUntilNotBusy();
    // this->SendCommand(0x4F);
    // this->SendData(0xAf);
    // this->SendData(0x02);
    this->SendCommand(0x26); //RED
    for (j = 0; j < height; j++)
    {
        for (i = 0; i < width; i++)
        {
            this->SendData(~ryimage[i + j * width]);
        }
    }
    this->SendCommand(0x22);
    this->SendData(0xC7);
    this->SendCommand(0x20);
    delay(100);
    this->WaitUntilNotBusy();
    printf("display\n");
}

/**
 * Takes 20 seconds!
 **/
void EPD75BHD::Display()
{
    this->Display(this->BlackImage, this->RYImage);
}

/**
 * EPD Deep Sleep Mode
 **/
void EPD75BHD::DeepSleep(void)
{
    this->SendCommand(0x10); //deep sleep
    this->SendData(0x01);
}