#include <iostream>

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
    int luaopen_luaLib(lua_State* luaEnv);
}

int averageFunc(lua_State* luaEnv)
{
    int n = lua_gettop(luaEnv);
    int sum{0};
    for(int i = 1; i <= n; ++i){
        sum += lua_tonumber(luaEnv, i);
    }
    lua_pushnumber(luaEnv, sum);
    return 1;
}

int sayHello(lua_State* luaEnv)
{
    std::cout << "Hello" << std::endl;
    return 0;
}

//导出函数列表
const struct luaL_Reg myLib[] = {
    {"averageFunc", averageFunc},
    {"sayHello", sayHello},
    {NULL, NULL}
};

int luaopen_luaLib(lua_State* luaEnv)
{
    luaL_register(luaEnv, "luaLib", myLib);
    return 1;
}
