#pragma once

#include "Log.h"

#ifdef AB_ENABLE_ASSERTS
	#define AB_ASSERT(x, ...) { if(!(x)) { AB_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define AB_ASSERT(x, ...)
#endif

#define INIT_GL_INFO 0

// set up OpenGL error checking