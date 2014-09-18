/*************************************************************************
	> File Name: luacpp.cpp
	> Author: cjj
	> Created Time: 2014年09月14日 星期日 13时03分23秒
 ************************************************************************/
#include <stdarg.h>
#include <stdio.h>
#include "luacpp.h"

void luacpp::print_error(lua_State *L, const char* fmt, ...)
{
	char text[4096];
	va_list args;
	va_start(args, fmt);
	vsprintf(text, fmt, args);
	va_end(args);
	printf("%s\n", text);
}

//read
template<>const char * luacpp::read(lua_State * L, int index)
{
	return (const char *)luaL_checkstring(L, index);
}

template<>int luacpp::read(lua_State * L, int index)
{
	int res = (int)luaL_checknumber(L, index);
	//printf("----luaL_checknumber---------%d\n", res);
	return res;
}

template<>float luacpp::read(lua_State * L, int index)
{
	return (float)luaL_checknumber(L, index);
}

/*---------------------------------------------------------------------------*/ 
/* push                                                                      */ 
/*---------------------------------------------------------------------------*/ 
template<>
void luacpp::push(lua_State *L, char ret)
{
	lua_pushnumber(L, ret);						
}

template<>
void luacpp::push(lua_State *L, unsigned char ret)
{
	lua_pushnumber(L, ret);						
}

template<>
void luacpp::push(lua_State *L, short ret)
{
	lua_pushnumber(L, ret);						
}

template<>
void luacpp::push(lua_State *L, unsigned short ret)
{
	lua_pushnumber(L, ret);						
}

template<>
void luacpp::push(lua_State *L, long ret)
{
	lua_pushnumber(L, ret);						
}

template<>
void luacpp::push(lua_State *L, unsigned long ret)
{
	lua_pushnumber(L, ret);						
}

template<>
void luacpp::push(lua_State *L, int ret)
{
	lua_pushnumber(L, ret);						
}

template<>
void luacpp::push(lua_State *L, unsigned int ret)
{
	lua_pushnumber(L, ret);						
}

template<>
void luacpp::push(lua_State *L, float ret)
{
	lua_pushnumber(L, ret);						
}

template<>
void luacpp::push(lua_State *L, double ret)
{
	lua_pushnumber(L, ret);						
}

template<>
void luacpp::push(lua_State *L, char* ret)
{
	lua_pushstring(L, ret);						
}

template<>
void luacpp::push(lua_State *L, const char* ret)
{
	lua_pushstring(L, ret);						
}

template<>
void luacpp::push(lua_State *L, bool ret)
{
	lua_pushboolean(L, ret);						
}




//pop
template<> void luacpp::pop(lua_State * L)
{
	lua_pop(L, 1);
}
