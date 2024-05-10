

FLASH_SIZE = 128K;      
SRAM_SIZE = 20K;        

STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x800;

MEMORY
{
  ram (xrw) : ORIGIN = 0x20000000, LENGTH = 20K
  rom (rx) : ORIGIN = 0x08000000, LENGTH = 128K
}


SECTIONS
{
   .text :
   {
      . = ALIGN(4);

      _sfixed = .;

      KEEP(*(.vectors .vectors.*))

      *(.text .text.* .gnu.linkonce.t.*)
      *(.rodata .rodata* .gnu.linkonce.r.*)

    /*
         C++ Runtime: initializers for static variables.
         C Runtime: designated constructors
         For C++, handles variables at file scope like this:  int f = some_func()
         For C, handles functions designated as constructors:  void intialize_thing(void) __attribute__((constructor));
         Executed by the C runtime at startup via __libc_init_array.
    */

      . = ALIGN(4);
      KEEP(*(.init))
      . = ALIGN(4);
      __preinit_array_start = .;
      KEEP (*(.preinit_array))
      __preinit_array_end = .;

      . = ALIGN(4);
      __init_array_start = .;
      KEEP (*(SORT(.init_array.*)))
      KEEP (*(.init_array))
      __init_array_end = .;

      /*
         C++ runtime: destructors for static variables.
         C runtime: designated finializers
         For C, handles functions designated as destructors: void destroy_thing(void) __attribute__((destructor));
      */

      . = ALIGN(4);
      KEEP(*(.fini))

      . = ALIGN(4);
      __fini_array_start = .;
      KEEP (*(.fini_array))
      KEEP (*(SORT(.fini_array.*)))
      __fini_array_end = .;


      /*
         C++ runtime: static constructors

         References:
      */
      . = ALIGN(4);
      KEEP (*crtbegin.o(.ctors))
      KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
      KEEP (*(SORT(.ctors.*)))
      KEEP (*crtend.o(.ctors))

      /*
         C++ runtime: static destructors and atexit()
         Note that in usual practice these aren't ever called because the program doesn't exit - except when powered off or reset.

      */

      . = ALIGN(4);
      KEEP (*crtbegin.o(.dtors))
      KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
      KEEP (*(SORT(.dtors.*)))
      KEEP (*crtend.o(.dtors))

      . = ALIGN(4);
      _efixed = .;
   } > rom

   /*_sidata
      ARM defines several special sections for exception handling.

      These are require for C++ and for C programs that try to examine backtraces.

      - exidx is used to contain index entries for stack unwinding.
      - extab names sections containing exception unwinding information.

      Essentially, each function that can throw an exception will have entries in the exidx and extab sections.

   */

   .ARM.extab : {
      *(.ARM.extab* .gnu.linkonce.armextab.*)
   } > rom

   .ARM.exidx : {
      PROVIDE(__exidx_start = .);
      *(.ARM.exidx* .gnu.linkonce.armexidx.*)
      PROVIDE(__exidx_end = .);
   } > rom

   /*
      The `.relocate` section includes mutable variables that have a
      default value and specially marked functions that should execute
      from RAM.

      This data is stored in ROM but is referenced from RAM. The
      program/runtime must copy the data from ROM to RAM on reset,
      hence, "relocate".

      Performance sensitive/critical functions can also be placed in
      RAM using this section:

         #define RAMFUNC __attribute__((section(".ramfunc")))

         void fast_function(void) RAMFUNC;

      In other linker scripts you might see this named as the `.data`
      section. That's what the ELF specification calls for, but the
      Microchip-provided SAM D startup scripts expect `.relocate`.

      This also sets the symbol `_etext` to the start of the relocation
      segment in flash. The startup script copies the data starting at
      `_etext` to `_srelocate` and ends when it reaches `_erelocate`.
      The `_etext` name is a bit unfortunate since it's not the end of
      the text segment, but rather the start of the read-only copy of the
      relocate section in flash. If I wrote the startup script I would have
      named these symbols differently.

   */

   .relocate :
   {
      . = ALIGN(4);
      _srelocate = .;
      *(.ramfunc .ramfunc.*);
      *(.data .data.*);
      . = ALIGN(4);
      _erelocate = .;
   } > ram AT> rom

   _etext = LOADADDR(.relocate);

   /*
      The BSS section reserves RAM space for declared but uninitialized
      variables.

      This is zeroed out by the startup script. The start-up script
      zeros out the area of RAM starting at `_szero` and ending at
      `_ezero`.

      This includes `COMMON` which is a bit of a legacy section. GCC
      defaults to `-fno-common` these days so there shouldn't be
      anything in there, but it's included for completeness.

      References:
   */

   .bss (NOLOAD) :
   {
      . = ALIGN(4);
      _szero = .;
      *(.bss .bss.*)
      *(COMMON)
      . = ALIGN(4);
      _ezero = .;
   } > ram

   /*
      Reserve the stack space in RAM.

      Cortex-M stacks grow down, so the stack starts at `_estack` and
      grows towards `_sstack`. The startup script sets the vector
      table's stack pointer to `_estack` on startup. `_sstack` is
      unused but included for completeness.

      The ARM procedure call standard (AAPCS) requires the stack to be
      aligned on an eight byte boundary.

      References:
      * ../third_party/samd21/gcc/gcc/startup_samd21.c
      * https://developer.arm.com/documentation/ihi0042/e/
        Section 5.2.1.2, Stack constraints at a public interface
   */

   .stack (NOLOAD):
   {
      . = ALIGN(8);
      _sstack = .;
      . = . + STACK_SIZE;
      . = ALIGN(8);
      _estack = .;
   } > ram

   /*
      Mark the end of the RAM and the start of unallocated space.

      If the program uses the malloc and the heap, then `_heap_start`
      can be used as the start of the heap. If the program doesn't
      use the heap then the `_heap_start` symbol is unused and could
      be removed. With `-specs=nano.specs`, the `_sbrk` syscall has
      to be implemented for malloc to work:

         extern int _heap_start;

         void *_sbrk(int incr) {
            static unsigned char *heap = NULL;
            unsigned char *prev_heap;

            if (heap == NULL) {
               heap = (unsigned char *)&_heap_start;
            }

            prev_heap = heap;
            heap += incr;

            return prev_heap;
         }

   */
   . = ALIGN(4);
   PROVIDE(_heap_start = .);
   _end = . ;
}

/*
   Absolute symbol definitions.

   This section defines some useful absolute symbols for the application  to use.
*/

