#pragma once

#include "Logger/Module.hpp"
#include "Mojula/Module.hpp"

#include <unordered_map>

#include <filesystem>
#include <fstream>
#include <memory>
#include <ostream>

namespace Light {

	class TxtFile
	{
	public:
		TxtFile(std::filesystem::path path, bool overwrite)
		    : m_InputStream(path, std::ofstream::binary | std::ofstream::out | (overwrite ? std::ofstream::trunc : std::ofstream::app))
		{
		}

		~TxtFile()
		{
			m_InputStream.flush();
			m_InputStream.close();
		}

		void Append(const std::string& txt)
		{
			m_InputStream << txt;
		}

		////////////////////////////////////////////////////////////////
		/// Operators
		friend std::ofstream& operator<<(std::shared_ptr<TxtFile> file, const std::string& txt)
		{
			file->Append(txt);
			return file->m_InputStream;
		}

	private:
		std::ofstream m_InputStream = {};
	};


	////////////////////////////////////////////////////////////////
	/// File data types

	class FileManagerModule : public Module
	{
	public:
		FileManagerModule()
		    : Module(false) {}

		virtual ~FileManagerModule() {}

		////////////////////////////////////////////////////////////////
		/// Module Interface
		virtual void OnConfig() final override {}

		virtual void OnInit() final override
		{
			LoggerCategoryCreateInfo categoryInfo {
				"FileManager",          // name
				LOGGER_DEFAULT_PATTERN, // pattern
				LogType::eStdoutColor,  // type
				NULL,                   // outputFile
			};

			Logger::CreateCategory(categoryInfo);
		}

		virtual void OnUpdate() final override {}

		virtual void OnDeinit() final override
		{
			m_TxtFiles.clear();
		}

		////////////////////////////////////////////////////////////////
		/// Exposed functions
		std::shared_ptr<TxtFile> CreateTxt(std::filesystem::path path);

	private:
		std::unordered_map<std::filesystem::path, std::shared_ptr<TxtFile>> m_TxtFiles;
	};


	class FileManager
	{
	public:
		FileManager()  = delete;
		~FileManager() = delete;

		static void Init(FileManagerModule* module) { s_Module = module; }

		////////////////////////////////////////////////////////////////
		/// CREATE
		static std::shared_ptr<TxtFile> CreateTxt(std::filesystem::path path)
		{
			return s_Module->CreateTxt(path);
		}

	private:
		static FileManagerModule* s_Module;
	};

} // namespace Light
