#ifndef CODEC_OSAL_HPP_
#define CODEC_OSAL_HPP_

#if defined(CODEC_PC_TEST) /* defined in Makefile */
  #include "stdint.h"
  #include "stdio.h"
  #define osalDbgCheck(c) {                     \
    if (!(c)){                                  \
      printf("failed\n");                       \
      exit(1);                                  \
    }                                           \
  }
#else
#include "main.h"
#endif

#endif /* CODEC_OSAL_HPP_ */
