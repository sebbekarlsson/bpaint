#ifndef BPAINT_MACROS_H
#define BPAINT_MACROS_H

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef ABS
#define ABS(a) MAX(-a, a)
#endif
#define BPAINT_OR(a, b) ((a) ? (a) : (b))

#define BPAINT_CLI_RED "\x1B[31m"
#define BPAINT_CLI_GREEN "\x1B[32m"
#define BPAINT_CLI_YELLLOW "\x1B[33m"
#define BPAINT_CLI_BLUE "\x1B[34m"
#define BPAINT_CLI_MAGENTA "\x1B[35m"
#define BPAINT_CLI_CYAN "\x1B[36m"
#define BPAINT_CLI_WHITE "\x1B[37m"
#define BPAINT_CLI_RESET "\x1B[0m"

#define BPAINT_WARNING(...)                                                    \
  {                                                                            \
    printf(BPAINT_CLI_RED "(BPAINT)(Warning)(%s): \n" BPAINT_CLI_RESET,        \
           __func__);                                                          \
    fprintf(__VA_ARGS__);                                                      \
  }

#define BPAINT_INFO(...)                                                       \
  {                                                                            \
    printf(BPAINT_CLI_BLUE "(BPAINT)(Info)(%s): \n" BPAINT_CLI_RESET,          \
           __func__);                                                          \
    fprintf(__VA_ARGS__);                                                      \
  }

#define BPAINT_WARNING_RETURN(ret, ...)                                        \
  {                                                                            \
    printf("\n****\n");                                                        \
    printf(BPAINT_CLI_RED "(BPAINT)(Warning)(%s): \n" BPAINT_CLI_RESET,        \
           __func__);                                                          \
    fprintf(__VA_ARGS__);                                                      \
    printf("\n****\n");                                                        \
    return ret;                                                                \
  }

#define BPAINT_WARNING_RETURN_NOT_INITIALIZED(ret)                             \
  {                                                                            \
    printf("\n****\n");                                                        \
    printf(BPAINT_CLI_RED "(BPAINT)(Warning)(%s): \n" BPAINT_CLI_RESET,        \
           __func__);                                                          \
    fprintf(stderr, "Not initialized.\n");                                     \
    printf("\n****\n");                                                        \
    return ret;                                                                \
  }

#define BPAINT_PANIC(fp, ...)                                                  \
  {                                                                            \
    BPAINT_WARNING(fp, __VA_ARGS__);                                           \
    exit(0);                                                                   \
  }

#define BPAINT_ASSERT_PANIC(expr)                                              \
  {                                                                            \
    if (!(expr)) {                                                             \
      BPAINT_PANIC(stderr, "Assertion failed: %s.\n", #expr);                  \
    }                                                                          \
  }

#define BPAINT_ASSERT_RETURN(expr, ret)                                        \
  {                                                                            \
    if (!(expr)) {                                                             \
      BPAINT_WARNING(stderr, "(BPAINT): Assertion failed: %s.\n", #expr);      \
      return ret;                                                              \
    }                                                                          \
  }

#define BPAINT_FREE(v) free(v)
#define BPAINT_REALLOC(ptr, len) realloc(ptr, len)
#define BPAINT_CALLOC(n, s) calloc(n, s)

#define BPAINT_GENERATE_ENUM(ENUM) ENUM,
#define BPAINT_GENERATE_STRING(STRING) #STRING,

#define BPAINT_PAD_PRINTF(pad, fmt, ...)                                       \
  printf("%*s" fmt, pad, "", __VA_ARGS__);
#define BPAINT_PAD_SNPRINTF(buff, cap, pad, fmt, ...)                          \
  snprintf(buff, cap, "%*s" fmt, pad, "", __VA_ARGS__);
#define BPAINT_PAD_PRINT(pad, buff) printf("%*s%s", pad, "", buff);

#define BPAINT_DEFINE_ENUM(enum_name, CAPITAL_NAME, FOR_EACH_MACRO)            \
  typedef enum { FOR_EACH_MACRO(BPAINT_GENERATE_ENUM) } enum_name;             \
  static const char *const CAPITAL_NAME##_TO_STR[] = {                         \
      FOR_EACH_MACRO(BPAINT_GENERATE_STRING)};

#define BPAINT_PRINT_N(n, v, end, fp)                                          \
  {                                                                            \
    for (int64_t g = 0; g < (n); g++) {                                        \
      fprintf(fp, "%s", v);                                                    \
    }                                                                          \
    fprintf(fp, end);                                                          \
  }

#endif
