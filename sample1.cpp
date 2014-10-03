#include "luacpp.h"
#include <unistd.h>

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
	luacpp::table * m_t;
	lua_State * m_l;
};

class object
{
public:
	object(lua_State * L = NULL):m_l(L){ }
	void bind(luacpp::table t)
	{
		m_tRef = luacpp::add_ref_table(m_l, t);
	}
	
	void proc()
	{
		//lua_rawgeti(m_l, LUA_REGISTRYINDEX, m_tRef);
		//luacpp::table tb = luacpp::read<luacpp::table>(m_l, -1);
		luacpp::table tb = luacpp::get_ref_table(m_l, m_tRef);
		luacpp::call<void>(m_l, "c_testTable", tb);
	}

private:
	lua_State * m_l;
	int	m_tRef;
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
	
	luacpp::class_add<object>(L, "object");
	luacpp::class_con<object>(L, luacpp::constructor<object>);
	luacpp::class_def<object>(L, "bind", &object::bind);
	luacpp::class_def<object>(L, "proc", &object::proc);
	

/*
	luacpp::call<void>(L, "c_test");
	int nRet = luacpp::call<int>(L, "c_testInt");
	printf("call:%d\n", nRet);
	const char * str = luacpp::call<const char*>(L, "c_testStr");
	printf("call:%s\n", str);


	luacpp::table temp(L);
	temp.set("name", "local table !!");
	luacpp::call<void>(L, "c_testTable", temp);
	
	luacpp::table ret = luacpp::call<luacpp::table>(L, "return_table", "give me a table !!");
	printf("ret.name =\t%s\n", ret.get<const char*>("name"));
*/
	object * obj = new object(L);
	
	luacpp::call<void>(L, "c_initObj", obj);
	obj->proc();
	int i = 0;
	while(1)
	{
		printf("--------------sleep:%d\n", ++i);
		sleep(1.5);
		obj->proc();
	}
	return 0;
}