/// @file	thread.h
/// @brief	������
///	@author	yoonzz320
///	@date	2016-02-03

#ifndef __THREAD_H
#define __THREAD_H

typedef void* (*ThreadFunc)(void*);

extern int ThreadStart(
					ThreadFunc	pRun,
					void*		pServerArg);

#endif	// __THREAD_H
