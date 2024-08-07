/*  dh_string_util - String utils by Dream Helium
    Copyright (C) 2022 Dream Helium
    This file is part of litematica_reader_c.

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

#ifndef DH_STRING_UTIL_H
#define DH_STRING_UTIL_H

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <glib.h>

#ifdef __cplusplus
extern "C"{
#endif



#ifdef DH_USE_OLD_INPUT_LINE_FUNC

/** The type of the dh_Line_IO */
typedef enum dh_out_type{
    Integer, Float, Double, Character, NumArray, String, DoubleArray, FloatArray,
    Empty, Compound, Unsupported
} dh_out_type;

typedef struct dh_LineOut
{
    /** Return type */
    dh_out_type type;

    union{
        /** Number value of the output */
        int64_t num_i;
        double num_f;

        /** Character value of the output */
        char val_c;

        /** String or NumArray value or Empty */
        struct{
          void* val;
          int len;
        };

        /** Compound value */
        struct{
            int type_num;
            struct dh_LineOut* val_com;
        };
    };

    /** byte of the num value */
    int byte;
}
dh_LineOut;

typedef struct dh_string_impl{
    /** vprintf() replacement */
    int (*vprintf_fn)(const char*, va_list);

    /** getline() replacement (without FILE*), in some cases you need to code implement by yourself */
    int (*getline_fn)(char**, size_t*);

    /** free memory function used in getline() */
    void (*getline_free)(void*);

    /** internal error print (Default: to stderr) */
    int (*err_print_fn)(const char*);
} dh_string_impl;

typedef struct _dh_limit dh_limit ;
typedef int (*DhTitlePrinter)(const char* title);
typedef int (*DhOptionPrinter)(int opt, const char* opt_name);
typedef int (*DhNumberSelector)(const char* ,int);

int dh_default_TitlePrinter(const char* title);
int dh_default_OptionPrinter(int opt, const char* opt_name);
int dh_default_selector(const char* tip, int opt, const char* opt_name);

/** Change implement of this string util, might be needed if using in GUI lib or other cases */
void dh_string_ChangeImpl(dh_string_impl* impl);

/** Get a line input and return output (1 number or character), return 64bit num by default */
G_DEPRECATED_FOR(InputLine_General)
dh_LineOut* InputLine_Get_OneOpt(int range_check, int need_num, int arg_num, ...);
G_DEPRECATED_FOR(InputLine_General)
dh_LineOut* InputLine_Get_OneOpt_WithByte(int byte, int range_check, int need_num, int arg_num, ...);
/** Get a line input and return output (n numbers or character)
 *  should pass like this: (nums): min and max, (char): char. */
G_DEPRECATED_FOR(InputLine_General)
dh_LineOut* InputLine_Get_MoreDigits(int range_check, int need_nums, int arg_num, ...);
G_DEPRECATED_FOR(InputLine_General)
dh_LineOut* InputLine_Get_MoreDigits_WithByte(int byte, int range_check, int need_nums, int arg_num, ...);
/** A better way to get output, byte will be ignored in Float/Double(or array) mode */
dh_LineOut* InputLine_General(int byte, dh_limit* limit, int get_string, const char* args, int allow_empty);
/** Transform a String to number array */
long* NumArray_From_String(const char* string, int* nums, int char_check);

dh_LineOut* dh_LineOut_CreateNum(int64_t num, int byte);
dh_LineOut* dh_LineOut_CreateFloat(double num);
dh_LineOut* dh_LineOut_CreateDouble(double num);
dh_LineOut* dh_LineOut_CreateChar(char c);
dh_LineOut* dh_LineOut_CreateNumArray(void* array, int len, int byte, int o_byte);
dh_LineOut* dh_LineOut_CreateString(const char* str);
dh_LineOut* dh_LineOut_CreateEmpty();

void dh_LineOut_Free(dh_LineOut* lo);
/** Initize a limit type
 *  WARNING: Recommend that don't edit struct directly */
dh_limit* dh_limit_Init(dh_out_type type);
/** Change conditons of using array, lens = -1 : use unlimited lens
 *  WARNING: You can edit the struct directly, but that's not recommended */
int dh_limit_SetArrayArgs(dh_limit* limit, int lens, int same_range, int check_repeated, int unlimited_lens);
/** Add int num for limit */
int dh_limit_AddInt(dh_limit* limit, int64_t min, int64_t max);
/** Add double num for limit */
int dh_limit_AddDouble(dh_limit* limit, double min, double max);
/** Free dh_limit */
void dh_limit_Free(dh_limit* limit);

#endif

typedef struct DhStrArray{
    char** val;
    int num;
} DhStrArray;

char*       dh_strdup(const char *o_str);
char*       dh_str_array_cat(DhStrArray* arr);
DhStrArray* dh_str_array_init(const char* str);
int         dh_str_array_add_str(DhStrArray** arr ,const char* str);
gboolean    dh_str_array_find_repeated(DhStrArray* arr, const char* str);
guint       dh_str_array_find_char(DhStrArray* arr, char key);
void        dh_str_array_free(DhStrArray* arr);

/** Use dh_getdelim */
int         dh_getline(char** input, size_t* n, FILE* stream);
#define     dh_string_getline(input, n, stream) dh_getline(input, n, stream)
int         dh_getdelim(char** input, size_t* n,int delim, FILE* stream);

#ifdef __cplusplus
}
#endif

#endif // DH_STRING_UTIL_H
