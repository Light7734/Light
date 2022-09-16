#include "FileManager.hpp"

namespace Light {

	FileManagerModule* FileManager::s_Module = {};

	FileManagerModule::FileManagerModule()
	    : Module(false)
	{
	}

	FileManagerModule::~FileManagerModule()
	{
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

	std::shared_ptr<TxtFile> FileManagerModule::CreateTxt(std::filesystem::path path)
	{
		std::filesystem::path filepath = path;

		if (!m_TxtFiles.contains(path))
		{
			std::filesystem::create_directories(path.remove_filename());
			m_TxtFiles[path] = std::make_shared<TxtFile>(filepath, true);
		}

		return m_TxtFiles[path];
	}


} // namespace Light
