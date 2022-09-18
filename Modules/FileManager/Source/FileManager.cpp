#include "FileManager.hpp"

namespace Light {

	FileManagerModule* FileManager::self = {};

	FileManagerModule::FileManagerModule()
	    : Module(false)
	{
		FileManager::self = this;
	}

	FileManagerModule::~FileManagerModule()
	{
		FileManager::self = {};
	}

	void FileManagerModule::OnConfig()
	{
	}

	void FileManagerModule::OnInit()
	{
		LoggerCategoryCreateInfo categoryInfo {
			"FileManager",            // name
			LOGGER_DEFAULT_PATTERN,   // pattern
			LoggerType::eStdoutColor, // type
			NULL,                     // outputFile
		};

		Logger::CreateCategory(categoryInfo);
	}

	void FileManagerModule::OnUpdate()
	{
	}

	void FileManagerModule::OnDeinit()
	{
		m_TxtFiles.clear();
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
