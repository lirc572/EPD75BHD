#include <EPD75BHD.hpp>
#include <cstdio>

void setup()
{
    EPD75BHD epd(13, 14, 15, 26, 27, 25);
    epd.Init();
    std::printf("Initialized!\n");
    epd.GFXSetCursor(20, 400);
    epd.GFXPrintf("Hello, world~\nHi, world!\n");
    epd.GFXSetColor(GFXColor::RED);
    epd.GFXSetCursor(50, 450);
    epd.GFXPrintf("%%c:(%c), %%d:(%d), %%s:(%s), %%x:(%x), %%%%: (%%)\nHahaha~~~\n", 'C', 5201314, "I <3 U", 85988116);
    epd.Display();
    std::printf("Done~\n");
}

void loop() {}
