#include "AnalogDigitalConverter.h"
#include "Debug.h"

AnalogDigitalConverter::AnalogDigitalConverter(uint32_t instance, adc16_converter_config_t adcConfig) : 
	mAdcConfig(adcConfig),
	mInstance(instance)
{
	ADC16_DRV_Init(mInstance, &mAdcConfig);
	for(uint32_t i = 0; i < mNumberOfChannels; i++)
		mAnalogInputs[i] = nullptr;
}

AnalogInput* AnalogDigitalConverter::CreateAnalogInput(uint32_t channelGroup, adc16_chn_config_t channelConfig)
{
	assert(channelConfig.chnIdx < mNumberOfChannels);
	mAnalogInputs[channelConfig.chnIdx] = new AnalogInput(mInstance, channelGroup, channelConfig);
	return mAnalogInputs[channelConfig.chnIdx];
}