#ifndef __RDOS__HARDWARECOMMUNICATION__PORT_H
#define __RDOS__HARDWARECOMMUNICATION__PORT_H

    #include <common/types.h>

namespace rdos
{
    namespace hardwarecommunication
    {
        class Port
            {
                protected:
                    Port(rdos::common::uint16_t portnumber);
                    // FIXME: Must be virtual (currently isnt because the kernel has no memory management yet)
                    ~Port();
                    rdos::common::uint16_t portnumber;
            };

            
            class Port8Bit : public Port
            {
                public:
                    Port8Bit(rdos::common::uint16_t portnumber);
                    ~Port8Bit();

                    virtual rdos::common::uint8_t Read();
                    virtual void Write(rdos::common::uint8_t data);

                protected:
                    static inline rdos::common::uint8_t Read8(rdos::common::uint16_t _port)
                    {
                        rdos::common::uint8_t result;
                        __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (_port));
                        return result;
                    }

                    static inline void Write8(rdos::common::uint16_t _port, rdos::common::uint8_t _data)
                    {
                        __asm__ volatile("outb %0, %1" : : "a" (_data), "Nd" (_port));
                    }
            };



            class Port8BitSlow : public Port8Bit
            {
                public:
                    Port8BitSlow(rdos::common::uint16_t portnumber);
                    ~Port8BitSlow();

                    virtual void Write(rdos::common::uint8_t data);
                protected:
                    static inline void Write8Slow(rdos::common::uint16_t _port, rdos::common::uint8_t _data)
                    {
                        __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (_data), "Nd" (_port));
                    }

            };



            class Port16Bit : public Port
            {
                public:
                    Port16Bit(rdos::common::uint16_t portnumber);
                    ~Port16Bit();

                    virtual rdos::common::uint16_t Read();
                    virtual void Write(rdos::common::uint16_t data);

                protected:
                    static inline rdos::common::uint16_t Read16(rdos::common::uint16_t _port)
                    {
                        rdos::common::uint16_t result;
                        __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (_port));
                        return result;
                    }

                    static inline void Write16(rdos::common::uint16_t _port, rdos::common::uint16_t _data)
                    {
                        __asm__ volatile("outw %0, %1" : : "a" (_data), "Nd" (_port));
                    }
            };



            class Port32Bit : public Port
            {
                public:
                    Port32Bit(rdos::common::uint16_t portnumber);
                    ~Port32Bit();

                    virtual rdos::common::uint32_t Read();
                    virtual void Write(rdos::common::uint32_t data);

                protected:
                    static inline rdos::common::uint32_t Read32(rdos::common::uint16_t _port)
                    {
                        rdos::common::uint32_t result;
                        __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (_port));
                        return result;
                    }

                    static inline void Write32(rdos::common::uint16_t _port, rdos::common::uint32_t _data)
                    {
                        __asm__ volatile("outl %0, %1" : : "a"(_data), "Nd" (_port));
                    }
            };
    }
}

    




#endif