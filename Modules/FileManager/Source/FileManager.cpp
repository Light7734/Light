#include "FileManager.hpp"

namespace Light {

	FileManagerModule* FileManager::self = {};

	FileManagerModule::FileManagerModule()
	{
		FileManager::self = this;

		LoggerCategoryCreateInfo categoryInfo {
			"FileManager",            // name
			LOGGER_DEFAULT_PATTERN,   // pattern
			LoggerType::eStdoutColor, // type
			NULL,                     // outputFile
		};

		Logger::CreateCategory(categoryInfo);
	}

	FileManagerModule::~FileManagerModule()
	{
		FileManager::self = {};

		m_TxtFiles.clear();
	}

	void FileManagerModule::OnTick()
	{
	}

	void FileManagerModule::OnSync()
	{
	}

	std::shared_ptr<TxtFile> FileManagerModule::Facade::CreateTxt(std::filesystem::path path)
	{
		std::filesystem::path filepath = path;

		if (!self->m_TxtFiles.contains(path))
		{
			std::filesystem::create_directories(path.remove_filename());
			self->m_TxtFiles[path] = std::make_shared<TxtFile>(filepath, true);
		}

		return self->m_TxtFiles[path];
	}

} // namespace Light
