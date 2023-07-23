
#include "klib.h"
#include "klib-macros.h"
#include "unity.h"

#define MAX_LINE_LENGTH 255

/* sometimes you may want to get at local data in a module.
 * for example: If you plan to pass by reference, this could be useful
 * however, it should often be avoided */
extern int Counter;

void setUp(void)
{
  /* This is run before EACH TEST */
}

void tearDown(void)
{
}

// 定义一个全局变量，用于保存重定向前的标准输出流
FILE *original_stdout;

// 重定向标准输出流到文件
FILE *redirect_stdout_to_file(const char *filename)
{
  // 保存原始的标准输出流
  original_stdout = stdout;
  // 打开文件，将标准输出流重定向到文件
  return freopen(filename, "w", stdout);
}

size_t read_from_file(FILE *f, char *buf, size_t buf_len)
{
  if(fgets(buf, buf_len, f) == NULL)
  {
    return 0;
  }
  return strlen(buf);
}

void test_function_printf(void)
{
  /* All of these should pass */
  FILE *pf = NULL;
  const char filename[] = "/tmp/test_result";
  char line[MAX_LINE_LENGTH];
  memset(line, 0, sizeof(line));

  pf = redirect_stdout_to_file(filename);
  printf("hello");
  read_from_file(pf, line, MAX_LINE_LENGTH);
  TEST_ASSERT_EQUAL_STRING("hello", line);
  fprintf(pf, "");
}
