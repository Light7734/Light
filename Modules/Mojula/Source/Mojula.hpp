#pragma once

#include <inttypes.h>
#include <string>
#include <vector>

namespace Light { namespace Mojula {

	/**
    * \brief Base module data struct for sharing data between modules
    *
    */
	struct ModuleData
	{
	};

	/**
    * \brief Base module class
    *
    */
	class Module
	{
	public:
		Module(std::string name, uint64_t uuid, std::vector<uint64_t> dependencies_uuid = {}, ModuleData* exposed_data = nullptr, bool tickable = false)
		    : m_Name(name), m_UUID(uuid), m_DependenciesUUID(dependencies_uuid), m_ExposedData(exposed_data), m_Tickable(tickable) {}

		Module(const Module&)            = delete;
		Module& operator=(const Module&) = delete;

		virtual ~Module() = default;

		virtual void OnBirth() = 0;
		virtual void OnDeath() = 0;

		virtual void Tick() = 0;

		virtual void StoreDependencyDataPointer(uint64_t module_uuid, ModuleData* dependency_data) = 0;

		inline const std::string& GetName() const { return m_Name; }

		inline uint64_t GetUUID() const { return m_UUID; }

		inline const std::vector<uint64_t>& GetDependenciesUUID() const { return m_DependenciesUUID; }

		inline ModuleData* GetDataPointer() { return m_ExposedData; }

		inline bool IsTickable() const { return m_Tickable; }

	protected:
		std::string m_Name;
		const uint64_t m_UUID;

		std::vector<uint64_t> m_DependenciesUUID = {};

		ModuleData* m_ExposedData = nullptr;

		bool m_Tickable = false;
	};

}} // namespace Light::Mojula
