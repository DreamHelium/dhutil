#include "dh_validator_cpp.hpp"
#include <any>
#include <iostream>

static void test1()
{
    dh::VectorValidator<int64_t> validator(false);
    validator.add_range(0, 100);
    validator.add_range(-1, 200);
    dh::Arg arg;
    arg.add_arg('t', "test", "Test for readline");
    std::any ret = dh::GetOutput::get_output(&validator, &arg, "test: ");
    try 
    {
        auto ret_val = std::any_cast<std::vector<int64_t>>(ret);
        for(auto val : ret_val)
            std::cout << val << std::endl;
    } catch (const std::bad_any_cast& e) {
    }
}

static void test2()
{
    std::vector<std::string> string_list;
    /* This is a sample of the string add & view func
     * Showing in Chinese */
    setlocale(LC_ALL, "");
    while(true)
    {
    dh::Arg arg;
    std::vector<std::string> vstr = {"add", "添加"};
    arg.add_arg('a', vstr, "添加字符串");
    arg.add_arg('v', "view", "查看字符串");
    std::cout << "[0] 添加字符串\n" << "[1] 查看字符串\n";
    char val = dh::GetOutput::get_output(&arg, "选择选项: ", true);
    if(val == 'a')
    {
        std::any str = dh::GetOutput::get_output("请输入字符串: ");
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

void test3()
{
        dh::GetOutput::get_output((DhReadlineFns*)nullptr, nullptr);
}

int main()
{
    std::cout << "[0] Regular test\n";
    std::cout << "[1] Test with Chinese\n";
    std::cout << "[2] No readline excpetion\n";
    dh::RangeValidator<int64_t> validator(0, 2);
    std::any ret = dh::GetOutput::get_output(&validator, nullptr, "Choose a example to show: ");
    try 
    {
        int64_t val = std::any_cast<int64_t>(ret);
        if(val == 0) test1();
        if(val == 1) test2();
        if(val == 2) test3();
    } catch (const std::bad_any_cast& e) { return -1; }
}