typedef char* va_list;

#define va_start(p, count) (p = (va_list)&count + sizeof(char*))
// 这行代码做了做了两件事情: 1.修改p_args; 2、取值
#define va_arg(p, t)(*(t*)((p += sizeof(char*)) - sizeof(char*)))
#define va_end(p)(p = 0)