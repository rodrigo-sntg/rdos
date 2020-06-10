#include <hardwarecommunication/pci.h>

using namespace rdos::common;
using namespace rdos::drivers;
using namespace rdos::hardwarecommunication;

void printf(char*);
void printHex(uint8_t);


PeripheralComponentInterconnectDeviceDescriptor::PeripheralComponentInterconnectDeviceDescriptor()
{

}
PeripheralComponentInterconnectDeviceDescriptor::~PeripheralComponentInterconnectDeviceDescriptor()
{
    
}

PeripheralComponentInterconnectController::PeripheralComponentInterconnectController()
: dataPort(0xCFC),
commandPort(0xCF8)
{
}

PeripheralComponentInterconnectController::~PeripheralComponentInterconnectController()
{
}

uint32_t PeripheralComponentInterconnectController::Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset)
{
    uint32_t id =  0x1 << 31
            | ((bus & 0xFF) << 16)
            | ((device & 0x1F) << 11)
            | ((function & 0x07) << 8)
            | (registeroffset & 0xFC);

    commandPort.Write(id);
    uint32_t result = dataPort.Read();
    return result >> (8 * (registeroffset % 4));
}

void PeripheralComponentInterconnectController::Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value)
{
    uint32_t id =  0x1 << 31
            | ((bus & 0xFF) << 16)
            | ((device & 0x1F) << 11)
            | ((function & 0x07) << 8)
            | (registeroffset & 0xFC);
    commandPort.Write(id);
    dataPort.Write(value);
}

bool PeripheralComponentInterconnectController::DeviceHasFunctions(uint16_t bus, uint16_t device)
{
    // o setimo bit de 0x0E diz se o device possui ou nao funcoes
    return Read(bus, device, 0, 0x0E) & (1 << 7);
}




void PeripheralComponentInterconnectController::SelectDrivers(rdos::drivers::DriverManager* driverManager, rdos::hardwarecommunication::InterruptManager* interrupts)
{
    for(int bus = 0; bus < 8; bus++)
    {
        for(int device = 0; device < 32; device++)
        {
            int numFunctions = DeviceHasFunctions(bus, device) ? 8 : 1;
            for(int function = 0; function < 8; function++)
            {
                PeripheralComponentInterconnectDeviceDescriptor dev = GetDeviceDescriptor(bus, device, function);

                if(dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF)
                    continue;

                for(int barNum = 0; barNum < 6; barNum++)
                {
                    BaseAddressRegister bar = GetBaseAddressRegister(bus,device,function,barNum);
                    if(bar.address && (bar.type == InputOutput))
                        dev.portBase = (uint32_t)bar.address;
                    
                    Driver* driver = GetDriver(dev, interrupts);
                    if(driver != 0 )
                        driverManager->AddDriver(driver);
                }

                printf("PCI BUS - ");
                printHex(bus & 0xFF);

                printf(", DEVICE - ");
                printHex(device & 0xFF);

                printf(", FUNCTION - ");
                printHex(function & 0xFF);

                printf(", VENDOR - ");
                printHex((dev.vendor_id & 0xFF00) >> 8);
                printHex(dev.vendor_id & 0xFF);

                printf(", DEVICE - ");
                printHex((dev.device_id & 0xFF00) >> 8);
                printHex(dev.device_id & 0xFF);
                printf("\n");
            }
        }
    }
}

Driver* PeripheralComponentInterconnectController::GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, InterruptManager* interrupts)
{
    Driver *driver = 0;
    switch (dev.vendor_id)
    {
        case 0x1022: // amd
            switch (dev.device_id)
            {
            case 0x2000: // am79c973
            
                // driver = (amd_am79c973*)MemoryManager::activeMemoryManager->malloc(sizeof(amd_am79c973));
                // if(driver != NULL)
                //     new (driver) amd_am79c973(...);
                
                printf("AMD am79c973 ");
                break;
            }
            break;
        case 0x8086: // intel
            break;
    }

    switch (dev.class_id)
    {
        case 0x03: // graficos
            switch (dev.subclass_id)
            {
                case 0x00:
                    printf("VGA ");
                    break;
            }
            break;
    }
    return driver;
}

PeripheralComponentInterconnectDeviceDescriptor PeripheralComponentInterconnectController::GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function)
{
    PeripheralComponentInterconnectDeviceDescriptor result;

    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = Read(bus,device,function,0x00);
    result.device_id = Read(bus,device,function,0x02);

    result.interface_id = Read(bus,device,function,0x09);
    result.subclass_id = Read(bus,device,function,0x0a);
    result.class_id = Read(bus,device,function,0x0b);

    result.revision = Read(bus,device,function,0x08);
    result.interrupt = Read(bus,device,function,0x3c);

    return result;
}

// BaseAddressRegister PeripheralComponentInterconnectController::GetBaseAddressRegister(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar)
// {
//     BaseAddressRegister result;

//     uint32_t headerType = Read(bus,device,function,0x0E) & 0x7F;
//     int maxBars = 6 - (4*headerType);

//     if(bar >= maxBars)
//         return result;
    
//     uint32_t barValue = Read(bus,device,function,0x10 + 4 * bar);
//     result.type = (barValue & 0x1 ) ? InputOutput : MemoryMapping;

//     if(result.type == MemoryMapping)
//     {
//         switch ((barValue >> 1) & 0x3)
//         {
//             case 0: // 32 bit
//             case 1: // 20 bit
//             case 2: // 64 bit
        
//             default:
//                 break;
//         }
//     }
//     else // InputOutput
//     {
//         result.address = (uint8_t* )(barValue & ~0x3);
//         result.prefetchable = false;
//     }

//     return result;
// }

BaseAddressRegister PeripheralComponentInterconnectController::GetBaseAddressRegister(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar)
{
    BaseAddressRegister result;
    
    
    uint32_t headertype = Read(bus, device, function, 0x0E) & 0x7F;
    int maxBARs = 6 - (4*headertype);
    if(bar >= maxBARs)
        return result;
    
    
    uint32_t bar_value = Read(bus, device, function, 0x10 + 4*bar);
    result.type = (bar_value & 0x1) ? InputOutput : MemoryMapping;
    uint32_t temp;
    
    
    
    if(result.type == MemoryMapping)
    {
        
        switch((bar_value >> 1) & 0x3)
        {
            
            case 0: // 32 Bit Mode
            case 1: // 20 Bit Mode
            case 2: // 64 Bit Mode
                break;
        }
        
    }
    else // InputOutput
    {
        result.address = (uint8_t*)(bar_value & ~0x3);
        result.prefetchable = false;
    }
    
    
    return result;
}

