#ifndef DYWOQATB_HXX
#define DYWOQATB_HXX

#ifdef __dywoqatb
  #define DYWOQATB_VERSION __dywoqatb
#else
  #define DYWOQATB_VERSION 202505LL
#endif

#ifdef __cplusplus
  #if __cplusplus >= 202002LL
    #define DYWOQATB_HAS_CXX20_SUPPORT 1
  #else
    #define DYWOQATB_HAS_CXX20_SUPPORT 0
  #endif
#else
  #error                                                                       \
      "[dywoq.atb.hxx] critical error: the code must be compiled by C++ compiler"
#endif

#if DYWOQATB_HAS_CXX20_SUPPORT && DYWOQATB_VERSION >= 202505LL
  #define DYWOQATB_HAS_BASE_SUPPORT 1
#else
  #define DYWOQATB_HAS_BASE_SUPPORT 0
#endif

#if defined(__clang__) || defined(__GNUC__)
  #ifdef __cpp_exceptions
    #define DYWOQATB_HAS_EXCEPTIONS 1
  #else
    #define DYWOQATB_HAS_EXCEPTIONS 0
  #endif
#elif defined(_MSC_VER)
  #ifdef _HAS_EXCEPTIONS
    #if _HAS_EXCEPTIONS
      #define DYWOQATB_HAS_EXCEPTIONS 1
    #elif
      #define DYWOQATB_HAS_EXCEPTIONS 0
    #endif
  #endif
#else
  #if defined(__clang__) || defined(__GNUC__)
    #warning                                                                   \
        "[dywoq.atb.hxx] warning: there's no macro for tracking exceptions' status; the value of DYWOQATB_HAS_EXCEPTIONS will be always 1 "
    #define DYWOQATB_HAS_EXCEPTIONS 1
  #elif defined(_MSC_VER)
    #pragma message(                                                           \
        "[dywoq.atb.hxx] warning: there's no macro for tracking exceptions' status; the value of DYWOQATB_HAS_EXCEPTIONS will be always 1 ")
    #define DYWOQATB_HAS_EXCEPTIONS 1
  #endif
#endif

#endif