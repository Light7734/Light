#include "ECS.hpp"

namespace Light {

	ECSModule* ECS::self = {};

	ECSModule::ECSModule()
	    : m_Registry()
	{
		ECS::self = this;
	}

	ECSModule::~ECSModule()
	{
		ECS::self = {};
	}

	////////////////////////////////////////////////////////////////
	/// Module Interface
	void ECSModule::OnConfig()
	{
	}

	void ECSModule::OnInit()
	{
	}

	void ECSModule::OnUpdate()
	{
	}

	void ECSModule::OnDeinit()
	{
	}

	Entity ECSModule::Facade::CreateEntity()
	{
		return self->m_Registry.create();
	}

} // namespace Light
