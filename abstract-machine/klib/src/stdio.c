#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...)
{
  va_list args; // 定义一个va_list类型的变量，用于承载可变参数列表
  int total = 0;
  int i = 0;
  // int arg_index = 0;
  const char* str = fmt;
  va_start(args, fmt); // 初始化可变参数列表，指向第一个可变参数
  // parsing fmt and using va_arg(args, <arg type>) and 

  for (i = 0 ; *str != 0; i++)
  {
    if (*str == '%')
    {
      // todo format 
      continue;
    }
    putch(*str);
    str++;
    total++;
  }

  va_end(args); // 结束可变参数的使用

  return total;
  // panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap)
{
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...)
{
  panic("Not implemented");
}

int snprintf(char *out, size_t n, const char *fmt, ...)
{
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap)
{
  panic("Not implemented");
}

#endif
