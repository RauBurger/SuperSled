#ifndef ANALOGINPUT_H
#define ANALOGINPUT_H

#include <stdint.h>

#include <fsl_adc16_driver.h>

class AnalogInput
{
public:
	uint16_t Read();

private:
	friend class AnalogDigitalConverter;
	AnalogInput(uint32_t instance, uint32_t channelGroup, adc16_chn_config_t channelConfig);

	adc16_chn_config_t mChannelConfig;
	uint32_t mInstance;
	uint32_t mChannelGroup;
};

#endif // ANALOGINPUT_H