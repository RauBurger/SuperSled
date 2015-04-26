/** ###################################################################
**      Filename    : PE_low_level_init.c
**      Processor   : MK22F51212
**      Version     : 1.0
**      Abstract    :
**         This file implements the MCU initialization and CPU component runtime methods.
** 
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file PE_low_level_init.c                                                  
** @version 01.00
** @brief
**         This file implements the MCU initialization and CPU component runtime methods.
*/         
/*!
**  @addtogroup PE_low_level_init_module PE_low_level_init module documentation
**  @{
*/         

/* MODULE PE_low_level_init.c */

#include "Events.h"
#include "Cpu.h"
#if CPU_HARDWARE_INIT
  #include "board.h"
#endif
#if CPU_PERIPHERALS_INIT
  #include "Peripherals_Init.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
** ===================================================================
**     Method      :  PE_low_level_init
**
**     Description :
**         Initializes components and provides common register 
**         initialization. The method is called automatically as a part 
**         of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void PE_low_level_init(void)
{
  /* RTOS initialization */

  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  
  /* Global routing function */
  #if CPU_HARDWARE_INIT
  hardware_init();
  #endif /* CPU_HARDWARE_INIT */
  
  /* Common initialization */
  
  #if CPU_COMMON_INIT
  Common_Init();
  #endif /* CPU_COMMON_INIT */
  
  /* Peripheral initialization components initialization */
  
  #if CPU_PERIPHERALS_INIT
  Peripherals_Init();
  #endif /* CPU_PERIPHERALS_INIT */
  
  /* Rest of components initialization */
  
  #if CPU_COMPONENTS_INIT
  Components_Init();
  #endif /* CPU_COMPONENTS_INIT */
  
}

#ifdef __cplusplus
}
#endif

/* END PE_low_level_init.c */

/*!
** @}
*/
/*
** ###################################################################
**
**     This file is a part of Processor Expert static initialization
**     library for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
