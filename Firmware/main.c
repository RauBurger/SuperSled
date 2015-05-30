#include <__cross_studio_io.h>
#include "Application.h"
#include <fsl_interrupt_manager.h>

void main(void)
{
    INT_SYS_EnableIRQGlobal();

	StartApp();

	debug_exit(0);
}
