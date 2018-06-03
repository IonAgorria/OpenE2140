//
// Created by Ion Agorria on 27/05/18
//

#include "luavm.h"
#include "utils.h"

std::unordered_map<lua_State*, std::shared_ptr<LuaVM>> LuaVM::stateVMs;

LuaVM::LuaVM() {
    log = Log::get("Lua");
    state = luaL_newstate();
}

LuaVM::~LuaVM() {
    if (state) {
        stateVMs[state] = nullptr;
        lua_close(state);
        state = nullptr;
    }
}

std::shared_ptr<LuaVM> LuaVM::create() {
    std::shared_ptr<LuaVM> vm;
    vm.reset(new LuaVM());
    lua_State* state = vm->state;

    //Store to state-vm map
    stateVMs[state] = vm;

    //Load libraries
    luaopen_base(state);
    luaopen_table(state);
    luaopen_string(state);
    luaopen_math(state);
    luaopen_debug(state);

    //Register functions
    lua_register(state, "print", &luaPrint);

    //Run
    luaL_dostring(state, "print(\"test\", 1, 2, print, {})");

    return vm;
}

std::shared_ptr<LuaVM> LuaVM::getStateVM(lua_State* L) {
    return stateVMs[L];
}

int LuaVM::luaPrint(lua_State *L) {
    int n = lua_gettop(L);

    //Iterate each element
    std::string line = "";
    for (int i=1; i<=n; i++)
    {
        if (1 < i) {
            line += "\t";
        }
        if (lua_isstring(L,i)) {
            line += lua_tostring(L, i);
        } else if (lua_isnil(L,i)) {
            line += "nil";
        } else if (lua_isboolean(L,i)) {
            line += lua_toboolean(L, i) ? "true" : "false";
        } else {
            unsigned long pointer = reinterpret_cast<unsigned long>(lua_topointer(L, i));
            line += std::string(luaL_typename(L, i)) + ": 0x" + Utils::toStringHex(pointer);
        }
    }

    std::shared_ptr<LuaVM> vm = getStateVM(L);
    vm->log->info(line);
    return 0;
}
