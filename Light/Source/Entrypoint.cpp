#include "Exception.hpp"
#include "Logger.hpp"
#include "Mojula.hpp"
#include "Window.hpp"

#include <unordered_map>
#include <vector>

#define CREATE_MODULE(ModuleName)                                          \
	{                                                                      \
		using namespace Light;                                             \
		ModuleName::Module* module = new ModuleName::Module();             \
		modules.push_back(module);                                         \
		modulesAPI[module->GetUUID()] = new ModuleName::ModuleAPI(module); \
	}

int main()
{
	using namespace Light;

	try
	{
		std::vector<Mojula::Module*> modules;
		std::vector<Mojula::Module*> tickableModules;
		std::unordered_map<uint64_t, Mojula::ModuleAPI*> modulesAPI;

		////////////////////////////////////////////////////////////////
		/// Create the modules and set up pointers to dependencies
		{
			CREATE_MODULE(Logger);
			CREATE_MODULE(Window);
			// CREATE_MODULE(Profiler);
			// CREATE_MODULE(SplashScreen);
			// CREATE_MODULE(VkSplashScreenRenderer);
		}

		for (auto* module : modules)
		{
			if (module->IsTickable())
			{
				tickableModules.push_back(module);
			}

			for (uint64_t dependencyUUID : module->GetDependenciesUUID())
			{
				module->StoreAPI(dependencyUUID, modulesAPI[dependencyUUID]);
			}
		}

		for (auto* module : modules)
		{
			module->OnInit();
		}

		////////////////////////////////////////////////////////////////
		/// Enter the game loop
		{
			bool shouldTerminateApp = false;
			while (!shouldTerminateApp)
			{
				for (auto* module : tickableModules)
				{
					module->OnUpdate();
					shouldTerminateApp |= module->HasRequestedAppTermination();
				}
			}
		}

		////////////////////////////////////////////////////////////////
		/// Gracefully destruct everything
		{
			for (auto* module : modules) // do it in reverse order...
			{
				module->OnDeinit();
			}

			for (auto* module : modules) // do it in reverse order...
			{
				delete module;
			}
		}
	}
	catch (Exception::Exception exception)
	{
		////////////////////////////////////////////////////////////////
		/// Save user progress if applicable

		////////////////////////////////////////////////////////////////
		/// Show/save information about what went wrong, where and when

		// return exception.code
	}

	return 0;
}
