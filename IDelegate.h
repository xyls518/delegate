#pragma once
#include <typeinfo>

namespace Delegate
{
	template<typename ReturnType, typename ...ParamType>
	class IDelegate
	{
	public:
		IDelegate() {}
		virtual ~IDelegate() {}
		virtual bool isType(const std::type_info& _type) = 0;
		virtual ReturnType invoke(ParamType ... params) = 0;
		virtual bool compare(IDelegate<ReturnType, ParamType...>* _delegate) const = 0;
	};
}
