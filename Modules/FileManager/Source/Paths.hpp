#include <filesystem>
#include <string>

class Paths
{
public:
	Paths()  = delete;
	~Paths() = delete;

	static std::filesystem::path GetLogDir()
	{
		return "Temp/Log";
	}

	constexpr std::string GetEngine()
	{
		return "/";
	}
};
