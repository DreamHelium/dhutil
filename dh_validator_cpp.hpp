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

#ifndef DH_VALIDATOR_CPP_H
#define DH_VALIDATOR_CPP_H

#include <glib-object.h>
#ifdef DH_EDITLINE_USED
#include <editline/readline.h>
#include "dh_readline_cpp.hpp"
#endif


#ifdef __cplusplus
extern "C"{
#endif

#include <stddef.h>
#include <stdint.h>

typedef char* (*DhReadlineFn)(const char*);
typedef void (*DhReadlineSetCompletion)(void*);
typedef int (*DhReadlineAddHistory)(const char*);
typedef void (*DhReadlineInit)(const char* name);

typedef struct DhReadlineFns{
    DhReadlineFn readline_fn;
    DhReadlineSetCompletion set_completion;
    DhReadlineAddHistory add_history;
    DhReadlineInit init;
} DhReadlineFns;

char* cpp_get_line(const char* prompt);
void dh_set_err_message(const char* str);
void dh_set_package_name(const char* str);
void* dh_int_validator_new(int64_t min, int64_t max, int base);
void* dh_arg_new();
void dh_arg_add_arg(void* arg, char c, const char* str, const char* description);
void dh_arg_add_arg_multi(void* arg, char c, char* const* strv, const char* description);
void dh_int_validator_free(void* ptr);
void dh_arg_free(void* ptr);
void dh_get_output(void* validator, void* arg, const char* prompt, GValue* val);
void dh_get_output_arg(void* arg, const char* prompt, gboolean add_validator, GValue* val);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <any>
#include <string>
#include <regex>
#include <vector>
#include <variant>

namespace dh
{
    static std::vector<std::string> string_split(std::string str, char delim)
    {
        /* Reference: https://zh.cppreference.com/w/cpp/string/basic_string/getline */
        /*  */
        std::vector<std::string> ret;
        std::istringstream stream;
        stream.str(str);

        for(std::string line; std::getline(stream, line, delim) ;)
            ret.push_back(line);
        return ret;
    }

    template<typename T>
    T get_num_internal(const char* str, char** end_ptr ,int base);

    inline void string_strip(std::string& str)
    {
        str.erase(0, str.find_first_not_of(' '))
        .erase(str.find_last_not_of(' ') + 1);
    }

    class Validator
    {
    private:
        std::any result;
        std::string result_str;
    public:
        virtual bool validate(std::string str) = 0;
        std::any get_result() {return result;}
        std::string get_result(bool placeholder){return result_str;}
        void set_result(std::any r) { result = r; }
    };

    template<typename T>
    class RangeValidator : public Validator
    {
    private:
        T min;
        T max;
        int base = 10;
    public:
        RangeValidator(T min_val, T max_val) : min(min_val), max(max_val)
        { }
        RangeValidator(T min_val, T max_val, int base) : RangeValidator<T>(min_val, max_val)
        {
            if(base == 0 || (base >= 2 && base <= 36))
                this->base = base;
            else this->base = 10;
        }
        bool validate(T val)
        {
            if(val >= min && val <= max)
                return true;
            else return false;
        }
        int get_base(){ return base; }
        bool validate(std::string str) override
        {
            string_strip(str);
            char* end_ptr;
            auto ret = get_num_internal<T>(str.c_str(), &end_ptr, base);
            if(*end_ptr == 0 && *str.c_str() != 0)
            {
                if(validate(ret))
                {
                    this->set_result(ret);
                    return true;
                }
                else return false;
            }
            else
                return false;
        }
    };

    class RegexValidator : public Validator
    {
        private:
        std::regex regex;
        public:
        RegexValidator(std::regex r)
        {
            regex = r;
        }
        bool validate(std::string str) override
        {
            if(std::regex_match(str, regex))
            {
                set_result(str);
                return true;
            }
            else return false;
        }
    };

    template<typename T>
    class VectorValidator : public Validator
    {
        private:
        char delim;
        bool same_range;
        using range = struct range
        {
            std::vector<T> min_range;
            std::vector<T> max_range;
        };
        std::variant<range, std::vector<T>> ranges;
        public:
        VectorValidator(bool same_range, char delim) : same_range(same_range), delim(delim)
        { }
        VectorValidator(std::vector<T> range, char delim)
        {
            same_range = true;
            ranges = range;
            this->delim = delim;
        }
        VectorValidator(T min, T max, char delim)
        {
            same_range = true;
            std::vector<T> r;
            r.push_back(min);
            r.push_back(max);
            ranges = r;
            this->delim = delim;
        }
        VectorValidator(std::vector<T> min, std::vector<T> max, char delim)
        {
            same_range = false;
            range r = {min, max};
            ranges = r;
            this->delim = delim;
        }

        VectorValidator(bool same_range) : VectorValidator<T>(same_range, ',')
        { }
        VectorValidator(std::vector<T> range) : VectorValidator<T>(range, ',')
        { }
        VectorValidator(T min, T max) : VectorValidator<T>(min, max, ',')
        { }
        VectorValidator(std::vector<T> min, std::vector<T> max) : VectorValidator<T>(min, max, ',')
        { }

        bool add_range(T min, T max)
        {
            if(same_range)
            {
                try
                {
                    std::vector<T> single_range = std::get<std::vector<T>>(ranges);
                    return false;
                }                
                catch(const std::bad_variant_access &e)
                {
                    std::vector<T> r;
                    r.push_back(min);
                    r.push_back(max);
                    ranges = r;
                    return true;
                }
            }
            else
            {
                try
                {
                    range complex_ranges = std::get<range>(ranges);
                    if(complex_ranges.max_range.empty())
                    {
                        std::vector<T> min_r = {min};
                        std::vector<T> max_r = {max};
                        range r = {min_r, max_r};
                        ranges = r;
                        return true;
                    }
                    complex_ranges.min_range.push_back(min);
                    complex_ranges.max_range.push_back(max);
                    ranges = complex_ranges;
                    return true;
                }                
                catch(const std::bad_variant_access &e)
                {
                    // std::vector<T> min_r = {min};
                    // std::vector<T> max_r = {max};
                    // range r = {min_r, max_r};
                    // ranges = r;
                    // return true;
                    return false; /* ? */
                }
            }
        }

        bool validate(std::vector<T> source)
        {
            if(same_range)
            {
                for(auto val : source)
                {
                    auto single_range = std::get<std::vector<T>>(ranges);
                    T min = single_range[0];
                    T max = single_range[1];
                    if(val < min || val > max)
                        return false;
                }
                return true;
            }
            else
            {
                auto rs = std::get<range>(ranges);
                auto min_range = rs.min_range;
                auto max_range = rs.max_range;
                auto min_len = min_range.size();
                auto max_len = max_range.size();
                if(min_len != max_len) return false;
                auto len = source.size();
                if(len != min_len) return false;
                for(int i = 0 ; i < len ; i++)
                {
                    if(source[i] < min_range[i] || source[i] > max_range[i])
                        return false;
                }
                return true;
            }
        }
        bool validate(std::string str) override
        {
            auto split_str = string_split(str, delim);
            if(same_range)
            {
                std::vector<T> val;
                for(auto single_str : split_str)
                {
                    auto single_range = std::get<std::vector<T>>(ranges);
                    RangeValidator<T> validator(single_range[0], single_range[1]);
                    if(validator.validate(single_str))
                    {
                        T r = std::any_cast<T>(validator.get_result());
                        val.push_back(r);
                    }
                    else return false;
                }
                this->set_result(val);
                return true;
            }
            else
            {
                auto rs = std::get<range>(ranges);
                auto min_range = rs.min_range;
                auto max_range = rs.max_range;
                std::vector<T> val;
                if((split_str.size() != min_range.size()) ||
                   (min_range.size() != max_range.size()))
                    return false;
                for(int i = 0 ; i < split_str.size() ; i++)
                {
                    RangeValidator<T> validator(min_range[i], max_range[i]);
                    if(validator.validate(split_str[i]))
                    {
                        T r = std::any_cast<T>(validator.get_result());
                        val.push_back(r);
                    }
                    else return false;
                }
                this->set_result(val);
                return true;
            }
        }
    };

    class Arg
    {
        private:
        int arg_pos;
        using arg_fullname = std::vector<std::string>;
        std::vector<char> args;
        std::vector<arg_fullname> arg_fullnames;
        std::vector<std::string> arg_descriptions;
        public:
        std::vector<char> get_args()
        { return args; }
        std::vector<arg_fullname> get_arg_fullnames()
        { return arg_fullnames; }
        int size()
        {
            return args.size();
        }
        void add_arg(char arg, arg_fullname fns, std::string description)
        {
            args.push_back(arg);
            arg_fullnames.push_back(fns);
            arg_descriptions.push_back(description);
        }
        void add_arg(char arg, std::string fn, std::string description)
        {
            arg_fullname afn(1, fn);
            add_arg(arg, afn, description);
        }
        std::string get_argument()
        {
            std::string ret = " [";
            for(int i = 0 ; i < args.size() ; i++)
            {
                std::string arg_str;
                char c = args[i];
                if(i == 0) c = toupper(c);
                arg_str.assign(1, c);
                ret += arg_str;
                ret += "/";
            }
            ret += "?]: ";
            return ret;
        }
        bool validate(std::string str)
        {
            string_strip(str);
            for(int i = 0 ; i < args.size() ; i++)
            {
                auto c = args[i];
                std::string c_s;
                c_s.assign(1, c);
                if(str == c_s)
                {
                    arg_pos = i;
                    return true;
                }
                c = toupper(c);
                c_s.clear();
                c_s.assign(1, c);
                if(str == c_s)
                {
                    arg_pos = i;
                    return true;
                }
            }
            for(int i = 0 ; i < arg_fullnames.size() ; i++)
            {
                auto fullnames = arg_fullnames[i];
                for(int j = 0 ; j < fullnames.size() ; j++)
                {
                    if(str == fullnames[j])
                    {
                        arg_pos = i;
                        return true;
                    }
                }
            }
            return false;
        }
        int get_pos()
        {
            return arg_pos;
        }
        char get_pos_char()
        {
            return args[arg_pos];
        }
        char get_pos_char(int pos)
        {
            return args[pos];
        }
        std::string get_pos_string()
        {
            return arg_fullnames[arg_pos][0];
        }
        std::string get_help()
        {
            std::string str = "The arguments are:\n";
            for(int i = 0 ; i < args.size() ; i++)
            {
                std::string single_str = "\"";
                single_str.append(1, args[i]);
                single_str.append("\" ");
                std::string fullname_str;
                for(int j = 0 ; j < arg_fullnames[i].size() ; j++)
                {
                    fullname_str.append("\"");
                    fullname_str.append(arg_fullnames[i][j]);
                    fullname_str.append("\" ");
                }
                single_str.append(fullname_str);
                single_str.append(":\"");
                single_str.append(arg_descriptions[i]);
                single_str.append("\"\n");
                str.append(single_str);
            }
            return str;
        }
    };

    class GetOutput
    {
        private:
        static DhReadlineFns fns;
        public:
        
        static std::string err_message;
        static const char* package_name;
        static void change_err_message(std::string str)
        {
            err_message = str;
        }
        static void change_package_name(const char* str)
        {
            package_name = str;
        }
        static std::any get_output(Validator* validator, Arg* arg, DhReadlineFns* fns, const char* prompt);
        static std::any get_output(Validator* validator, Arg* arg, const char* prompt)
        {
            return get_output(validator, arg, &fns, prompt);
        }
        static std::any get_output(Arg* arg, DhReadlineFns* fns, const char* prompt)
        {
            return get_output(nullptr, arg, fns, prompt);
        }
        static std::any get_output(Arg* arg, const char* prompt)
        {
            return get_output(arg, &fns, prompt);
        }
        static char get_output(Arg* arg, DhReadlineFns* fns, const char* prompt, bool use_validator)
        {
            RangeValidator<int64_t>* validator = nullptr;
            if(use_validator && arg)
            {
                validator = new RangeValidator<int64_t>(0, arg->size() - 1);
            }
            std::any ret = get_output(validator, arg, fns, prompt);
            try 
            {
                int pos = std::any_cast<int64_t>(ret);
                return arg->get_pos_char(pos);
            } catch (const std::bad_any_cast& e) { }
            try 
            {
                return std::any_cast<char>(ret);
            } catch (const std::bad_any_cast& e) {
                return 0;
            }
            
        }
        static char get_output(Arg* arg, const char* prompt, bool use_validator)
        {
            return get_output(arg, &fns, prompt, use_validator);
        }
        static std::any get_output(DhReadlineFns* fns, const char* prompt)
        {
            return get_output(nullptr, nullptr, fns, prompt);
        }
        static std::any get_output(const char* prompt)
        {
            return get_output(&fns, prompt);
        }
    };

    inline std::string GetOutput::err_message = "Validate error!";
    inline const char* GetOutput::package_name = "dhutil";

    #ifdef DH_EDITLINE_USED
    inline DhReadlineFns GetOutput::fns = {readline, dhutil_set_completion, add_history, init_readline};
    #else
    inline DhReadlineFns GetOutput::fns = {cpp_get_line, NULL, NULL, NULL};
    #endif
}

#endif /* __cplusplus */

#endif /* DH_VALIDATOR_CPP_H */
