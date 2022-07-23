#pragma once

#include <GLAD/glad.h>
#include <engine/Log.h>

const char* GetMessageType(GLenum type)
{
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR: return "GL_DEBUG_TYPE_ERROR";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
	case GL_DEBUG_TYPE_PORTABILITY: return "GL_DEBUG_TYPE_PORTABILITY";
	case GL_DEBUG_TYPE_PERFORMANCE: return "GL_DEBUG_TYPE_PERFORMANCE";
	case GL_DEBUG_TYPE_MARKER: return "GL_DEBUG_TYPE_MARKER";
	case GL_DEBUG_TYPE_PUSH_GROUP: return "GL_DEBUG_TYPE_PUSH_GROUP";
	case GL_DEBUG_TYPE_POP_GROUP: return "GL_DEBUG_TYPE_POP_GROUP";
	case GL_DEBUG_TYPE_OTHER: return "GL_DEBUG_TYPE_OTHER";
	default: return "UNRECOGNIZED MESSAGE TYPE";
	}
}

const char* GetSeverityName(GLenum severity)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
	case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM (WARNINGS)";
	case GL_DEBUG_SEVERITY_LOW: return "LOW (LESSER WARNINGS)";
	case GL_DEBUG_SEVERITY_NOTIFICATION: return "INFO";
	default: return "UNRECOGNIZED GL SEVERITY TYPE";
	}
}

// see if later add source and other stuff
void APIENTRY MessageCallback(GLenum source, GLenum type, GLenum id, GLenum severity, GLenum length, const GLchar* message, const void* useParam)
{
	if (type == GL_DEBUG_TYPE_ERROR)
	{
		AB_ERROR("GL CALLBACK [{0}]-[{1}] Message: {2}", GetMessageType(type), GetSeverityName(severity), message);
		AB_ASSERT(false, "REVIEW ABOVE ERROR");
	}
#if INIT_GL_INFO
	else
		AB_INFO("GL CALLBACK[{0}]-[{1}] Message: {2}", GetMessageType(type), GetSeverityName(severity), message); 
#endif
}
