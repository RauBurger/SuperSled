/* ###################################################################
**     Filename    : Events.h
**     Project     : SuperSled
**     Processor   : MK22FN512VLH12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-04-25, 14:49, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file Events.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "fsl_device_registers.h"
#include "clockMan1.h"
#include "pin_init.h"
#include "gpio1.h"
#include "i2cCom1.h"
#include "layer1.h"
#include "uartCom1.h"
#include "spiCom1.h"
#include "dmaController1.h"
#include "adConv1.h"
#include "voltageRef1.h"

#ifdef __cplusplus
extern "C" {
#endif 


void adConv1_OnAdcDone0(void);

/*! adConv1 IRQ handler */
void ADC1_IRQHandler(void);

/*! spiCom1 IRQ handler */
void SPI0_IRQHandler(void);

void uartCom1_RxCallback(uint32_t instance, void * uartState);

void uartCom1_TxCallback(uint32_t instance, void * uartState);

/*! i2cCom1 IRQ handler */
void I2C0_IRQHandler(void);

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __Events_H*/
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.4 [05.11]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
