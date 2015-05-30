/*
 * Application.cpp
 *
 *  Created on: Apr 25, 2015
 *      Author: rrau
 */

#include "Application.h"
#include "InterruptManager.h"
#include "gpio1.h"
#include "i2cCom1.h"
#include "uartCom1.h"
extern "C" {
#include "adConv1.h"
}

#include "string.h"

uint32_t itoa(uint32_t val, char buff[])
{
	uint32_t tmpVal = 1;
	uint8_t tmpChar = 0;
	uint32_t divisor = 10;
	uint32_t numDigits = 0;

	while(tmpVal != 0)
	{
		tmpVal = val/divisor;
		divisor *= 10;
		numDigits++;
	}

	divisor = 1;
	for(uint32_t i = numDigits-1; i >= 0 ; i--)
	{
		tmpChar = (val/divisor)%10;
		divisor *= 10;

		if(i > numDigits)
			break;

		buff[i] = tmpChar + 48;
	}

	return numDigits;
}

Application::Application()
{
	//InterruptManager::Init();

	//mI2CMaster = new I2CMaster(0, 100, 0x50);
}

Application::~Application()
{

}

void Application::Run()
{
	GPIO_DRV_SetPinOutput(PTA19);
	GPIO_DRV_SetPinOutput(PTA5);

	char str[10];
	uint32_t num = itoa(10, str);
	str[num] = '\n';
	str[num+1] = '\r';
	UART_DRV_SendDataBlocking(FSL_UARTCOM1, (const uint8_t*)str, num+2, 100);

	uint8_t pressureBuff[2];
	while(true)
	{
		uint8_t dummy[2];
		// Tell sensor to start conversion. So even though we don't want to receive any data with this command,
		// if we don't give the driver a valid receive buffer it will try to de-ref address 0x0 even though it's
		// been provided with a size of 0. Further we need to tell it that we want to receive 1 byte of data so
		// it properly stops the transmission after only sending the address. shit's broke yo.
		i2c_status_t err = I2C_DRV_MasterReceiveDataBlocking(FSL_I2CCOM1, &i2cCom1_MasterConfig0, NULL, 0, dummy, 1, 100);
		memset(pressureBuff, 0, 2);
		/*
		//I2CMaster::I2CStatus err;// = mI2CMaster->WriteAddress(100, I2CMaster::Direction_Read, true);

		if(err != I2CMaster::I2CStatus_OK)
		{
			char errCode[3];
			uint32_t errStrLen = 24;
			char errStr[100] = "1: MR failed with error ";
			uint32_t errNum = itoa(err, errCode);
			for(uint32_t i = 0; i < errNum; i++)
			{
				errStr[errStrLen+i] = errCode[i];
			}
			errStr[errStrLen+errNum] = '\n';
			errStr[errStrLen+errNum+1] = '\r';

			UART_DRV_SendDataBlocking(FSL_UARTCOM1, (const uint8_t*)errStr, errStrLen+errNum+2, 100);
		}
		for(int i = 0; i < 100000; i++){};

		// Read data from sensor
		//err = mI2CMaster->WriteAddress(100, I2CMaster::Direction_Read);
		if(err != I2CMaster::I2CStatus_OK)
		{
			char errCode[3];
			uint32_t errStrLen = 24;
			char errStr[100] = "2: MR failed with error ";
			uint32_t errNum = itoa(err, errCode);
			for(uint32_t i = 0; i < errNum; i++)
			{
				errStr[errStrLen+i] = errCode[i];
			}
			errStr[errStrLen+errNum] = '\n';
			errStr[errStrLen+errNum+1] = '\r';

			UART_DRV_SendDataBlocking(FSL_UARTCOM1, (const uint8_t*)errStr, errStrLen+errNum+2, 100);
		}
		//err = mI2CMaster->Read(pressureBuff, 2, 100);
		if(err != I2CMaster::I2CStatus_OK)
		{
			char errCode[3];
			uint32_t errStrLen = 24;
			char errStr[100] = "3: MR failed with error ";
			uint32_t errNum = itoa(err, errCode);
			for(uint32_t i = 0; i < errNum; i++)
			{
				errStr[errStrLen+i] = errCode[i];
			}
			errStr[errStrLen+errNum] = '\n';
			errStr[errStrLen+errNum+1] = '\r';

			UART_DRV_SendDataBlocking(FSL_UARTCOM1, (const uint8_t*)errStr, errStrLen+errNum+2, 100);
		}
		*/
		err = I2C_DRV_MasterReceiveDataBlocking(FSL_I2CCOM1, &i2cCom1_MasterConfig0, NULL, 0, pressureBuff, 2, 100);
		uint8_t status = (pressureBuff[0] & 0xC0) >> 6;
		uint16_t pressure = (pressureBuff[0] & 0x3F) << 8 | pressureBuff[1];
		uint16_t adcVal = ADC16_DRV_GetConvValueRAW(FSL_ADCONV1, 0);
		num = itoa(adcVal, str);

		str[num] = '\n';
		str[num+1] = '\r';
		UART_DRV_SendDataBlocking(FSL_UARTCOM1, (const uint8_t*)str, num+2, 100);

		GPIO_DRV_TogglePinOutput(PTA19);

		for(int i = 0; i < 100000; i++){};
	}
}

void StartApp()
{
	Application app;
	app.Run();
}


