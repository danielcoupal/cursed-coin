/***************************************************************************************************
 *
 * File        : logging.h
 * Project     : cursed-coin
 * Copyright   : Copyleft
 * Created on  : 2023-02-17
 * Notes       : None
 *
 **************************************************************************************************/

/**
 * @file logging.h
 *
 * @brief
 *      Easily-configurable logging functions
 *
 * @author Daniel Coupal
 */

/**
 * @defgroup COMMON common
 * @{
 */ 

#ifndef LOGGING_H_
#define LOGGING_H_

#ifdef __cplusplus
 extern "C" {
#endif

/*==================================================================================================
 = INCLUDE FILES
 =================================================================================================*/
/* Standard header files */
#include <stdio.h>

/* Library header files */

/* User header files */
 
 
/*==================================================================================================
 = DEFINITIONS
 =================================================================================================*/
/**
 * @name Logging Verbosity
 * Set the verbosity of LOG_PRINTFx macros. The highest LOG_VERBOSITY_x value is used.
 * 
 * Example:
 * @code
 * // Where: -DLOG_VERBOSITY_THIS=0 -DLOG_VERBOSITY_ALL=1
 * int x = 0;
 * if(1 <= LOG_VERBOSITY_THIS) {
 *     x = 1;
 * }
 * LOG_PRINTF_V("The result is %d", x); // -> prints "The result is 1"
 * LOG_PRINTF_VV("Also, buy eggs tonight."); // -> noop
 * @endcode
 * @{
 * 
 * @def LOG_VERBOSITY_THIS
 *      Sets the verbosity of logging for a given module.
 *      This should only be defined against individual files or modules.
 *      This value is overridden by LOG_VERBOSITY_ALL if it is higher, so only this macro is needed
 *      to determine behavior.
 * 
 * @def LOG_VERBOSITY_ALL
 *      Sets the verbosity of logging for the project.
 *      This should be defined globally.
 * 
 * @}
 * End Logging Verbosity
 */
#ifndef LOG_VERBOSITY_THIS
#   define LOG_VERBOSITY_THIS   (0)
#endif
#ifdef LOG_VERBOSITY_ALL
#   if (LOG_VERBOSITY_THIS < LOG_VERBOSITY_ALL)
#       undef  LOG_VERBOSITY_THIS
#       define LOG_VERBOSITY_THIS LOG_VERBOSITY_ALL
#   endif
#else
#   define LOG_VERBOSITY_ALL    (0)
#endif

/**
 * @brief Print formatted message to logging output
 */
#define LOG_PRINTF(fmt, ...)        \
    do                              \
    {                               \
        FILE * f = fopen("log.txt", "a");     \
        fprintf(f, fmt, ##__VA_ARGS__); \
        fprintf(f, "\n");               \
        fflush(f);\
        fclose(f);\
    } while (0)

#define LOG_PUTCHAR(c)              \
    do                              \
    {                               \
        FILE * f = fopen("log.txt", "a");     \
        fputc(c, f);\
        fflush(f);\
        fclose(f);\
    } while (0)

#if (1 <= LOG_VERBOSITY_THIS)
#   define LOG_PRINTF_V(fmt, ...) LOG_PRINTF(fmt, ##__VA_ARGS__)
#   define LOG_PUTCHAR_V(c) LOG_PUTCHAR(c)
#else
#   define LOG_PRINTF_V(fmt, ...) ((void*)0)
#   define LOG_PUTCHAR_V(fmt, ...) ((void*)0)
#endif

#if (2 <= LOG_VERBOSITY_THIS)
#   define LOG_PRINTF_VV(fmt, ...) LOG_PRINTF(fmt, ##__VA_ARGS__)
#   define LOG_PUTCHAR_VV(c) LOG_PUTCHAR(c)
#else
#   define LOG_PRINTF_VV(fmt, ...) ((void*)0)
#   define LOG_PUTCHAR_VV(fmt, ...) ((void*)0)
#endif

#define WARNING_PRINTF(fmt, ...)      \
    do                              \
    {                               \
        printf("WARNING: ");          \
        printf(fmt, ##__VA_ARGS__); \
        printf("\n");               \
    } while (0)

#define ERROR_PRINTF(fmt, ...)      \
    do                              \
    {                               \
        printf("ERROR  : ");          \
        printf(fmt, ##__VA_ARGS__); \
        printf("\n");               \
    } while (0)

/*==================================================================================================
 = GLOBAL VARIABLES
 =================================================================================================*/

 
/*==================================================================================================
 = PUBLIC FUNCTION PROTOTYPES
 =================================================================================================*/


#ifdef __cplusplus
 }
#endif

#endif /* LOGGING_H_ */

/**
 * End group def
 * @}
 */ 
/* END OF FILE ****************************************************************/

