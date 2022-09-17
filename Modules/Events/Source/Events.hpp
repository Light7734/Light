#pragma once

#include <functional>

namespace Light {

	/** @brief Notifies the listeners upon invoke */
	template<typename... Args>
	class Notifier
	{
	public:
		Notifier()
		{
		}

		void AddListener(std::function<bool(Args...)> func)
		{
			m_Funcs.push_back(func);
		}

		bool Invoke(Args&&... args)
		{
			for (auto func : m_Funcs)
			{
				if (func(std::forward<Args>(args)...))
				{
					return true;
				}
			}

			return false;
		}

	private:
		std::vector<std::function<bool(Args...)>> m_Funcs = {};
	};

	/** @brief Invokable reference to a method */
	template<typename Ret, typename... Args>
	class Delagate
	{
	public:
		template<typename ClassType, typename... BoundArgs>
		void Bind(Ret (ClassType::*func)(Args...), BoundArgs&&... bound_args)
		{
			m_Func = std::bind(func, std::forward<BoundArgs>(bound_args)...);
		}

	private:
		std::function<Ret(Args...)> m_Func = {};
	};

} // namespace Light
