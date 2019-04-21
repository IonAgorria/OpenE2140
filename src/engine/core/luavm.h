//
// Created by Ion Agorria on 14/05/18
//
#ifndef OPENE2140_LUAVM_H
#define OPENE2140_LUAVM_H

#include "core/common.h"
#include "io/log.h"
#include "lua.hpp"

class LuaVM {
protected:
    /**
     * Lua VM constructor
     */
    LuaVM();

    /**
     * Log for object
     */
    log_ptr log;

    /**
     * Lua VM state struct
     */
    lua_State* state;

    /**
     * Each lua state mapped to it's VM instance
     */
    static std::unordered_map<lua_State*, std::shared_ptr<LuaVM>> stateVMs;

    /**
     * Prints to log
     *
     * @param L lua state calling this function
     * @return 
     */
    static int luaPrint(lua_State *L);
public:
    /**
     * Lua VM destructor
     */
    ~LuaVM();

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(LuaVM)

    /**
     * Factory to create a new Lua VM
     *
     * @return lua vm
     */
    static std::shared_ptr<LuaVM> create();
};


#endif //OPENE2140_LUAVM_H
