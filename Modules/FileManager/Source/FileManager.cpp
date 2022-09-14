#include "FileManager.hpp"

namespace Light {

	FileManagerModule* FileManager::s_Module = {};

	std::shared_ptr<TxtFile> FileManagerModule::CreateTxt(std::filesystem::path path)
	{
		if (!m_TxtFiles.contains(path))
		{
			std::filesystem::create_directories(path.remove_filename());
			m_TxtFiles[path] = std::make_shared<TxtFile>(path, true);
		}

		return m_TxtFiles[path];
	}


} // namespace Light
