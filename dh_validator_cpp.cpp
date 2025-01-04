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
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <stdexcept>

std::vector<std::string> string_list;

namespace dh{
    template<>
    int64_t get_num_internal<int64_t>(const char* str, char** end_ptr, int base)
    {
        return strtoll(str, end_ptr, base);
    }

    template<>
    double get_num_internal<double>(const char* str, char** end_ptr, int base)
    {
        return strtod(str, end_ptr);
    }

    template<>
    uint64_t get_num_internal<uint64_t>(const char *str, char **end_ptr, int base)
    {
        return strtoull(str, end_ptr, base);
    }

    std::any GetOutput::get_output(Validator* validator, Arg* arg, DhReadlineFns* fns, const char* prompt)
    {
        RangeValidator<int64_t>* int_validator;
        if((int_validator = dynamic_cast<RangeValidator<int64_t>*>(validator)) != nullptr)
        {
            if(int_validator->get_base() == 16)
            {
                std::cerr << "Warning: the base is 16, might be conflict with arg!" << std::endl;
            }
        }
        while(true && fns && fns->readline_fn)
        {
            if(fns->set_completion) fns->set_completion((void*)arg);
            if(fns->init) fns->init(package_name);
            
            std::string new_prompt = prompt;
            if(arg) new_prompt += arg->get_argument();

            char* get_str = fns->readline_fn(new_prompt.c_str());
            std::string str;
            if(get_str) str = get_str;
            if(fns->add_history && get_str) fns->add_history(get_str);
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
                std::cerr << err_message << std::endl;
                if(!get_str) return nullptr; /* EOF */
                continue;
            }
            else if(get_str)
                return str;
            else return nullptr;
        }
        std::cerr << "No readline function specified!" << std::endl;
        throw std::runtime_error("No readline function specified!");
    }
}

extern "C"
{
#include <glib-object.h>

    char* cpp_get_line(const char* prompt)
    {
        if(prompt) std::cout << prompt;
        std::string str;
        if(std::getline(std::cin, str))
        {
            int str_size = str.size();
            char* ret = (char*)malloc((str_size+1) * sizeof(char));
            strcpy(ret, str.c_str());
            return ret;
        }
        else return nullptr;
    }

    void dh_set_err_message(const char* str)
    {
        dh::GetOutput::change_err_message(str);
    }

    void dh_set_package_name(const char* str)
    {
        dh::GetOutput::change_package_name(str);
    }

    void* dh_int_validator_new(int64_t min, int64_t max, int base)
    {
        return (void*)new dh::RangeValidator<int64_t>(min, max, base);
    }

    void* dh_arg_new()
    {
        return (void*)new dh::Arg;
    }

    void dh_arg_add_arg(void* arg, char c, const char* str, const char* description)
    {
        dh::Arg* args = dynamic_cast<dh::Arg*>((dh::Arg*)arg);
        if(args)
            args->add_arg(c, str, description);
    }

    void dh_arg_add_arg_multi(void* arg, char c, char* const* strv, const char* description)
    {
        dh::Arg* args = dynamic_cast<dh::Arg*>((dh::Arg*)arg);
        if(args)
        {
            std::vector<std::string> string_list;
            for(; strv && *strv ; strv++)
                string_list.push_back(*strv);
            args->add_arg(c, string_list, description);
        }
    }

    void dh_int_validator_free(void* ptr)
    {
        dh::RangeValidator<int64_t>* validator = dynamic_cast<dh::RangeValidator<int64_t>*>((dh::RangeValidator<int64_t>*)ptr);
        if(validator)
            delete validator;
    }

    void dh_arg_free(void* ptr)
    {
        dh::Arg* args = dynamic_cast<dh::Arg*>((dh::Arg*)ptr);
        if(args)
            delete args;
    }

    static void set_value(std::any o_val, GValue* value)
    {
        /* Now only support int64_t, double, uint64_t and string */
        try 
        {
            int64_t val = std::any_cast<int64_t>(o_val);
            g_value_init(value, G_TYPE_INT64);
            g_value_set_int64(value, val);
        } catch (const std::bad_any_cast& e) { }
        try 
        {
            double val = std::any_cast<double>(o_val);
            g_value_init(value, G_TYPE_DOUBLE);
            g_value_set_double(value, val);
        } catch (const std::bad_any_cast& e) { }
        try 
        {
            uint64_t val = std::any_cast<uint64_t>(o_val);
            g_value_init(value, G_TYPE_UINT64);
            g_value_set_uint64(value, val);
        } catch (const std::bad_any_cast& e) { }
        try 
        {
            std::string str = std::any_cast<std::string>(o_val);
            g_value_init(value, G_TYPE_STRING);
            g_value_set_string(value, str.c_str());
        } catch (const std::bad_any_cast& e) { }
        try 
        {
            char c = std::any_cast<char>(o_val);
            g_value_init(value, G_TYPE_UCHAR);
            g_value_set_uchar(value, c);
        } catch (const std::bad_any_cast& e) { }
    }

    void dh_get_output(void* validator, void* arg, const char* prompt, GValue* val)
    {
        dh::Validator* v = dynamic_cast<dh::Validator*>((dh::Validator*)validator);
        dh::Arg* a = dynamic_cast<dh::Arg*>((dh::Arg*)arg);
        std::any ret = dh::GetOutput::get_output(v, a, prompt);
        set_value(ret, val);
    }

    void dh_get_output_arg(void* arg, const char* prompt, gboolean add_validator, GValue* val)
    {
        dh::Arg* a = dynamic_cast<dh::Arg*>((dh::Arg*)arg);
        char ret = dh::GetOutput::get_output(a, prompt, add_validator);
        set_value(ret, val);
    }
}