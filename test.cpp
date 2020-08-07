// test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include "CMultiDelegate.h"
#include "CStaticDelegate.h"
using namespace std;

using namespace Delegate;


void Say()
{
	printf("void Say(): Hello world\n");
}



void abc(int b) {
	int* p = NULL;
	*p = b;
	printf("这里是abc ： %d\n", b);
}


int sum(int a, int b) {
	//printf("%s", __func__);
	int c = a + b;
	//int* p = NULL;
	//*p = b;
	return c;
}




class Base
{
public:
	// 类成员函数
	void classFunc() { std::cout << "Base func1" << std::endl; }
};


class A
{
public:
	static void StaticFunc(int a)
	{
		int* p = NULL;
		*p = 3;
		printf("这里是成员静态函数 ： %d\n", a);
	}
	void MemberFunc(int a)
	{
		//int* p = NULL;
		//*p = 3;
		printf("这里是成员非静态函数 ： %d\n", a);

		//throw runtime_error("I am an exception");
	}


};

int main()
{

		//abc(3);
		//sum(1, 2);

		CMultiDelegate<void> onclick;
		onclick += newDelegate(Say);
		//onclick -= newDelegate(Say);
		onclick.bind("Say");
		onclick();


		Base b;

		CMultiDelegate<void> myDelegate;
		myDelegate += newDelegate(&b, &Base::classFunc);
		myDelegate.bind("Base::classFunc");
		myDelegate();


		CMultiDelegate<void, int> e;
		e += newDelegate(A::StaticFunc);
		e.bind("A::StaticFunc");
		e(1);
		e -= newDelegate(A::StaticFunc);
		e += newDelegate(&A(), &A::MemberFunc);
		e.bind("A::MemberFunc");
		e(1);

		CMultiDelegate<void,int> obj;
		obj += newDelegate(abc);
		obj.bind("abc");
		obj(3);


		CMultiDelegate<int,int,int> s;
		s += newDelegate(sum);
		s.bind("sum");
		std::vector<int> result = s(1,1);
		
		printf("sum ： %d\n", result[0]);

		std::cout << "ok!\n";

    
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
