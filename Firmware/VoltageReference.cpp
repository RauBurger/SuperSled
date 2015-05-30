#include "VoltageReference.h"
#include <MK22F51212.h>
#include <fsl_clock_manager.h>

VoltageReference::VoltageReference(bool regulatorEnabled, bool chopEnabled, uint8_t trim, bool secondOrderCurve, BufferMode bufferMode)
{
	uint8_t trmVal = 0;
	uint8_t scVal = 0;

	CLOCK_SYS_EnableVrefClock(0);

	chopEnabled ? trmVal |= (1 << VREF_TRM_CHOPEN_SHIFT) : trmVal = trmVal;
	trmVal |= (trim & VREF_TRM_TRIM_MASK) << VREF_TRM_TRIM_SHIFT;
	VREF->TRM = trmVal;

	regulatorEnabled ? scVal |= (1 << VREF_SC_REGEN_SHIFT) : scVal = scVal;
	secondOrderCurve ? scVal |= (1 << VREF_SC_ICOMPEN_SHIFT) : scVal = scVal;
	scVal |= ((uint8_t)bufferMode << VREF_SC_MODE_LV_SHIFT) & VREF_SC_MODE_LV_MASK;

	VREF->SC = scVal;
}

void VoltageReference::Enable()
{
	uint8_t scVal = VREF->SC;
	scVal |= (1 << VREF_SC_VREFEN_SHIFT);

	VREF->SC = scVal;

	while(VREF->SC & VREF_SC_VREFST_MASK) {}
}

void VoltageReference::Disable()
{
	uint8_t scVal = VREF->SC;
	scVal &= ~(1 << VREF_SC_VREFEN_SHIFT);

	VREF->SC = scVal;
}