#include "luacpp.h"
class A
{
public:
	A(){
		printf("A constructor!&A:%p\n", this);
	}
	
	void show()
	{
		int a = 10;
		printf("A::show(), a:%d\n", a);
	}
};

int xxx(lua_State * L)
{
	printf("xxx!\n");
	return 0;
}

int main()
{
	lua_State * L = luaL_newstate();
	luaL_openlibs(L);
	
	if(luaL_loadfile(L, "sample1.lua") || lua_pcall(L, 0, 0, 0))
	{
		printf("%s\n", lua_tostring(L, -1)); 
		return -1;
	}
	
	luacpp::class_add<A>(L, "A");
	luacpp::class_con<A>(L, luacpp::constructor<A>);
	luacpp::class_def<A>(L, "show", &A::show);
	printf("----------------------------\n");
	luacpp::call<void>(L, "c_test");
	int nRet = luacpp::call<int>(L, "c_testInt");
	printf("call:%d\n", nRet);
	const char * str = luacpp::call<const char*>(L, "c_testStr");
	printf("call:%s\n", str);
	


	return 0;
}