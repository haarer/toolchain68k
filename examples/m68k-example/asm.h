/*
 * some assemblers choke on '#' as an immediate value. As gcc can also
 * use '&', use that in those cases.
 */
#ifndef __IMMEDIATE_PREFIX__
#define __IMMEDIATE_PREFIX__ #
#endif

/* ANSI concatenation macros.  */
#define CONCAT1(a, b) CONCAT2(a, b)
#define CONCAT2(a, b) a ## b

/* use the right prefix for global labels.  */
#define SYM(x) CONCAT1 (__USER_LABEL_PREFIX__,x)

/* use the right prefix for registers.  */
#define REG(x) CONCAT1 (__REGISTER_PREFIX__,x)

/* use the right prefix for immediate values.  */
#define IMM(x) CONCAT1 (__IMMEDIATE_PREFIX__,x)

/* use the right prefix for register names */
#define d0 REG (d0)
#define d1 REG (d1)
#define d2 REG (d2)
#define d3 REG (d3)
#define d4 REG (d4)
#define d5 REG (d5)
#define d6 REG (d6)
#define d7 REG (d7)
#define a0 REG (a0)
#define a1 REG (a1)
#define a2 REG (a2)
#define a3 REG (a3)
#define a4 REG (a4)
#define a5 REG (a5)
#define a6 REG (a6)
#define a7 REG (a7)
#define fp REG (fp)
#define fp0 REG (fp0)
#define fp1 REG (fp1)
#define fp2 REG (fp2)
#define fp3 REG (fp3)
#define fp4 REG (fp4)
#define fp5 REG (fp5)
#define fp6 REG (fp6)
#define fp7 REG (fp7)
#define sp REG (sp)
#define usp REG (usp)
#define vbr REG (vbr)
#define mbb REG (mbb)
#define sr REG (sr)
#define fpcr REG (fpcr)
#define fpsr REG (fpsr)
#define fpi REG (fpi)

