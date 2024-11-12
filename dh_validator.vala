#!/usr/bin/env -S vala --internal-header dh_validator.h --pkg readline -C

delegate string? DhLineinFunc(string? prompt);

interface DhValidator <T, V> : GLib.Object{
    public abstract bool in_field(V val);
    public abstract void set_range(T min_val,T max_val);
}

class DhIntValidator : Object, DhValidator <int64?, int64?>{
    int64 min;
    int64 max;
    bool ignore = false;
    public DhIntValidator.ignore_range(bool ir){
        ignore = ir;
    }
    public DhIntValidator(int64 min, int64 max){
        this.set_range(min, max);
    }
    public bool in_field(int64? val) {
        if(ignore) return true;
        if(val >= min && val <= max)
            return true;
        else return false;
    }
    public void set_range(int64? min_val, int64? max_val){
        if(min_val > max_val){
            min = max_val;
            max = min_val;
        }
        else {
            min = min_val;
            max = max_val;
        }
    }
}

class DhUIntValidator : Object, DhValidator <uint64?, uint64?>{
    uint64 min;
    uint64 max;
    bool ignore = false;
    public DhUIntValidator.ignore_range(bool ir){
        ignore = ir;
    }
    public DhUIntValidator(uint64 min, uint64 max){
        this.set_range(min, max);
    }
    public bool in_field(uint64? val) {
        if(ignore) return true;
        if(val >= min && val <= max)
            return true;
        else return false;
    }
    public void set_range(uint64? min_val, uint64? max_val){
        if(min_val > max_val){
            min = max_val;
            max = min_val;
        }
        else {
            min = min_val;
            max = max_val;
        }
    }
}

class DhDoubleValidator : Object, DhValidator<double?, double?>{
    double min;
    double max;
    bool ignore = false;
    public DhDoubleValidator.ignore_range(bool ir){
        ignore = ir;
    }
    public DhDoubleValidator(double min, double max){
        this.set_range(min, max);
    }
    public bool in_field(double? val) {
        if(ignore) return true;
        if(val >= min && val <= max)
            return true;
        else return false;
    }
    public void set_range(double? min_val, double? max_val){
        if(min_val > max_val){
            min = max_val;
            max = min_val;
        }
        else {
            min = min_val;
            max = max_val;
        }
    }
}

class DhRegexValidator : Object, DhValidator<Regex?, string>{
    Regex reg;
    public DhRegexValidator(Regex r){
        this.set_range(r, null);
    }
    public bool in_field(string val) {
        if(reg == null)
            return false;
        else return reg.match(val);
    }
    public void set_range(Regex? min_val, Regex? max_val){
        if(min_val != null)
            reg = min_val;
        else reg = max_val;
    }
}

/* If you don't want to compile regex,
 * I provide MatchValidator */

class DhMatchValidator : Object, DhValidator<string, string>{
    string match_str;
    bool ignore = false;
    public DhMatchValidator.ignore_range(bool ir){
        ignore = ir;
    }
    public DhMatchValidator(string str){
        this.set_range(str, "");
    }
    public bool in_field(string val) {
        if(ignore) return true;
        if(match_str == null)
            return false;
        else return Regex.match_simple(match_str, val);
    }
    public void set_range(string min_val, string max_val){
        if(min_val != null)
            match_str = min_val;
        else match_str = max_val;
    }
}

class DhIntArrayValidator : Object, DhValidator<List<int64?>?, List<int64?>?>{
    List<int64?> min = null;
    List<int64?> max = null;
    public string split_str;
    bool allow_repeated;
    bool ignore = false;
    public DhIntArrayValidator.ignore_range(bool ir){
        ignore = ir;
    }
    public bool in_field(List<int64?>? val){
        if(ignore) return true;
        uint len = min.length();
        bool same_range = false;
        if(len == 1)
            same_range = true;
        if(!allow_repeated){
            for(int i = 0 ; i < val.length() ; i++)
            {
                int64 temp = val.nth_data(i);
                for(int j = i + 1; j < val.length() ; j++){
                    int64 temp_1 = val.nth_data(j);
                    if(temp == temp_1){
                        return false;
                    }
                }
            }
        }
        for(int i = 0 ; i < val.length() ; i++)
        {
            int64 min_val;
            int64 max_val;
            if(same_range){
                min_val = (int64)min.nth_data(0);
                max_val = (int64)max.nth_data(0);
            }
            else{
                min_val = (int64)min.nth_data(i);
                max_val = (int64)max.nth_data(i);
            }
            if(val.nth_data(i) < min_val || val.nth_data(i) > max_val)
                return false;
        }
        return true;
    }
    public void set_range(List<int64?>? min_val, List<int64?>? max_val){
        min = (List<int64?>)min_val.copy();
        max = (List<int64?>)max_val.copy();
    }
    public void add_range(int64 min_val, int64 max_val){
        min.append(min_val);
        max.append(max_val);
    }
    public void set_split_str(string sp_str){
        split_str = sp_str;
    }
    public void set_allow_repeated(bool ar){
        allow_repeated = ar;
    }
}

class DhArgInfo : Object{
    public List<char> arg;
    public List<string> arg_fullname;
    public List<string> description;
    char default_arg = 0;
    public void add_arg(char new_arg, string new_arg_fullname, string new_description){
        arg.append(new_arg);
        arg_fullname.append(new_arg_fullname);
        description.append(new_description);
        if(arg.length() == 1 && default_arg == 0)
            default_arg = arg.nth_data(0);
    }
    public string help_message(string gettext_package){
        string str = dgettext("dhutil" ,"The arguments are:\n");
        for(int i = 0 ; i < arg.length() ; i++){
            if(arg.nth_data(0) != 0){
                string printf_str = "\"%c\", \t\"%s\", \t\"%s\"\n";
                printf_str = printf_str.printf(arg.nth_data(i), arg_fullname.nth_data(i), dgettext(gettext_package, description.nth_data(i)));
                str += printf_str;
            }
            else{
                string printf_str = "\"\", \t\"%s\", \t\"%s\"\n";
                printf_str = printf_str.printf(arg_fullname.nth_data(i), dgettext(gettext_package, description.nth_data(i)));
                str += printf_str;
            }
        }
        return str;
    }
    public char match_char(string str){
        string pstr = str.ascii_down();
        for(int i = 0 ; i < arg.length() ; i++)
        {
            string match_str = "^%c$";
            match_str = match_str.printf(arg.nth_data(i));
            if(Regex.match_simple(match_str, pstr) && match_str != "^$")
                return arg.nth_data(i);
            match_str = "^%s$";
            match_str = match_str.printf(arg_fullname.nth_data(i));
            if(Regex.match_simple(match_str, pstr))
                return arg.nth_data(i);
        }
        if(pstr == "")
            return default_arg;
        return 0;
    }
    public string? match_string(string str){
        string pstr = str;
        for(int i = 0 ; i < arg.length() ; i++)
        {
            string match_str = null;
            if(arg.nth_data(i) != 0){
                match_str = "^%c$";
                match_str = match_str.printf(arg.nth_data(i));
                if(Regex.match_simple(match_str, pstr) && match_str != "^$")
                    return arg_fullname.nth_data(i);
            }
            match_str = "^%s$";
            match_str = match_str.printf(arg_fullname.nth_data(i));
            if(Regex.match_simple(match_str, pstr))
                return arg_fullname.nth_data(i);
        }
        if(pstr == "")
            return arg_fullname.nth_data(0);
        return null;
    }

    public void change_default_arg(char arg){
        this.default_arg = arg;
        int default_arg_pos = -1;
        for(int i = 0 ; i < this.arg.length() ; i++)
        {
            if(this.arg.nth_data(i) == arg)
            {
                default_arg_pos = i;
                break;
            }
        }
        char old_arg = this.arg.nth_data(0);
        this.arg.nth(0).data = arg;
        this.arg.nth(default_arg_pos).data = old_arg;
        string old_str = this.arg_fullname.nth_data(0);
        this.arg_fullname.nth(0).data = this.arg_fullname.nth_data(default_arg_pos);
        this.arg_fullname.nth(default_arg_pos).data = old_str;
        string old_description = this.description.nth_data(0);
        this.description.nth(0).data = this.description.nth_data(default_arg_pos);
        this.description.nth(default_arg_pos).data = old_description;
    }
}

class DhOut : Object{
    private static DhArgInfo info = null;
    private bool match_string = false;
    private bool output_str_while_nov = true;
    private bool show_opt = false;
    public signal void sig_eof();

    private void init_readline(){
        Readline.readline_name = "dhutil";
        Readline.attempted_completion_function = dhutil_completion;
    }

    private string get_opt(DhArgInfo arg)
    {
        string str = "[%s]";
        string arg_str = "";
        for(int i = 0 ; i < arg.arg.length() ; i++)
        {
            string single_arg = string.nfill(1, arg.arg.nth_data(i));
            if(i == 0) single_arg = single_arg.ascii_up(1);
            arg_str += single_arg;
            arg_str += "/";
        }
        arg_str += "?";
        str = str.printf(arg_str);
        return str;
    }

    public void set_show_opt(bool option)
    {
        show_opt = option;
    }

    public void no_output_string_while_no_validator(){
        output_str_while_nov = false;
    }

    private static string[]? dhutil_completion (string str, int a, int b){
        string[] matches = null;
        if(a == 0)
            matches = Readline.completion_matches(str, dhutil_compeuntry_func_static);
        return matches;
    }

    public void output_match_string_than_arg(){
        match_string = true;
    }

    private static string? dhutil_compeuntry_func(string str, int state){
        uint list_index = state;
        string name;

        if(info != null){
            while((name = info.arg_fullname.nth_data(list_index)) != null){
                list_index++;
                if(name.has_prefix(str))
                    return name;
            }
            list_index -= info.arg.length();
            if(str.length == 0){
                string ret;
                char return_char;
                while((return_char = info.arg.nth_data(list_index)) != 0){
                    list_index++;
                    ret = string.nfill(1, return_char);
                        return ret;
                }
            }

        }
        return null;
    }

    private static string? dhutil_compeuntry_func_static(string str, int state){
        return dhutil_compeuntry_func(str, state);
    }

    public Value read_and_output(string tip_message, string gettext_package,
                        DhArgInfo? arg, DhValidator? validator, bool get_array){
        return read_and_output_custom(null, tip_message, gettext_package,
                                      arg, validator, get_array, true);
    }

    public Value read_and_output_custom(DhLineinFunc? func, string tip_message, string gettext_package,
                        DhArgInfo? arg, DhValidator? validator, bool get_array, bool use_readline){
        init_readline();
        while(true){
            info = arg;
            string str;
            if(use_readline){
                string prompt = gettext(tip_message);
                if(show_opt && arg != null)
                {
                    prompt += " ";
                    prompt += get_opt(arg);
                    prompt += ": ";
                }
                str = Readline.readline(prompt);
            }
            else{
                print(dgettext(gettext_package, tip_message));
                str = func(null);
            }
            if(str == null) /* Error occured */
            {
                sig_eof();
                return Type.NONE;
            }
            else{
                if(str[0] != 0)
                    Readline.History.add(str);
                if(arg != null){ /* Try to match args */
                    if(Regex.match_simple("^\\?$", remove_blank(str))){
                        print(arg.help_message(gettext_package));
                        continue;
                    }
                    else{
                        if(match_string){
                            string? ret = arg.match_string(remove_blank(str));
                            if(ret != null)
                                return ret;
                        }
                        else {
                        char ret = arg.match_char(remove_blank(str));
                        if(ret != 0)
                            return ret;
                        }
                    }
                    /* Match unsuccess */
                }
                if(validator != null){ /* Try to match validator */
                    str = remove_blank(str);
                    if(validator.get_type() == typeof(DhIntValidator))
                    {
                        DhIntValidator int_validator = validator as DhIntValidator;
                        if(Regex.match_simple("^[\\-|\\+]?[0-9]+$", str))
                        {
                            int64 ret = int64.parse(str, 10);
                            if(int_validator.in_field(ret))
                                return ret;
                        }
                    }
                    else if(validator.get_type() == typeof(DhUIntValidator))
                    {
                        DhUIntValidator uint_validator = validator as DhUIntValidator;
                        if(Regex.match_simple("^[\\+]?[0-9]+$", str))
                        {
                            uint64 ret = uint64.parse(str, 10);
                            if(uint_validator.in_field(ret))
                                return ret;
                        }
                    }
                    else if(validator.get_type() == typeof(DhDoubleValidator))
                    {
                        DhDoubleValidator double_validator = validator as DhDoubleValidator;
                        string end_str;
                        double ret = str.to_double(out end_str);
                        if(end_str == "")
                            if(double_validator.in_field(ret))
                                return ret;
                    }
                    else if(validator.get_type() == typeof(DhRegexValidator))
                    {
                        DhRegexValidator regex_validator = validator as DhRegexValidator;
                        if(regex_validator.in_field(str))
                            return str;
                    }
                    else if(validator.get_type() == typeof(DhMatchValidator))
                    {
                        DhMatchValidator match_validator = validator as DhMatchValidator;
                        if(match_validator.in_field(str))
                            return str;
                    }
                    else if(validator.get_type() == typeof(DhIntArrayValidator))
                    {
                        DhIntArrayValidator iav = validator as DhIntArrayValidator;
                        List<int64?> ret = null;
                        string[] after_str = str.split(iav.split_str);
                        bool success = true;
                        /* get int array */
                        for(int i = 0 ; i < after_str.length ; i++){
                            int64 temp;
                            if(int64.try_parse(after_str[i], out temp))
                                ret.append(temp);
                            else{
                                success = false;
                                break;
                            }
                        }
                        /* Determine whether in field */
                        if(success){
                            if(iav.in_field(ret))
                                return ret;
                        }
                    }
                } /* Match unsuccess */
                else if(output_str_while_nov) return str;
                print("Unsuccess!\n");
            }
        }
    }

    public Value read_and_output_as_int_custom(DhLineinFunc? func, string tip_message, string gettext_package,
                        DhArgInfo? arg, int64 min, int64 max, bool get_array, bool use_readline){
        DhIntValidator validator = new DhIntValidator(min, max);
        return read_and_output_custom(func, tip_message, gettext_package, arg, validator, get_array, use_readline);
    }

    public Value read_and_output_as_int(string tip_message, string gettext_package,
                        DhArgInfo? arg, int64 min, int64 max, bool get_array){
        return read_and_output_as_int_custom(null, tip_message, gettext_package, arg, min, max, get_array, true);
    }

    private string remove_blank(string str){
        return str.strip();
    }
}
