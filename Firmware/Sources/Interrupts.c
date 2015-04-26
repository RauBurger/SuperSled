/*
 * Interrupts.c
 *
 *  Created on: Apr 25, 2015
 *      Author: rrau
 */
#include "i2cCom1.h"

void I2C0_IRQHandler(void)
{
    I2C_DRV_MasterIRQHandler(FSL_I2CCOM1);
}

/* Implementation of I2C0 handler named in startup code. */
void I2C1_IRQHandler(void)
{
    I2C_DRV_MasterIRQHandler(1);
}
