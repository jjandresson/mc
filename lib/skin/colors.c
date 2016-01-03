/*
   Skins engine.
   Work with colors

   Copyright (C) 2009-2015
   Free Software Foundation, Inc.

   Written by:
   Slava Zanko <slavazanko@gmail.com>, 2009
   Egmont Koblinger <egmont@gmail.com>, 2010
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
#include <string.h>

#include "internal.h"

#include "lib/tty/color.h"

/*** global variables ****************************************************************************/

int mc_skin_color__cache[MC_SKIN_COLOR_CACHE_COUNT];

/*** file scope macro definitions ****************************************************************/

/*** file scope type declarations ****************************************************************/

/*** file scope variables ************************************************************************/

/*** file scope functions ************************************************************************/

static mc_skin_color_t *
mc_skin_color_get_from_hash (mc_skin_t * mc_skin, const gchar * group, const gchar * key)
{
    gchar kname[BUF_TINY];
    mc_skin_color_t *mc_skin_color;

    if (group == NULL || key == NULL)
        return NULL;

    if (mc_skin == NULL)
        mc_skin = &mc_skin__default;

    g_snprintf (kname, sizeof (kname), "%s.%s", group, key);
    mc_skin_color = (mc_skin_color_t *) g_hash_table_lookup (mc_skin->colors, (gpointer) kname);

    return mc_skin_color;
}

/* --------------------------------------------------------------------------------------------- */

#if 0
static void
mc_skin_color_remove_from_hash (mc_skin_t * mc_skin, const gchar * group, const gchar * key)
{
    gchar kname[BUF_TINY];
    if (group == NULL || key == NULL)
        return;

    if (mc_skin == NULL)
        mc_skin = &mc_skin__default;

    g_snprintf (kname, sizeof (kname), "%s.%s", group, key);
    g_hash_table_remove (mc_skin->colors, (gpointer) kname);
}
#endif

/* --------------------------------------------------------------------------------------------- */

static void
mc_skin_color_add_to_hash (mc_skin_t * mc_skin, const gchar * group, const gchar * key,
                           mc_skin_color_t * mc_skin_color)
{
    gchar *kname;

    kname = g_strdup_printf ("%s.%s", group, key);
    if (kname != NULL)
    {
        if (g_hash_table_lookup (mc_skin->colors, (gpointer) kname) != NULL)
            g_hash_table_remove (mc_skin->colors, (gpointer) kname);

        g_hash_table_insert (mc_skin->colors, (gpointer) kname, (gpointer) mc_skin_color);
    }
}

/* --------------------------------------------------------------------------------------------- */

static mc_skin_color_t *
mc_skin_color_get_with_defaults (const gchar * group, const gchar * name)
{
    mc_skin_color_t *color = mc_skin_color_get_from_hash (NULL, group, name);

    if (color)
        return color;

    color = mc_skin_color_get_from_hash (NULL, group, "_default_");
    if (color)
        return color;

    color = mc_skin_color_get_from_hash (NULL, "core", "_default_");
    return color;
}

/* --------------------------------------------------------------------------------------------- */

static gchar **
mc_skin_get_string_list (mc_skin_t *mc_skin, const gchar *group, const gchar *key, gsize *pcount)
{
    const gchar *raw = mc_skin_get_string (mc_skin, group, key, NULL);
    gchar **values = NULL;

    if (raw)
        values = g_strsplit (raw, ";", -1);

    if (pcount)
    {
        gsize count;
        for (count = 0; values != NULL && *(values + count) != NULL; ++count)
            ;
        *pcount = count;
    }

    return values;
}

static mc_skin_color_t *
mc_skin_color_get_from_ini_file (mc_skin_t * skin, const gchar * group, const gchar * key)
{
    gsize items_count;
    mc_skin_color_t *color = NULL;
    mc_skin_color_t *defs = NULL;
    gchar **values = mc_skin_get_string_list (skin, group, key, &items_count);

    if (!values || !*values)
        goto done;

    color = g_malloc0 (sizeof *color);
    if (!color)
        goto done;

    /* [syntax] group defaults come from 'editor' group */
    if (strcasecmp ("syntax", group) == 0)
        defs = mc_skin_color_get_from_hash (skin, "editor", "_default_");
    else
    {
        /* substitute colors from [syntax] group */
        if (items_count == 1 && values[0][0])
        {
            mc_skin_color_t *tmp = mc_skin_color_get_from_hash (skin, "syntax", values[0]);
            if (tmp)
                color = tmp;
        }

        defs = mc_skin_color_get_with_defaults (group, "_default_");
    }

    color->fgcolor = (items_count > 0 && values[0][0]) ? g_strstrip (g_strdup (values[0])) :
        defs ? g_strdup (defs->fgcolor) : NULL;
    color->bgcolor = (items_count > 1 && values[1][0]) ? g_strstrip (g_strdup (values[1])) :
        defs ? g_strdup (defs->bgcolor) : NULL;
    color->attrs   = (items_count > 2 && values[2][0]) ? g_strstrip (g_strdup (values[2])) :
        defs ? g_strdup (defs->attrs) : NULL;

    color->pair_index =
        tty_try_alloc_color_pair2 (color->fgcolor, color->bgcolor, color->attrs, FALSE);

  done:
    if (values)
        g_strfreev (values);

    return color;
}

/* --------------------------------------------------------------------------------------------- */

static void
mc_skin_color_set_default_for_terminal (mc_skin_t * mc_skin)
{
    mc_skin_color_t *mc_skin_color;
    mc_skin_color = g_try_new0 (mc_skin_color_t, 1);
    if (mc_skin_color != NULL)
    {
        mc_skin_color->fgcolor = g_strdup ("default");
        mc_skin_color->bgcolor = g_strdup ("default");
        mc_skin_color->attrs = NULL;
        mc_skin_color->pair_index =
            tty_try_alloc_color_pair2 (mc_skin_color->fgcolor, mc_skin_color->bgcolor,
                                       mc_skin_color->attrs, FALSE);
        mc_skin_color_add_to_hash (mc_skin, "skin", "terminal_default_color", mc_skin_color);
    }
}

/* --------------------------------------------------------------------------------------------- */

static void
mc_skin_color_cache_init (void)
{
    DEFAULT_COLOR = mc_skin_color_get ("skin", "terminal_default_color");
    NORMAL_COLOR = mc_skin_color_get ("core", "_default_");
    MARKED_COLOR = mc_skin_color_get ("core", "marked");
    SELECTED_COLOR = mc_skin_color_get ("core", "selected");
    MARKED_SELECTED_COLOR = mc_skin_color_get ("core", "markselect");
    DISABLED_COLOR = mc_skin_color_get ("core", "disabled");
    REVERSE_COLOR = mc_skin_color_get ("core", "reverse");
    HEADER_COLOR = mc_skin_color_get ("core", "header");
    COMMAND_MARK_COLOR = mc_skin_color_get ("core", "commandlinemark");

    COLOR_NORMAL = mc_skin_color_get ("dialog", "_default_");
    COLOR_FOCUS = mc_skin_color_get ("dialog", "dfocus");
    COLOR_HOT_NORMAL = mc_skin_color_get ("dialog", "dhotnormal");
    COLOR_HOT_FOCUS = mc_skin_color_get ("dialog", "dhotfocus");
    COLOR_TITLE = mc_skin_color_get ("dialog", "dtitle");

    ERROR_COLOR = mc_skin_color_get ("error", "_default_");
    ERROR_FOCUS = mc_skin_color_get ("error", "errdfocus");
    ERROR_HOT_NORMAL = mc_skin_color_get ("error", "errdhotnormal");
    ERROR_HOT_FOCUS = mc_skin_color_get ("error", "errdhotfocus");
    ERROR_TITLE = mc_skin_color_get ("error", "errdtitle");

    MENU_ENTRY_COLOR = mc_skin_color_get ("menu", "_default_");
    MENU_SELECTED_COLOR = mc_skin_color_get ("menu", "menusel");
    MENU_HOT_COLOR = mc_skin_color_get ("menu", "menuhot");
    MENU_HOTSEL_COLOR = mc_skin_color_get ("menu", "menuhotsel");
    MENU_INACTIVE_COLOR = mc_skin_color_get ("menu", "menuinactive");

    PMENU_ENTRY_COLOR = mc_skin_color_get ("popupmenu", "_default_");
    PMENU_SELECTED_COLOR = mc_skin_color_get ("popupmenu", "menusel");
    PMENU_TITLE_COLOR = mc_skin_color_get ("popupmenu", "menutitle");

    BUTTONBAR_HOTKEY_COLOR = mc_skin_color_get ("buttonbar", "hotkey");
    BUTTONBAR_BUTTON_COLOR = mc_skin_color_get ("buttonbar", "button");

    STATUSBAR_COLOR = mc_skin_color_get ("statusbar", "_default_");

    GAUGE_COLOR = mc_skin_color_get ("core", "gauge");
    INPUT_COLOR = mc_skin_color_get ("core", "input");
    INPUT_HISTORY_COLOR = mc_skin_color_get ("core", "inputhistory");
    COMMAND_HISTORY_COLOR = mc_skin_color_get ("core", "commandhistory");
    INPUT_MARK_COLOR = mc_skin_color_get ("core", "inputmark");
    INPUT_UNCHANGED_COLOR = mc_skin_color_get ("core", "inputunchanged");

    HELP_NORMAL_COLOR = mc_skin_color_get ("help", "_default_");
    HELP_ITALIC_COLOR = mc_skin_color_get ("help", "helpitalic");
    HELP_BOLD_COLOR = mc_skin_color_get ("help", "helpbold");
    HELP_LINK_COLOR = mc_skin_color_get ("help", "helplink");
    HELP_SLINK_COLOR = mc_skin_color_get ("help", "helpslink");
    HELP_TITLE_COLOR = mc_skin_color_get ("help", "helptitle");

    VIEW_NORMAL_COLOR = mc_skin_color_get ("viewer", "_default_");
    VIEW_BOLD_COLOR = mc_skin_color_get ("viewer", "viewbold");
    VIEW_UNDERLINED_COLOR = mc_skin_color_get ("viewer", "viewunderline");
    VIEW_SELECTED_COLOR = mc_skin_color_get ("viewer", "viewselected");

    EDITOR_NORMAL_COLOR = mc_skin_color_get ("editor", "_default_");
    EDITOR_BOLD_COLOR = mc_skin_color_get ("editor", "editbold");
    EDITOR_MARKED_COLOR = mc_skin_color_get ("editor", "editmarked");
    EDITOR_WHITESPACE_COLOR = mc_skin_color_get ("editor", "editwhitespace");
    EDITOR_RIGHT_MARGIN_COLOR = mc_skin_color_get ("editor", "editrightmargin");
    LINE_STATE_COLOR = mc_skin_color_get ("editor", "editlinestate");
    EDITOR_BACKGROUND = mc_skin_color_get ("editor", "editbg");
    EDITOR_FRAME = mc_skin_color_get ("editor", "editframe");
    EDITOR_FRAME_ACTIVE = mc_skin_color_get ("editor", "editframeactive");
    EDITOR_FRAME_DRAG = mc_skin_color_get ("editor", "editframedrag");

    BOOK_MARK_COLOR = mc_skin_color_get ("editor", "bookmark");
    BOOK_MARK_FOUND_COLOR = mc_skin_color_get ("editor", "bookmarkfound");

    DFF_ADD_COLOR = mc_skin_color_get ("diffviewer", "added");
    DFF_CHG_COLOR = mc_skin_color_get ("diffviewer", "changedline");
    DFF_CHH_COLOR = mc_skin_color_get ("diffviewer", "changednew");
    DFF_CHD_COLOR = mc_skin_color_get ("diffviewer", "changed");
    DFF_DEL_COLOR = mc_skin_color_get ("diffviewer", "removed");
    DFF_ERROR_COLOR = mc_skin_color_get ("diffviewer", "error");
}

/* --------------------------------------------------------------------------------------------- */

static gboolean
mc_skin_color_check_inisection (const gchar * group)
{
    return !((strcasecmp ("skin", group) == 0) || (strcasecmp ("syntax", group) == 0)
             || (strcasecmp ("lines", group) == 0) || (strncasecmp ("widget-", group, 7) == 0));
}

/* --------------------------------------------------------------------------------------------- */

static void
mc_skin_color_check_bw_mode (mc_skin_t * mc_skin)
{
    gint _G;

    if (tty_use_colors () && !mc_global.tty.disable_colors)
        return;

    LUAMC_GUARD (Lg);
    lua_getglobal (Lg, "_G");
    _G = lua_gettop (Lg);

    lua_pushnil (Lg);
    while (lua_next (Lg, _G) != 0)
    {
        const gchar *group = lua_tostring (Lg, -2);
        if (mc_skin_color_check_inisection (group))
        {
            lua_pushnil (Lg);
            luaMC_rawsetfield (Lg, _G, group);
        }
        lua_pop (Lg, 1);		/* pop value, leave key */
    }
    lua_pop (Lg, 1);			/* pop _G */
    LUAMC_UNGUARD (Lg);

    mc_skin_hardcoded_blackwhite_colors (mc_skin);
}

/* --------------------------------------------------------------------------------------------- */
/*** public functions ****************************************************************************/
/* --------------------------------------------------------------------------------------------- */

gboolean
mc_skin_color_parse_ini_file (mc_skin_t *skin)
{
    gint _G;
    mc_skin_color_t *color;

    mc_skin_color_check_bw_mode (skin);

    /* First, set up default colors */
    mc_skin_color_set_default_for_terminal (skin);
    color = mc_skin_color_get_from_ini_file (skin, "core", "_default_");
    if (!color)
        return FALSE;
    tty_color_set_defaults (color->fgcolor, color->bgcolor, color->attrs);
    mc_skin_color_add_to_hash (skin, "core", "_default_", color);

    LUAMC_GUARD (Lg);
    lua_getglobal (Lg, "_G");			/* push _G */
    _G = lua_gettop (Lg);

    lua_pushnil (Lg);
    while (lua_next (Lg, _G) != 0)		/* push next group & table */
    {
        const gchar *group = lua_tostring (Lg, -2);
        if (mc_skin_color_check_inisection (group) && lua_istable (Lg, -1))
        {
            gint t = lua_gettop (Lg);

            lua_pushnil (Lg);
            while (lua_next (Lg, t) != 0)	/* push next name & value */
            {
                const gchar *name = lua_tostring (Lg, -2);
                color = mc_skin_color_get_from_ini_file (skin, group, name);
                if (color != NULL)
                    mc_skin_color_add_to_hash (skin, group, name, color);
                lua_pop (Lg, 1);		/* pop inner value, leaving key */
            }
        }
        lua_pop (Lg, 1);			/* pop outer value, leaving key */
    }
    lua_pop (Lg, 1);				/* pop _G */
    LUAMC_UNGUARD (Lg);

    mc_skin_color_cache_init ();
    return TRUE;
}

/* --------------------------------------------------------------------------------------------- */

int
mc_skin_color_get (const gchar * group, const gchar * name)
{
    mc_skin_color_t *mc_skin_color;

    mc_skin_color = mc_skin_color_get_with_defaults (group, name);

    return (mc_skin_color != NULL) ? mc_skin_color->pair_index : 0;
}

/* --------------------------------------------------------------------------------------------- */

int
mc_skin_color_lookup (const gchar * group, const gchar * name)
{
    mc_skin_color_t *mc_skin_color;

    mc_skin_color = mc_skin_color_get_from_hash (NULL, group, name);

    return (mc_skin_color != NULL) ? mc_skin_color->pair_index : 0;
}

/* --------------------------------------------------------------------------------------------- */
