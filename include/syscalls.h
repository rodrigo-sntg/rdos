#ifndef __RDOS__SYSCALLS_H
#define __RDOS__SYSCALLS_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <multitasking.h>

namespace rdos
{
    
    class SyscallHandler : public hardwarecommunication::InterruptHandler
    {
        
    public:
        SyscallHandler(hardwarecommunication::InterruptManager* interruptManager, rdos::common::uint8_t InterruptNumber);
        ~SyscallHandler();
        
        virtual rdos::common::uint32_t HandleInterrupt(rdos::common::uint32_t esp);

    };
    
    
}


#endif