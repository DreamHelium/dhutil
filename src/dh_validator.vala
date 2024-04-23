interface DhValidator <T, V> : GLib.Object{
    public abstract bool in_field(V val);
    public abstract void set_range(T min_val,T max_val);
}

class DhIntValidator : Object, DhValidator <int64?, int64?>{
    int64 min;
    int64 max;
    public bool in_field(int64? val) {
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
    public bool in_field(uint64? val) {
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
    public bool in_field(double? val) {
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
    public bool in_field(string val) {
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
        string str = _("The arguments are:\n");
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
        for(int i = 0 ; i < arg.length() ; i++)
        {
            string match_str = "^%c$";
            match_str = match_str.printf(arg.nth_data(i));
            if(Regex.match_simple(match_str, str))
                return arg.nth_data(i);
            match_str = "^%s$";
            match_str = match_str.printf(arg_fullname.nth_data(i));
            if(Regex.match_simple(match_str, str))
                return arg.nth_data(i);
        }
        return 0;
    }
}

class DhOut : Object{
    public Value read_and_output(string tip_message, string gettext_package,
                        DhArgInfo? arg, DhValidator validator){
        while(true){
            print(dgettext(gettext_package, tip_message));
            string str = stdin.read_line();
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
                } /* Match unsuccess */
                print("Unsuccess!\n");
            }
        }
    }
}

