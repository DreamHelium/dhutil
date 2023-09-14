# 梦氦之工具库

## 依赖
- `cJSON`
- `glib2`（包括`GObject`和`GIO`）

## 使用

### `InputLine`系列函数

此库的目的之一是为了能获取整行输入，为此，我提供了一系列以`InputLine`为开头的函数。但是因为有点久远了，准备重写，因此不在此详细介绍。

### POSIX字符串处理函数包裹（在`dh_string_util.h`中）

- `dh_strdup`，类似于`strdup`，开辟出一块新的内存用于复制字符串，需要使用`free()`释放。
- `dh_getdelim`，等同于`getdelim`，获取分隔符前的字符串。
- `dh_string_getline`，获取整行输入，等同于`getdelim`中`delim`参数使用`\n`。

### 文件处理相关（在`file_util.h`中）

都可以顾名思义，不再赘述。

### GList扩展（在`list_util.h`中）

- `dh_search_in_list`，搜索列表中含有的字符串，新得到的列表的数据由`dh_strdup`提供，所以需要在`free_full`中提供`free()`。
- `dh_search_in_list_custom`，自行提供函数搜索，数据是旧列表中的，不需要手动`free`，但是需要原列表数据未释放。

### DhGeneral接口及DhIsoc类（在`dh_generaliface.h`及`dh_general_isoc.h`中）

提供`vprintf`、选项打印和选择器的函数接口，以及一个在ISO C中的参考实现。接口使用GObject对象系统编程，如需自行实现，可参考`dh_general_isoc.c`。

