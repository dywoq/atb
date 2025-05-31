#ifndef DYWOQATB_HXX
#define DYWOQATB_HXX

#include <concepts>
#include <exception>
#include <format>
#include <optional>
#include <stdexcept>

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
  requires std::derived_from<_Err, std::exception>
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
  _Tp __got_;
  _Tp __expected_;

public:
  explicit test(std::optional<const char *> __name, _Tp __got, _Tp __expected) noexcept
      : __name_(std::move(__name)), __got_(std::move(__got)), __expected_(std::move(__expected)) {}

  const char *name() const DYWOQATB_NOEXCEPT {
    DYWOQATB_TRY_CATCH_BLOCK_START
    return __name_.value();
    DYWOQATB_TRY_CATCH_BLOCK_END(std::bad_optional_access, test_error)
  }

  const _Tp &got() const noexcept { return __got_; }

  const _Tp &expected() const noexcept { return __expected_; }

  operator bool() const noexcept { return __got_ == __expected_; }
};

struct test_runner {
  template <typename _Tp>
    requires std::equality_comparable<_Tp>
  static void run(const test<_Tp> &__test) DYWOQATB_NOEXCEPT {
    if (__test.got() != __test.expected()) {
      const char *__fmt_name = std::string(__test.name()).empty() ? "unknown" : __test.name();
      std::string __fmt_msg = std::format("[dywoq.atb.hxx] test with name \"{}\" failed; got {}, expected: {}",
                                          __fmt_name, __test.got(), __test.expected());
      test_error __err(__fmt_msg.c_str());
      __internal::__throw_exception<test_error>(__err.what());
    }
  }

  template <typename _Tp>
    requires std::equality_comparable<_Tp>
  static void run(const std::optional<const char *> &__name, const _Tp &__got,
                  const _Tp &__expected) DYWOQATB_NOEXCEPT {
    test<_Tp> __test(__name, __got, __expected);
    run(__test);
  }
};

DYWOQATB_END_NAMESPACE
#endif

#endif