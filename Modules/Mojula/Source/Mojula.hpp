#pragma once

#include <inttypes.h>

#include <string>
#include <vector>

namespace Light {

	/** @brief Base module class
     * Modules are the building blocks of the program, their functionality is exposed using a facade class
     * eg. LoggerModule(class that implements the ugly details)
     *     Logger(facade class with static functions and no constructors)
     *
     * They can optionally tick every application frame
     *
     * There SHOULD NOT be a two way dependency between modules!
     * 
    * @todo More Docs */
	class Module
	{
	public:
		enum class TickType : uint8_t
		{
			eNone,
			eGameThread,
			eRenderThread,

			nCount,
		};

	public:
		/** @param tick_type Which thread should module tick in?
		 * @param syncable Should OnSync be called every frame? */
		Module(TickType tick_type = TickType::eNone, bool syncable = false);

		Module(const Module&)            = delete;
		Module& operator=(const Module&) = delete;

		virtual ~Module() = default;

		virtual inline bool HasRequestedAppTermination() const { return false; }

		virtual void OnTick() = 0;
		virtual void OnSync() = 0;

		inline TickType GetTickType() const { return m_TickType; }
		inline bool IsSyncable() const { return m_Syncable; }

	private:
		const TickType m_TickType = {};
		const bool m_Syncable     = {};
	};


} // namespace Light
