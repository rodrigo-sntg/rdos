#ifndef __RDOS__HARDWARECOMMUNICATION__PCI_H
#define __RDOS__HARDWARECOMMUNICATION__PCI_H

#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecommunication/port.h>
#include <hardwarecommunication/interrupts.h>

namespace rdos
{
    namespace hardwarecommunication
    {
        enum BaseAddressRegisterType
        {
            MemoryMapping = 0,
            InputOutput = 1
        };


        class BaseAddressRegister
        {
        public:
            bool prefetchable;
            rdos::common::uint8_t* address;
            rdos::common::uint32_t size;
            BaseAddressRegisterType type;
        };

        class PeripheralComponentInterconnectDeviceDescriptor
        {
        private:
            /* data */
        public:
            rdos::common::uint32_t portBase;
            rdos::common::uint32_t interrupt;

            rdos::common::uint16_t bus;
            rdos::common::uint16_t device;
            rdos::common::uint16_t function;

            rdos::common::uint16_t vendor_id;
            rdos::common::uint16_t device_id;

            rdos::common::uint8_t class_id;
            rdos::common::uint8_t subclass_id;
            rdos::common::uint8_t interface_id;

            rdos::common::uint8_t revision;


            PeripheralComponentInterconnectDeviceDescriptor(/* args */);
            ~PeripheralComponentInterconnectDeviceDescriptor();
        };
        
        
        class PeripheralComponentInterconnectController
        {
            Port32Bit dataPort;
            Port32Bit commandPort;
        private:
            /* data */
        public:
            PeripheralComponentInterconnectController(/* args */);
            ~PeripheralComponentInterconnectController();

            rdos::common::uint32_t Read(rdos::common::uint16_t bus, rdos::common::uint16_t device, rdos::common::uint16_t function, rdos::common::uint32_t registeroffset);
            void Write(rdos::common::uint16_t bus, rdos::common::uint16_t device, rdos::common::uint16_t function, rdos::common::uint32_t registeroffset, rdos::common::uint32_t value);

            bool DeviceHasFunctions(rdos::common::uint16_t bus, rdos::common::uint16_t device);

            void SelectDrivers(rdos::drivers::DriverManager* driverManager, rdos::hardwarecommunication::InterruptManager* interrupts);
            rdos::drivers::Driver* GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, rdos::hardwarecommunication::InterruptManager* interrupts);
            PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(rdos::common::uint16_t bus, rdos::common::uint16_t device, rdos::common::uint16_t function);
            BaseAddressRegister GetBaseAddressRegister(rdos::common::uint16_t bus, rdos::common::uint16_t device, rdos::common::uint16_t function, rdos::common::uint16_t bar);

        };

        
    }
}

#endif
