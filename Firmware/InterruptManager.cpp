/*
 * InterruptManager.cpp
 *
 *  Created on: Apr 26, 2015
 *      Author: rrau
 */

#include "InterruptManager.h"
#include "fsl_interrupt_manager.h"

const uint32_t InterruptManager::mNumIrqs = NUMBER_OF_INT_VECTORS;
IInterruptDelegate* InterruptManager::mInterruptDelegates[InterruptManager::mNumIrqs];
uint32_t InterruptManager::mI2C0irq = IRQn::I2C0_IRQn;
bool InterruptManager::mInitialized = false;

void InterruptManager::Init()
{
	for(uint32_t i = 0; i < mNumIrqs; i++)
	{
		mInterruptDelegates[i] = nullptr;
	}

	INT_SYS_InstallHandler((IRQn_Type)mI2C0irq, InterruptManager::I2C0Interrupt);

	mInitialized = true;
}

void InterruptManager::I2C0Interrupt()
{
	if(mInterruptDelegates[mI2C0irq] != nullptr && mInitialized)
		mInterruptDelegates[mI2C0irq]->Invoke();
}
