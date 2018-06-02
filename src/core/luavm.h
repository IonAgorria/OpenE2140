//
// Created by Ion Agorria on 14/05/18
//
#ifndef OPENE2140_LUAVM_H
#define OPENE2140_LUAVM_H

#include "core/config.h"
#include "core/io/log.h"
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
     * Returns VM instance where this state belongs to, this is used as a workaround because you can't register
     * instance methods as callable lua functions
     *
     * @param L lua state
     * @return lua VM
     */
    static std::shared_ptr<LuaVM> getStateVM(lua_State *L);

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
