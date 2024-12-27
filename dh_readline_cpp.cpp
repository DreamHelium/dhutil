/*  dh_readline_cpp - Readline support
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

#include "dh_readline_cpp.hpp"
#include <readline/readline.h>
#include "dh_validator_cpp.hpp"
#include "dh_string_util.h"
#include <iostream>

static dh::Arg* arg = nullptr;

void dhutil_set_completion(void* args)
{
    arg = dynamic_cast<dh::Arg*>((dh::Arg*)args);
}

/* Generator function for command completion.  STATE lets us know whether
   to start from scratch; without any state (i.e. STATE == 0), then we
   start at the top of the list. */
static char* command_generator(const char *text, int state)
{
    int list_index = state, len = 0;
    // char *name;

    // /* If this is a new word to complete, initialize now.  This includes
    //     saving the length of TEXT for efficiency, and initializing the index
    //     variable to 0. */
    // if (!state)
    // {
    //     list_index = 0;
        len = strlen(text);
    // }
    if(arg)
    {
        std::vector<std::string> fullnames;
        for(auto fullname : arg->get_arg_fullnames())
        {
            for(auto str : fullname)
            {
                fullnames.push_back(str);
            }
        }
        std::string str;
        while(list_index < fullnames.size() && !(str = fullnames[list_index]).empty())
        {
            list_index++;
            if(!strncmp(str.c_str(), text, len)) 
                return dh_strdup(str.c_str());
        }
        list_index -= fullnames.size();
        if(*text == 0)
        {
            std::string ret;
            char ret_char;
            while(list_index < arg->get_args().size() && (ret_char = arg->get_args()[list_index]) != 0)
            {
                list_index++;
                ret.assign(1, ret_char);
                return dh_strdup(ret.c_str());
            }
        }
    }

    /* Return the next name which partially matches from the command list. */
    // while (name = commands[list_index].name)
    //     {
    //     list_index++;

    //     if (strncmp (name, text, len) == 0)
    //         return (dupstr(name));
    //     }

    /* If no names matched, then return NULL. */
    return nullptr;
}

/* Attempt to complete on the contents of TEXT.  START and END bound the
   region of rl_line_buffer that contains the word to complete.  TEXT is
   the word to complete.  We can use the entire contents of rl_line_buffer
   in case we want to do some simple parsing.  Return the array of matches,
   or NULL if there aren't any. */
static char** dhutil_completion (const char* text, int start, int end)
{
  char** matches = NULL;

  /* If this word is at the start of the line, then it is a command
     to complete.  Otherwise it is the name of a file in the current
     directory. */
  if (start == 0)
    matches = rl_completion_matches (text, command_generator);
  return (matches);
}

void init_readline(const char* name)
{
    rl_readline_name = name;
    rl_attempted_completion_function = dhutil_completion;
}





