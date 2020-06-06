#ifndef __RDOS__HARDWARECOMMUNICATION__INTERRUPTMANAGER_H
#define __RDOS__HARDWARECOMMUNICATION__INTERRUPTMANAGER_H

#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/port.h>

namespace rdos
{
    namespace hardwarecommunication
    {
        class InterruptManager;

        class InterruptHandler
        {
        protected:
            rdos::common::uint8_t interruptNumber;
            InterruptManager* interruptManager;

            InterruptHandler(rdos::common::uint8_t interruptNumber, InterruptManager* interruptManager);
            ~InterruptHandler();
        public:
            virtual rdos::common::uint32_t HandleInterrupt(rdos::common::uint32_t esp);

        };

        class InterruptManager
        {
            friend class InterruptHandler;
            protected:
            
                static InterruptManager* ActiveInterruptManager;
                InterruptHandler* handlers[256];

                struct GateDescriptor
                {
                    rdos::common::uint16_t handlerAddressLowBits;
                    rdos::common::uint16_t gdt_codeSegmentSelector;
                    rdos::common::uint8_t reserved;
                    rdos::common::uint8_t access;
                    rdos::common::uint16_t handlerAddressHighBits;
                } __attribute__((packed));

                static GateDescriptor interruptDescriptorTable[256];

                struct InterruptDescriptorTablePointer
                {
                    rdos::common::uint16_t size;
                    rdos::common::uint32_t base;
                } __attribute__((packed));

                rdos::common::uint16_t hardwareInterruptOffset;
                //static InterruptManager* ActiveInterruptManager;
                static void SetInterruptDescriptorTableEntry(rdos::common::uint8_t interrupt,
                    rdos::common::uint16_t codeSegmentSelectorOffset, void (*handler)(),
                    rdos::common::uint8_t DescriptorPrivilegeLevel, rdos::common::uint8_t DescriptorType);


                static void InterruptIgnore();

                static void HandleInterruptRequest0x00();
                static void HandleInterruptRequest0x01();
                static void HandleInterruptRequest0x02();
                static void HandleInterruptRequest0x03();
                static void HandleInterruptRequest0x04();
                static void HandleInterruptRequest0x05();
                static void HandleInterruptRequest0x06();
                static void HandleInterruptRequest0x07();
                static void HandleInterruptRequest0x08();
                static void HandleInterruptRequest0x09();
                static void HandleInterruptRequest0x0A();
                static void HandleInterruptRequest0x0B();
                static void HandleInterruptRequest0x0C();
                static void HandleInterruptRequest0x0D();
                static void HandleInterruptRequest0x0E();
                static void HandleInterruptRequest0x0F();
                static void HandleInterruptRequest0x31();

                static void HandleException0x00();
                static void HandleException0x01();
                static void HandleException0x02();
                static void HandleException0x03();
                static void HandleException0x04();
                static void HandleException0x05();
                static void HandleException0x06();
                static void HandleException0x07();
                static void HandleException0x08();
                static void HandleException0x09();
                static void HandleException0x0A();
                static void HandleException0x0B();
                static void HandleException0x0C();
                static void HandleException0x0D();
                static void HandleException0x0E();
                static void HandleException0x0F();
                static void HandleException0x10();
                static void HandleException0x11();
                static void HandleException0x12();
                static void HandleException0x13();

                static rdos::common::uint32_t HandleInterrupt(rdos::common::uint8_t interruptNumber, rdos::common::uint32_t esp);
                rdos::common::uint32_t DoHandleInterrupt(rdos::common::uint8_t interruptNumber, rdos::common::uint32_t esp);

                Port8BitSlow picMasterCommandPort;
                Port8BitSlow picMasterDataPort;
                Port8BitSlow picSlaveCommandPort;
                Port8BitSlow picSlaveDataPort;

            public:
                InterruptManager(rdos::common::uint16_t hardwareInterruptOffset, rdos::GlobalDescriptorTable* globalDescriptorTable);
                ~InterruptManager();
                rdos::common::uint16_t HardwareInterruptOffset();
                void Activate();
                void DeActivate();
        };

    }
}

#endif
