#ifndef __RDOS__GUI__RENDER_H
#define __RDOS__GUI__RENDER_H

#include <drivers/vga.h>
#include <common/graphicscontext.h>

namespace rdos{
    namespace gui{

        class Pixel{
            public:
                rdos::common::uint8_t r;
                rdos::common::uint8_t g;
                rdos::common::uint8_t b;
        };

        class Render : public rdos::drivers::VideoGraphicsArray{
            private:
                Pixel pixels[320][200];

            public:
                Render(
                    common::int32_t w, common::int32_t h
                );

                ~Render();

                void display(common::GraphicsContext* gc);

                void PutPixel(rdos::common::int32_t x, rdos::common::int32_t y, rdos::common::uint8_t r, rdos::common::uint8_t g, rdos::common::uint8_t b);
        };
    }
}


#endif 