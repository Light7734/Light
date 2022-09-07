#include "Exception.hpp"
#include "Logger.hpp"
#include "Mojula.hpp"

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
		Logger::ModuleAPI* loggerAPI;

		////////////////////////////////////////////////////////////////
		/// Create the modules and set up pointers to dependencies
		{
			CREATE_MODULE(Logger);
			loggerAPI = static_cast<Logger::ModuleAPI*>(modulesAPI[69ull]);
			loggerAPI->CreateCategory("Main");

			LOG(Main, info, "Constructing module(s)...");
		}

		LOG(Main, info, "Constructed {} module(s)", modules.size());

		LOG(Main, info, "Storing modules APIs...");
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

		////////////////////////////////////////////////////////////////
		/// Enter the game loop
		{
			LOG(Main, info, "Initializing module(s)...");
			for (auto* module : modules)
			{
				module->OnInit();
			}

			LOG(Main, info, "Entering main loop with {} tickable module(s)", tickableModules.size());
			while (true)
			{
				for (auto* module : tickableModules)
				{
					module->OnUpdate();
				}

				break;
			}
		}

		////////////////////////////////////////////////////////////////
		/// Gracefully destruct everything
		{
			LOG(Main, info, "De-initializing {} module(s)... bye!", modules.size());
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
