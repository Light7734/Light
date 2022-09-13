#include "Exception/Module.hpp"
#include "Logger/Module.hpp"
#include "Mojula/Module.hpp"
#include "Time/Module.hpp"
#include "Window/Module.hpp"

#include <unordered_map>

#include <vector>

#define CREATE_MODULE(ModuleName)                              \
	{                                                          \
		using namespace Light;                                 \
		ModuleName##Module* module = new ModuleName##Module(); \
		modules.push_back(module);                             \
		ModuleName::Init(module);                              \
	}

int main()
{
	using namespace Light;

	try
	{
		std::vector<Module*> modules;
		std::vector<Module*> tickableModules;

		////////////////////////////////////////////////////////////////
		/// Create the modules and set up pointers to dependencies
		{
			CREATE_MODULE(Logger);
			CREATE_MODULE(Window);
			CREATE_MODULE(Time);
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
