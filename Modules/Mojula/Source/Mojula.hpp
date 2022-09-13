#pragma once

#include <inttypes.h>

#include <string>
#include <vector>

namespace Light {

	/**
    * \brief Base module class
    *
    */
	class Module
	{
	public:
		/**
         * @param tickable          does the module's Update function needs to be called every frame?
        */
		Module(bool tickable = false)
		    : m_Tickable(tickable) {}

		Module(const Module&)            = delete;
		Module& operator=(const Module&) = delete;

		virtual ~Module() = default;

		virtual inline bool HasRequestedAppTermination() const { return false; }

		//! The name might be a bit misleading here, OnConfig doesn't mean when the module is configured but rather called to change other module's config struct using their API
		virtual void OnConfig() = 0;
		virtual void OnInit()   = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDeinit() = 0;

		inline bool IsTickable() const { return m_Tickable; }

	private:
		bool m_Tickable = false;
	};


} // namespace Light
