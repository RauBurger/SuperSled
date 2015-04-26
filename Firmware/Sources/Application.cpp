/*
 * Application.cpp
 *
 *  Created on: Apr 25, 2015
 *      Author: rrau
 */

#include "Application.h"
#include "gpio1.h"
#include "i2cCom1.h"
#include "uartCom1.h"
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

}

Application::~Application()
{

}

void Application::Run()
{
	GPIO_DRV_Init(NULL, gpio1_OutConfig0);
	GPIO_DRV_SetPinOutput(PTA1);
	GPIO_DRV_SetPinOutput(PTA2);

	UART_DRV_Init(FSL_UARTCOM1, &uartCom1_State, &uartCom1_InitConfig0);

	I2C_DRV_MasterInit(FSL_I2CCOM1, &i2cCom1_MasterState);
	I2C_DRV_MasterSetBaudRate(FSL_I2CCOM1, &i2cCom1_MasterConfig0);

	char str[10];
	uint32_t num = itoa(10, str);
	str[num] = '\n';
	str[num+1] = '\r';
	UART_DRV_SendDataBlocking(FSL_UARTCOM1, (const uint8_t*)str, num+2, 100);

	uint8_t pressureBuff[2];
	while(true)
	{
		// Tell sensor to start conversion
		i2c_status_t err = I2C_DRV_MasterReceiveDataBlocking(FSL_I2CCOM1, &i2cCom1_MasterConfig0, NULL, 0, NULL, 0, 100);

		if(err != kStatus_I2C_Success)
		{
			char errStr[100] = "MR failed\n\r";
			UART_DRV_SendDataBlocking(FSL_UARTCOM1, (const uint8_t*)errStr, 11, 100);
		}
		memset(pressureBuff, 0, 2);
		for(int i = 0; i < 100000; i++){};

		// Read data from sensor
		err = I2C_DRV_MasterReceiveDataBlocking(FSL_I2CCOM1, &i2cCom1_MasterConfig0, NULL, 0, pressureBuff, 2, 100);

		uint16_t pressure = pressureBuff[0] << 8 | pressureBuff[1];

		num = itoa(pressure, str);

		str[num] = '\n';
		str[num+1] = '\r';
		UART_DRV_SendDataBlocking(FSL_UARTCOM1, (const uint8_t*)str, num+2, 100);

		GPIO_DRV_TogglePinOutput(PTA1);

		for(int i = 0; i < 100000; i++){};
	}
}

void StartApp()
{
	Application app;
	app.Run();
}


