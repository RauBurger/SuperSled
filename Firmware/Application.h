/*
 * Application.h
 *
 *  Created on: Apr 25, 2015
 *      Author: rrau
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#ifdef __cplusplus
#include "I2CMaster.h"
#include "VoltageReference.h"
#include "AnalogDigitalConverter.h"

class Application
{
public:
	Application();
	~Application();

	void Run();
private:
	VoltageReference* mVref;
	I2CMaster* mI2CMaster;
	AnalogDigitalConverter* mAdc0;
    AnalogDigitalConverter* mAdc1;
	AnalogInput* mTemp1;
	AnalogInput* mTemp2;
};

#endif

#ifdef __cplusplus
extern "C" {
#endif

void StartApp();

#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_H_ */
