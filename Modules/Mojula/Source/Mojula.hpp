#pragma once

#include <inttypes.h>

#include <string>
#include <vector>

namespace Light {

	/** @brief Base module class
     * Modules are the building blocks of the program, their functionality is exposed using a facade class
     * eg. LoggerModule(class that implements the ugly detauls)
     *     Logger(facade class with static functions and no constructors)
     *
     * They can optionally tick every application frame
     *
     * There SHOULD NOT be a two way dependency between modules!
     * 
     * @todo More Docs
     *
     */
	class Module
	{
	public:
		/** @param tickable Should OnUpdate be called every frame? */
		Module(bool tickable = false);

		Module(const Module&)            = delete;
		Module& operator=(const Module&) = delete;

		virtual ~Module() = default;

		virtual inline bool HasRequestedAppTermination() const { return false; }

		/** @brief Name is misleading ; used to configure other modules' config struct before their OnInit
            @todo Implement module configuration */
		virtual void OnConfig() = 0;

		virtual void OnInit()   = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDeinit() = 0;

		inline bool IsTickable() const { return m_Tickable; }

	private:
		bool m_Tickable = false;
	};


} // namespace Light
