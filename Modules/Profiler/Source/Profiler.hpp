#pragma once

#include "FileManager/Module.hpp"
#include "Mojula/Module.hpp"
#include "Time/Module.hpp"

#include <fstream>
#include <memory>
#include <ostream>

/** @def PROFILE_SCOPE(name)
 * Measures the execution time of the entire scope and submits the results to the Profiler
 * @note Use on the first line of the scope
 * @param name Name of the scope */
#define PROFILE_SCOPE(name)                    PROFILE_SCOPE_INDIRECTION(name, __LINE__)
#define PROFILE_SCOPE_INDIRECTION(name, line)  PROFILE_SCOPE_INDIRECTION2(name, line)
#define PROFILE_SCOPE_INDIRECTION2(name, line) ::Light::ScopeProfiler scope_profiler##line(name)

namespace Light {

	/** @todo Docs */
	struct ScopeResult
	{
		const char* name;
		uint64_t start, duration;
		uint32_t threadID;
	};

	/** @brief Handles measuring the execution speed of functions
     * @todo Measure things other than the execution speed
     */
	class ProfilerModule final : public Module
	{
	public:
		ProfilerModule();
		virtual ~ProfilerModule() override;

		////////////////////////////////////////////////////////////////
		/// Module Interface
		virtual void OnTick() override;
		virtual void OnSync() override;

	private:
		std::shared_ptr<TxtFile> m_OutputFile = {};

	public:
		/** @brief Facade of the ProfilerModule */
		class Facade
		{
			friend ProfilerModule;

		public:
			Facade()  = delete;
			~Facade() = delete;

			/** @brief Submits the scope profiler's results to be saved 
             * @param scope_result The scope profiler's result */
			static void SubmitScopeResult(const ScopeResult& scope_result);

		private:
			static ProfilerModule* self;
		};
	};

	using Profiler = ProfilerModule::Facade;

	/** @brief Measures execution speed of a scope using RAII magic */
	class ScopeProfiler
	{
	public:
		ScopeProfiler(const char* name);

		~ScopeProfiler();

	private:
		const char* m_Name;
		const uint64_t m_Start;
	};

} // namespace Light
