#pragma once

#include "IDelegate.h"
#include <list>
#include <vector>
#include <string>


namespace Delegate
{
	template<typename ReturnType, typename ...ParamType> //有返回值
	class CMultiDelegate
	{

	public:

		typedef std::list<IDelegate<ReturnType, ParamType...>*> ListDelegate;
		typedef typename ListDelegate::iterator ListDelegateIterator;
		typedef typename ListDelegate::const_iterator ConstListDelegateIterator;

		CMultiDelegate() { }
		~CMultiDelegate() { clear(); }

		bool empty() const
		{
			for (ConstListDelegateIterator iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
			{
				if (*iter) return false;
			}
			return true;
		}

		void clear()
		{
			for (ListDelegateIterator iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
			{
				if (*iter)
				{
					delete (*iter);
					(*iter) = nullptr;
				}
			}
		}


		CMultiDelegate<ReturnType, ParamType...>& operator+=(IDelegate<ReturnType, ParamType...>* _delegate)
		{
			for (ListDelegateIterator iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
			{
				if ((*iter) && (*iter)->compare(_delegate))
				{
					delete _delegate;
					return *this;
				}
			}
			mListDelegates.push_back(_delegate);
			return *this;
		}

		CMultiDelegate<ReturnType, ParamType...>& operator-=(IDelegate<ReturnType, ParamType...>* _delegate)
		{
			for (ListDelegateIterator iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
			{
				if ((*iter) && (*iter)->compare(_delegate))
				{
					if ((*iter) != _delegate) delete (*iter);       //避免同一个地址被delete两次
					(*iter) = 0;
					break;
				}
			}
			delete _delegate;
			return *this;
		}

		std::vector<ReturnType> operator()(ParamType... params) //有返回值的操作符（）重载
		{
			ListDelegateIterator iter = mListDelegates.begin();
			std::vector<ReturnType> _Results;
			while (iter != mListDelegates.end())
			{
				if (0 == (*iter))
				{
					iter = mListDelegates.erase(iter);
				}
				else
				{
					try {
						_Results.push_back((*iter)->invoke(params...));
					}
					catch (const std::exception& e) { //普通抛出异常捕获
						std::cout << e.what() << std::endl;
					}
					catch (...)//所有错误捕获
					{

						printf("Exception is catch! function is %s \n", funcName.c_str());

					}
					
					++iter;
				}
			}
			return _Results;
		}
	
		void bind(std::string fname) {
			funcName = fname;
		}
	private:
		CMultiDelegate<ReturnType, ParamType...>(const CMultiDelegate& _event);
		CMultiDelegate<ReturnType, ParamType...>& operator=(const CMultiDelegate& _event);
	private:
		ListDelegate mListDelegates;
	private:
		std::string funcName;
	};

	template< typename ...ParamType> //无返回值定义
	class CMultiDelegate<void, ParamType...>
	{

	public:

		typedef std::list<IDelegate<void, ParamType...>*> ListDelegate;
		typedef typename ListDelegate::iterator ListDelegateIterator;
		typedef typename ListDelegate::const_iterator ConstListDelegateIterator;

		CMultiDelegate() { }
		~CMultiDelegate() { clear(); }

		bool empty() const
		{
			for (ConstListDelegateIterator iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
			{
				if (*iter) return false;
			}
			return true;
		}

		void clear()
		{
			for (ListDelegateIterator iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
			{
				if (*iter)
				{
					delete (*iter);
					(*iter) = nullptr;
				}
			}
		}

		CMultiDelegate<void, ParamType...>& operator+=(IDelegate<void, ParamType...>* _delegate)
		{
			for (ListDelegateIterator iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
			{
				if ((*iter) && (*iter)->compare(_delegate))
				{
					delete _delegate;
					return *this;
				}
			}
			mListDelegates.push_back(_delegate);
			return *this;
		}

		CMultiDelegate<void, ParamType...>& operator-=(IDelegate<void, ParamType...>* _delegate)
		{
			for (ListDelegateIterator iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
			{
				if ((*iter) && (*iter)->compare(_delegate))
				{
					if ((*iter) != _delegate) delete (*iter);       //避免同一个地址被delete两次
					(*iter) = 0;
					break;
				}
			}
			delete _delegate;
			return *this;
		}

		void operator()(ParamType... params)//无返回符的操作符重载
		{
			ListDelegateIterator iter = mListDelegates.begin();
			while (iter != mListDelegates.end())
			{
				if (0 == (*iter))
				{
					iter = mListDelegates.erase(iter);
				}
				else
				{
					try {
						(*iter)->invoke(params...);//所有函数或者方法都在这里执行
					}
					catch (const std::exception& e) { //普通抛出异常捕获
						std::cout << e.what() << std::endl;
					}
					catch (...)//所有错误捕获
					{
						printf("Exception is catch! function is %s \n", funcName.c_str());
					}
					++iter;
				}
			}
		}
	void bind(std::string fname) {
			funcName = fname;
	}
	private:
		CMultiDelegate<void, ParamType...>(const CMultiDelegate& _event);
		CMultiDelegate<void, ParamType...>& operator=(const CMultiDelegate& _event);

	private:
		ListDelegate mListDelegates;

	private:
		std::string funcName;
	};


}