#ifndef DYWOQATB_HXX
#define DYWOQATB_HXX

#include <concepts>
#include <exception>
#include <optional>
#include <stdexcept>
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
#  error "[dywoq.atb.hxx] critical error: the code must be compiled by C++ compiler"
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
#    warning                                                                                                           \
        "[dywoq.atb.hxx] warning: there's no macro for tracking exceptions' status; the value of DYWOQATB_HAS_EXCEPTIONS will be always 1 "
#    define DYWOQATB_HAS_EXCEPTIONS 1
#  elif defined(_MSC_VER)
#    pragma message(                                                                                                   \
        "[dywoq.atb.hxx] warning: there's no macro for tracking exceptions' status; the value of DYWOQATB_HAS_EXCEPTIONS will be always 1 ")
#    define DYWOQATB_HAS_EXCEPTIONS 1
#  endif
#endif

#if !DYWOQATB_HAS_EXCEPTIONS
#  define DYWOQATB_NOEXCEPT noexcept
#else
#  define DYWOQATB_NOEXCEPT
#endif

#if DYWOQATB_HAS_EXCEPTIONS
#  define DYWOQATB_TRY_CATCH_BLOCK_START try {
#else
#  define DYWOQATB_TRY_CATCH_BLOCK_START
#endif

#if DYWOQATB_HAS_EXCEPTIONS
#  define DYWOQATB_TRY_CATCH_BLOCK_END(exception_to_catch, exception_to_throw)                                         \
    }                                                                                                                  \
    catch (const exception_to_catch &__e) {                                                                            \
      throw exception_to_throw(__e.what());                                                                            \
    }                                                                                                                  \
    catch (...) {                                                                                                      \
      throw exception_to_throw("an unknown error");                                                                    \
    }
#else
#  define DYWOQATB_TRY_CATCH_BLOCK_END(exceptions_to_catch, exceptions_to_throw)
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

template <typename _Tp>
  requires std::equality_comparable<_Tp>
struct test {
private:
  std::optional<const char *> __name_;
  std::optional<_Tp> __got_;
  std::optional<_Tp> __expected_;

public:
  explicit test(std::optional<const char *> __name, std::optional<_Tp> __got, std::optional<_Tp> __expected) noexcept
      : __name_(std::move(__name)), __got_(std::move(__got)), __expected_(std::move(__expected)) {}

  const char *name() const DYWOQATB_NOEXCEPT {
    DYWOQATB_TRY_CATCH_BLOCK_START
    return __name_.value();
    DYWOQATB_TRY_CATCH_BLOCK_END(std::bad_optional_access, test_error)
  }

  const _Tp &got() const DYWOQATB_NOEXCEPT {
    DYWOQATB_TRY_CATCH_BLOCK_START
    return __got_.value();
    DYWOQATB_TRY_CATCH_BLOCK_END(std::bad_optional_access, test_error)
  }

  const _Tp &expected() const DYWOQATB_NOEXCEPT {
    DYWOQATB_TRY_CATCH_BLOCK_START
    return __expected_.value();
    DYWOQATB_TRY_CATCH_BLOCK_END(std::bad_optional_access, test_error)
  }

  operator bool() const DYWOQATB_NOEXCEPT {
    DYWOQATB_TRY_CATCH_BLOCK_START
    return __got_.value() == __expected_.value();
    DYWOQATB_TRY_CATCH_BLOCK_END(std::bad_optional_access, test_error)
  }
};

DYWOQATB_END_NAMESPACE
#endif

#endif