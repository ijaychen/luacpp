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
	return (int)luaL_checknumber(L, index);
}

template<>float luacpp::read(lua_State * L, int index)
{
	return (float)luaL_checknumber(L, index);
}



//pop
template<> void luacpp::pop(lua_State * L)
{
	lua_pop(L, 1);
}
