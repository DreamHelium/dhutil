/*  dh_validator_cpp - Validator class
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

#include "dh_validator_cpp.hpp"
#include <any>
#include <iostream>
#include <cstring>

std::vector<std::string> string_list;

static char* get_line(const char* prompt)
{
    if(prompt) std::cout << prompt;
    std::string str;
    std::getline(std::cin, str);
    if(!str.empty())
    {
        int str_size = str.size();
        char* ret = (char*)malloc((str_size+1) * sizeof(char));
        strcpy(ret, str.c_str());
        return ret;
    }
    else return nullptr;
}

static DhReadlineFns fns = {get_line, NULL, NULL};

namespace dh{
    template<typename T>
    std::any get_output(Validator<T>* validator, Arg* arg, DhReadlineFns* fns, const char* prompt)
    {
        IntValidator* int_validator;
        if((int_validator = dynamic_cast<IntValidator*>(validator)) != nullptr)
        {
            if(int_validator->get_base() == 16)
            {
                std::cerr << "Warning: the base is 16, might be conflict with arg!" << std::endl;
            }
        }
        while(true && fns)
        {
            char* get_str = fns->readline_fn(prompt);
            std::string str = get_str;
            if(fns->add_history) fns->add_history(get_str);
            free(get_str);
            std::string striped_str = str;
            string_strip(striped_str);
            if(arg && striped_str == "?")
            {
                std::cout << arg->get_help();
                continue;
            }
            
            if(validator && validator->validate(str))
            {
                return validator->get_result();
            }
            else if(arg && arg->validate(str))
            {
                return arg->get_pos_char();
            }
            else if(validator || arg)
            {
                std::cerr << "Validate Error!" << std::endl;
                if(str.empty()) return nullptr;
                continue;
            }
            else return str;
        }
    }
}

extern "C"
{
    /* This is a sample of the string add & view func
     * Showing in Chinese */
    void test()
    {
        while(true)
        {
        dh::Arg arg;
        std::vector<std::string> vstr = {"add", "添加"};
        arg.add_arg('a', vstr, "添加字符串");
        arg.add_arg('v', "view", "查看字符串");
        std::cout << "[0] 添加字符串\n" << "[1] 查看字符串\n";
        char val = dh::get_output(&arg, &fns, "选择选项: ", true);
        if(val == 'a')
        {
            std::any str = dh::get_output(nullptr, &fns, "请输入字符串: ");
            try {
                std::string str_val = std::any_cast<std::string>(str);
                string_list.push_back(str_val);
            } catch (const std::bad_any_cast& e) {
                break;
            }
        }
        else if(val == 'v')
        {
            for(auto str : string_list)
                std::cout << str << std::endl;
        }
        else if(val == 0) 
        {
            break;}
        }
    }
}