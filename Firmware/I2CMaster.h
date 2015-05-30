/*
 * I2CMaster.h
 *
 *  Created on: Apr 26, 2015
 *      Author: rrau
 */

#ifndef I2CMASTER_H_
#define I2CMASTER_H_

#include <fsl_os_abstraction.h>

#include <stdint.h>

class I2CMaster
{
public:

	typedef enum
	{
		Direction_Write = 0,
		Direction_Read = 1
	} Direction;

	typedef enum
	{
		I2CStatus_OK = 0x0U,
		I2CStatus_OutOfRange = 0x1U,
	    I2CStatus_Fail = 0x2U,
	    I2CStatus_Busy = 0x3U, // The master is already performing a transfer.
	    I2CStatus_Timeout = 0x4U, // The transfer timed out.
	    I2CStatus_ReceivedNak = 0x5U, // The slave device sent a NAK in response to a byte.
	    I2CStatus_SlaveTxUnderrun = 0x6U, // I2C Slave TX Underrun error.
	    I2CStatus_SlaveRxOverrun = 0x7U, // I2C Slave RX Overrun error.
	    I2CStatus_AribtrationLost = 0x8U, // I2C Arbitration Lost error.
	} I2CStatus;

	I2CMaster(uint32_t instance, uint32_t baudRate, uint8_t address);
	I2CStatus WriteAddress(uint32_t timeoutMs, Direction dir, bool lone = false);
	I2CStatus Read(uint8_t rxBuffer[], uint32_t rxBufferSize, uint32_t timeoutMs);
	I2CStatus Write(uint8_t txBuffer[], uint32_t txBufferSize, uint32_t timeoutMs);
	void InterruptHandler();

private:

	void SetBaudRate();
	I2CStatus Wait(uint32_t timeoutMs);

	uint32_t mInstance;
	uint32_t mBaseAddress;
	uint32_t mI2cIrqNum;
	uint32_t mBaudRate;
	uint8_t mAddress;
	semaphore_t mSyncSem;
	semaphore_t mTransactionSem;
	volatile bool mIdle;

	uint8_t* mBuffer;
	uint32_t mSize;
	Direction mDir;
	I2CStatus mStatus;
};


#endif /* I2CMASTER_H_ */
