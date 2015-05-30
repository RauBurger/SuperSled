#ifndef ANALOGDIGITALCONVERTER_H
#define ANALOGDIGITALCONVERTER_H

#include <fsl_adc16_driver.h>

#include "AnalogInput.h"

class AnalogDigitalConverter
{
public:
	AnalogDigitalConverter(uint32_t instance, adc16_converter_config_t adcConfig);

	AnalogInput* CreateAnalogInput(uint32_t channelGroup, adc16_chn_config_t channelConfig);
	
private:
	static const uint32_t mNumberOfChannels = 16;
	AnalogInput* mAnalogInputs[mNumberOfChannels];

    adc16_converter_config_t mAdcConfig;
	uint32_t mInstance;
};

#endif // ANALOGDIGITALCONVERTER_H