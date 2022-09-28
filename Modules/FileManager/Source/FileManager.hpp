#pragma once

#include "Logger/Module.hpp"
#include "Mojula/Module.hpp"

#include <unordered_map>

#include <filesystem>
#include <fstream>
#include <memory>
#include <ostream>

namespace Light {

	/** @brief Manipulates txt files 
     * @todo Docs */
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

		friend std::ofstream& operator<<(std::shared_ptr<TxtFile> file, const std::string& txt)
		{
			file->Append(txt);
			return file->m_InputStream;
		}

	private:
		std::ofstream m_InputStream = {};
	};

	/** @brief Manages file manipulation by creating file handler classes */
	class FileManagerModule : public Module
	{
	public:
		FileManagerModule();
		virtual ~FileManagerModule() override;

		////////////////////////////////////////////////////////////////
		/// Module Interface
		virtual void OnTick() override;
		virtual void OnSync() override;

	private:
		std::unordered_map<std::filesystem::path, std::shared_ptr<TxtFile>> m_TxtFiles;

	public:
		/** @brief Facade of the FileManagerModule */
		class Facade
		{
			friend FileManagerModule;

		public:
			Facade()  = delete;
			~Facade() = delete;

			/** @brief Creates txt file handle 
             *  @param path Path to txt file */
			static std::shared_ptr<TxtFile> CreateTxt(std::filesystem::path path);

		private:
			static FileManagerModule* self;
		};
	};

	using FileManager = FileManagerModule::Facade;

} // namespace Light
