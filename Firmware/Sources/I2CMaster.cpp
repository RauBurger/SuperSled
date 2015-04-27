/*
 * I2CMaster.cpp
 *
 *  Created on: Apr 26, 2015
 *      Author: rrau
 */

#include "I2CMaster.h"
#include "InterruptManager.h"

#include <fsl_i2c_hal.h>
#include <fsl_i2c_common.h>
#include <fsl_clock_manager.h>

#include <assert.h>

I2CMaster::I2CMaster(uint32_t instance, uint32_t baudRate, uint8_t address) :
	mInstance(instance),
	mBaseAddress(g_i2cBaseAddr[instance]),
	mI2cIrqNum(g_i2cIrqId[instance]),
	mBaudRate(baudRate),
	mAddress(address)
{
	assert(mInstance < HW_I2C_INSTANCE_COUNT);

	InterruptManager::InstallInterrupt(mI2cIrqNum, new InterruptDelegate<I2CMaster>(this, &I2CMaster::InterruptHandler));

	CLOCK_SYS_EnableI2cClock(instance);

	OSA_SemaCreate(&mSyncSem, 0);
	OSA_SemaCreate(&mTransactionSem, 1);

	I2C_HAL_Init(mBaseAddress);

	mIdle = true;

	I2C_HAL_Enable(mBaseAddress);

	SetBaudRate();
}

void I2CMaster::SetBaudRate()
{
    uint32_t i2cClockFreq = CLOCK_SYS_GetI2cFreq(mInstance);
    I2C_HAL_SetBaudRate(mBaseAddress, i2cClockFreq, mBaudRate, NULL);
}

I2CMaster::I2CStatus I2CMaster::Wait(uint32_t timeoutMs)
{
	osa_status_t syncStatus;
	I2CStatus status = I2CStatus_OK;

	do
	{
		syncStatus = OSA_SemaWait(&mSyncSem, timeoutMs);
	}
	while(syncStatus == kStatus_OSA_Idle);

    if (syncStatus != kStatus_OSA_Success)
    {
        status = I2CStatus_Timeout;
    }
    else
    {
    	status = mStatus;
    }

    return status;
}

I2CMaster::I2CStatus I2CMaster::WriteAddress(uint32_t timeoutMs, I2CMaster::Direction dir, bool lone)
{
	uint8_t address = mAddress;
	osa_status_t syncStatus;
	do
	{
		syncStatus = OSA_SemaWait(&mTransactionSem, timeoutMs);
	}
	while(syncStatus == kStatus_OSA_Idle);
    if (syncStatus != kStatus_OSA_Success)
    {
        return I2CStatus_Timeout;
    }

    mDir = Direction_Write;
    // Set direction to send for sending of address.
    I2C_HAL_SetDirMode(mBaseAddress, kI2CSend);

    // Enable i2c interrupt.
	I2C_HAL_ClearInt(mBaseAddress);
	I2C_HAL_SetIntCmd(mBaseAddress, true);

    // Generate start signal.
    I2C_HAL_SendStart(mBaseAddress);
	dir == Direction_Write ? address = (mAddress & 0xFE) : address = mAddress | 0x1;
	mSize = 1;

    // Send first byte in address buffer to trigger interrupt.
    I2C_HAL_WriteByte(mBaseAddress, address);

    // Wait for the transfer to finish.
    I2CStatus status = Wait(OSA_WAIT_FOREVER);
    if(status == I2CStatus_Timeout)
    {
    	OSA_SemaPost(&mTransactionSem);
    	return status;
    }

    if(lone)
    	I2C_HAL_SendStop(mBaseAddress);

    OSA_SemaPost(&mTransactionSem);

	return I2CStatus_OK;
}

I2CMaster::I2CStatus I2CMaster::Read(uint8_t rxBuffer[], uint32_t rxBufferSize, uint32_t timeoutMs)
{
	osa_status_t syncStatus;
	do
	{
		syncStatus = OSA_SemaWait(&mTransactionSem, timeoutMs);
	}
	while(syncStatus == kStatus_OSA_Idle);
    if (syncStatus != kStatus_OSA_Success)
    {
        return I2CStatus_Timeout;
    }

    I2CStatus status = I2CStatus_OK;
	mStatus = I2CStatus_OK;

    mDir = Direction_Read;
    I2C_HAL_SetDirMode(mBaseAddress, kI2CReceive);

    if(rxBufferSize != 0)
    {
    	mSize = rxBufferSize;
    	mBuffer = rxBuffer;

        /* Send NAK if only one byte to read. */
    	if (mSize == 0x1U)
        {
            I2C_HAL_SendNak(mBaseAddress);
        }
        else
        {
            I2C_HAL_SendAck(mBaseAddress);
        }

        /* Dummy read to trigger receive of next byte in interrupt. */
        I2C_HAL_ReadByte(mBaseAddress);

        /* Wait for the transfer to finish.*/
        status = Wait(timeoutMs);
        if(status != I2CStatus_OK)
        {
        	OSA_SemaPost(&mTransactionSem);
        	return status;
        }
    }
    else
    {
    	mSize = 1;
    	I2C_HAL_SendNak(mBaseAddress);
    	I2C_HAL_ReadByte(mBaseAddress);
    	status = Wait(timeoutMs);
    	/*
    	status = Wait(timeoutMs);
        if(status != I2CStatus_OK)
        {
        	OSA_SemaPost(&mTransactionSem);
        	return status;
        }
        */
    	//I2C_HAL_SendStop(mBaseAddress);
    }

    // Wait for the STOP signal finish.
    uint32_t i = 0;
    while(I2C_HAL_GetStatusFlag(mBaseAddress, kI2CBusBusy))
    {
        if(++i == 2)
        {
            // Something is wrong because the bus is still busy.
            mStatus = I2CStatus_Busy;
            break;
        }
        else
        {
            OSA_TimeDelay(1U);
        }
    }

	OSA_SemaPost(&mTransactionSem);
	return mStatus;
}

I2CMaster::I2CStatus I2CMaster::Write(uint8_t txBuffer[], uint32_t txBufferSize, uint32_t timeoutMs)
{
	osa_status_t syncStatus;
	do
	{
		syncStatus = OSA_SemaWait(&mTransactionSem, timeoutMs);
	}
	while(syncStatus == kStatus_OSA_Idle);
    if (syncStatus != kStatus_OSA_Success)
    {
        return I2CStatus_Timeout;
    }

	mStatus = I2CStatus_OK;
	mDir = Direction_Write;


	OSA_SemaPost(&mTransactionSem);
	return I2CStatus_OK;
}

void I2CMaster::InterruptHandler()
{
	I2C_HAL_ClearInt(mBaseAddress);

	// Exit immediately if there is no transfer in progress OR not in master mode.
	if ((!I2C_HAL_GetStatusFlag(mBaseAddress, kI2CBusBusy)) ||
		(!I2C_HAL_IsMaster(mBaseAddress)))
	{
		OSA_SemaPost(&mSyncSem);
		return;
	}

	if(((mSize-1) == 0) && (mDir == Direction_Write))
	{
		OSA_SemaPost(&mSyncSem);
		return;
	}

	if(mDir == Direction_Write)
	{
        // Check whether we got an ACK or NAK from the former byte we sent.
        if (I2C_HAL_GetStatusFlag(mBaseAddress, kI2CReceivedNak))
        {
            // Record that we got a NAK.
        	mStatus = I2CStatus_ReceivedNak;

            // Got a NAK, so we're done with this transfer.
    		OSA_SemaPost(&mSyncSem);
    		return;
        }
        else
        {
        	I2C_HAL_WriteByte(mBaseAddress, *(++mBuffer));
        }
	}
	else
	{
		if(mSize == 0x1U)
		{
			I2C_HAL_SendStop(mBaseAddress);
			OSA_SemaPost(&mSyncSem);
		}
		if(mSize == 0x2U)
			I2C_HAL_SendNak(mBaseAddress);
		else
			I2C_HAL_SendAck(mBaseAddress);

		*(mBuffer++) = I2C_HAL_ReadByte(mBaseAddress);
	}
	mSize--;
}
