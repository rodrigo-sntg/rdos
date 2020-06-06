#ifndef __RDOS__DRIVERS__KEYBOARD_H
#define __RDOS__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>
#include <hardwarecommunication/port.h>


namespace rdos
{
    namespace drivers
    {
        class KeyboardEventHandler
        {
        public:
            KeyboardEventHandler();

            virtual void OnKeyDown(char);
            virtual void OnKeyUp(char);
        };

        class KeyboardDriver : public rdos::hardwarecommunication::InterruptHandler, public Driver
        {
            rdos::hardwarecommunication::Port8Bit dataport;
            rdos::hardwarecommunication::Port8Bit commandport;

            KeyboardEventHandler* handler;
        public:
            KeyboardDriver(rdos::hardwarecommunication::InterruptManager* manager, KeyboardEventHandler* handler);
            ~KeyboardDriver();
            virtual rdos::common::uint32_t HandleInterrupt(rdos::common::uint32_t esp);
            virtual void Activate();
        };
    }
}


#endif