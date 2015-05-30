/*
 * Application.cpp
 *
 *  Created on: Apr 25, 2015
 *      Author: rrau
 */

#include "Application.h"
#include "InterruptManager.h"
#include "pin_init.h"

#include <fsl_i2c_master_driver.h>
#include <fsl_uart_driver.h>
#include <fsl_gpio_driver.h>
#include <fsl_adc16_driver.h>
#include <fsl_clock_manager.h>
#include <fsl_lptmr_hal.h>
#include <fsl_port_hal.h>
#include <fsl_device_registers.h>
#include <fsl_sim_hal.h>
#include <fsl_mcg_hal.h>
#include <fsl_clock_MK22F51212.h>

#include <__cross_studio_io.h>
#include <string.h>

typedef enum
{
	PA5 = GPIO_MAKE_PIN(GPIOA_IDX, 5U),
   	PA19 = GPIO_MAKE_PIN(GPIOA_IDX, 19U)
} GpioAPins;

#define FSL_I2CCOM1 0
#define FSL_ADCONV1 1

void adConv1_OnAdcDone0()
{
}

Application::Application()
{
	OSA_Init();

	// Enable clock for PORTs
	SIM_HAL_EnableClock(SIM,kSimClockGatePortA);
	SIM_HAL_EnableClock(SIM,kSimClockGatePortB);
	SIM_HAL_EnableClock(SIM,kSimClockGatePortC);
	SIM_HAL_EnableClock(SIM,kSimClockGatePortD);
	SIM_HAL_EnableClock(SIM,kSimClockGatePortE);

	// Setup board clock source.
	g_xtal0ClkFreq = 8000000U;            // Value of the external crystal or oscillator clock frequency of the system oscillator (OSC) in Hz
	g_xtalRtcClkFreq = 32768U;            // Value of the external 32k crystal or oscillator clock frequency of the RTC in Hz

	configure_gpio_pins(0U);
	configure_i2c_pins(0U);
	configure_jtag_pins(0U);
	configure_osc_pins(0U);
	configure_spi_pins(0U);
	configure_tpiu_pins(0U);
	configure_uart_pins(1U);

	mVref = new VoltageReference(true, true, 64, false, VoltageReference::BufferMode::BufferMode_HighPower);
	mVref->Enable();

    adc16_converter_config_t adConv1_InitConfig0;
	adConv1_InitConfig0.lowPowerEnable = false;
	adConv1_InitConfig0.clkDividerMode = kAdc16ClkDividerOf1;
	adConv1_InitConfig0.longSampleTimeEnable = false;
	adConv1_InitConfig0.resolution = kAdc16ResolutionBitOf16;
	adConv1_InitConfig0.clkSrc = kAdc16ClkSrcOfBusClk;
	adConv1_InitConfig0.asyncClkEnable = false;
	adConv1_InitConfig0.highSpeedEnable = false;
	adConv1_InitConfig0.longSampleCycleMode = kAdc16LongSampleCycleOf24;
	adConv1_InitConfig0.hwTriggerEnable = false;
	adConv1_InitConfig0.refVoltSrc = kAdc16RefVoltSrcOfVref;
	adConv1_InitConfig0.continuousConvEnable = true;
	adConv1_InitConfig0.dmaEnable = false;

	adc16_chn_config_t adConv1_ChnConfig0;
	adConv1_ChnConfig0.chnIdx = kAdc16Chn0d;
	adConv1_ChnConfig0.convCompletedIntEnable = false;
	adConv1_ChnConfig0.diffConvEnable = false;

	mAdc0 = new AnalogDigitalConverter(0, adConv1_InitConfig0);
	mAdc1 = new AnalogDigitalConverter(1, adConv1_InitConfig0);

	mTemp1 = mAdc0->CreateAnalogInput(0, adConv1_ChnConfig0);
	mTemp2 = mAdc1->CreateAnalogInput(0, adConv1_ChnConfig0);
	//InterruptManager::Init();

	//mI2CMaster = new I2CMaster(0, 100, 0x50);
}

Application::~Application()
{

}

void Application::Run()
{
	i2c_master_state_t i2cCom1_MasterState;
	i2c_device_t i2cCom1_MasterConfig0;

	i2cCom1_MasterConfig0.baudRate_kbps = 100U;
	i2cCom1_MasterConfig0.address = 40U;

	gpio_output_pin_user_config_t gpio1_OutConfig[3];

	gpio1_OutConfig[0].pinName = PA19;
	gpio1_OutConfig[0].config.outputLogic = 0;
	gpio1_OutConfig[0].config.slewRate = kPortFastSlewRate;
	gpio1_OutConfig[0].config.isOpenDrainEnabled = false;
    gpio1_OutConfig[0].config.driveStrength = kPortLowDriveStrength;

    gpio1_OutConfig[1].pinName = PA5;
	gpio1_OutConfig[1].config.outputLogic = 0;
	gpio1_OutConfig[1].config.slewRate = kPortFastSlewRate;
	gpio1_OutConfig[1].config.isOpenDrainEnabled = false;
    gpio1_OutConfig[1].config.driveStrength = kPortLowDriveStrength;

	gpio1_OutConfig[2].pinName = GPIO_PINS_OUT_OF_RANGE;

	I2C_DRV_MasterInit(FSL_I2CCOM1, &i2cCom1_MasterState);

	GPIO_DRV_Init(NULL, gpio1_OutConfig);

	GPIO_DRV_ClearPinOutput(PA5);
	GPIO_DRV_SetPinOutput(PA19);

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
	
		//I2CMaster::I2CStatus err;// = mI2CMaster->WriteAddress(100, I2CMaster::Direction_Read, true);
		/*
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
		*/
		/*
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
		
		uint16_t temp1Val = mTemp1->Read();
		float temp1Voltage = (float)temp1Val/65536.0F * 3.3F;

		uint16_t temp2Val = mTemp2->Read();
		float temp2Voltage = (float)temp1Val/65536.0F * 3.3F;

		debug_printf("pressure = %d\t\ttemp1Val = %d\ttemp1Voltage = %f\t\ttemp2Val = %d\ttemp2Voltage = %f\r", pressure, temp1Val, temp1Voltage, temp2Val, temp2Voltage);

        GPIO_DRV_TogglePinOutput(PA19);
        GPIO_DRV_TogglePinOutput(PA5);
	}
}

void StartApp()
{
	Application app;
	app.Run();
}


