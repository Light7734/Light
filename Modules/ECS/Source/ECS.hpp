#pragma once

#include "Mojula/Module.hpp"

#include <entt/entt.hpp>

namespace Light {

	using Entity = ::entt::entity;

	class ECSModule final : public Module
	{
	public:
		ECSModule();
		virtual ~ECSModule() override;

		////////////////////////////////////////////////////////////////
		/// Module Interface
		virtual void OnTick() override;
		virtual void OnSync() override;

	private:
		entt::registry m_Registry;

	public:
		/** @brief Facade of ECSModule */
		class Facade
		{
			friend ECSModule;

		public:
			Facade()  = delete;
			~Facade() = delete;

			static inline Entity CreateEntity()
			{
				return self->m_Registry.create();
			}

			template<typename... Components>
			static inline auto Group()
			{
			}

			template<typename T, typename... Args>
			static inline T& AddComponent(Entity entity, Args&&... args)
			{
				return self->m_Registry.emplace<T>(entity, std::forward<Args>(args)...);
			}

			template<typename T>
			static inline T& GetComponent(Entity entity)
			{
				return self->m_Registry.get<T>(entity);
			}

			template<typename T>
			static inline bool HasComponent(Entity entity)
			{
				return self->m_Registry.any_of<T>(entity);
			}

			template<typename T>
			static inline size_t RemoveComponent(Entity entity)
			{
				return self->m_Registry.remove<T>(entity);
			}

			static inline uint32_t GetUUID(Entity entity)
			{
				return static_cast<uint32_t>(entity);
			}

		private:
			static ECSModule* self;
		};
	};

	using ECS = ::Light::ECSModule::Facade;

} // namespace Light
