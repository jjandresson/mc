/*
   Skins engine.
   Interface functions

   Copyright (C) 2015
   Free Software Foundation, Inc.

   Written by:
   Jon Andresson <jjandresson@gmail.com>, 2015

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
#include <stdlib.h>

#include "internal.h"
#include "lib/util.h"

#include "lib/tty/color.h"      /* tty_use_256colors(); */

/*** global variables ****************************************************************************/

mc_skin_t mc_skin__default;

/*** file scope macro definitions ****************************************************************/

/*** file scope type declarations ****************************************************************/

/*** file scope variables ************************************************************************/

static gboolean mc_skin_is_init = FALSE;

/* --------------------------------------------------------------------------------------------- */
/*** file scope functions ************************************************************************/
/* --------------------------------------------------------------------------------------------- */

static void
mc_skin_hash_destroy_value (gpointer data)
{
    mc_skin_color_t *mc_skin_color = (mc_skin_color_t *) data;
    g_free (mc_skin_color->fgcolor);
    g_free (mc_skin_color->bgcolor);
    g_free (mc_skin_color->attrs);
    g_free (mc_skin_color);
}

/* --------------------------------------------------------------------------------------------- */

static char *
mc_skin_get_default_name (void)
{
    char *tmp_str;

    /* from command line */
    if (mc_global.tty.skin != NULL)
        return g_strdup (mc_global.tty.skin);

    /* from envirovement variable */
    tmp_str = getenv ("MC_SKIN");
    if (tmp_str != NULL)
        return g_strdup (tmp_str);

    /*  from config. Or 'default' if no present in config */
    return mc_config_get_string (mc_main_config, CONFIG_APP_SECTION, "skin", "default");
}

/* --------------------------------------------------------------------------------------------- */

static void
mc_skin_reinit (void)
{
    mc_skin_deinit ();
    mc_skin__default.name = mc_skin_get_default_name ();
    mc_skin__default.colors = g_hash_table_new_full (g_str_hash, g_str_equal,
                                                     g_free, mc_skin_hash_destroy_value);
}

/* --------------------------------------------------------------------------------------------- */

static void
mc_skin_try_to_load_default (void)
{
    mc_skin_reinit ();
    g_free (mc_skin__default.name);
    mc_skin__default.name = g_strdup ("default");
    if (!mc_skin_ini_file_load (&mc_skin__default))
    {
        mc_skin_reinit ();
        mc_skin_set_hardcoded_skin (&mc_skin__default);
    }

}

/* --------------------------------------------------------------------------------------------- */
/*** public functions ****************************************************************************/
/* --------------------------------------------------------------------------------------------- */

gboolean
mc_skin_init (const gchar * skin_override, GError ** mcerror)
{
    gboolean is_good_init = TRUE;

    mc_return_val_if_error (mcerror, FALSE);

    mc_skin__default.have_256_colors = FALSE;

    mc_skin__default.name =
        skin_override != NULL ? g_strdup (skin_override) : mc_skin_get_default_name ();

    mc_skin__default.colors = g_hash_table_new_full (g_str_hash, g_str_equal,
                                                     g_free, mc_skin_hash_destroy_value);

    if (!mc_skin_ini_file_load (&mc_skin__default))
    {
        mc_propagate_error (mcerror, 0,
                            _("Unable to load '%s' skin.\nDefault skin has been loaded"),
                            mc_skin__default.name);
        mc_skin_try_to_load_default ();
        is_good_init = FALSE;
    }

    if (!mc_skin_ini_file_parse (&mc_skin__default))
    {
        mc_propagate_error (mcerror, 0,
                            _("Unable to parse '%s' skin.\nDefault skin has been loaded"),
                            mc_skin__default.name);

        mc_skin_try_to_load_default ();
        (void) mc_skin_ini_file_parse (&mc_skin__default);
        is_good_init = FALSE;
    }
    if (is_good_init && !tty_use_256colors () && mc_skin__default.have_256_colors)
    {
        mc_propagate_error (mcerror, 0,
                            _
                            ("Unable to use '%s' skin with 256 colors support\non non-256 colors terminal.\nDefault skin has been loaded"),
                            mc_skin__default.name);
        mc_skin_try_to_load_default ();
        (void) mc_skin_ini_file_parse (&mc_skin__default);
        is_good_init = FALSE;
    }
    mc_skin_is_init = TRUE;
    return is_good_init;
}

/* --------------------------------------------------------------------------------------------- */

void
mc_skin_deinit (void)
{
    tty_color_free_all_tmp ();
    tty_color_free_all_non_tmp ();

    MC_PTR_FREE (mc_skin__default.name);
    g_hash_table_destroy (mc_skin__default.colors);
    mc_skin__default.colors = NULL;

    MC_PTR_FREE (mc_skin__default.description);

    mc_skin_is_init = FALSE;
}

/* --------------------------------------------------------------------------------------------- */

void
mc_skin_set_string (mc_skin_t *mc_skin, const gchar *group, const gchar *name, const gchar *value)
{
    LUAMC_GUARD (Lg);
    lua_getglobal (Lg, "_G");
    luaMC_rawgetfield (Lg, -1, group);	/* push _G[group] */
    if (lua_isnil (Lg, -1))
    {
        lua_pop (Lg, 1);		/* pop `nil` */
        lua_newtable (Lg);		/* push {} */
        lua_pushstring (Lg, group);	/* push group */
        lua_pushvalue (Lg, -2);		/* copy of {} for rawset */
        lua_rawset (Lg, -4);		/* _G[group] = {} */
    }
    if (lua_istable (Lg, -1))
    {
        lua_pushstring (Lg, name);	/* push name */
        lua_pushstring (Lg, value);	/* push value */
        lua_rawset (Lg, -3);		/* group[name] = value */
    }
    lua_pop (Lg, 2);			/* pop _G[group] and _G tables */
    LUAMC_UNGUARD (Lg);
}


gchar *
mc_skin_get_string (mc_skin_t *mc_skin, const gchar *group, const gchar *name, const gchar *def)
{
    gchar *str;

    LUAMC_GUARD (Lg);
    lua_getglobal (Lg, "_G");
    luaMC_rawgetfield (Lg, -1, group);	/* push _G[group] */
    if (!lua_istable (Lg, -1))
    {
        lua_pop (Lg, 2);		/* pop _G[group] and _G */
        return g_strdup (def);
    }

    luaMC_rawgetfield (Lg, -1, name);	/* push group[name] */
    if (lua_isstring (Lg, -1))
        str = g_strdup (lua_tostring (Lg, -1));
    else
        str = g_strdup (def);

    lua_pop (Lg, 3);			/* pop group[name], _G[group] and _G */
    LUAMC_UNGUARD (Lg);

    return str;
}


gboolean
mc_skin_get_bool (mc_skin_t *mc_skin, const gchar *group, const gchar *name, gboolean def)
{
    gboolean truth;

    LUAMC_GUARD (Lg);
    lua_getglobal (Lg, "_G");
    luaMC_rawgetfield (Lg, -1, group);	/* push _G[group] */
    if (!lua_istable (Lg, -1))
    {
        lua_pop (Lg, 2);		/* pop _G[group] and _G */
        return def;
    }

    luaMC_rawgetfield (Lg, -1, name);	/* push group[name] */
    if (lua_isboolean (Lg, -1))
        truth = lua_toboolean (Lg, -1);
    else
        truth = def;

    lua_pop (Lg, 3);			/* pop group[name], _G[group] and _G */
    LUAMC_UNGUARD (Lg);

    return truth;
}


gchar *
mc_skin_get (const gchar * group, const gchar * key, const gchar * default_value)
{
    if (mc_global.tty.ugly_line_drawing)
    {
        return g_strdup (default_value);
    }
    return mc_skin_get_string (&mc_skin__default, group, key, default_value);
}

/* --------------------------------------------------------------------------------------------- */

gchar *
mc_skin_get_name (void)
{
    return g_strdup (mc_skin__default.name);
}

/* --------------------------------------------------------------------------------------------- */
