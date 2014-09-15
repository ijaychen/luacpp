/*************************************************************************
	> File Name: luacpp.h
	> Author: cjj
	> Created Time: 2014年09月14日 星期日 11时43分49秒
 ************************************************************************/

#ifndef LUA_CPP_H
#define LUA_CPP_H
#include "lua.hpp"
#include <new>
#include <stdio.h>
#include <string.h>
namespace luacpp{

	void	print_error(lua_State *L, const char* fmt, ...);	
	
	template<typename T>  
	T upvalue_(lua_State *L)
	{
		return *(T*)lua_touserdata(L, lua_upvalueindex(1));
	}
	
	struct user
	{
		user(void* p) : m_p(p) {}
		virtual ~user() {}
		void* m_p;
	};
	
	template<typename T>
	struct val2user : user
	{
		val2user() : user(new T) {}

		template<typename T1>
		val2user(T1 t1) : user(new T(t1)) {}

		template<typename T1, typename T2>
		val2user(T1 t1, T2 t2) : user(new T(t1, t2)) {}

		template<typename T1, typename T2, typename T3>
		val2user(T1 t1, T2 t2, T3 t3) : user(new T(t1, t2, t3)) {}

		template<typename T1, typename T2, typename T3, typename T4>
		val2user(T1 t1, T2 t2, T3 t3, T4 t4) : user(new T(t1, t2, t3,t4)) {}

		template<typename T1, typename T2, typename T3, typename T4, typename T5>
		val2user(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5) : user(new T(t1, t2, t3,t4,t5)) {}

		~val2user() { delete ((T*)m_p); }
	};
	
	template<typename T>
	struct class_name
	{
		// global name
		static const char* name(const char* name = NULL)
		{
			static char temp[256] = "";
			if(name) strcpy(temp, name);
			return temp;
		}
	};
	
	//read
	template<typename T>
	T read(lua_State * L, int index);
	template<> const char * read(lua_State * L, int index);
	template<> int read(lua_State * L, int index);
	template<> float read(lua_State * L, int index);

	//pop
	template<typename T>
	T pop(lua_State * L){	T t = read<T>(L, -1); lua_pop(L, -1); return t;	}
	
	template<> void pop(lua_State * L);

	//call
	template<typename RT>
	RT call(lua_State * L, const char * name)
	{
		lua_getglobal(L, name);
		if(lua_isfunction(L, -1))
		{			
			if(0 != lua_pcall(L, 0, 1, 0))
			{
				print_error(L, lua_tostring(L, -1));
			}
		}
		else
		{
			print_error(L, "attempt to call a function!");
		}
		return pop<RT>(L);
	}
	
	template<typename T>
	void class_add(lua_State* L, const char* name) 
	{ 
		class_name<T>::name(name);
		lua_newtable(L);
		printf("class_add:%s\n", name);
		
		luaL_newmetatable(L, class_name<T>::name());
		lua_pushstring(L,"__index");
		lua_pushvalue(L,-2);
		lua_settable(L,-3);		
		lua_setglobal(L, name);
	}
	
	template<typename T, typename F>
	void class_def(lua_State* L, const char* name, F func) 
	{ 
		luaL_getmetatable(L, class_name<T>::name());	
		if(lua_istable(L, -1))
		{
			lua_pushstring(L, name);					
			new(lua_newuserdata(L,sizeof(F))) F(func);		
			push_functor(L, func);
			lua_rawset(L, -3);
		}
		lua_pop(L, 1);
	}

	template<typename T, typename F>
	void class_con(lua_State* L,F func)
	{		
		lua_getglobal(L, class_name<T>::name());
		luaL_newmetatable(L, class_name<T>::name());
		lua_pushstring(L, "__call");
		lua_pushcclosure(L,func, 0);
		lua_rawset(L, -3);	
		lua_setmetatable(L, -2);
		lua_pop(L,1);
	}
	
	template<typename T>
	int constructor(lua_State *L) 
	{ 
		printf("----constructor-----\n");
		new(lua_newuserdata(L, sizeof(val2user<T>))) val2user<T>();
		luaL_getmetatable(L, class_name<T>::name());
		lua_setmetatable(L, -2);
		return 1; 
	}
	
	template<typename RVal, typename T, typename T1=void, typename T2=void, typename T3=void, typename T4=void, typename T5=void>
	struct mem_functor
	{
		static int invoke(lua_State *L) { push(L,(read<T*>(L,1)->*upvalue_<RVal(T::*)(T1,T2,T3,T4,T5)>(L))(read<T1>(L,2),read<T2>(L,3),read<T3>(L,4),read<T4>(L,5),read<T5>(L,6)));; return 1; }
	};
	
	template<typename T>
	struct mem_functor<void,T>
	{
		static int invoke(lua_State *L)  
		{
			T** obj = (T**)luaL_checkudata(L, 1, class_name<T>::name());
			((*obj)->*upvalue_<void(T::*)()>(L))();
		}
	};

	/*
	template<typename RTval, typename T> 
	struct mem_functor<RTval,T,lua_State*>
	{
		static int invoke(lua_State *L)
		{ 
			T** obj = (T**)luaL_checkudata(L, 1, class_name<T>::name());
			return ((*obj)->*upvalue_<int(T::*)(lua_State*)>(L))(L); 
		}
	};
	*/
	
	template<typename RVal, typename T>
	void push_functor(lua_State *L, RVal (T::*func)()) 
	{ 
		lua_pushcclosure(L, mem_functor<RVal,T>::invoke, 1); 
	}
	
	
}
#endif
