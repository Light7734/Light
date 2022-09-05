#include "Exception.hpp"
#include "Logger.hpp"
#include "Mojula.hpp"

#include <unordered_map>
#include <vector>

#define CREATE_MODULE(ModuleName)                                  \
	{                                                              \
		using namespace Light;                                     \
		Mojula::Module* module = new ModuleName::ModuleName();     \
		modules.push_back(module);                                 \
		modulesData[module->GetUUID()] = module->GetDataPointer(); \
	}

int main()
{
	using namespace Light;

	try
	{
		std::vector<Mojula::Module*> modules;
		std::vector<Mojula::Module*> tickableModules;
		std::unordered_map<uint64_t, Mojula::ModuleData*> modulesData;

		////////////////////////////////////////////////////////////////
		/// Create the modules and set up pointers to dependencies
		{
			CREATE_MODULE(Logger);
		}

		for (auto* module : modules)
		{
			if (module->IsTickable())
			{
				tickableModules.push_back(module);
			}

			for (uint64_t dependencyUUID : module->GetDependenciesUUID())
			{
				module->StoreDependencyDataPointer(dependencyUUID, modulesData[dependencyUUID]);
			}
		}

		////////////////////////////////////////////////////////////////
		/// Enter the game loop
		{
			for (auto* module : modules)
			{
				module->OnBirth();
			}

			while (true)
			{
				for (auto* module : tickableModules)
				{
					module->Tick();
				}
			}
		}

		////////////////////////////////////////////////////////////////
		/// Gracefully destruct everything
		{
			for (auto* module : modules) // do it in reverse order...
			{
				module->OnDeath();
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
		{
			//     ???
		}

		////////////////////////////////////////////////////////////////
		/// Show/save information about what went wrong, where and when
		{
            std::cout << exception.what();
		}

		// return exception.code
	}

	return 0;
}
