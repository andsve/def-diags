#define LIB_NAME "diags"
#define MODULE_NAME LIB_NAME
#define DLIB_LOG_DOMAIN LIB_NAME

// include the Defold SDK
#include <dmsdk/sdk.h>
#include <cstdlib>

#if defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_LINUX)

#include "mlabbe-nativefiledialog/src/nfd.h"

static int Diags_Open(lua_State* L)
{
    int top = lua_gettop(L);

    const nfdchar_t *filter_list = 0x0;
    const nfdchar_t *default_path = 0x0;
    nfdchar_t *out_path = 0x0;

    if (top > 0 && !lua_isnil(L, 1)) {
        filter_list = luaL_checkstring(L, 1);
    }
    if (top > 1 && !lua_isnil(L, 2)) {
        default_path = luaL_checkstring(L, 2);
    }

    nfdresult_t res = NFD_OpenDialog( filter_list, default_path, &out_path );
    lua_pushinteger(L, res);
    if (res == NFD_OKAY) {
        lua_pushstring(L, out_path);
        free(out_path);
        return 2;
    } else if (res == NFD_ERROR) {
        lua_pushstring(L, NFD_GetError());
        return 2;
    }
    return 1;
}

static int Diags_OpenMultiple(lua_State* L)
{
    int top = lua_gettop(L);

    const nfdchar_t *filter_list = 0x0;
    const nfdchar_t *default_path = 0x0;
    nfdpathset_t path_set;

    if (top > 0 && !lua_isnil(L, 1)) {
        filter_list = luaL_checkstring(L, 1);
    }
    if (top > 1 && !lua_isnil(L, 2)) {
        default_path = luaL_checkstring(L, 2);
    }

    nfdresult_t res = NFD_OpenDialogMultiple( filter_list, default_path, &path_set );
    lua_pushinteger(L, res);
    if (res == NFD_OKAY) {

        lua_newtable(L);

        for ( size_t i = 0; i < NFD_PathSet_GetCount(&path_set); ++i )
        {
            nfdchar_t *path = NFD_PathSet_GetPath(&path_set, i);
            lua_pushstring(L, path);
            lua_rawseti(L, -2, i+1);
        }
        NFD_PathSet_Free(&path_set);

        return 2;
    } else if (res == NFD_ERROR) {
        lua_pushstring(L, NFD_GetError());
        return 2;
    }
    return 1;
}

static int Diags_Save(lua_State* L)
{
    int top = lua_gettop(L);

    const nfdchar_t *filter_list = 0x0;
    const nfdchar_t *default_path = 0x0;
    nfdchar_t *out_path = 0x0;

    if (top > 0 && !lua_isnil(L, 1)) {
        filter_list = luaL_checkstring(L, 1);
    }
    if (top > 1 && !lua_isnil(L, 2)) {
        default_path = luaL_checkstring(L, 2);
    }

    nfdresult_t res = NFD_SaveDialog( filter_list, default_path, &out_path );
    lua_pushinteger(L, res);
    if (res == NFD_OKAY) {
        lua_pushstring(L, out_path);
        free(out_path);
        return 2;
    } else if (res == NFD_ERROR) {
        lua_pushstring(L, NFD_GetError());
        return 2;
    }
    return 1;
}

static int Diags_Folder(lua_State* L)
{
    int top = lua_gettop(L);

    const nfdchar_t *default_path = 0x0;
    nfdchar_t *out_path = 0x0;

    if (top > 0 && !lua_isnil(L, 1)) {
        default_path = luaL_checkstring(L, 1);
    }

    nfdresult_t res = NFD_PickFolder( default_path, &out_path );
    lua_pushinteger(L, res);
    if (res == NFD_OKAY) {
        lua_pushstring(L, out_path);
        free(out_path);
        return 2;
    } else if (res == NFD_ERROR) {
        lua_pushstring(L, NFD_GetError());
        return 2;
    }
    return 1;
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
    {"open", Diags_Open},
    {"open_multiple", Diags_OpenMultiple},
    {"open_folder", Diags_Folder},
    {"save", Diags_Save},
    {0, 0}
};

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);

    // Register lua names
    luaL_register(L, MODULE_NAME, Module_methods);

    // Push result constants
    lua_pushinteger(L, NFD_ERROR);
    lua_setfield(L, -2, "ERROR");
    lua_pushinteger(L, NFD_OKAY);
    lua_setfield(L, -2, "OKAY");
    lua_pushinteger(L, NFD_CANCEL);
    lua_setfield(L, -2, "CANCEL");

    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

static dmExtension::Result AppInitializeDiags(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result InitializeDiags(dmExtension::Params* params)
{
    LuaInit(params->m_L);
    return dmExtension::RESULT_OK;
}

static dmExtension::Result UpdateDiags(dmExtension::Params* params)
{
    // lua_State* L = params->m_L;
    // int top = lua_gettop(L);
    // if (g_LastAppMenuResult != 0 && g_AppMenuCB != LUA_NOREF)
    // {
    //     lua_rawgeti(L, LUA_REGISTRYINDEX, g_AppMenuCB);
    //     lua_pushinteger(L, g_LastAppMenuResult);
    //     lua_call(L, 1, 0);
    // }
    // g_LastAppMenuResult = 0;
    // assert(top == lua_gettop(L));
    return dmExtension::RESULT_OK;
}

static dmExtension::Result AppFinalizeDiags(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeDiags(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(diags, LIB_NAME, AppInitializeDiags, AppFinalizeDiags, InitializeDiags, UpdateDiags, 0, FinalizeDiags)

#else

static dmExtension::Result AppInitializeExtension(dmExtension::AppParams* params)
{
    dmLogWarning("Registered %s (null) Extension\n", MODULE_NAME);
    return dmExtension::RESULT_OK;
}

static dmExtension::Result InitializeExtension(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result AppFinalizeExtension(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeExtension(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(diags, LIB_NAME, AppInitializeExtension, AppFinalizeExtension, InitializeExtension, 0, 0, FinalizeExtension)

#endif
