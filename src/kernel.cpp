#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>

using namespace rdos;
using namespace rdos::common;
using namespace rdos::drivers;
using namespace rdos::hardwarecommunication;

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
    printf("> GDT created!\n");
    
    printf("> Creating Interrupts...\n");
    InterruptManager interrupts(0x20, &gdt);
    printf("> Interrupts created!\n");

    DriverManager driverManager;
    
    printf("> Initializing Drivers...\n");
    PrintfKeyboardEventHandler kbhandler;

    KeyboardDriver keyboard(&interrupts, &kbhandler);
    driverManager.AddDriver(&keyboard);
    printf("> Keyboard driver created!\n");

    MouseToConsole mouseHandler;
    MouseDriver mouse(&interrupts, &mouseHandler);
    driverManager.AddDriver(&mouse);
    printf("> mouse driver created!\n");

    driverManager.ActivateAll();
    printf("> Drivers activated!\n");

    interrupts.Activate();
    printf("> Clock interrupts activated.\n");

    while(1);
}