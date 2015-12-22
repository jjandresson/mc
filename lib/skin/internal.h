#ifndef MC__SKIN_INTERNAL_H
#define MC__SKIN_INTERNAL_H

#include "lib/global.h"
#include "lib/skin.h"

/*** typedefs(not structures) and defined constants **********************************************/

/*** enums ***************************************************************************************/

/*** structures declarations (and typedefs of structures)*****************************************/

typedef struct mc_skin_color_struct
{
    gchar *fgcolor;
    gchar *bgcolor;
    gchar *attrs;
    int pair_index;
} mc_skin_color_t;

typedef struct mc_skin_struct
{
    gchar *name;
    gchar *description;
    mc_config_t *config;
    GHashTable *colors;
    gboolean have_256_colors;
} mc_skin_t;

/*** global variables defined in .c file *********************************************************/

extern mc_skin_t mc_skin__default;

/*** declarations of public functions ************************************************************/

gboolean mc_skin_ini_file_load (mc_skin_t *);
gboolean mc_skin_ini_file_parse (mc_skin_t *);
void mc_skin_set_hardcoded_skin (mc_skin_t *);

gboolean mc_skin_ini_file_parse_colors (mc_skin_t *);
gboolean mc_skin_color_parse_ini_file (mc_skin_t *);
void mc_skin_lines_parse_ini_file (mc_skin_t *);

void mc_skin_hardcoded_ugly_lines (mc_skin_t *);
void mc_skin_hardcoded_space_lines (mc_skin_t *);
void mc_skin_hardcoded_blackwhite_colors (mc_skin_t *);

/*** inline functions ****************************************************************************/

#endif /* MC__SKIN_INTERNAL_H */
