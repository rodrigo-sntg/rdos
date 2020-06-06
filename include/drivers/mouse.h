#ifndef __RDOS__DRIVERS__MOUSE_H
#define __RDOS__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>
#include <hardwarecommunication/interrupts.h>

namespace rdos
{
    namespace drivers
    {
        class MouseEventHandler
        {
        public:
            MouseEventHandler();

            virtual void OnActivate();
            virtual void OnMouseDown(rdos::common::uint8_t button);
            virtual void OnMouseUp(rdos::common::uint8_t button);
            virtual void OnMouseMove(int x, int y);
        };

        class MouseDriver : public rdos::hardwarecommunication::InterruptHandler, public Driver
        {
            rdos::hardwarecommunication::Port8Bit dataport;
            rdos::hardwarecommunication::Port8Bit commandport;
            rdos::common::uint8_t buffer[3];
            rdos::common::uint8_t offset;
            rdos::common::uint8_t buttons;

            MouseEventHandler* handler;
        public:
            MouseDriver(rdos::hardwarecommunication::InterruptManager* manager, MouseEventHandler* handler);
            ~MouseDriver();
            virtual rdos::common::uint32_t HandleInterrupt(rdos::common::uint32_t esp);
            virtual void Activate();
        };

    }
}

#endif