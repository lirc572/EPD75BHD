#include <EPD75BHD.hpp>
#include <cstdio>

void setup()
{
    EPD75BHD epd(13, 14, 15, 26, 27, 25);
    epd.Init();
    std::printf("Initialized!\n");
    epd.Clear();
    std::printf("Cleared!\n");
    epd.GFXClear(GFXColor::BLACK);
    epd.GFXClearRect(50, 50, epd.WIDTH-50, epd.HEIGHT-50, GFXColor::RED);
    epd.GFXDrawPoint(epd.WIDTH/2, epd.HEIGHT/2, GFXColor::WHITE, 4);
    epd.GFXDrawPoint(epd.WIDTH/2, epd.HEIGHT/2+10, GFXColor::WHITE, 8);
    epd.GFXDrawPoint(epd.WIDTH-3, epd.HEIGHT/2+10, GFXColor::WHITE, 10);
    epd.Display();
    std::printf("Done~\n");
}

void loop() {}
