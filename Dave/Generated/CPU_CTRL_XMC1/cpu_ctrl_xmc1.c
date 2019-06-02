/**
 * @file cpu_ctrl_xmc1.c
 * @date 2017-02-22
 *
 * NOTE:
 * This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
 */
/**
 * @cond
 ***********************************************************************************************************************
 * CPU_CTRL_XMC1 v4.0.8 - Sets the priority grouping for NVIC
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes
 * with Infineon Technologies AG (dave@infineon.com).
 ***********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-02-16:
 *     - Initial version<br>
 * 2015-08-31:
 *     - Added hardfault handler
 * 2017-02-22:
 *     - Fix compilation warnings
 *     - Changed hard fault handler to avoid reading from the stack in case an stack overflow caused the fault.
 * @endcond
 *
 */
/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/
#include "cpu_ctrl_xmc1.h"

/***********************************************************************************************************************
 * MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * LOCAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * LOCAL ROUTINES
 **********************************************************************************************************************/

/**********************************************************************************************************************
* API IMPLEMENTATION
**********************************************************************************************************************/

/*
 * API to retrieve the version of the CPU_CTRL_XMC1 APP
 */
DAVE_APP_VERSION_t CPU_CTRL_XMC1_GetAppVersion(void)
{
  DAVE_APP_VERSION_t version;

  version.major = CPU_CTRL_XMC1_MAJOR_VERSION;
  version.minor = CPU_CTRL_XMC1_MINOR_VERSION;
  version.patch = CPU_CTRL_XMC1_PATCH_VERSION;

  return (version);
}

/* Dummy Init API to maintain backward compatibility */
CPU_CTRL_XMC1_STATUS_t CPU_CTRL_XMC1_Init(CPU_CTRL_XMC1_t *const handler)
{
  (void)handler;
  return CPU_CTRL_XMC1_STATUS_SUCCESS;
}

#if (HARDFAULT_ENABLED == 1)
/**
 * This is called from the HardFault_Handler with a pointer the Fault stack
 * as the parameter. We can then read the values from the stack and place them
 * into local variables for ease of reading.
 * We then read the various Fault Status and Address Registers to help decode
 * cause of the fault.
 * The function ends with a BKPT instruction to force control back into the debugger
 * Original code from http://www.support.code-red-tech.com/CodeRedWiki/DebugHardFault
 */
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
volatile uint32_t stacked_r0;
volatile uint32_t stacked_r1;
volatile uint32_t stacked_r2;
volatile uint32_t stacked_r3;
volatile uint32_t stacked_r12;
volatile uint32_t stacked_lr;
volatile uint32_t stacked_pc;
volatile uint32_t stacked_psr;

void __HardFault_Handler(uint32_t args[])
{
  stacked_r0 = ((uint32_t)args[0]);
  stacked_r1 = ((uint32_t)args[1]);
  stacked_r2 = ((uint32_t)args[2]);
  stacked_r3 = ((uint32_t)args[3]);
  stacked_r12 = ((uint32_t)args[4]);
  stacked_lr = ((uint32_t)args[5]);
  stacked_pc = ((uint32_t)args[6]);
  stacked_psr = ((uint32_t)args[7]);

  __asm("BKPT 0\n") ; // Break into the debugger
}

/*KEIL*/
#if defined(__CC_ARM)
__asm void HardFault_Handler(void)
{
  EXTERN __HardFault_Handler [CODE]

    movs r0,#4      /* load bit mask into R0 */
    mov r1, lr      /* load link register into R1 */
    tst r0, r1      /* compare with bitmask */
    beq _MSP        /* if bitmask is set: stack pointer is in PSP. Otherwise in MSP */
    mrs r0, psp     /* otherwise: stack pointer is in PSP */
    b __HardFault_Handler        /* go to part which loads the PC */
_MSP                /* stack pointer is in MSP register */
    mrs r0, msp     /* load stack pointer into R0 */
    b __HardFault_Handler        /* go to part which loads the PC */
}

#endif

/*IAR*/
#if defined(__ICCARM__)
void HardFault_Handler(void)
{
  asm(
  "movs r0,#4 \n"       /* load bit mask into R0 */
  "mov r1, lr \n"       /* load link register into R1 */
  "tst r0, r1 \n"       /* compare with bitmask */
  "beq _MSP \n"        /* if bitmask is set: stack pointer is in PSP. Otherwise in MSP */
  "mrs r0, psp \n"      /* otherwise: stack pointer is in PSP */
  "b __HardFault_Handler \n"         /* go to part which loads the PC */
  "_MSP: \n"            /* stack pointer is in MSP register */
  "mrs r0, msp \n"      /* load stack pointer into R0 */
  "b __HardFault_Handler \n"  /* decode more information. R0 contains pointer to stack frame */
    );
}
#endif

/*TASKING*/
#if defined(__TASKING__)
void HardFault_Handler(void)
{
  __asm volatile (
" movs r0,#4      \n"  /* load bit mask into R0 */
" mov r1, lr      \n"  /* load link register into R1 */
" tst r0, r1      \n"  /* compare with bitmask */
" beq _MSP        \n"  /* if bitmask is set: stack pointer is in PSP. Otherwise in MSP */
" mrs r0, psp     \n"  /* otherwise: stack pointer is in PSP */
" b __HardFault_Handler        \n"  /* go to part which loads the PC */
"_MSP:              \n"  /* stack pointer is in MSP register */
" mrs r0, msp     \n"  /* load stack pointer into R0 */
" b __HardFault_Handler   \n"  /* decode more information. R0 contains pointer to stack frame */
  );
}
#endif

/*GCC*/
#if defined(__GNUC__)
__attribute__((naked))
void HardFault_Handler(void)
{
  __asm volatile (
" movs r0,#4      \n"  /* load bit mask into R0 */
" mov r1, lr      \n"  /* load link register into R1 */
" tst r0, r1      \n"  /* compare with bitmask */
" beq _MSP        \n"  /* if bitmask is set: stack pointer is in PSP. Otherwise in MSP */
" mrs r0, psp     \n"  /* otherwise: stack pointer is in PSP */
" b __HardFault_Handler        \n"  /* go to part which loads the PC */
"_MSP:              \n"  /* stack pointer is in MSP register */
" mrs r0, msp     \n"  /* load stack pointer into R0 */
" b __HardFault_Handler   \n"  /* decode more information. R0 contains pointer to stack frame */
  );
}
#endif
#endif
