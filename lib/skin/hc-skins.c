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

/*** global variables ****************************************************************************/

/*** file scope macro definitions ****************************************************************/

#define set_lines(x,y) mc_skin_set_string(mc_skin, "Lines", x, y)

/*** file scope type declarations ****************************************************************/

/*** file scope variables ************************************************************************/

/*** file scope functions ************************************************************************/
/* --------------------------------------------------------------------------------------------- */

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
    /* single lines */
    set_lines ("vert", " ");
    set_lines ("horiz", " ");
    set_lines ("lefttop", " ");
    set_lines ("righttop", " ");
    set_lines ("leftbottom", " ");
    set_lines ("rightbottom", " ");
    set_lines ("topmiddle", " ");
    set_lines ("bottommiddle", " ");
    set_lines ("leftmiddle", " ");
    set_lines ("rightmiddle", " ");
    set_lines ("cross", " ");

    set_lines ("dvert", " ");
    set_lines ("dhoriz", " ");
    set_lines ("dlefttop", " ");
    set_lines ("drighttop", " ");
    set_lines ("dleftbottom", " ");
    set_lines ("drightbottom", " ");
    set_lines ("dtopmiddle", " ");
    set_lines ("dbottommiddle", " ");
    set_lines ("dleftmiddle", " ");
    set_lines ("drightmiddle", " ");
}

/* --------------------------------------------------------------------------------------------- */

void
mc_skin_hardcoded_ugly_lines (mc_skin_t * mc_skin)
{
    /* single lines */
    set_lines ("vert", "|");
    set_lines ("horiz", "-");
    set_lines ("lefttop", "+");
    set_lines ("righttop", "+");
    set_lines ("leftbottom", "+");
    set_lines ("rightbottom", "+");
    set_lines ("topmiddle", "-");
    set_lines ("bottommiddle", "-");
    set_lines ("leftmiddle", "|");
    set_lines ("rightmiddle", "|");
    set_lines ("cross", "+");

    /* double lines */
    set_lines ("dvert", "|");
    set_lines ("dhoriz", "-");
    set_lines ("dlefttop", "+");
    set_lines ("drighttop", "+");
    set_lines ("dleftbottom", "+");
    set_lines ("drightbottom", "+");
    set_lines ("dtopmiddle", "-");
    set_lines ("dbottommiddle", "-");
    set_lines ("dleftmiddle", "|");
    set_lines ("drightmiddle", "|");
}

/* --------------------------------------------------------------------------------------------- */
