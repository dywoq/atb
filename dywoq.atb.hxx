#ifndef DYWOQATB_HXX
#define DYWOQATB_HXX

#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>

#ifdef __dywoqatb
#  define DYWOQATB_VERSION __dywoqatb
#else
#  define DYWOQATB_VERSION 202505LL
#endif

#ifdef __cplusplus
#  if __cplusplus >= 202002LL
#    define DYWOQATB_HAS_CXX20_SUPPORT 1
#  else
#    define DYWOQATB_HAS_CXX20_SUPPORT 0
#  endif
#else
#  error                                                                       \
      "[dywoq.atb.hxx] critical error: the code must be compiled by C++ compiler"
#endif

#if DYWOQATB_HAS_CXX20_SUPPORT && DYWOQATB_VERSION >= 202505LL
#  define DYWOQATB_HAS_BASE_SUPPORT 1
#else
#  define DYWOQATB_HAS_BASE_SUPPORT 0
#endif

#if defined(__clang__) || defined(__GNUC__)
#  ifdef __cpp_exceptions
#    define DYWOQATB_HAS_EXCEPTIONS 1
#  else
#    define DYWOQATB_HAS_EXCEPTIONS 0
#  endif
#elif defined(_MSC_VER)
#  ifdef _HAS_EXCEPTIONS
#    if _HAS_EXCEPTIONS
#      define DYWOQATB_HAS_EXCEPTIONS 1
#    elif
#      define DYWOQATB_HAS_EXCEPTIONS 0
#    endif
#  endif
#else
#  if defined(__clang__) || defined(__GNUC__)
#    warning                                                                   \
        "[dywoq.atb.hxx] warning: there's no macro for tracking exceptions' status; the value of DYWOQATB_HAS_EXCEPTIONS will be always 1 "
#    define DYWOQATB_HAS_EXCEPTIONS 1
#  elif defined(_MSC_VER)
#    pragma message(                                                           \
        "[dywoq.atb.hxx] warning: there's no macro for tracking exceptions' status; the value of DYWOQATB_HAS_EXCEPTIONS will be always 1 ")
#    define DYWOQATB_HAS_EXCEPTIONS 1
#  endif
#endif

#if !DYWOQATB_HAS_EXCEPTIONS
#  define DYWOQATB_NOEXCEPT noexcept
#else
#  define DYWOQATB_NOEXCEPT
#endif

#define DYWOQATB_BEGIN_NAMESPACE namespace dywoq::atb {
#define DYWOQATB_END_NAMESPACE }

#if DYWOQATB_HAS_BASE_SUPPORT
DYWOQATB_BEGIN_NAMESPACE
namespace __internal {

// AN IMPORTANT NOTE
//
// if exceptions are disabled, the function uses std::terminate to terminate the
// program. however, local smart pointers variables are not freed automatically.
// you need to use reset() function from smart pointers before terminating,
// as it will cause no memory leaks.
template <typename _Err>
  requires std::is_base_of_v<_Err, std::exception>
void __throw_exception(const char *__msg) DYWOQATB_NOEXCEPT {
#  if DYWOQATB_HAS_EXCEPTIONS
  throw _Err(__msg);
#  else
  std::cout << "[dywoq::atb::__internal] the exception was thrown in "
               "-fno-exceptions (/EHsc- for msvc) mode with message \""
            << __msg << "\"";
  std::terminate();
#  endif
}

} // namespace __internal

struct test_error : public std::logic_error {
  using std::logic_error::logic_error;
};

DYWOQATB_END_NAMESPACE
#endif

#endif