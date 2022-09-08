#pragma once

#include <inttypes.h>
#include <string>
#include <vector>

namespace Light { namespace Mojula {

	/**
    * \brief Base module class
    *
    */
	class Module
	{
	public:
		/**
         * @param name              name of the module
         * @param uuid              uuid of the module
         * @param dependencies_uuid uuid of the modules that this module depends on
         * @param tickable          does the module's Update function needs to be called every frame?
        */
		Module(const char* name, uint64_t uuid, std::vector<uint64_t> dependencies_uuid = {}, bool tickable = false)
		    : m_Name(name), m_UUID(uuid), m_DependenciesUUID(dependencies_uuid), m_Tickable(tickable) {}

		Module(const Module&)            = delete;
		Module& operator=(const Module&) = delete;

		virtual ~Module() = default;

		virtual inline bool HasRequestedAppTermination() const { return false; }

		virtual void StoreAPI(uint64_t module_uuid, class ModuleAPI* api) = 0;

		//! The name might be a bit misleading here, OnConfig doesn't mean when the module is configured but rather called to change other module's config struct using their API
		virtual void OnConfig() = 0;
		virtual void OnInit()   = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDeinit() = 0;

		inline const std::string& GetName() const { return m_Name; }

		inline uint64_t GetUUID() const { return m_UUID; }

		inline const std::vector<uint64_t>& GetDependenciesUUID() const { return m_DependenciesUUID; }

		inline bool IsTickable() const { return m_Tickable; }

	protected:
		std::string m_Name;
		const uint64_t m_UUID;

		std::vector<uint64_t> m_DependenciesUUID = {};

		bool m_Tickable = false;
	};

	/**
    * \brief Base module api class for exposing data &| functions to other modules
    *
    */
	class ModuleAPI
	{
	};


}} // namespace Light::Mojula
