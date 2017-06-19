/*
 * debug.c
 *
 */

#include <stdint.h>
#include <stdio.h>

void assert_func(const char *file, int line, const char *func, const char *failedExpr)
{
    printf("ASSERT ERROR \" %s \": file \"%s\" Line \"%d\" function name \"%s\" \n", failedExpr, file , line, func);

    for (;;)
    {}
}

/* stm32 StdPeriph driver assert_param */
void assert_failed(uint8_t* file, uint32_t line)
{

  printf("ASSERT ERROR : file \"%s\" Line \"%d\" \n", file , line);

  for (;;)
  {}

}

