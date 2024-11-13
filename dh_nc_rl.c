/*  dh_nc_rl - Ncurses Readline replacement by Dream Helium
    Copyright (C) 2024 Dream Helium
    This file is part of dhutil.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include "dh_nc_rl.h"
#include "dh_string_util.h"
#include "glib.h"
#include <ncursesw/ncurses.h>

static WINDOW* input_line = NULL;
static WINDOW* info_line = NULL;
static const char* default_arg_tip = NULL;
static char* input = NULL;
static int input_len = 0;

typedef struct DhArg {
    char arg;
    DhStrArray* full_arg;
    char* description;
    int arg_num;
} DhArg;

static void init_line(int len)
{
    int info_line_len = len > LINES ? LINES : len;
    input_line = newwin(1, COLS, LINES - 1, 0);
    info_line = newwin(info_line_len, COLS, LINES - info_line_len - 1, 0);
    noecho();
}

static void delete_line()
{
    delwin(input_line);
    delwin(info_line);
}

static void dharg_free(gpointer data)
{
    DhArg* arg = data;
    dh_str_array_free(arg->full_arg);
    g_free(arg->description);
    g_free(arg);
}

static int compare_arg(gconstpointer a, gconstpointer b)
{
    const DhArg* arg_a = a;
    const DhArg* arg_b = b;
    return arg_a->arg_num - arg_b->arg_num;
}

void dh_nc_rl_set_default_arg_tip(const char* tip)
{
    default_arg_tip = tip;
}

static void input_line_get_char()
{
    char c = wgetch(input_line);
    if(!input || input_len <= strlen(input))
    {
        if(!input) input = g_new0(char, 5);
        input_len += 5;
        input = g_realloc(input, (input_len + 1) * sizeof(char));
    }
    char* input_d = input;
    while(*input_d) input_d++;
    input_d[0] = c;
    input_d[1] = 0;

    wprintw(input_line, "%s", "input");
    wrefresh(input_line);
    g_usleep(G_USEC_PER_SEC * 1);
}

char dh_nc_rl_get_arg(DhArgs* args)
{
    init_line(g_list_length(args));
    input_line_get_char();
    delete_line();
    return 0;
}

DhArgs* dh_args_add_arg(DhArgs* o_args, char arg, DhStrArray* full_arg, char* description, int arg_num)
{
    GList* ret = NULL;
    DhArg* dh_arg = g_new0(DhArg, 1);
    dh_arg->arg = arg;
    dh_arg->full_arg = full_arg;
    dh_arg->arg_num = arg_num;
    dh_arg->description = g_strdup(description);

    ret = g_list_append(o_args, dh_arg);
    ret = g_list_sort(ret, compare_arg);
    return ret;
}

void dh_args_free(DhArgs* arg)
{
    g_list_free_full(arg, dharg_free);
}