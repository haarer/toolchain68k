*
*       MOTOROLA, INC.
*       Advanced MCU Division
*       Austin, Texas
*
*       Title : EQUATES
*       Description : This is a table of EQUates for all of the
*                     registers in the MC68332.
**************************************************************************


*****  SIM Module Registers  *****

.equ SIMCR     ,0xFFFA00           //SIM Module Configuration Register
.equ SIMTR     ,0xFFFA02           //System Integration Test Register
.equ SYNCR     ,0xFFFA04           //Clock Synthesizer Control Register
.equ RSR       ,0xFFFA07           //Reset Status Register
.equ SIMTRE    ,0xFFFA08           //System Integration Test Register (E Clock)
.equ PORTE0    ,0xFFFA11           //Port E Data Register (same data as PORTE1)
.equ PORTE1    ,0xFFFA13           //Port E Data Register (same data as PORTE0)
.equ DDRE      ,0xFFFA15           //Port E Data Direction Register
.equ PEPAR     ,0xFFFA17           //Port E Pin Assignment Register
.equ PORTF0    ,0xFFFA19           //Port F Data Register (same data as PORTF1)
.equ PORTF1    ,0xFFFA1B           //Port F Data Register (same data as PORTF0)
.equ DDRF      ,0xFFFA1D           //Port F Data Direction Register
.equ PFPAR     ,0xFFFA1F           //Port F Pin Assignment Register
.equ SYPCR     ,0xFFFA21           //System Protection Control Register
.equ PICR      ,0xFFFA22           //Periodic Interrupt Control Register
.equ PITR      ,0xFFFA24           //Periodic Interrupt Timing Register
.equ SWSR      ,0xFFFA27           //Software Service Register
.equ TSTMSRA   ,0xFFFA30           //Master Shift Register A
.equ TSTMSRB   ,0xFFFA32           //Master Shift Register B
.equ TSTSC     ,0xFFFA34           //Test Module Shift Count
.equ TSTRC     ,0xFFFA36           //Test Module Repetition Count
.equ CREG      ,0xFFFA38           //Test Submodule Control Register
.equ DREG      ,0xFFFA3A           //Distributed Register
.equ PORTC     ,0xFFFA41           //Port C Data Register
.equ CSPAR0    ,0xFFFA44           //Chip-Select Pin Assignment Register 0
.equ CSPAR1    ,0xFFFA46           //Chip-Select Pin Assignment Register 1
.equ CSBARBT   ,0xFFFA48           //Chip-Select Boot Base Address Register
.equ CSORBT    ,0xFFFA4A           //Chip-Select Boot Option Register
.equ CSBAR0    ,0xFFFA4C           //Chip-Select 0 Base Address Register
.equ CSOR0     ,0xFFFA4E           //Chip Select 0 Option Register 
.equ CSBAR1    ,0xFFFA50           //Chip-Select 1 Base Address Register
.equ CSOR1     ,0xFFFA52           //Chip-Select 1 Option Register
.equ CSBAR2    ,0xFFFA54           //Chip-Select 2 Base Address Register
.equ CSOR2     ,0xFFFA56           //Chip-Select 2 Option Register
.equ CSBAR3    ,0xFFFA58           //Chip-Select 3 Base Address Register
.equ CSOR3     ,0xFFFA5A           //Chip-Select 3 Option Register
.equ CSBAR4    ,0xFFFA5C           //Chip-Select 4 Base Address Register
.equ CSOR4     ,0xFFFA5E           //Chip-Select 4 Option Register
.equ CSBAR5    ,0xFFFA60           //Chip-Select 5 Base Address Register
.equ CSOR5     ,0xFFFA62           //Chip-Select 5 Option Register
.equ CSBAR6    ,0xFFFA64           //Chip-Select 6 Base Address Register
.equ CSOR6     ,0xFFFA66           //Chip-Select 6 Option Register
.equ CSBAR7    ,0xFFFA68           //Chip-Select 7 Base Address Register
.equ CSOR7     ,0xFFFA6A           //Chip-Select 7 Option Register
.equ CSBAR8    ,0xFFFA6C           //Chip-Select 8 Base Address Register
.equ CSOR8     ,0xFFFA6E           //Chip-Select 8 Option Register
.equ CSBAR9    ,0xFFFA70           //Chip-Select 9 Base Address Register
.equ CSOR9     ,0xFFFA72           //Chip-Select 9 Option Register
.equ CSBAR10   ,0xFFFA74           //Chip-Select 10 Base Address Register
.equ CSOR10    ,0xFFFA76           //Chip-Select 10 Option Register


*****  SRAM Module Registers  *****

.equ TRAMMCR   ,0xFFFB00           //RAM Module Configuration Register
.equ TRAMTST   ,0xFFFB02           //RAM Test Register
.equ TRAMBAR   ,0xFFFB04           //RAM Base Address High Register

*****  QSM Address Map *****

.equ QSMMCR    ,0xFFFC00           //QSM Module Configuration Register
.equ QTEST     ,0xFFFC02           //QSM Test Register
.equ QUILR     ,0xFFFC04           //QSM Interrupt Levels Register
.equ QIVR      ,0xFFFC05           //QSM Interrupt Vector Register
.equ SCCR0     ,0xFFFC08           //SCI Control Register 0
.equ SCCR1     ,0xFFFC0A           //SCI Control Register 1
.equ SCSR      ,0xFFFC0C           //SCI Status Register
.equ SCDR      ,0xFFFC0E           //SCI Data Register
.equ PORTQS    ,0xFFFC15           //QSM Port Data Register
.equ PQSPAR    ,0xFFFC16           //QSM Pin Assignment Register
.equ DDRQS     ,0xFFFC17           //QSM Data Direction Register
.equ SPCR0     ,0xFFFC18           //QSPI Control Register 0
.equ SPCR1     ,0xFFFC1A           //QSPI Control Register 1
.equ SPCR2     ,0xFFFC1C           //QSPI Control Register 2
.equ SPCR3     ,0xFFFC1E           //QSPI Control Register 3
.equ SPSR      ,0xFFFC1F           //QSPI Status Register
.equ RR0       ,0xFFFD00           //spi rec.ram 0
.equ RR1       ,0xFFFD02           //spi rec.ram 1
.equ RR2       ,0xFFFD04           //spi rec.ram 2
.equ RR3       ,0xFFFD06           //spi rec.ram 3
.equ RR4       ,0xFFFD08           //spi rec.ram 4
.equ RR5       ,0xFFFD0A           //spi rec.ram 5
.equ RR6       ,0xFFFD0C           //spi rec.ram 6
.equ RR7       ,0xFFFD0E           //spi rec.ram 7
.equ RR8       ,0xFFFD00           //spi rec.ram 8
.equ RR9       ,0xFFFD02           //spi rec.ram 9
.equ RRA       ,0xFFFD04           //spi rec.ram A
.equ RRB       ,0xFFFD06           //spi rec.ram B
.equ RRC       ,0xFFFD08           //spi rec.ram C
.equ RRD       ,0xFFFD0A           //spi rec.ram D
.equ RRE       ,0xFFFD0C           //spi rec.ram E
.equ RRF       ,0xFFFD0E           //spi rec.ram F
.equ TR0       ,0xFFFD20           //spi txd.ram 0
.equ TR1       ,0xFFFD22           //spi txd.ram 1
.equ TR2       ,0xFFFD24           //spi txd.ram 2
.equ TR3       ,0xFFFD26           //spi txd.ram 3
.equ TR4       ,0xFFFD28           //spi txd.ram 4
.equ TR5       ,0xFFFD2A           //spi txd.ram 5
.equ TR6       ,0xFFFD2C           //spi txd.ram 6
.equ TR7       ,0xFFFD2E           //spi txd.ram 7
.equ TR8       ,0xFFFD30           //spi txd.ram 8
.equ TR9       ,0xFFFD32           //spi txd.ram 9
.equ TRA       ,0xFFFD34           //spi txd.ram A
.equ TRB       ,0xFFFD36           //spi txd.ram B
.equ TRC       ,0xFFFD38           //spi txd.ram C
.equ TRD       ,0xFFFD3A           //spi txd.ram D
.equ TRE       ,0xFFFD3C           //spi txd.ram E
.equ TRF       ,0xFFFD3E           //spi txd.ram F
.equ CR0       ,0xFFFD40           //spi cmd.ram 0
.equ CR1       ,0xFFFD41           //spi cmd.ram 1
.equ CR2       ,0xFFFD42           //spi cmd.ram 2
.equ CR3       ,0xFFFD43           //spi cmd.ram 3
.equ CR4       ,0xFFFD44           //spi cmd.ram 4
.equ CR5       ,0xFFFD45           //spi cmd.ram 5
.equ CR6       ,0xFFFD46           //spi cmd.ram 6
.equ CR7       ,0xFFFD47           //spi cmd.ram 7
.equ CR8       ,0xFFFD48           //spi cmd.ram 8
.equ CR9       ,0xFFFD49           //spi cmd.ram 9
.equ CRA       ,0xFFFD4A           //spi cmd.ram A
.equ CRB       ,0xFFFD4B           //spi cmd.ram B
.equ CRC       ,0xFFFD4C           //spi cmd.ram C
.equ CRD       ,0xFFFD4D           //spi cmd.ram D
.equ CRE       ,0xFFFD4E           //spi cmd.ram E
.equ CRF       ,0xFFFD4F           //spi cmd.ram F

*****  TPU Time Processor Unit  *****
 
.equ TPUMCR    ,0xFFFE00           //TPU Module Configuration Register
.equ TCR       ,0xFFFE02           //Test Configuration Register
.equ DSCR      ,0xFFFE04           //Development Support Control Register
.equ DSSR      ,0xFFFE06           //Development Support Status Register
.equ TICR      ,0xFFFE08           //TPU Interrupt Configuration Register
.equ CIER      ,0xFFFE0A           //Channel interrupt Enable Register
.equ CFSR0     ,0xFFFE0C           //Channel Function Select Register 0
.equ CFSR1     ,0xFFFE0E           //Channel Function Select Register 1
.equ CFSR2     ,0xFFFE10           //Channel Function Select Register 2
.equ CFSR3     ,0xFFFE12           //Channel Function Select Register 3
.equ HSQR0     ,0xFFFE14           //Host Sequence Register 0
.equ HSQR1     ,0xFFFE16           //Host Sequence Register 1
.equ HSRR0     ,0xFFFE18           //Host Service Request Register 0
.equ HSRR1     ,0xFFFE1A           //Host Service REquest Register 1
.equ CPR0      ,0xFFFE1C           //Channel Priority Register 0
.equ CPR1      ,0xFFFE1E           //Channel Priority Register 1
.equ CISR      ,0xFFFE20           //Channel Interrupt Status Register
.equ LR        ,0xFFFE22           //Link Register
.equ SGLR      ,0xFFFF24           //Service Grant Latch Register
.equ DCNR      ,0xFFFF26           //Decoded Channel Number Register

.equ PRAM      ,0xFFFF00           //TPU Parameter RAM (Start address)
