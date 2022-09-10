#pragma once

#include "Timer.hpp"

#include <Mojula/Module.hpp>

#include "Base.hpp"

namespace Light { namespace Time {

	class Module : public Mojula::Module
	{
	public:
		Module()
		    : Mojula::Module(MODULE_NAME, MODULE_UUID, {}, true) {}

		virtual void StoreAPI(uint64_t module_uuid, class Mojula::ModuleAPI* api) final override {};

		//! The name might be a bit misleading here, OnConfig doesn't mean when the module is configured but rather called to change other module's config struct using their API
		virtual void OnConfig() final override;
		virtual void OnInit() final override;

		virtual void OnUpdate() final override
		{
			static bool firstFrame = true;

			m_DeltaTime = firstFrame ? 1.0 / 60.0 : m_Timer.ElapsedTime();
			firstFrame  = false;

			m_Timer.Reset();
		}

		virtual void OnDeinit() final override;


		////////////////////////////////////////////////////////////////
		//// API Functions
		inline double GetDeltaTime() const { return m_DeltaTime; }

	private:
		Timer m_Timer;
		double m_DeltaTime;
	};

	class ModuleAPI
	{
	public:
		ModuleAPI(Module* module)
		    : m_Module(module) {}

		inline double GetDeltaTime() const { return m_Module->GetDeltaTime(); }

	private:
		Module* m_Module;
	};

}} // namespace Light::Time
