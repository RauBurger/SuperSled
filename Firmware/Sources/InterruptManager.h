/*
 * InterruptManager.h
 *
 *  Created on: Apr 26, 2015
 *      Author: rrau
 */

#ifndef INTERRUPTMANAGER_H_
#define INTERRUPTMANAGER_H_

#include <stdint.h>

#include "Debug.h"

class IInterruptDelegate
{
public:
	virtual void Invoke() = 0;

};

template <class T>
class InterruptDelegate : public IInterruptDelegate
{
public:

	typedef void(T::*IrqHandler)();

	InterruptDelegate(T* o, IrqHandler f)
	{
		mObject = o;
		mIrqHandler = f;
	}

	void Invoke()
	{
		(mObject->*mIrqHandler)();
	}

private:
	T* mObject;
	IrqHandler mIrqHandler;
};

class InterruptManager
{
public:

	static void Init();

	static void InstallInterrupt(int32_t irqNum, IInterruptDelegate* delegate)
	{
		assert(irqNum < mNumIrqs);
		mInterruptDelegates[irqNum] = delegate;
	}

	static void I2C0Interrupt();

private:
	static const uint32_t mNumIrqs;
	static IInterruptDelegate* mInterruptDelegates[];
	static bool mInitialized;

	static uint32_t mI2C0irq;
};


#endif /* INTERRUPTMANAGER_H_ */
