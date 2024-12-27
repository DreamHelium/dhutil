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

typedef char* (*DhReadlineFn)(const char*);
typedef void (*DhReadlineAddCompletion)(char, char**);
typedef void (*DhReadlineAddHistory)(const char*);

typedef struct DhReadlineFns{
    DhReadlineFn readline_fn;
    DhReadlineAddCompletion add_completion;
    DhReadlineAddHistory add_history;
} DhReadlineFns;

#ifdef __cplusplus
#include <any>
#include <string>
#include <regex>
#include <vector>

namespace dh
{
    inline void string_strip(std::string& str)
    {
        str.erase(0, str.find_first_not_of(' '))
        .erase(str.find_last_not_of(' ') + 1);
    }

    template<typename T>
    class Validator
    {
    private:
        T result;
        std::string result_str;
    public:
        virtual bool validate(std::string& str) = 0;
        T get_result() {return result;}
        std::string get_result(bool placeholder){return result_str;}
        void set_result(T r) { result = r; }
    };

    template<typename T>
    class RangeValidator : public Validator<T>
    {
    private:
        T min;
        T max;
    public:
        virtual bool validate(std::string& str) = 0;
        RangeValidator(T min_val, T max_val)
        {
            min = min_val;
            max = max_val;
        }
        bool validate(T val)
        {
            if(val >= min && val <= max)
                return true;
            else return false;
        }
    };

    class IntValidator : public RangeValidator<int64_t>
    {
    private:
        int base = 10;
    public:
    IntValidator(int64_t min, int64_t max) : RangeValidator<int64_t>(min, max)
    {};
    IntValidator(int64_t min, int64_t max, int base): RangeValidator<int64_t>(min, max)
    {
        if(base == 0 || (base >= 2 && base <= 36))
            this->base = base;
        else this->base = 10;
    };
    using RangeValidator<int64_t>::validate;
    bool validate(std::string& str) override
    {
        string_strip(str);
        char* eptr;
        auto val = std::strtoll(str.c_str(), &eptr, base);
        if(*eptr == 0)
        {
            set_result(val);
            return validate(val);
        }
        else return false;
    }
    int get_base(){ return base; }
    };

    class RegexValidator : public Validator<std::regex>
    {
        private:
        std::regex regex;
        std::string result;
        public:
        RegexValidator(std::regex r)
        {
            regex = r;
        }
        bool validate(std::string& str)
        {
            if(std::regex_match(str, regex))
            {
                result = str;
                return true;
            }
            else return false;
        }
    };

    template<typename T>
    class VectorValidator : public Validator<std::vector<T>>
    {
        private:
        bool same_range;
        union
        {
            struct
            {
                std::vector<T> min_range;
                std::vector<T> max_range;
            };
            /* only min and max needed */
            std::vector<T> single_range;
        };
        public:
        VectorValidator(std::vector<T> range)
        {
            same_range = true;
            single_range = range;
        }
        VectorValidator(T min, T max)
        {
            same_range = true;
            single_range.push_back(min);
            single_range.push_back(max);
        }
        VectorValidator(std::vector<T> min, std::vector<T> max)
        {
            same_range = false;
            min_range = min;
            max_range = max;
        } 
        bool validate(std::vector<T> source)
        {
            if(same_range)
            {
                for(auto val : source)
                {
                    T min = single_range[0];
                    T max = single_range[1];
                    if(val < min || val > max)
                        return false;
                }
                return true;
            }
            else
            {
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
        bool validate(std::string& str)
        {
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

    template<typename T>
    std::any get_output(Validator<T>* validator, Arg* arg, DhReadlineFns* fns, const char* prompt);
    inline std::any get_output(Arg* arg, DhReadlineFns* fns, const char* prompt)
    {
        Validator<int>* ptr = nullptr;
        return get_output(ptr, arg, fns, prompt);
    }
    inline char get_output(Arg* arg, DhReadlineFns* fns, const char* prompt, bool use_validator)
    {
        IntValidator* validator = nullptr;
        if(use_validator && arg)
        {
            validator = new IntValidator(0, arg->size() - 1);
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
    inline std::any get_output(DhReadlineFns* fns, const char* prompt)
    {
        return get_output(fns, prompt);
    }
}

#endif /* __cplusplus */

#endif /* DH_VALIDATOR_CPP_H */
