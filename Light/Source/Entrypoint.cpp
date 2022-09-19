#include "ECS/Module.hpp"
#include "Exception/Module.hpp"
#include "FileManager/Module.hpp"
#include "Logger/Module.hpp"
#include "Mojula/Module.hpp"
#include "Profiler/Module.hpp"
#include "Time/Module.hpp"
#include "Window/Module.hpp"

#include <vector>

int main()
{
	using namespace Light;

	try
	{
		std::vector<Module*> modules;
		std::vector<Module*> tickableModules;

		////////////////////////////////////////////////////////////////
		/// Create and initialize the modules
		{
			// @todo This piece of code should be generated by a build script so we can customize which modules needs to be loaded
			modules.push_back(new LoggerModule());
			modules.push_back(new WindowModule());
			modules.push_back(new TimeModule());
			modules.push_back(new FileManagerModule());
			modules.push_back(new ProfilerModule());
			modules.push_back(new ECSModule());
		}

		for (auto* module : modules)
		{
			if (module->IsTickable())
			{
				tickableModules.push_back(module);
			}
		}

		for (auto* module : modules)
		{
			module->OnInit();
		}

		////////////////////////////////////////////////////////////////
		/// Main game loop
		{
			bool shouldTerminateApp = false;
			while (!shouldTerminateApp)
			{
				PROFILE_SCOPE("Frame");

				for (auto* module : tickableModules)
				{
					module->OnUpdate();
					shouldTerminateApp |= module->HasRequestedAppTermination();
				}
			}
		}

		////////////////////////////////////////////////////////////////
		/// Gracefully destroy everything
		{
			for (size_t i = modules.size() - 1ull; i < modules.size(); i--)
			{
				modules[i]->OnDeinit();
			}

			for (size_t i = modules.size() - 1ull; i < modules.size(); i--)
			{
				delete modules[i];
			}
		}
	}
	catch (Exception exception)
	{
		////////////////////////////////////////////////////////////////
		/// Save user progress if applicable

		////////////////////////////////////////////////////////////////
		/// Show/save information about what went wrong, where and when

		return -1;
	}

	return 0;
}
