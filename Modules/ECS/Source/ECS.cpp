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

	void ECSModule::OnTick()
	{
	}

	void ECSModule::OnSync()
	{
	}

} // namespace Light
