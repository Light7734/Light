#pragma once

#include "Base.hpp"

#include <exception>
#include <inttypes.h>
#include <sstream>
#include <string>

namespace Light { namespace Exception {

	/**
    * @brief Base exception class, use this to terminate on error
    *
    * There's a try-catch(Exception) in main, throw this exception to log what went wrong and terminate
    **/
	class Exception : public std::exception
	{
	public:
		/**
        * @param file        file that generated the exception
        * @param line        line that generated the exception
        * @param module_uuid uuid of the module that generated the exception
        * @param module_name name of the module that generated the exception
        * @param statement   stringified statement(or expression) that generated the exception
        * @param error_code  developer error code eg. VULKANRENDERER_VALIDATION_ERROR
        * @param description detailed description of what went wrong
        */
		Exception(const char* file, uint32_t line, uint32_t module_uuid, const char* module_name, const char* statement, const char* error_code, const char* description)
		    : m_File(file), m_Line(line), m_ModuleUUID(module_uuid), m_ModuleName(module_name), m_Statement(statement), m_ErrorCode(error_code), m_Description(description) {}

		virtual const char* what()
		{
			std::ostringstream stream;

			stream << m_ErrorCode << " thrown:\n";
			stream << "Module    -> " << m_ModuleName << "(" << m_ModuleUUID << ")\n";
			stream << "Location  -> " << m_File << ":" << m_Line << '\n';
			stream << "Statement -> " << m_Statement << '\n';
			stream << "Description ->" << m_Description << '\n';

			// to hell with it
			static std::string what;
			what = stream.str();
			return what.c_str();
		}

	protected:
		const char* m_File;          // !< file that generated the exception
		const uint32_t m_Line;       // !< line that generated the exception
		const uint64_t m_ModuleUUID; // !< uuid of the module that generated the exception
		const char* m_ModuleName;    // !< name of the module that generated the exception
		const char* m_Statement;     // !< stringified statement(or expression) that generated the exception
		const char* m_ErrorCode;     // !< developer error code eg. VULKANRENDERER_VALIDATION_ERROR
		const char* m_Description;   // !< detailed description of what went wrong
	};

}} // namespace Light::Exception
