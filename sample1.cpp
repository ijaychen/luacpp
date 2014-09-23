#include "luacpp.h"
#include "world_packet.h"

class base
{
public:
	base():m_base(123){}
	void showBase() const {
		printf("m_base:%d\n", m_base);
	}
private:
	int m_base;
	
};

class A : public base
{
public:
	A(){
		m_id = 0;
	}
	
	~A(){
		printf("A dis constructor!&A:%p\n", this);
	}
	
	void show()
	{	
		int a = 10;
		printf("A::show(), a:%d\n", a);
		showBase();
	}
	
	int get()
	{
		int a = 10;
		return a;
	}
	
	int set(int id)
	{
		m_id = id;
		printf("m_id:%d\n", m_id);
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
	
	luacpp::class_add<WorldPacket>(L, "WorldPacket");
	luacpp::class_con<WorldPacket>(L, luacpp::constructor<WorldPacket, unsigned short>);
	luacpp::class_def<WorldPacket>(L, "GetOpcode", &WorldPacket::GetOpcode);
	
	luacpp::call<void>(L, "c_test");
/*	int nRet = luacpp::call<int>(L, "c_testInt");
	printf("call:%d\n", nRet);
	const char * str = luacpp::call<const char*>(L, "c_testStr");
	printf("call:%s\n", str);
*/
	

	return 0;
}