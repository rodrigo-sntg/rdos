#include <common/types.h>
#include <gdt.h>
#include <memorymanagement.h>

#include <hardwarecommunication/interrupts.h>
#include <syscalls.h>
#include <hardwarecommunication/pci.h>

#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <drivers/ata.h>

#include <gui/desktop.h>
#include <gui/window.h>
#include <gui/render.h>

#include <multitasking.h>

#include <drivers/amd_am79c973.h>
#include <net/etherframe.h>
#include <net/arp.h>

using namespace rdos;
using namespace rdos::common;
using namespace rdos::drivers;
using namespace rdos::hardwarecommunication;
using namespace rdos::gui;
using namespace rdos::net;

// #define GRAPHICSMODE

void sysprintf(char* str)
{
    asm("int $0x80" : : "a" (4), "b" (str));
}

void printf(char* str){
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    static uint8_t x = 0, y = 0;

    for(int i = 0; str[i] != '\0'; ++i){

        switch (str[i])
        {
        case '\n':
            y++;
            x = 0;
            break;

        default:
            VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) |  str[i];
            x++;
            break;
        }

        
        if(x >= 80){
            y++;
            x = 0;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) |  ' ';
            x = 0;
            y = 0;
        }
    }
}

void printHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0x0F];
    foo[1] = hex[key & 0x0F];
    printf(foo);
}

void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0x0F];
    foo[1] = hex[key & 0x0F];
    printf(foo);
}

void printfHex16(uint16_t key)
{
    printfHex((key >> 8) & 0xFF);
    printfHex( key & 0xFF);
}
void printfHex32(uint32_t key)
{
    printfHex((key >> 24) & 0xFF);
    printfHex((key >> 16) & 0xFF);
    printfHex((key >> 8) & 0xFF);
    printfHex( key & 0xFF);
}

class PrintfKeyboardEventHandler: public KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        char* foo = " ";
        foo[0] = c;
        printf(foo);
    }
};

class MouseToConsole: public MouseEventHandler
{
    int8_t x, y;
public:
    MouseToConsole()
    {
        
    }

    virtual void OnActivate(){
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }

    void OnMouseMove(int xOffset, int yOffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);

        x += xOffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        y += yOffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
        
    }
// private:
//     /* data */
// public:
//     kernel(/* args */);
//     ~kernel();
};



void taskA()
{
    while(true)
    {

        sysprintf("AA");
    }
}
void taskB()
{
    while(true)
    {

        sysprintf("BB");
    }
}
typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*magicnumber*/){
    printf("Hey you!\n");

    GlobalDescriptorTable gdt;
    // printf("> GDT created!\n");

    uint32_t* memupper = (uint32_t*)(((size_t)multiboot_structure) + 8);
    size_t heap = 10*1024*1024;
    MemoryManager memoryManager(heap, (*memupper)*1024 - heap - 10*1024);

    // printf("heap: 0x");
    printHex((heap >> 24) & 0xFF);
    printHex((heap >> 16) & 0xFF);
    printHex((heap >> 8 ) & 0xFF);
    printHex((heap      ) & 0xFF);

    void* allocated = memoryManager.malloc(1024);
    // printf("\nallocated: 0x");
    printHex(((size_t)allocated >> 24) & 0xFF);
    printHex(((size_t)allocated >> 16) & 0xFF);
    printHex(((size_t)allocated >> 8 ) & 0xFF);
    printHex(((size_t)allocated      ) & 0xFF);
    // printf("\n");

    TaskManager taskManager;

    /*
    Task task1(&gdt, taskA);
    Task task2(&gdt, taskB);
    taskManager.AddTask(&task1);
    taskManager.AddTask(&task2);
    */

    
    // printf("> Creating Interrupts...\n");
    InterruptManager interrupts(0x20, &gdt, &taskManager);
    SyscallHandler syscalls(&interrupts, 0x80);
    // printf("> Interrupts created!\n");

    
    #ifdef GRAPHICSMODE
        Desktop desktop(320,200, 0x00,0x00,0xA8);
    #endif
    
    DriverManager driverManager;
    
    // printf("> Initializing Drivers...\n");

    #ifdef GRAPHICSMODE
        KeyboardDriver keyboard(&interrupts, &desktop);
    #else
        PrintfKeyboardEventHandler kbhandler;
        KeyboardDriver keyboard(&interrupts, &kbhandler);
    #endif
    driverManager.AddDriver(&keyboard);
    // printf("> Keyboard driver created!\n");

    
    #ifdef GRAPHICSMODE
        MouseDriver mouse(&interrupts, &desktop);
    #else
        MouseToConsole mousehandler;
        MouseDriver mouse(&interrupts, &mousehandler);
    #endif
    
    driverManager.AddDriver(&mouse);
    // printf("> mouse driver created!\n");

    
    PeripheralComponentInterconnectController PCIController;
    PCIController.SelectDrivers(&driverManager, &interrupts);


    // VideoGraphicsArray vga;
     #ifdef GRAPHICSMODE
            VideoGraphicsArray vga;
    #endif
    //initialize render frame
    #ifdef GRAPHICSMODE
        printf("> Instantiating Graphics!\n");
        Render rend(320,200);   //arguments do nothing for now. 320,200 is hardcoded into "gui/render.h"
    #endif
    
    driverManager.ActivateAll();
    // printf("> Drivers activated!\n");

    #ifdef GRAPHICSMODE
        vga.SetMode(320,200,8);
        Window win1(&desktop, 10,10,20,20, 0xA8,0x00,0x00);
        desktop.AddChild(&win1);
        Window win2(&desktop, 40,15,30,30, 0x00,0xA8,0x00);
        desktop.AddChild(&win2);
    #endif

    printf("\nS-ATA primary master: ");
    AdvancedTechnologyAttachment ata0m(true, 0x1F0);
    ata0m.Identify();
    
    printf("\nS-ATA primary slave: ");
    AdvancedTechnologyAttachment ata0s(false, 0x1F0);
    ata0s.Identify();

    char* buffer = "fuck";

    ata0s.Write28(0, (uint8_t*)buffer, 4);
    ata0s.Flush();
    
    ata0s.Read28(0, 25);
    
    // printf("\nS-ATA secondary master: ");
    // AdvancedTechnologyAttachment ata1m(true, 0x170);
    // ata1m.Identify();
    
    // printf("\nS-ATA secondary slave: ");
    // AdvancedTechnologyAttachment ata1s(false, 0x170);
    // ata1s.Identify();

    // third: 0x1E8
    // fourth: 0x168

    // IP 10.0.2.15
    uint8_t ip1 = 10, ip2 = 0, ip3 = 2, ip4 = 15;
    uint32_t ip_be = ((uint32_t)ip4 << 24)
                | ((uint32_t)ip3 << 16)
                | ((uint32_t)ip2 << 8)
                | (uint32_t)ip1;

    // IP 10.0.2.2
    uint8_t gip1 = 10, gip2 = 0, gip3 = 2, gip4 = 2;
    uint32_t gip_be = ((uint32_t)gip4 << 24)
                   | ((uint32_t)gip3 << 16)
                   | ((uint32_t)gip2 << 8)
                   | (uint32_t)gip1;

    amd_am79c973* eth0 = (amd_am79c973*)(driverManager.drivers[2]);

    eth0->SetIPAddress(ip_be);

    // eth0->Send((uint8_t*)"Hello Network\n", 13);
    EtherFrameProvider etherframe(eth0);
    AddressResolutionProtocol arp(&etherframe);    
    // etherframe.Send(0xFFFFFFFFFFFF, 0x0608, (uint8_t*)"FOO", 3);


    interrupts.Activate();
    printf("\n\n\n\n\n\n\n\n");
    arp.Resolve(gip_be);
    printf("> Clock interrupts activated.\n");

    #ifdef GRAPHICSMODE
        vga.SetMode(320,200,8);
    #endif

    while(1)
    {
        #ifdef GRAPHICSMODE
            // Desktop desktop(320,200, 0x00,0x00,0xA8);
            //render new frame
            desktop.Draw(&rend);

            //display rendered frame
            rend.display(&vga);
        #endif
    }
}