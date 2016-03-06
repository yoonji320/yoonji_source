
/// @file		debug.h
/// @brief		디버그 메시지 출력 매크로
/// @author		hansch
/// @date		2007-12-27

#ifndef __DEBUG_H
#define __DEBUG_H

#ifdef DEBUG

	#include <stdio.h>
	#include <string.h>
	#include <unistd.h>
	#include <errno.h>

	#define debug(msg, args...)		fprintf(stderr, "##### DEBUG : PID [%d], FILE [%s], LINE [%d], ERROR [%s] MSG ["msg"]\n", getpid(), __FILE__, __LINE__, strerror(errno), ##args); fflush(stderr)

#else

	#define debug(msg, args...)

#endif	// DEBUG

#endif	// __DEBUG_H

