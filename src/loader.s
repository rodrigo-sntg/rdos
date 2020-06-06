/**
 * loader
 * This is a assembly file responsible for loading programs and libs
 * set the magic number and flags, checksum on the multibook
 * then call the constructors functions, put it in the registers
 * Here I'm using 32 bits arch, that's why I'm using the register E and not R (64bits)
 * 
 * @author Rodrigo Santiago
 * @date Mai 31 2020
 */

.set MAGIC, 0X1BADB002
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

 .section .text
 .extern kernelMain
 .extern callConstructors
 .global loader

 loader:
    mov $kernel_stack, %esp

    call callConstructors

    push %eax
    push %ebx
    call kernelMain

_stop:
    cli
    hlt
    jmp _stop


.section .bss
.space 2*1024*1024; # 2 mb
kernel_stack: