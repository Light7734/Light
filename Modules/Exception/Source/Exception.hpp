#pragma once

#include <inttypes.h>

#include <exception>
#include <sstream>
#include <string>

/** @def ASSERT
* Throws Light::Exception upon false evaluation of x
* @param x The statement/expression to evaluate
* @param description The reason for termination upon false evaluation of x
* @note Won't evaluate x on non-debug builds
*/
#define ASSERT(x, description)                                             \
	{                                                                      \
		if (!(x))                                                          \
		{                                                                  \
			throw ::Light::Exception(__FILE__, __LINE__, #x, description); \
		}                                                                  \
	}

/** @def DASSERT
* Throws Light::Exception upon false evaluation of x
* @param x The statement/expression to evaluate
* @param description The reason for termination upon false evaluation of x
* @note Won't evaluate x on non-debug builds
*/
#ifdef LIGHT_BUILD_TYPE_DEBUG
	#define DASSERT(x, description)                                           \
		{                                                                     \
			if (!(x))                                                         \
			{                                                                 \
				throw ::Light::Exception(__FILE__, __LINE__, #x, description) \
			}                                                                 \
		}
#else
	#define DASSERT(x, ...)
#endif

/** @def CHECK
* Logs @a ... upon false evaluation of @a x
* @param x The statement/expression to evaluate
* @param ... The vardiadic argument to log
*/
#define CHECK(x, ...)              \
	{                              \
		if (!(x))                  \
		{                          \
			LOG(err, __VA_ARGS__); \
		}                          \
	}

/** @def DCHECK
* Logs @a ... upon false evaluation of @a x
* @param x The statement/expression to evaluate
* @param ... The vardiadic argument to log
* @note Won't evaluate x on non-debug builds
*/
#ifdef LIGHT_BUILD_TYPE_DEBUG
	#define DCHECK(x, ...)             \
		{                              \
			if (!(x))                  \
			{                          \
				LOG(err, __VA_ARGS__); \
			}                          \
		}
#else
	#define DCHECK(x, ...)
#endif

namespace Light {

	/** @brief Throw this to terminate the application on error
     @note Only use this exception for termination (try-catch is in main for handling this)
    */
	class Exception : public std::exception
	{
	public:
		/**
        * @param file        file that generated the exception
        * @param line        line that generated the exception
        * @param statement   stringified statement(or expression) that generated the exception
        * @param description detailed description of what went wrong
        */
		Exception(const char* file, uint32_t line, const char* statement, const char* description)
		    : m_File(file), m_Line(line), m_Statement(statement), m_Description(description) {}

		virtual const char* what()
		{
			std::ostringstream stream;

			stream << "[ EXCEPTION ]:\n";
			stream << "Location  -> " << m_File << ":" << m_Line << '\n';
			stream << "Statement -> " << m_Statement << '\n';
			stream << "Description ->" << m_Description << '\n';

			// to hell with it
			static std::string wtf;
			wtf = stream.str();
			return wtf.c_str();
		}

	protected:
		const char* m_File;
		const uint32_t m_Line;
		const char* m_Statement;
		const char* m_Description;
	};

} // namespace Light
