/*
 * Application.h
 *
 *  Created on: Apr 25, 2015
 *      Author: rrau
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#ifdef __cplusplus

class Application
{
public:
	Application();
	~Application();

	void Run();
private:


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
