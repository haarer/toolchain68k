#ifndef MC68332_H
#define MC68332_H

/* SIM System Integration Module */
#define SIM_BASE       (0x00FFFA00)
#define SIMCR          (*((volatile unsigned short *) (SIM_BASE + 0x00)))
#define SIMTR          (*((volatile unsigned short *) (SIM_BASE + 0x02)))
#define SYNCR          (*((volatile unsigned short *) (SIM_BASE + 0x04)))
#define RSR            (*((volatile unsigned char  *) (SIM_BASE + 0x07)))
#define SIMTRE         (*((volatile unsigned short *) (SIM_BASE + 0x08)))
#define PORTE0         (*((volatile unsigned char  *) (SIM_BASE + 0x11)))
#define PORTE1         (*((volatile unsigned char  *) (SIM_BASE + 0x13)))
#define DDRE           (*((volatile unsigned char  *) (SIM_BASE + 0x15)))
#define PEPAR          (*((volatile unsigned char  *) (SIM_BASE + 0x17)))
#define PORTF0         (*((volatile unsigned char  *) (SIM_BASE + 0x19)))
#define PORTF1         (*((volatile unsigned char  *) (SIM_BASE + 0x1B)))
#define DDRF           (*((volatile unsigned char  *) (SIM_BASE + 0x1D)))
#define PFPAR          (*((volatile unsigned char  *) (SIM_BASE + 0x1F)))
#define SYPCR          (*((volatile unsigned char  *) (SIM_BASE + 0x21)))
#define PICR           (*((volatile unsigned short *) (SIM_BASE + 0x22)))
#define PITR           (*((volatile unsigned short *) (SIM_BASE + 0x24)))
#define SWSR           (*((volatile unsigned char  *) (SIM_BASE + 0x27)))
#define TSTMSRA        (*((volatile unsigned short *) (SIM_BASE + 0x30)))
#define TSTMSRB        (*((volatile unsigned short *) (SIM_BASE + 0x32)))
#define TSTSC          (*((volatile unsigned short *) (SIM_BASE + 0x34)))
#define TSTRC          (*((volatile unsigned short *) (SIM_BASE + 0x36)))
#define CREG           (*((volatile unsigned short *) (SIM_BASE + 0x38)))
#define DREG           (*((volatile unsigned short *) (SIM_BASE + 0x3A)))
#define PORTC          (*((volatile unsigned char  *) (SIM_BASE + 0x41)))
#define CSPAR0         (*((volatile unsigned short *) (SIM_BASE + 0x44)))
#define CSPAR1         (*((volatile unsigned short *) (SIM_BASE + 0x46)))
#define CSBARBT        (*((volatile unsigned short *) (SIM_BASE + 0x48)))
#define CSORBT         (*((volatile unsigned short *) (SIM_BASE + 0x4A)))
#define CSBAR0         (*((volatile unsigned short *) (SIM_BASE + 0x4C)))
#define CSOR0          (*((volatile unsigned short *) (SIM_BASE + 0x4E)))
#define CSBAR1         (*((volatile unsigned short *) (SIM_BASE + 0x50)))
#define CSOR1          (*((volatile unsigned short *) (SIM_BASE + 0x52)))
#define CSBAR2         (*((volatile unsigned short *) (SIM_BASE + 0x54)))
#define CSOR2          (*((volatile unsigned short *) (SIM_BASE + 0x56)))
#define CSBAR3         (*((volatile unsigned short *) (SIM_BASE + 0x58)))
#define CSOR3          (*((volatile unsigned short *) (SIM_BASE + 0x5A)))
#define CSBAR4         (*((volatile unsigned short *) (SIM_BASE + 0x5C)))
#define CSOR4          (*((volatile unsigned short *) (SIM_BASE + 0x5E)))
#define CSBAR5         (*((volatile unsigned short *) (SIM_BASE + 0x60)))
#define CSOR5          (*((volatile unsigned short *) (SIM_BASE + 0x62)))
#define CSBAR6         (*((volatile unsigned short *) (SIM_BASE + 0x64)))
#define CSOR6          (*((volatile unsigned short *) (SIM_BASE + 0x66)))
#define CSBAR7         (*((volatile unsigned short *) (SIM_BASE + 0x68)))
#define CSOR7          (*((volatile unsigned short *) (SIM_BASE + 0x6A)))
#define CSBAR8         (*((volatile unsigned short *) (SIM_BASE + 0x6C)))
#define CSOR8          (*((volatile unsigned short *) (SIM_BASE + 0x6E)))
#define CSBAR9         (*((volatile unsigned short *) (SIM_BASE + 0x70)))
#define CSOR9          (*((volatile unsigned short *) (SIM_BASE + 0x72)))
#define CSBAR10        (*((volatile unsigned short *) (SIM_BASE + 0x74)))
#define CSOR10         (*((volatile unsigned short *) (SIM_BASE + 0x76)))

/* TPURAM Time Processing Unit RAM */
#define TPURAM_BASE    (0x00FFFB00)
#define TRAMMCR        (*((volatile unsigned short *) (TPURAM_BASE + 0x00)))
#define TRAMTST        (*((volatile unsigned short *) (TPURAM_BASE + 0x02)))
#define TRAMBAR        (*((volatile unsigned short *) (TPURAM_BASE + 0x04)))

/* QSM Queued Serial Module */
#define QSM_BASE       (0x00FFFC00)
#define QSMCR          (*((volatile unsigned short *) (QSM_BASE + 0x00)))
#define QTEST          (*((volatile unsigned short *) (QSM_BASE + 0x02)))
#define QILR           (*((volatile unsigned char  *) (QSM_BASE + 0x04)))
#define QIVR           (*((volatile unsigned char  *) (QSM_BASE + 0x05)))
#define SCCR0          (*((volatile unsigned short *) (QSM_BASE + 0x08)))
#define SCCR1          (*((volatile unsigned short *) (QSM_BASE + 0x0A)))
#define SCSR           (*((volatile unsigned short *) (QSM_BASE + 0x0C)))
#define SCDR           (*((volatile unsigned short *) (QSM_BASE + 0x0E)))
#define PORTQS         (*((volatile unsigned char  *) (QSM_BASE + 0x15)))
#define PQSPAR         (*((volatile unsigned char  *) (QSM_BASE + 0x16)))
#define DDRQS          (*((volatile unsigned char  *) (QSM_BASE + 0x17)))
#define SPCR0          (*((volatile unsigned short *) (QSM_BASE + 0x18)))
#define SPCR1          (*((volatile unsigned short *) (QSM_BASE + 0x1A)))
#define SPCR2          (*((volatile unsigned short *) (QSM_BASE + 0x1C)))
#define SPCR3          (*((volatile unsigned char  *) (QSM_BASE + 0x1E)))
#define SPSR           (*((volatile unsigned char  *) (QSM_BASE + 0x1F)))

/* QSMRAM Queued Serial Module RAM */
#define QSMRAM_BASE    (0x00FFFD00)
#define RR_START       (*((volatile unsigned short *) (QSMRAM_BASE + 0x00)))
#define TR_START       (*((volatile unsigned short *) (QSMRAM_BASE + 0x20)))
#define CR_START       (*((volatile unsigned char  *) (QSMRAM_BASE + 0x40)))

/* TPU Time Processing Unit */
#define TPU_BASE       (0x00FFFE00)
#define TPUMCR         (*((volatile unsigned short *) (TPU_BASE + 0x00)))
#define TCR            (*((volatile unsigned short *) (TPU_BASE + 0x02)))
#define DSCR           (*((volatile unsigned short *) (TPU_BASE + 0x04)))
#define DSSR           (*((volatile unsigned short *) (TPU_BASE + 0x06)))
#define TICR           (*((volatile unsigned short *) (TPU_BASE + 0x08)))
#define CIER           (*((volatile unsigned short *) (TPU_BASE + 0x0A)))
#define CFSR0          (*((volatile unsigned short *) (TPU_BASE + 0x0C)))
#define CFSR1          (*((volatile unsigned short *) (TPU_BASE + 0x0E)))
#define CFSR2          (*((volatile unsigned short *) (TPU_BASE + 0x10)))
#define CFSR3          (*((volatile unsigned short *) (TPU_BASE + 0x12)))
#define HSQR0          (*((volatile unsigned short *) (TPU_BASE + 0x14)))
#define HSQR1          (*((volatile unsigned short *) (TPU_BASE + 0x16)))
#define HSRR0          (*((volatile unsigned short *) (TPU_BASE + 0x18)))
#define HSRR1          (*((volatile unsigned short *) (TPU_BASE + 0x1A)))
#define CPR0           (*((volatile unsigned short *) (TPU_BASE + 0x1C)))
#define CPR1           (*((volatile unsigned short *) (TPU_BASE + 0x1E)))
#define CISR           (*((volatile unsigned short *) (TPU_BASE + 0x20)))
#define LR             (*((volatile unsigned short *) (TPU_BASE + 0x22)))
#define SGLR           (*((volatile unsigned short *) (TPU_BASE + 0x24)))
#define DCNR           (*((volatile unsigned short *) (TPU_BASE + 0x26)))

/* TPU Time Processing Unit Parameter RAM */
#define TPUPAR_BASE    (0x00FFFF00)
#define TPUPAR_CH0_0   (*((volatile unsigned short *) (TPUPAR_BASE + 0x00)))
#define TPUPAR_CH0_1   (*((volatile unsigned short *) (TPUPAR_BASE + 0x02)))
#define TPUPAR_CH0_2   (*((volatile unsigned short *) (TPUPAR_BASE + 0x04)))
#define TPUPAR_CH0_3   (*((volatile unsigned short *) (TPUPAR_BASE + 0x06)))
#define TPUPAR_CH0_4   (*((volatile unsigned short *) (TPUPAR_BASE + 0x08)))
#define TPUPAR_CH0_5   (*((volatile unsigned short *) (TPUPAR_BASE + 0x0A)))
#define TPUPAR_CH1_0   (*((volatile unsigned short *) (TPUPAR_BASE + 0x10)))
#define TPUPAR_CH1_1   (*((volatile unsigned short *) (TPUPAR_BASE + 0x12)))
#define TPUPAR_CH1_2   (*((volatile unsigned short *) (TPUPAR_BASE + 0x14)))
#define TPUPAR_CH1_3   (*((volatile unsigned short *) (TPUPAR_BASE + 0x16)))
#define TPUPAR_CH1_4   (*((volatile unsigned short *) (TPUPAR_BASE + 0x18)))
#define TPUPAR_CH1_5   (*((volatile unsigned short *) (TPUPAR_BASE + 0x1A)))
#define TPUPAR_CH2_0   (*((volatile unsigned short *) (TPUPAR_BASE + 0x20)))
#define TPUPAR_CH2_1   (*((volatile unsigned short *) (TPUPAR_BASE + 0x22)))
#define TPUPAR_CH2_2   (*((volatile unsigned short *) (TPUPAR_BASE + 0x24)))
#define TPUPAR_CH2_3   (*((volatile unsigned short *) (TPUPAR_BASE + 0x26)))
#define TPUPAR_CH2_4   (*((volatile unsigned short *) (TPUPAR_BASE + 0x28)))
#define TPUPAR_CH2_5   (*((volatile unsigned short *) (TPUPAR_BASE + 0x2A)))
#define TPUPAR_CH3_0   (*((volatile unsigned short *) (TPUPAR_BASE + 0x30)))
#define TPUPAR_CH3_1   (*((volatile unsigned short *) (TPUPAR_BASE + 0x32)))
#define TPUPAR_CH3_2   (*((volatile unsigned short *) (TPUPAR_BASE + 0x34)))
#define TPUPAR_CH3_3   (*((volatile unsigned short *) (TPUPAR_BASE + 0x36)))
#define TPUPAR_CH3_4   (*((volatile unsigned short *) (TPUPAR_BASE + 0x38)))
#define TPUPAR_CH3_5   (*((volatile unsigned short *) (TPUPAR_BASE + 0x3A)))
#define TPUPAR_CH4_0   (*((volatile unsigned short *) (TPUPAR_BASE + 0x40)))
#define TPUPAR_CH4_1   (*((volatile unsigned short *) (TPUPAR_BASE + 0x42)))
#define TPUPAR_CH4_2   (*((volatile unsigned short *) (TPUPAR_BASE + 0x44)))
#define TPUPAR_CH4_3   (*((volatile unsigned short *) (TPUPAR_BASE + 0x46)))
#define TPUPAR_CH4_4   (*((volatile unsigned short *) (TPUPAR_BASE + 0x48)))
#define TPUPAR_CH4_5   (*((volatile unsigned short *) (TPUPAR_BASE + 0x4A)))
#define TPUPAR_CH5_0   (*((volatile unsigned short *) (TPUPAR_BASE + 0x50)))
#define TPUPAR_CH5_1   (*((volatile unsigned short *) (TPUPAR_BASE + 0x52)))
#define TPUPAR_CH5_2   (*((volatile unsigned short *) (TPUPAR_BASE + 0x54)))
#define TPUPAR_CH5_3   (*((volatile unsigned short *) (TPUPAR_BASE + 0x56)))
#define TPUPAR_CH5_4   (*((volatile unsigned short *) (TPUPAR_BASE + 0x58)))
#define TPUPAR_CH5_5   (*((volatile unsigned short *) (TPUPAR_BASE + 0x5A)))
#define TPUPAR_CH6_0   (*((volatile unsigned short *) (TPUPAR_BASE + 0x60)))
#define TPUPAR_CH6_1   (*((volatile unsigned short *) (TPUPAR_BASE + 0x62)))
#define TPUPAR_CH6_2   (*((volatile unsigned short *) (TPUPAR_BASE + 0x64)))
#define TPUPAR_CH6_3   (*((volatile unsigned short *) (TPUPAR_BASE + 0x66)))
#define TPUPAR_CH6_4   (*((volatile unsigned short *) (TPUPAR_BASE + 0x68)))
#define TPUPAR_CH6_5   (*((volatile unsigned short *) (TPUPAR_BASE + 0x6A)))
#define TPUPAR_CH7_0   (*((volatile unsigned short *) (TPUPAR_BASE + 0x70)))
#define TPUPAR_CH7_1   (*((volatile unsigned short *) (TPUPAR_BASE + 0x72)))
#define TPUPAR_CH7_2   (*((volatile unsigned short *) (TPUPAR_BASE + 0x74)))
#define TPUPAR_CH7_3   (*((volatile unsigned short *) (TPUPAR_BASE + 0x76)))
#define TPUPAR_CH7_4   (*((volatile unsigned short *) (TPUPAR_BASE + 0x78)))
#define TPUPAR_CH7_5   (*((volatile unsigned short *) (TPUPAR_BASE + 0x7A)))
#define TPUPAR_CH8_0   (*((volatile unsigned short *) (TPUPAR_BASE + 0x80)))
#define TPUPAR_CH8_1   (*((volatile unsigned short *) (TPUPAR_BASE + 0x82)))
#define TPUPAR_CH8_2   (*((volatile unsigned short *) (TPUPAR_BASE + 0x84)))
#define TPUPAR_CH8_3   (*((volatile unsigned short *) (TPUPAR_BASE + 0x86)))
#define TPUPAR_CH8_4   (*((volatile unsigned short *) (TPUPAR_BASE + 0x88)))
#define TPUPAR_CH8_5   (*((volatile unsigned short *) (TPUPAR_BASE + 0x8A)))
#define TPUPAR_CH9_0   (*((volatile unsigned short *) (TPUPAR_BASE + 0x90)))
#define TPUPAR_CH9_1   (*((volatile unsigned short *) (TPUPAR_BASE + 0x92)))
#define TPUPAR_CH9_2   (*((volatile unsigned short *) (TPUPAR_BASE + 0x94)))
#define TPUPAR_CH9_3   (*((volatile unsigned short *) (TPUPAR_BASE + 0x96)))
#define TPUPAR_CH9_4   (*((volatile unsigned short *) (TPUPAR_BASE + 0x98)))
#define TPUPAR_CH9_5   (*((volatile unsigned short *) (TPUPAR_BASE + 0x9A)))
#define TPUPAR_CH10_0  (*((volatile unsigned short *) (TPUPAR_BASE + 0xA0)))
#define TPUPAR_CH10_1  (*((volatile unsigned short *) (TPUPAR_BASE + 0xA2)))
#define TPUPAR_CH10_2  (*((volatile unsigned short *) (TPUPAR_BASE + 0xA4)))
#define TPUPAR_CH10_3  (*((volatile unsigned short *) (TPUPAR_BASE + 0xA6)))
#define TPUPAR_CH10_4  (*((volatile unsigned short *) (TPUPAR_BASE + 0xA8)))
#define TPUPAR_CH10_5  (*((volatile unsigned short *) (TPUPAR_BASE + 0xAA)))
#define TPUPAR_CH11_0  (*((volatile unsigned short *) (TPUPAR_BASE + 0xB0)))
#define TPUPAR_CH11_1  (*((volatile unsigned short *) (TPUPAR_BASE + 0xB2)))
#define TPUPAR_CH11_2  (*((volatile unsigned short *) (TPUPAR_BASE + 0xB4)))
#define TPUPAR_CH11_3  (*((volatile unsigned short *) (TPUPAR_BASE + 0xB6)))
#define TPUPAR_CH11_4  (*((volatile unsigned short *) (TPUPAR_BASE + 0xB8)))
#define TPUPAR_CH11_5  (*((volatile unsigned short *) (TPUPAR_BASE + 0xBA)))
#define TPUPAR_CH12_0  (*((volatile unsigned short *) (TPUPAR_BASE + 0xC0)))
#define TPUPAR_CH12_1  (*((volatile unsigned short *) (TPUPAR_BASE + 0xC2)))
#define TPUPAR_CH12_2  (*((volatile unsigned short *) (TPUPAR_BASE + 0xC4)))
#define TPUPAR_CH12_3  (*((volatile unsigned short *) (TPUPAR_BASE + 0xC6)))
#define TPUPAR_CH12_4  (*((volatile unsigned short *) (TPUPAR_BASE + 0xC8)))
#define TPUPAR_CH12_5  (*((volatile unsigned short *) (TPUPAR_BASE + 0xCA)))
#define TPUPAR_CH13_0  (*((volatile unsigned short *) (TPUPAR_BASE + 0xD0)))
#define TPUPAR_CH13_1  (*((volatile unsigned short *) (TPUPAR_BASE + 0xD2)))
#define TPUPAR_CH13_2  (*((volatile unsigned short *) (TPUPAR_BASE + 0xD4)))
#define TPUPAR_CH13_3  (*((volatile unsigned short *) (TPUPAR_BASE + 0xD6)))
#define TPUPAR_CH13_4  (*((volatile unsigned short *) (TPUPAR_BASE + 0xD8)))
#define TPUPAR_CH13_5  (*((volatile unsigned short *) (TPUPAR_BASE + 0xDA)))
#define TPUPAR_CH14_0  (*((volatile unsigned short *) (TPUPAR_BASE + 0xE0)))
#define TPUPAR_CH14_1  (*((volatile unsigned short *) (TPUPAR_BASE + 0xE2)))
#define TPUPAR_CH14_2  (*((volatile unsigned short *) (TPUPAR_BASE + 0xE4)))
#define TPUPAR_CH14_3  (*((volatile unsigned short *) (TPUPAR_BASE + 0xE6)))
#define TPUPAR_CH14_4  (*((volatile unsigned short *) (TPUPAR_BASE + 0xE8)))
#define TPUPAR_CH14_5  (*((volatile unsigned short *) (TPUPAR_BASE + 0xEA)))
#define TPUPAR_CH14_6  (*((volatile unsigned short *) (TPUPAR_BASE + 0xEC)))
#define TPUPAR_CH14_7  (*((volatile unsigned short *) (TPUPAR_BASE + 0xEE)))
#define TPUPAR_CH15_0  (*((volatile unsigned short *) (TPUPAR_BASE + 0xF0)))
#define TPUPAR_CH15_1  (*((volatile unsigned short *) (TPUPAR_BASE + 0xF2)))
#define TPUPAR_CH15_2  (*((volatile unsigned short *) (TPUPAR_BASE + 0xF4)))
#define TPUPAR_CH15_3  (*((volatile unsigned short *) (TPUPAR_BASE + 0xF6)))
#define TPUPAR_CH15_4  (*((volatile unsigned short *) (TPUPAR_BASE + 0xF8)))
#define TPUPAR_CH15_5  (*((volatile unsigned short *) (TPUPAR_BASE + 0xFA)))
#define TPUPAR_CH15_6  (*((volatile unsigned short *) (TPUPAR_BASE + 0xFC)))
#define TPUPAR_CH15_7  (*((volatile unsigned short *) (TPUPAR_BASE + 0xFE)))

#endif