#pragma once

#include "Source/Exception.hpp"

////////////////////////////////////////////////////////////////
/// ASSERT will throw Exception if x evaluates as false
#define ASSERT(x, ...)                                                                                          \
	{                                                                                                           \
		if (!(x))                                                                                               \
		{                                                                                                       \
			throw ::Light::Exception::Exception(__FILE__, __LINE__, MODULE_UUID, MODULE_NAME, #x, __VA_ARGS__); \
		}                                                                                                       \
	}


////////////////////////////////////////////////////////////////
/// D_ASSERT (Debug ASSERT) : same as ASSERT, but won't run (won't execute x either) on shipping builds
#ifndef LIGHT_BUILT_TYPE_SHIPPING
	#define D_ASSERT(x, ...)                                                                                       \
		{                                                                                                          \
			if (!(x))                                                                                              \
			{                                                                                                      \
				throw ::Light::Exception::Exception(__FILE__, __LINE__, MODULE_UUID, MODULE_NAME, #x, __VA_ARGS__) \
			}                                                                                                      \
		}
#else
	#define D_ASSERT(x, ...)
#endif

////////////////////////////////////////////////////////////////
/// CHECK will log err if x evaluates as false
#define CHECK(x, ...)              \
	{                              \
		if (!(x))                  \
		{                          \
			LOG(err, __VA_ARGS__); \
		}                          \
	}

////////////////////////////////////////////////////////////////
/// D_CHECK (debug CHECK) : same as CHECK, but won't run (won't execute x either) on shipping builds
/// Use this on non-shipping builds to call slow checkker functions to make sure everything is as it should be
#ifndef LIGHT_BUILT_TYPE_SHIPPING
	#define D_CHECK(x, ...)            \
		{                              \
			if (!(x))                  \
			{                          \
				LOG(err, __VA_ARGS__); \
			}                          \
		}
#else
	#define D_CHECK(x, ...)
#endif
