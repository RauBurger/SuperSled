/*
 * Debug.h
 *
 *  Created on: Apr 27, 2015
 *      Author: rrau
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef assert
#undef assert
#endif

inline void assert(bool ex)
{
	if(!ex)
	{
		int i = 0;
		while(true){}
	}
}

inline void __assert_func(const char *, int, const char *, const char *)
{
	while(true){}
}

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_H_ */
