OUTPUT_ARCH(m68k)
STARTUP(crt0.o)
INPUT(vector.o)
SEARCH_DIR(.)
GROUP(-lc -lgcc)

__DYNAMIC  =  0;

MEMORY
{
  vec    (rwx) : ORIGIN = 0x00100000, LENGTH = 0x00000400
  ram    (rwx) : ORIGIN = 0x00100400, LENGTH = 0x000E0000 - 0x400
  ram2   (rwx) : ORIGIN = 0x001E0000, LENGTH = 0x00030000
}

PROVIDE (__stack = 0x20FFFC);
PROVIDE (__heap_limit = __stack);
PROVIDE (__start = __text_start);
PROVIDE (__vec_start = __ram_vec_start);

SECTIONS
{
  .text :
  {
    __rom_vec_start = .;
    vector.o(.vector)

    __text_start = . ;
    *(.text)
    . = ALIGN (16);

    *(.eh_frame)
    . = ALIGN (16);

    *(.gnu.linkonce.t.*)

    . = ALIGN(0x4);
     __CTOR_LIST__ = .;
    ___CTOR_LIST__ = .;
    LONG((__CTOR_END__ - __CTOR_LIST__) / 4 - 2)
    *(.ctors)
    LONG(0)
    __CTOR_END__ = .;
    __DTOR_LIST__ = .;
    ___DTOR_LIST__ = .;
    LONG((__DTOR_END__ - __DTOR_LIST__) / 4 - 2)
    *(.dtors)
     LONG(0)
    __DTOR_END__ = .;
    *(.rodata .rodata.*)
    *(.gcc_except_table)

    . = ALIGN(0x2);

    *(.lit)

    . = ALIGN(16);
    __end_text = .;
    etext = .;
  } > ram

  .data : AT ( ADDR( .text ) + SIZEOF( .text ) )
  {
    __data_start = .;
    *(.shdata)
    *(.data)
    *(.gnu.linkonce.d.*)
    . = ALIGN (16);
    _edata = .;
    __data_end = .;
  } > ram2

  .ramvec :
  {
    __ram_vec_start = .;
   . = __ram_vec_start + 0x400;
    __ram_vec_end = .;
  } > vec

  .bss :
  {
    . = ALIGN(0x4);
    __bss_start = . ;
    *(.shbss)
    *(.bss)
    *(COMMON)
    __bss_end =  ALIGN (0x8);
    __end = __bss_end;
  } > ram2

  .stab 0 (NOLOAD) :
  {
    *(.stab)
  }

  .stabstr 0 (NOLOAD) :
  {
    *(.stabstr)
  }

  .gdbdata :
  {
    *(.gdbdata)
  } > ram
}
