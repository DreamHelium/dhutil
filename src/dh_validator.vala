delegate string? DhLineinFunc();

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
    List<char> arg;
    List<string> arg_fullname;
    List<string> description;
    public void add_arg(char new_arg, string new_arg_fullname, string new_description)
    {
        arg.append(new_arg);
        arg_fullname.append(new_arg_fullname);
        description.append(new_description);
    }
    public string help_message(string gettext_package)
    {
        string str = dgettext("dhutil" ,"The arguments are:\n");
        for(int i = 0 ; i < arg.length() ; i++){
            str += "\"";
            string temp_str = string.nfill(1, arg.nth_data(i));
            str += temp_str;
            str += "\", \"";
            str += arg_fullname.nth_data(i);
            str += "\", \"";
            str += dgettext(gettext_package, description.nth_data(i));
            str += "\"\n";
        }
        return str;
    }
    public char match_char(string str)
    {
        string pstr = str.ascii_down();
        for(int i = 0 ; i < arg.length() ; i++)
        {
            string match_str = "^%c$";
            match_str = match_str.printf(arg.nth_data(i));
            if(Regex.match_simple(match_str, pstr))
                return arg.nth_data(i);
            match_str = "^%s$";
            match_str = match_str.printf(arg_fullname.nth_data(i));
            if(Regex.match_simple(match_str, pstr))
                return arg.nth_data(i);
        }
        if(pstr == "")
            return arg.nth_data(0);
        return 0;
    }
}

class DhOut : Object{
    public Value read_and_output(string tip_message, string gettext_package,
                        DhArgInfo? arg, DhValidator? validator, bool get_array){
        return read_and_output_custom(stdin.read_line, tip_message, gettext_package,
                                      arg, validator, get_array);
    }

    public Value read_and_output_custom(DhLineinFunc func, string tip_message, string gettext_package,
                        DhArgInfo? arg, DhValidator? validator, bool get_array){
        while(true){
            print(dgettext(gettext_package, tip_message));
            string str = func();
            if(str == null) /* Error occured */
                return Type.NONE;
            else{
                if(arg != null){ /* Try to match args */
                    if(Regex.match_simple("^\\?$", str))
                    {
                        print(arg.help_message(gettext_package));
                        continue;
                    }
                    else
                    {
                        char ret = arg.match_char(str);
                        if(ret != 0)
                            return ret;
                    }
                    /* Match unsuccess */
                }
                if(validator != null){ /* Try to match validator */
                    if(validator.get_type() == typeof(DhIntValidator))
                    {
                        if(Regex.match_simple("^[\\-|\\+]?[0-9]+$", str))
                        {
                            int64 ret = int64.parse(str, 10);
                            if((validator as DhIntValidator).in_field(ret))
                                return ret;
                        }
                    }
                    else if(validator.get_type() == typeof(DhUIntValidator))
                    {
                        if(Regex.match_simple("^[\\+]?[0-9]+$", str))
                        {
                            uint64 ret = uint64.parse(str, 10);
                            if((validator as DhUIntValidator).in_field(ret))
                                return ret;
                        }
                    }
                    else if(validator.get_type() == typeof(DhDoubleValidator))
                    {
                        string end_str;
                        double ret = str.to_double(out end_str);
                        if(end_str == "")
                            if((validator as DhDoubleValidator).in_field(ret))
                                return ret;
                    }
                    else if(validator.get_type() == typeof(DhRegexValidator))
                    {
                        if((validator as DhRegexValidator).in_field(str))
                            return str;
                    }
                    else if(validator.get_type() == typeof(DhMatchValidator))
                    {
                        if((validator as DhMatchValidator).in_field(str))
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
                else return str;
                print("Unsuccess!\n");
            }
        }
    }

    public Value read_and_output_as_int_custom(DhLineinFunc func, string tip_message, string gettext_package,
                        DhArgInfo? arg, int64 min, int64 max, bool get_array){
        DhIntValidator validator = new DhIntValidator(min, max);
        validator.set_range(min, max);
        return read_and_output_custom(func, tip_message, gettext_package, arg, validator, get_array);
    }

    public Value read_and_output_as_int(string tip_message, string gettext_package,
                        DhArgInfo? arg, int64 min, int64 max, bool get_array){
        return read_and_output_as_int_custom(stdin.read_line, tip_message, gettext_package, arg, min, max, get_array);
    }
}
