/**
 * HEADER FILE
 * Global Descriptor Table
 * 
 * This is a data structore from intel x86 processors which define
 * the characteristics of memory usage during the execution of the program.
 * 
 * @author Rodrigo Santiago
 * @date Mai 31 2020
 */

#ifndef __RDOS__GDT_H
#define __RDOS__GDT_H

#include <common/types.h>

namespace rdos
{
    class GlobalDescriptorTable
    {
        public:
            class SegmentDescriptor
            {
            private:
                rdos::common::uint16_t limit_lo;
                rdos::common::uint16_t base_lo;
                rdos::common::uint8_t base_hi;
                rdos::common::uint8_t type;
                rdos::common::uint8_t flags_limit_hi;
                rdos::common::uint8_t base_vhi;
            public:
                SegmentDescriptor(rdos::common::uint32_t base, rdos::common::uint32_t limit, rdos::common::uint8_t type);
                rdos::common::uint32_t Base();
                rdos::common::uint32_t Limit();

            } __attribute__((packed));

        SegmentDescriptor nullSegmentSelector;
        SegmentDescriptor unusedSegmentSelector;
        SegmentDescriptor codeSegmentSelector;
        SegmentDescriptor dataSegmentSelector;
    
    public:
        GlobalDescriptorTable();
        ~GlobalDescriptorTable();

        rdos::common::uint16_t CodeSegmentSelector();
        rdos::common::uint16_t DataSegmentSelector();
    };
}


#endif