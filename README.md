## 一、为什么要使用全局委托
1.可以让 c++ 访问空指针和野指针，捕获所有异常的方法

2.c++通用委托实现, 支持任意个数参数和任意类型返回类型

3.让c++程序更健壮，更加优雅的崩溃和退出

4.如何理解委托呢？

> 所谓委托，就是把工作交给别人来完成。我的理解就是在程序中，把本类的函数指针交给其它类，让其它类在需要的时候调用这个函数，完成某个功能。说简单点，就是一个函数回调的机制。为了实现这样一个回调的功能，我们可以通过对外暴露接口的方式，让外部持有该类的接口，必要时进行接口的回调。但是这样增加了调用者（受托者）和被调用者（几乎是委托者）的耦合关系，因为调用者必须知道这个接口，需要include接口头文件。为了减少这种耦合，常用的技术就是使用boost::function对象。委托者把要委托的函数指针打包成一个function对象，扔给受托者，让受托者在合适的时机完成功能，这样两边都可以不知道对方是如何实现的，没有接口的耦合，大家都只要#include function对象就行了。

## 二、如何使用全局委托
首先在vs 里面设置 项目->配置属性->c/c++ ->代码生成->启用c++异常->(是 (/EHsc) 改为 是，但有 SEH 异常 (/EHa)) 这样就能够使用try catch 捕获空指针或者是野指针访问的异常

然后

引入头文件
> #include "CMultiDelegate.h"
> #include "CStaticDelegate.h"

代码示例
```cpp
		//普通函数的调用
		CMultiDelegate<void> onclick;
		onclick += newDelegate(Say);
		//onclick -= newDelegate(Say);
		onclick.bind("Say"); //给每一个调用的方法或者函数做标记
		onclick();


		Base b;
		//普通类的调用
		CMultiDelegate<void> myDelegate;
		myDelegate += newDelegate(&b, &Base::classFunc);
		myDelegate.bind("Base::classFunc");
		myDelegate();

		//类的静态调用
		CMultiDelegate<void, int> e;
		e += newDelegate(A::StaticFunc);
		e.bind("A::StaticFunc");
		e(1);
		//类的动态调用
		e -= newDelegate(A::StaticFunc);
		e += newDelegate(&A(), &A::MemberFunc);
		e.bind("A::MemberFunc");
		e(1);
		//无返回值 传int 参数调用
		CMultiDelegate<void,int> obj;
		obj += newDelegate(abc);
		obj.bind("abc");
		obj(3);

		//int 返回值 传int，int参数调用
		CMultiDelegate<int,int,int> s;
		s += newDelegate(sum);
		s.bind("sum");
		std::vector<int> result = s(1,1);
		
		printf("sum ： %d\n", result[0]);

		std::cout << "ok!\n";
```
## 三、关于当前c++异常捕获的情况
```cpp
					//有返回值捕获异常方法或函数实现
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
```

```cpp
					//无返回值捕获异常方法或者函数
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
```
## 四、实现效果
	这样就算是不打印堆栈的情况下，也可以准确的捕获到崩溃异常的方法。本例不使用windbug dbhelp 类的windows平台下的特定库。都是用的c++标准库。这样稍微改造就可以在windows/linux 下打造一个 全局异常处理的 工具出来。c++ 程序员再也不用写一大堆的try catch 了。再也不用担心指针访问越界的问题了。

运行效果
```bash
void Say(): Hello world
Base func1
Exception is catch! function is A::StaticFunc
这里是成员非静态函数 ： 1
Exception is catch! function is abc
sum ： 2
ok!
```

可以很明显的看到 两个异常的方法和函数都被捕获到了。而且不印象后续sum代码的运行，简直是完美啊。
本例代码地址

https://github.com/xyls518/delegate

##五、代码结构
- 代码结构：
- CMultiDelegate.h //托管定义的文件头,实现操作符重载,实现异常捕获
- CStaticDelegate.h //new 方法的实现，以及偏特化的处理,实现异常捕获
- IDelegate.h //接口文件类的定义
- MyDelegate.h //整合所有文件，但是没有实现异常捕获