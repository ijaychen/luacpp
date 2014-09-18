#include "luacpp.h"
class A
{
public:
	A(){
		printf("A constructor!&A:%p\n", this);
		m_id = 0;
	}
	
	~A(){
		printf("A dis constructor!&A:%p\n", this);
	}
	
	void show()
	{
		printf("A show!&A:%p\n", this);
		int a = 10;
		printf("A::show(), a:%d\n", a);
	}
	
	int get()
	{
		printf("A get!&A:%p\n", this);
		int a = 10;
		return a;
	}
	
	int set(int id)
	{
		//m_id = id;
		printf("A::set %d\n", id);
		printf("m_id:%d\n", m_id);
		printf("A set!&A:%p\n", this);
		return id;
	}
	
private:
	int m_id;
};

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
	luacpp::class_def<A>(L, "get", &A::get);
	luacpp::class_def<A>(L, "set", &A::set);
	
	printf("----------------------------\n");
	luacpp::call<void>(L, "c_test");
	int nRet = luacpp::call<int>(L, "c_testInt");
	printf("call:%d\n", nRet);
	const char * str = luacpp::call<const char*>(L, "c_testStr");
	printf("call:%s\n", str);
	


	return 0;
}