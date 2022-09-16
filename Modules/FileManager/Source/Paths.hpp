#include <filesystem>
#include <string>

namespace Light {

	/** @brief Used to retrive directory paths */
	class Paths
	{
	public:
		Paths()  = delete;
		~Paths() = delete;

		/** @return Path to temporary files directory */
		static std::filesystem::path GetTempDir()
		{
			return "Temp/";
		}

		/** @return Path to Log files directory */
		static std::filesystem::path GetLogsDir()
		{
			return GetTempDir() / "Log/";
		}

		/** @return Current path */
		static std::filesystem::path GetCurrentDir()
		{
			return std::filesystem::current_path();
		}
	};

} // namespace Light
