#include <EPD75BHD.hpp>
#include <PPM.hpp>
#include <SDC.hpp>
#include <cstdio>

void setup()
{
    EPD75BHD epd(13, 14, 15, 26, 27, 25);
    epd.Init();
    std::printf("Initialized!\n");
    epd.Clear();
    std::printf("Cleared!\n");
    SDC* sd = new SDC();
    std::printf("Reading /test.ppm\n");
    if (!DrawPPM(*(sd->fs), "/test.ppm", epd, 0, 0)) {
        std::printf("Done drawing image!\n");
        epd.Display();
    } else {
        std::printf("Read file failed..\n");
    }
    std::printf("Done~\n");
}

void loop() {}
