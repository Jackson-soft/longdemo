#include <iostream>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
int luaopen_luaLib(lua_State *luaEnv);
}

auto averageFunc(lua_State *luaEnv) -> int {
    int n = lua_gettop(luaEnv);
    int sum{0};
    for (int i = 1; i <= n; ++i) {
        sum += lua_tonumber(luaEnv, i);
    }
    lua_pushnumber(luaEnv, sum);
    return 1;
}

auto sayHello(lua_State *luaEnv) -> int {
    std::cout << "Hello" << std::endl;
    return 0;
}

// 导出函数列表
static const struct luaL_Reg myLib[] = {{"averageFunc", averageFunc}, {"sayHello", sayHello}, {nullptr, nullptr}};

int luaopen_luaLib(lua_State *luaEnv) {
    lua_newtable(luaEnv);
    luaL_setfuncs(luaEnv, myLib, 0);
    return 1;
}
