#include <filesystem>
#include <string>

class Paths
{
public:
	Paths()  = delete;
	~Paths() = delete;

	static std::filesystem::path GetTempDir()
	{
		return "Temp/";
	}

	static std::filesystem::path GetLogsDir()
	{
		return GetTempDir() / "Log/";
	}

	static std::filesystem::path GetCurrentDir()
	{
		return std::filesystem::current_path();
	}
};
