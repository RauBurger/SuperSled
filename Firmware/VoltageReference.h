#ifndef VOLTAGEREFERENCE_H
#define VOLTAGEREFERENCE_H

#include <stdint.h>

class VoltageReference
{
public:

	typedef enum
	{
		BufferMode_Bandgap = 0U,
		BufferMode_HighPower = 1U,
		BufferMode_LowPower = 2U
	} BufferMode;

	VoltageReference(bool regulatorEnabled, bool chopEnabled, uint8_t trim, bool secondOrderCurve, BufferMode bufferMode);

	void Enable();
	void Disable();
private:

};

#endif // VOLTAGEREFERENCE_H