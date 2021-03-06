/*
 * Interrupts.c
 *
 *  Created on: Apr 25, 2015
 *      Author: rrau
 */
#include "i2cCom1.h"
/*
void I2C0_IRQHandler(void)
{
    I2C_DRV_MasterIRQHandler(FSL_I2CCOM1);
}
*/
void GenericFaultHandler(unsigned long *fault_args)
{
	volatile unsigned long stacked_r0 ;
	volatile unsigned long stacked_r1 ;
	volatile unsigned long stacked_r2 ;
	volatile unsigned long stacked_r3 ;
	volatile unsigned long stacked_r12 ;
	volatile unsigned long stacked_lr ;
	volatile unsigned long stacked_pc ;
	volatile unsigned long stacked_psr ;
	volatile unsigned long _CFSR ;
	volatile unsigned long _HFSR ;
	volatile unsigned long _DFSR ;
	volatile unsigned long _AFSR ;
	volatile unsigned long _BFAR ;
	volatile unsigned long _MMAR ;

	stacked_r0 = ((unsigned long)fault_args[0]) ;
	stacked_r1 = ((unsigned long)fault_args[1]) ;
	stacked_r2 = ((unsigned long)fault_args[2]) ;
	stacked_r3 = ((unsigned long)fault_args[3]) ;
	stacked_r12 = ((unsigned long)fault_args[4]) ;
	stacked_lr = ((unsigned long)fault_args[5]) ;
	stacked_pc = ((unsigned long)fault_args[6]) ;
	stacked_psr = ((unsigned long)fault_args[7]) ;

	// Configurable Fault Status Register
	// Consists of MMSR, BFSR and UFSR
	_CFSR = (*((volatile unsigned long *)(0xE000ED28)));

	// Hard Fault Status Register
	_HFSR = (*((volatile unsigned long *)(0xE000ED2C)));

	// Debug Fault Status Register
	_DFSR = (*((volatile unsigned long *)(0xE000ED30)));

	// Auxiliary Fault Status Register
	_AFSR = (*((volatile unsigned long *)(0xE000ED3C)));

	// Read the Fault Address Registers. These may not contain valid values.
	// Check BFARVALID/MMARVALID to see if they are valid values
	// MemManage Fault Address Register
	_MMAR = (*((volatile unsigned long *)(0xE000ED34)));
	// Bus Fault Address Register
	_BFAR = (*((volatile unsigned long *)(0xE000ED38)));
	while(1){}
}

void HardFault_Handler(unsigned long *hardfault_args)
{
	GenericFaultHandler(hardfault_args);
}

void MemManage_Handler(unsigned long *hardfault_args)
{
	GenericFaultHandler(hardfault_args);
}

void BusFault_Handler(unsigned long *hardfault_args)
{
	GenericFaultHandler(hardfault_args);
}

void UsageFault_Handler(unsigned long *hardfault_args)
{
	GenericFaultHandler(hardfault_args);
}
