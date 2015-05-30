#include "AnalogInput.h"
#include "Debug.h"

AnalogInput::AnalogInput(uint32_t instance, uint32_t channelGroup, adc16_chn_config_t channelConfig) :
	mInstance(instance),
	mChannelConfig(channelConfig),
	mChannelGroup(channelGroup)
{
	ADC16_DRV_ConfigConvChn(mInstance, mChannelGroup, &mChannelConfig);
}

uint16_t AnalogInput::Read()
{
	return ADC16_DRV_GetConvValueRAW(mInstance, mChannelGroup);
}