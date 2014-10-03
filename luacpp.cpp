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
template<>
const char * luacpp::read(lua_State * L, int index)
{
	return (const char *)luaL_checkstring(L, index);
}

template<>
unsigned short luacpp::read(lua_State * L, int index)
{
	unsigned short res = (unsigned short)luaL_checknumber(L, index);
	//printf("----luaL_checknumber---------%d\n", res);
	return res;
}

template<>int 
luacpp::read(lua_State * L, int index)
{
	int res = (int)luaL_checknumber(L, index);
	//printf("----luaL_checknumber---------%d\n", res);
	return res;
}

template<>float 
luacpp::read(lua_State * L, int index)
{
	return (float)luaL_checknumber(L, index);
}

template<>
luacpp::table luacpp::read(lua_State *L, int index)
{
	return table(L, index);
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

template<>	
void luacpp::push(lua_State *L, luacpp::table ret)
{
	lua_pushvalue(L, ret.m_obj->m_index);
}


//pop
template<> 
void luacpp::pop(lua_State * L)
{
	lua_pop(L, 1);
}

template<>	
luacpp::table luacpp::pop(lua_State *L)
{
	return table(L, lua_gettop(L));
}

/*---------------------------------------------------------------------------*/ 
/* table object on stack                                                     */ 
/*---------------------------------------------------------------------------*/ 
luacpp::table_obj::table_obj(lua_State* L, int index)
	:m_L(L)
	,m_index(index)
	,m_ref(0)
{
	if(lua_isnil(m_L, m_index))
	{
		m_pointer = NULL;
		lua_remove(m_L, m_index);
	}
	else
	{
		m_pointer = lua_topointer(m_L, m_index);
	}
}

luacpp::table_obj::~table_obj()
{
	if(validate())
	{
		lua_remove(m_L, m_index);
	}
}

void luacpp::table_obj::inc_ref()
{
	++m_ref;
}

void luacpp::table_obj::dec_ref()
{
	if(--m_ref == 0)
		delete this;
}

bool luacpp::table_obj::validate()
{
	if(m_pointer != NULL)
	{
		if(m_pointer == lua_topointer(m_L, m_index))
		{
			return true;
		}
		else
		{
			int top = lua_gettop(m_L);

			for(int i=1; i<=top; ++i)
			{
				if(m_pointer == lua_topointer(m_L, i))
				{
					m_index = i;
					return true;
				}
			}

			m_pointer = NULL;
			return false;
		}
	}
	else
	{
        return false;
	}
}

/*---------------------------------------------------------------------------*/ 
/* Table Object Holder                                                       */ 
/*---------------------------------------------------------------------------*/ 
luacpp::table::table(lua_State* L)
{
	lua_newtable(L);

	m_obj = new table_obj(L, lua_gettop(L));

	m_obj->inc_ref();
}

luacpp::table::table(lua_State* L, const char* name)
{
	lua_getglobal(L, name);
	if(lua_istable(L, -1) == 0)
	{
		lua_pop(L, 1);

		lua_newtable(L);
		lua_pushstring(L, name);
		lua_pushvalue(L, -2);
		lua_settable(L, -3);
	}

	m_obj = new table_obj(L, lua_gettop(L));
}

luacpp::table::table(lua_State* L, int index)
{
	if(index < 0)
	{
		index = lua_gettop(L) + index + 1;
	}

	m_obj = new table_obj(L, index);

	m_obj->inc_ref();
}

luacpp::table::table(const table& input)
{
	m_obj = input.m_obj;

	m_obj->inc_ref();
}

luacpp::table::~table()
{
	m_obj->dec_ref();
}

int luacpp::add_ref_table(lua_State *L, luacpp::table t)
{
	luacpp::push<luacpp::table>(L, t);
	return luaL_ref(L, LUA_REGISTRYINDEX);
}

luacpp::table luacpp::get_ref_table(lua_State * L, int ref)
{
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	return luacpp::read<luacpp::table>(L, -1);
}