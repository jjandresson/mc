/*
   Skins engine.
   Set of hardcoded skins

   Copyright (C) 2009-2015
   Free Software Foundation, Inc.

   Written by:
   Slava Zanko <slavazanko@gmail.com>, 2009
   Andrew Borodin <aborodin@vmail.ru>, 2012

   This file is part of the Midnight Commander.

   The Midnight Commander is free software: you can redistribute it
   and/or modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the License,
   or (at your option) any later version.

   The Midnight Commander is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>

#include "internal.h"
#include "lib/tty/tty.h"

/*** global variables ****************************************************************************/

/*** file scope macro definitions ****************************************************************/

/*** file scope type declarations ****************************************************************/

/*** file scope variables ************************************************************************/

/*** file scope functions ************************************************************************/

/* Must be in the same order as enum mc_tty_frm_t entries! */
static const char *tty_frm_map[] = {
    "vert",
    "horiz",
    "lefttop",
    "righttop",
    "leftbottom",
    "rightbottom",
    "topmiddle",
    "bottommiddle",
    "leftmiddle",
    "rightmiddle",
    "cross",
    "dvert",
    "dhoriz",
    "dlefttop",
    "drighttop",
    "dleftbottom",
    "drightbottom",
    "dtopmiddle",
    "dbottommiddle",
    "dleftmiddle",
    "drightmiddle",
    NULL,
};


static int
TTYLines (lua_State *L)
{
    int i;

    luaL_checktype (L, 1, LUA_TTABLE);
    for (i = 0; i < MC_TTY_FRM_MAX; ++i)
    {
        lua_getfield (L, 1, tty_frm_map[i]);
        if (lua_isnil (L, -1))
            return luaL_error (L, "missing key '%s' in TTYLines", tty_frm_map[i]);
        mc_tty_frm[i] = mc_tty_normalize_lines_char (lua_tostring (L, -1));
        lua_pop (L, 1);
    }

    return 1;
}


/* --------------------------------------------------------------------------------------------- */
/*** public functions ****************************************************************************/
/* --------------------------------------------------------------------------------------------- */

void
mc_skin_hardcoded_blackwhite_colors (mc_skin_t * mc_skin)
{
    mc_skin_set_string (mc_skin, "core", "_default_", "default;default");
    mc_skin_set_string (mc_skin, "core", "selected", "A_REVERSE");
    mc_skin_set_string (mc_skin, "core", "marked", "A_BOLD");
    mc_skin_set_string (mc_skin, "core", "markselect", "A_BOLD_REVERSE");
    mc_skin_set_string (mc_skin, "core", "disabled", "default");
    mc_skin_set_string (mc_skin, "core", "reverse", "A_REVERSE");
    mc_skin_set_string (mc_skin, "dialog", "_default_", "A_REVERSE");
    mc_skin_set_string (mc_skin, "dialog", "dfocus", "A_BOLD");
    mc_skin_set_string (mc_skin, "dialog", "dhotnormal", "A_UNDERLINE");
    mc_skin_set_string (mc_skin, "dialog", "dhotfocus", "A_UNDERLINE");
    mc_skin_set_string (mc_skin, "error", "_default_", "A_BOLD");
    mc_skin_set_string (mc_skin, "menu", "_default_", "A_REVERSE");
    mc_skin_set_string (mc_skin, "menu", "menuhot", "A_BOLD");
    mc_skin_set_string (mc_skin, "menu", "menusel", "default");
    mc_skin_set_string (mc_skin, "menu", "menuhotsel", "A_UNDERLINE");
    mc_skin_set_string (mc_skin, "menu", "menuinactive", "A_REVERSE");
    mc_skin_set_string (mc_skin, "popupmenu", "_default_", "A_REVERSE");
    mc_skin_set_string (mc_skin, "popupmenu", "menusel", "default");
    mc_skin_set_string (mc_skin, "popupmenu", "menutitle", "A_REVERSE");
    mc_skin_set_string (mc_skin, "statusbar", "_default_", "A_REVERSE");
    mc_skin_set_string (mc_skin, "help", "_default_", "A_REVERSE");
    mc_skin_set_string (mc_skin, "help", "helpitalic", "A_REVERSE");
    mc_skin_set_string (mc_skin, "help", "helpbold", "A_REVERSE");
    mc_skin_set_string (mc_skin, "help", "helpslink", "A_BOLD");
    mc_skin_set_string (mc_skin, "viewer", "viewunderline", "A_UNDERLINE");
    mc_skin_set_string (mc_skin, "editor", "editbold", "A_BOLD");
    mc_skin_set_string (mc_skin, "editor", "editmarked", "A_REVERSE");
    mc_skin_set_string (mc_skin, "editor", "editframeactive", "A_BOLD");
    mc_skin_set_string (mc_skin, "editor", "editframedrag", "A_REVERSE");
    mc_skin_set_string (mc_skin, "buttonbar", "hotkey", "default");
    mc_skin_set_string (mc_skin, "buttonbar", "button", "A_REVERSE");
}

/* --------------------------------------------------------------------------------------------- */

void
mc_skin_hardcoded_space_lines (mc_skin_t * mc_skin)
{
    int errors;

    LUAMC_GUARD (Lg);
    lua_getglobal (Lg, "_G");
    lua_pushcfunction (Lg, TTYLines);
    luaMC_rawsetfield (Lg, -2, "TTYLines");
    lua_pop (Lg, 1);

    errors = luaL_dostring (Lg, "\n\
      declare 'Lines' --FIXME!\n\
      Lines = TTYLines {\n\
        -- single lines\n\
        vert = ' ',\n\
        horiz = ' ',\n\
        lefttop = ' ',\n\
        righttop = ' ',\n\
        leftbottom = ' ',\n\
        rightbottom = ' ',\n\
        topmiddle = ' ',\n\
        bottommiddle = ' ',\n\
        leftmiddle = ' ',\n\
        rightmiddle = ' ',\n\
        cross = ' ',\n\
      \n\
        -- double lines\n\
        dvert = ' ',\n\
        dhoriz = ' ',\n\
        dlefttop = ' ',\n\
        drighttop = ' ',\n\
        dleftbottom = ' ',\n\
        drightbottom = ' ',\n\
        dtopmiddle = ' ',\n\
        dbottommiddle = ' ',\n\
        dleftmiddle = ' ',\n\
        drightmiddle = ' ',\n\
      }\n\
    ");

    if (errors)
        /* never returns! */
        luaL_error (Lg, "unable to load TTYLines: %s", lua_tostring (Lg, -1));

    LUAMC_UNGUARD (Lg);
}

/* --------------------------------------------------------------------------------------------- */

void
mc_skin_hardcoded_ugly_lines (mc_skin_t * mc_skin)
{
    int errors;

    LUAMC_GUARD (Lg);
    lua_getglobal (Lg, "_G");
    lua_pushcfunction (Lg, TTYLines);
    luaMC_rawsetfield (Lg, -2, "TTYLines");
    lua_pop (Lg, 1);

    errors = luaL_dostring (Lg, "\n\
      declare 'Lines' --FIXME!\n\
      Lines = TTYLines {\n\
        -- single lines\n\
        vert = '|',\n\
        horiz = '-',\n\
        lefttop = '+',\n\
        righttop = '+',\n\
        leftbottom = '+',\n\
        rightbottom = '+',\n\
        topmiddle = '-',\n\
        bottommiddle = '-',\n\
        leftmiddle = '|',\n\
        rightmiddle = '|',\n\
        cross = '+',\n\
      \n\
        -- double lines\n\
        dvert = '|',\n\
        dhoriz = '-',\n\
        dlefttop = '+',\n\
        drighttop = '+',\n\
        dleftbottom = '+',\n\
        drightbottom = '+',\n\
        dtopmiddle = '-',\n\
        dbottommiddle = '-',\n\
        dleftmiddle = '|',\n\
        drightmiddle = '|',\n\
      }\n\
    ");

    if (errors)
        /* never returns! */
        luaL_error (Lg, "unable to load TTYLines: %s", lua_tostring (Lg, -1));

    LUAMC_UNGUARD (Lg);
}

/* --------------------------------------------------------------------------------------------- */
