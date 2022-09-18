#pragma once

#include <functional>

#define NOTIF_BINDING(name, var, num_args, ...)        NOTIF_BINDING##num_args(name, var, __VA_ARGS__)
#define STATIC_NOTIF_BINDING(name, var, num_args, ...) STATIC_NOTIF_BINDING##num_args(name, var, __VA_ARGS__)

#define NOTIF_BINDING0(name, var, ...)                                 \
	template<typename T>                                               \
	inline void Bind_##name(bool (T::*func)(__VA_ARGS__), T* instance) \
	{                                                                  \
		var.AddListener(std::bind(func, instance));                    \
	}


#define NOTIF_BINDING1(name, var, ...)                                     \
	template<typename T>                                                   \
	inline void Bind_##name(bool (T::*func)(__VA_ARGS__), T* instance)     \
	{                                                                      \
		var.AddListener(std::bind(func, instance, std::placeholders::_1)); \
	}


#define NOTIF_BINDING2(name, var, ...)                                                            \
	template<typename T>                                                                          \
	inline void Bind_##name(bool (T::*func)(__VA_ARGS__), T* instance)                            \
	{                                                                                             \
		var.AddListener(std::bind(func, instance, std::placeholders::_1, std::placeholders::_2)); \
	}


#define NOTIF_BINDING3(name, var, ...)                                                                                   \
	template<typename T>                                                                                                 \
	inline void Bind_##name(bool (T::*func)(__VA_ARGS__), T* instance)                                                   \
	{                                                                                                                    \
		var.AddListener(std::bind(func, instance, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)); \
	}


#define NOTIF_BINDING4(name, var, ...)                                                                                                          \
	template<typename T>                                                                                                                        \
	inline void Bind_##name(bool (T::*func)(__VA_ARGS__), T* instance)                                                                          \
	{                                                                                                                                           \
		var.AddListener(std::bind(func, instance, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)); \
	}

#define STATIC_NOTIF_BINDING0(name, var, ...)                                 \
	template<typename T>                                                      \
	static inline void Bind_##name(bool (T::*func)(__VA_ARGS__), T* instance) \
	{                                                                         \
		var.AddListener(std::bind(func, instance));                           \
	}

#define STATIC_NOTIF_BINDING1(name, var, ...)                                 \
	template<typename T>                                                      \
	static inline void Bind_##name(bool (T::*func)(__VA_ARGS__), T* instance) \
	{                                                                         \
		var.AddListener(std::bind(func, instance, std::placeholders::_1));    \
	}                                                                         \
	static inline void BindStatic_##name(bool (*func)(__VA_ARGS__))           \
	{                                                                         \
		var.AddListener(std::bind(func, std::placeholders::_1));              \
	}


#define STATIC_NOTIF_BINDING2(name, var, ...)                                                     \
	template<typename T>                                                                          \
	static inline void Bind_##name(bool (T::*func)(__VA_ARGS__), T* instance)                     \
	{                                                                                             \
		var.AddListener(std::bind(func, instance, std::placeholders::_1, std::placeholders::_2)); \
	}


#define STATIC_NOTIF_BINDING3(name, var, ...)                                                                            \
	template<typename T>                                                                                                 \
	static inline void Bind_##name(bool (T::*func)(__VA_ARGS__), T* instance)                                            \
	{                                                                                                                    \
		var.AddListener(std::bind(func, instance, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)); \
	}


#define STATIC_NOTIF_BINDING4(name, var, ...)                                                                                                   \
	template<typename T>                                                                                                                        \
	static inline void Bind_##name(bool (T::*func)(__VA_ARGS__), T* instance)                                                                   \
	{                                                                                                                                           \
		var.AddListener(std::bind(func, instance, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)); \
	}

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
