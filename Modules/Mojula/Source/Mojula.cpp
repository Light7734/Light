#include "Mojula.hpp"

namespace Light {

	Module::Module(TickType tick_type /* = false */, bool syncable /* = false */)
	    : m_TickType(tick_type), m_Syncable(syncable)
	{
	}

} // namespace Light
