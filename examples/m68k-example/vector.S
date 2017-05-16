#include "asm.h"

  .extern __stack
  .extern start

  .align 4
  .section  .vector
SYM(__vec_start):
  .long   __stack               | 0x000 Reset: Initial Stack Pointer
  .long   __start               | 0x004 Reset: Initial Program Counter
  .long   VecDefault            | 0x008 Bus Error
  .long   VecDefault            | 0x00C Address Error
  .long   VecDefault            | 0x010 Illegal Instruction
  .long   VecDefault            | 0x014 Zero Divison
  .long   VecDefault            | 0x018 CHK Instruction
  .long   VecDefault            | 0x01C TRAP Instruction
  .long   VecDefault            | 0x020 Privilege Violation
  .long   VecDefault            | 0x024 Trace
  .long   VecDefault            | 0x028 Unimplemented line-a opcode
  .long   VecDefault            | 0x02C Unimplemented line-b opcode
  .long   VecDefault            | 0x030 Hardware Breakpoint
  .long   VecDefault            | 0x034 reserved
  .long   VecDefault            | 0x038 Format Error
  .long   VecDefault            | 0x03C Uninitialized Interrupt
  .org    0x6C                   | 0x040 - 0x05C reserved
  .long   IRQSpurious           | 0x060 Spurious Interrupt
  .long   IRQDefault            | 0x064 Level 1 Interrupt
  .long   IRQDefault            | 0x068 Level 2 Interrupt
  .long   IRQDefault            | 0x06C Level 3 Interrupt
  .long   IRQDefault            | 0x070 Level 4 Interrupt
  .long   IRQDefault            | 0x074 Level 5 Interrupt
  .long   IRQDefault            | 0x078 Level 6 Interrupt
  .long   IRQDefault            | 0x07C Level 7 Interrupt
  .long   TrapDefault           | 0x080 TRAP 0
  .long   TrapDefault           | 0x084 TRAP 1
  .long   TrapDefault           | 0x088 TRAP 2
  .long   TrapDefault           | 0x08C TRAP 3
  .long   TrapDefault           | 0x090 TRAP 4
  .long   TrapDefault           | 0x094 TRAP 5
  .long   TrapDefault           | 0x098 TRAP 6
  .long   TrapDefault           | 0x09C TRAP 7
  .long   TrapDefault           | 0x0A0 TRAP 8
  .long   TrapDefault           | 0x0A4 TRAP 9
  .long   TrapDefault           | 0x0A8 TRAP 10
  .long   TrapDefault           | 0x0AC TRAP 11
  .long   TrapDefault           | 0x0B0 TRAP 12
  .long   TrapDefault           | 0x0B4 TRAP 13
  .long   TrapDefault           | 0x0B8 TRAP 14
  .long   TrapDefault           | 0x0BC TRAP 15
  .org    0x100                  | 0x0C0 - 0x0FC reserved
  .long   IRQDefault            | 0x100 User-Defined Interrupt 0
  .long   IRQDefault            | 0x104 User-Defined Interrupt 1
  .long   IRQDefault            | 0x108 User-Defined Interrupt 2
  .long   IRQDefault            | 0x10C User-Defined Interrupt 3
  .long   IRQDefault            | 0x110 User-Defined Interrupt 4
  .long   IRQDefault            | 0x114 User-Defined Interrupt 5
  .long   IRQDefault            | 0x118 User-Defined Interrupt 6
  .long   IRQDefault            | 0x11C User-Defined Interrupt 7
  .long   IRQDefault            | 0x120 User-Defined Interrupt 8
  .long   IRQDefault            | 0x124 User-Defined Interrupt 9
  .long   IRQDefault            | 0x128 User-Defined Interrupt 10
  .long   IRQDefault            | 0x12C User-Defined Interrupt 11
  .long   IRQDefault            | 0x130 User-Defined Interrupt 12
  .long   IRQDefault            | 0x134 User-Defined Interrupt 13
  .long   IRQDefault            | 0x138 User-Defined Interrupt 14
  .long   IRQDefault            | 0x13C User-Defined Interrupt 15
  .long   IRQDefault            | 0x140 User-Defined Interrupt 16
  .long   IRQDefault            | 0x144 User-Defined Interrupt 17
  .long   IRQDefault            | 0x148 User-Defined Interrupt 18
  .long   IRQDefault            | 0x14C User-Defined Interrupt 19
  .long   IRQDefault            | 0x150 User-Defined Interrupt 20
  .long   IRQDefault            | 0x154 User-Defined Interrupt 21
  .long   IRQDefault            | 0x158 User-Defined Interrupt 22
  .long   IRQDefault            | 0x15C User-Defined Interrupt 23
  .long   IRQDefault            | 0x160 User-Defined Interrupt 24
  .long   IRQDefault            | 0x164 User-Defined Interrupt 25
  .long   IRQDefault            | 0x168 User-Defined Interrupt 26
  .long   IRQDefault            | 0x16C User-Defined Interrupt 27
  .long   IRQDefault            | 0x170 User-Defined Interrupt 28
  .long   IRQDefault            | 0x174 User-Defined Interrupt 29
  .long   IRQDefault            | 0x178 User-Defined Interrupt 30
  .long   IRQDefault            | 0x17C User-Defined Interrupt 31
  .long   IRQDefault            | 0x180 User-Defined Interrupt 32
  .long   IRQDefault            | 0x184 User-Defined Interrupt 33
  .long   IRQDefault            | 0x188 User-Defined Interrupt 34
  .long   IRQDefault            | 0x18C User-Defined Interrupt 35
  .long   IRQDefault            | 0x190 User-Defined Interrupt 36
  .long   IRQDefault            | 0x194 User-Defined Interrupt 37
  .long   IRQDefault            | 0x198 User-Defined Interrupt 38
  .long   IRQDefault            | 0x19C User-Defined Interrupt 39
  .long   IRQDefault            | 0x1A0 User-Defined Interrupt 40
  .long   IRQDefault            | 0x1A4 User-Defined Interrupt 41
  .long   IRQDefault            | 0x1A8 User-Defined Interrupt 42
  .long   IRQDefault            | 0x1AC User-Defined Interrupt 43
  .long   IRQDefault            | 0x1B0 User-Defined Interrupt 44
  .long   IRQDefault            | 0x1B4 User-Defined Interrupt 45
  .long   IRQDefault            | 0x1B8 User-Defined Interrupt 46
  .long   IRQDefault            | 0x1BC User-Defined Interrupt 47
  .long   IRQDefault            | 0x1C0 User-Defined Interrupt 48
  .long   IRQDefault            | 0x1C4 User-Defined Interrupt 49
  .long   IRQDefault            | 0x1C8 User-Defined Interrupt 50
  .long   IRQDefault            | 0x1CC User-Defined Interrupt 51
  .long   IRQDefault            | 0x1D0 User-Defined Interrupt 52
  .long   IRQDefault            | 0x1D4 User-Defined Interrupt 53
  .long   IRQDefault            | 0x1D8 User-Defined Interrupt 54
  .long   IRQDefault            | 0x1DC User-Defined Interrupt 55
  .long   IRQDefault            | 0x1E0 User-Defined Interrupt 56
  .long   IRQDefault            | 0x1E4 User-Defined Interrupt 57
  .long   IRQDefault            | 0x1E8 User-Defined Interrupt 58
  .long   IRQDefault            | 0x1EC User-Defined Interrupt 59
  .long   IRQDefault            | 0x1F0 User-Defined Interrupt 60
  .long   IRQDefault            | 0x1F4 User-Defined Interrupt 61
  .long   IRQDefault            | 0x1F8 User-Defined Interrupt 62
  .long   IRQDefault            | 0x1FC User-Defined Interrupt 63
  .long   IRQDefault            | 0x200 User-Defined Interrupt 64
  .long   IRQDefault            | 0x204 User-Defined Interrupt 65
  .long   IRQDefault            | 0x208 User-Defined Interrupt 66
  .long   IRQDefault            | 0x20C User-Defined Interrupt 67
  .long   IRQDefault            | 0x210 User-Defined Interrupt 68
  .long   IRQDefault            | 0x214 User-Defined Interrupt 69
  .long   IRQDefault            | 0x218 User-Defined Interrupt 70
  .long   IRQDefault            | 0x21C User-Defined Interrupt 71
  .long   IRQDefault            | 0x220 User-Defined Interrupt 72
  .long   IRQDefault            | 0x224 User-Defined Interrupt 73
  .long   IRQDefault            | 0x228 User-Defined Interrupt 74
  .long   IRQDefault            | 0x22C User-Defined Interrupt 75
  .long   IRQDefault            | 0x230 User-Defined Interrupt 76
  .long   IRQDefault            | 0x234 User-Defined Interrupt 77
  .long   IRQDefault            | 0x238 User-Defined Interrupt 78
  .long   IRQDefault            | 0x23C User-Defined Interrupt 79
  .long   IRQDefault            | 0x240 User-Defined Interrupt 80
  .long   IRQDefault            | 0x244 User-Defined Interrupt 81
  .long   IRQDefault            | 0x248 User-Defined Interrupt 82
  .long   IRQDefault            | 0x24C User-Defined Interrupt 83
  .long   IRQDefault            | 0x250 User-Defined Interrupt 84
  .long   IRQDefault            | 0x254 User-Defined Interrupt 85
  .long   IRQDefault            | 0x258 User-Defined Interrupt 86
  .long   IRQDefault            | 0x25C User-Defined Interrupt 87
  .long   IRQDefault            | 0x260 User-Defined Interrupt 88
  .long   IRQDefault            | 0x264 User-Defined Interrupt 89
  .long   IRQDefault            | 0x268 User-Defined Interrupt 90
  .long   IRQDefault            | 0x26C User-Defined Interrupt 91
  .long   IRQDefault            | 0x270 User-Defined Interrupt 92
  .long   IRQDefault            | 0x274 User-Defined Interrupt 93
  .long   IRQDefault            | 0x278 User-Defined Interrupt 94
  .long   IRQDefault            | 0x27C User-Defined Interrupt 95
  .long   IRQDefault            | 0x280 User-Defined Interrupt 96
  .long   IRQDefault            | 0x284 User-Defined Interrupt 97
  .long   IRQDefault            | 0x288 User-Defined Interrupt 98
  .long   IRQDefault            | 0x28C User-Defined Interrupt 99
  .long   IRQDefault            | 0x290 User-Defined Interrupt 100
  .long   IRQDefault            | 0x294 User-Defined Interrupt 101
  .long   IRQDefault            | 0x298 User-Defined Interrupt 102
  .long   IRQDefault            | 0x29C User-Defined Interrupt 103
  .long   IRQDefault            | 0x2A0 User-Defined Interrupt 104
  .long   IRQDefault            | 0x2A4 User-Defined Interrupt 105
  .long   IRQDefault            | 0x2A8 User-Defined Interrupt 106
  .long   IRQDefault            | 0x2AC User-Defined Interrupt 107
  .long   IRQDefault            | 0x2B0 User-Defined Interrupt 108
  .long   IRQDefault            | 0x2B4 User-Defined Interrupt 109
  .long   IRQDefault            | 0x2B8 User-Defined Interrupt 110
  .long   IRQDefault            | 0x2BC User-Defined Interrupt 111
  .long   IRQDefault            | 0x2C0 User-Defined Interrupt 112
  .long   IRQDefault            | 0x2C4 User-Defined Interrupt 113
  .long   IRQDefault            | 0x2C8 User-Defined Interrupt 114
  .long   IRQDefault            | 0x2CC User-Defined Interrupt 115
  .long   IRQDefault            | 0x2D0 User-Defined Interrupt 116
  .long   IRQDefault            | 0x2D4 User-Defined Interrupt 117
  .long   IRQDefault            | 0x2D8 User-Defined Interrupt 118
  .long   IRQDefault            | 0x2DC User-Defined Interrupt 119
  .long   IRQDefault            | 0x2E0 User-Defined Interrupt 120
  .long   IRQDefault            | 0x2E4 User-Defined Interrupt 121
  .long   IRQDefault            | 0x2E8 User-Defined Interrupt 122
  .long   IRQDefault            | 0x2EC User-Defined Interrupt 123
  .long   IRQDefault            | 0x2F0 User-Defined Interrupt 124
  .long   IRQDefault            | 0x2F4 User-Defined Interrupt 125
  .long   IRQDefault            | 0x2F8 User-Defined Interrupt 126
  .long   IRQDefault            | 0x2FC User-Defined Interrupt 127
  .long   IRQDefault            | 0x300 User-Defined Interrupt 128
  .long   IRQDefault            | 0x304 User-Defined Interrupt 129
  .long   IRQDefault            | 0x308 User-Defined Interrupt 130
  .long   IRQDefault            | 0x30C User-Defined Interrupt 131
  .long   IRQDefault            | 0x310 User-Defined Interrupt 132
  .long   IRQDefault            | 0x314 User-Defined Interrupt 133
  .long   IRQDefault            | 0x318 User-Defined Interrupt 134
  .long   IRQDefault            | 0x31C User-Defined Interrupt 135
  .long   IRQDefault            | 0x320 User-Defined Interrupt 136
  .long   IRQDefault            | 0x324 User-Defined Interrupt 137
  .long   IRQDefault            | 0x328 User-Defined Interrupt 138
  .long   IRQDefault            | 0x32C User-Defined Interrupt 139
  .long   IRQDefault            | 0x330 User-Defined Interrupt 140
  .long   IRQDefault            | 0x334 User-Defined Interrupt 141
  .long   IRQDefault            | 0x338 User-Defined Interrupt 142
  .long   IRQDefault            | 0x33C User-Defined Interrupt 143
  .long   IRQDefault            | 0x340 User-Defined Interrupt 144
  .long   IRQDefault            | 0x344 User-Defined Interrupt 145
  .long   IRQDefault            | 0x348 User-Defined Interrupt 146
  .long   IRQDefault            | 0x34C User-Defined Interrupt 147
  .long   IRQDefault            | 0x350 User-Defined Interrupt 148
  .long   IRQDefault            | 0x354 User-Defined Interrupt 149
  .long   IRQDefault            | 0x358 User-Defined Interrupt 150
  .long   IRQDefault            | 0x35C User-Defined Interrupt 151
  .long   IRQDefault            | 0x360 User-Defined Interrupt 152
  .long   IRQDefault            | 0x364 User-Defined Interrupt 153
  .long   IRQDefault            | 0x368 User-Defined Interrupt 154
  .long   IRQDefault            | 0x36C User-Defined Interrupt 155
  .long   IRQDefault            | 0x370 User-Defined Interrupt 156
  .long   IRQDefault            | 0x374 User-Defined Interrupt 157
  .long   IRQDefault            | 0x378 User-Defined Interrupt 158
  .long   IRQDefault            | 0x37C User-Defined Interrupt 159
  .long   IRQDefault            | 0x380 User-Defined Interrupt 160
  .long   IRQDefault            | 0x384 User-Defined Interrupt 161
  .long   IRQDefault            | 0x388 User-Defined Interrupt 162
  .long   IRQDefault            | 0x38C User-Defined Interrupt 163
  .long   IRQDefault            | 0x390 User-Defined Interrupt 164
  .long   IRQDefault            | 0x394 User-Defined Interrupt 165
  .long   IRQDefault            | 0x398 User-Defined Interrupt 166
  .long   IRQDefault            | 0x39C User-Defined Interrupt 167
  .long   IRQDefault            | 0x3A0 User-Defined Interrupt 168
  .long   IRQDefault            | 0x3A4 User-Defined Interrupt 169
  .long   IRQDefault            | 0x3A8 User-Defined Interrupt 170
  .long   IRQDefault            | 0x3AC User-Defined Interrupt 171
  .long   IRQDefault            | 0x3B0 User-Defined Interrupt 172
  .long   IRQDefault            | 0x3B4 User-Defined Interrupt 173
  .long   IRQDefault            | 0x3B8 User-Defined Interrupt 174
  .long   IRQDefault            | 0x3BC User-Defined Interrupt 175
  .long   IRQDefault            | 0x3C0 User-Defined Interrupt 176
  .long   IRQDefault            | 0x3C4 User-Defined Interrupt 177
  .long   IRQDefault            | 0x3C8 User-Defined Interrupt 178
  .long   IRQDefault            | 0x3CC User-Defined Interrupt 179
  .long   IRQDefault            | 0x3D0 User-Defined Interrupt 180
  .long   IRQDefault            | 0x3D4 User-Defined Interrupt 181
  .long   IRQDefault            | 0x3D8 User-Defined Interrupt 182
  .long   IRQDefault            | 0x3DC User-Defined Interrupt 183
  .long   IRQDefault            | 0x3E0 User-Defined Interrupt 184
  .long   IRQDefault            | 0x3E4 User-Defined Interrupt 185
  .long   IRQDefault            | 0x3E8 User-Defined Interrupt 186
  .long   IRQDefault            | 0x3EC User-Defined Interrupt 187
  .long   IRQDefault            | 0x3F0 User-Defined Interrupt 188
  .long   IRQDefault            | 0x3F4 User-Defined Interrupt 189
  .long   IRQDefault            | 0x3F8 User-Defined Interrupt 190
  .long   IRQDefault            | 0x3FC User-Defined Interrupt 191
SYM(__vec_end):

  .section  .text

VecDefault:
  bra VecDefault

IRQDefault:
  bra IRQDefault

IRQSpurious:
  bra IRQSpurious

TrapDefault:
  bra TrapDefault
