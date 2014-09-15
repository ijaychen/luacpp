/*************************************************************************
	> File Name: test.cpp
	> Author: cjj
	> Created Time: 2014年09月14日 星期日 21时02分29秒
 ************************************************************************/
#include <stdio.h>
#include <new>
class A
{
public:
	void test(){printf("test\n");}
};

typedef void (A::*pClassFunc)();

pClassFunc * newClass(size_t size)
{
	//void * tmp = new int(4);
	//return tmp;
	return new pClassFunc;
}

int main()
{
	printf("&A::test:%p\n", &A::test);
	//pClassFunc ff = 
	new(newClass(sizeof(pClassFunc))) 
		pClassFunc(&A::test);
	return 0;
}
