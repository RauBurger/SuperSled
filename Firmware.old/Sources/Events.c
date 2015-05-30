/* ###################################################################
**     Filename    : Events.c
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
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

void adConv1_OnAdcDone0(void)
{
  /* Write your code here ... */
}

/*! adConv1 IRQ handler */
void ADC1_IRQHandler(void)
{
  /* Write your code here ... */
}

/*! spiCom1 IRQ handler */
void SPI0_IRQHandler(void)
{
#if SPICOM1_DMA_MODE
  DSPI_DRV_EdmaIRQHandler(FSL_SPICOM1);
#else
  DSPI_DRV_IRQHandler(FSL_SPICOM1);
#endif
  /* Write your code here ... */
}

void uartCom1_RxCallback(uint32_t instance, void * uartState)
{
  /* Write your code here ... */
}

void uartCom1_TxCallback(uint32_t instance, void * uartState)
{
  /* Write your code here ... */
}

/*! i2cCom1 IRQ handler */
void I2C0_IRQHandler(void)
{
  I2C_DRV_IRQHandler(FSL_I2CCOM1);
}

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

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
