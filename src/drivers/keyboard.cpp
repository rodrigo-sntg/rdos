#include <drivers/keyboard.h>

using namespace rdos::common;
using namespace rdos::drivers;
using namespace rdos::hardwarecommunication;



KeyboardEventHandler::KeyboardEventHandler()
{

}
void KeyboardEventHandler::OnKeyDown(char)
{

}
void KeyboardEventHandler::OnKeyUp(char)
{

}

KeyboardDriver::KeyboardDriver(InterruptManager* manager, KeyboardEventHandler *handler)
: InterruptHandler(manager, 0x21),
dataport(0x60),
commandport(0x64)
{
    this->handler = handler;
}
KeyboardDriver::~KeyboardDriver()
{

}

void printf(char*);
void printHex(uint8_t);

void KeyboardDriver::Activate(){
    while (commandport.Read() & 0x1)
        dataport.Read();
    commandport.Write(0xAE); // ativando os interrupts
    commandport.Write(0x20); // pega o estado atual
    uint8_t status = (dataport.Read() | 1) & ~0x10;
    commandport.Write(0x60); // muda o estado
    dataport.Write(status);

    dataport.Write(0xF4); // ativando o keyboard
    

}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t key = dataport.Read();

    if(handler == 0)
        return esp;

    static bool Shift = false;


    switch (key)
    {
        
        case 0x0F: if(Shift) handler->OnKeyDown('\t'); else handler->OnKeyDown('\t'); break;
        case 0x2B: if(Shift) handler->OnKeyDown('|'); else handler->OnKeyDown('\\'); break;

        case 0x29: if(Shift) handler->OnKeyDown('~'); else handler->OnKeyDown('`'); break;
        case 0x02: if(Shift) handler->OnKeyDown('!'); else handler->OnKeyDown('1'); break;
        case 0x03: if(Shift) handler->OnKeyDown('@'); else handler->OnKeyDown('2'); break;
        case 0x04: if(Shift) handler->OnKeyDown('$'); else handler->OnKeyDown('3'); break;
        case 0x05: if(Shift) handler->OnKeyDown('%'); else handler->OnKeyDown('4'); break;
        case 0x06: if(Shift) handler->OnKeyDown('^'); else handler->OnKeyDown('5'); break;
        case 0x07: if(Shift) handler->OnKeyDown('&'); else handler->OnKeyDown('6'); break;
        case 0x08: if(Shift) handler->OnKeyDown('&'); else handler->OnKeyDown('7'); break;
        case 0x09: if(Shift) handler->OnKeyDown('*'); else handler->OnKeyDown('8'); break;
        case 0x0A: if(Shift) handler->OnKeyDown('('); else handler->OnKeyDown('9'); break;
        case 0x0B: if(Shift) handler->OnKeyDown('0'); else handler->OnKeyDown(')'); break;
        case 0x0C: if(Shift) handler->OnKeyDown('_'); else handler->OnKeyDown('-'); break;
        case 0x0D: if(Shift) handler->OnKeyDown('+'); else handler->OnKeyDown('='); break;
        case 0x1A: if(Shift) handler->OnKeyDown('{'); else handler->OnKeyDown('['); break;
        case 0x1B: if(Shift) handler->OnKeyDown('}'); else handler->OnKeyDown(']'); break;
        case 0x10: if(Shift) handler->OnKeyDown('Q'); else handler->OnKeyDown('q'); break;
        case 0x11: if(Shift) handler->OnKeyDown('W'); else handler->OnKeyDown('w'); break;
        case 0x12: if(Shift) handler->OnKeyDown('E'); else handler->OnKeyDown('e'); break;
        case 0x13: if(Shift) handler->OnKeyDown('R'); else handler->OnKeyDown('r'); break;
        case 0x14: if(Shift) handler->OnKeyDown('T'); else handler->OnKeyDown('t'); break;
        case 0x15: if(Shift) handler->OnKeyDown('Y'); else handler->OnKeyDown('y'); break;
        case 0x16: if(Shift) handler->OnKeyDown('U'); else handler->OnKeyDown('u'); break;
        case 0x17: if(Shift) handler->OnKeyDown('I'); else handler->OnKeyDown('i'); break;
        case 0x18: if(Shift) handler->OnKeyDown('O'); else handler->OnKeyDown('o'); break;
        case 0x19: if(Shift) handler->OnKeyDown('P'); else handler->OnKeyDown('p'); break;
        case 0x1E: if(Shift) handler->OnKeyDown('A'); else handler->OnKeyDown('a'); break;
        case 0x1F: if(Shift) handler->OnKeyDown('S'); else handler->OnKeyDown('s'); break;
        case 0x20: if(Shift) handler->OnKeyDown('D'); else handler->OnKeyDown('d'); break;
        case 0x21: if(Shift) handler->OnKeyDown('F'); else handler->OnKeyDown('f'); break;
        case 0x22: if(Shift) handler->OnKeyDown('G'); else handler->OnKeyDown('g'); break;
        case 0x23: if(Shift) handler->OnKeyDown('H'); else handler->OnKeyDown('h'); break;
        case 0x24: if(Shift) handler->OnKeyDown('J'); else handler->OnKeyDown('j'); break;
        case 0x25: if(Shift) handler->OnKeyDown('K'); else handler->OnKeyDown('k'); break;
        case 0x26: if(Shift) handler->OnKeyDown('L'); else handler->OnKeyDown('l'); break;
        case 0x27: if(Shift) handler->OnKeyDown(':'); else handler->OnKeyDown(';'); break;
        case 0x2C: if(Shift) handler->OnKeyDown('Z'); else handler->OnKeyDown('z'); break;
        case 0x2D: if(Shift) handler->OnKeyDown('X'); else handler->OnKeyDown('x'); break;
        case 0x2E: if(Shift) handler->OnKeyDown('C'); else handler->OnKeyDown('c'); break;
        case 0x2F: if(Shift) handler->OnKeyDown('V'); else handler->OnKeyDown('v'); break;
        case 0x30: if(Shift) handler->OnKeyDown('B'); else handler->OnKeyDown('b'); break;
        case 0x31: if(Shift) handler->OnKeyDown('N'); else handler->OnKeyDown('n'); break;
        case 0x32: if(Shift) handler->OnKeyDown('M'); else handler->OnKeyDown('m'); break;
        case 0x33: if(Shift) handler->OnKeyDown('<'); else handler->OnKeyDown(','); break;
        case 0x34: if(Shift) handler->OnKeyDown('>'); else handler->OnKeyDown('.'); break;
        case 0x35: if(Shift) handler->OnKeyDown('?'); else handler->OnKeyDown('/'); break;
        
        case 0x28: if(Shift) handler->OnKeyDown('"'); else handler->OnKeyDown('\''); break;

        case 0x1C: handler->OnKeyDown('\n'); break;
        case 0x39: handler->OnKeyDown(' '); break;

        case 0x2A: case 0x36: Shift = true; break;
        case 0xAA: case 0xB6: Shift = false; break;

        case 0x45: break; // numlock
    
        default:
            if(key < 0x80)
            {
                // char* foo = "KEYBOARD 0X00";
                // char* hex = "0123456789ABCDEF";
                // foo[11] = hex[(key >> 4) & 0x0F];
                // foo[12] = hex[key & 0x0F];
                // printf(foo);
                printf("KEYBOARD 0X");
                printHex(key);
            }
            break;
    }

    

    return esp;
}