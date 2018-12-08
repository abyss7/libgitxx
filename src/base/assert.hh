#pragma once

#include <base/log.hh>

namespace git {
void GetStackTrace(List<String>& frames, ui8 max_depth = 62) noexcept;
}  // namespace git

// There is a trick how to use lambda as expression:
//
//   CHECK([&]{ return false; }());
//                             ^^
//
#define CHECK(expr)                           \
  if (!(expr))                                \
  [] {                                        \
    using namespace ::git;                    \
    using namespace ::std;                    \
    List<String> frames;                      \
    Log log(ASSERT);                          \
    GetStackTrace(frames);                    \
    log << "CHECK failed: " << #expr << endl; \
    for (const auto& frame : frames) {        \
      log << "\t" << frame << endl;           \
    }                                         \
    return log;                               \
  }()

// TODO: investigate why clang can't link with |__builtin_assume()|

#if defined(NDEBUG)
#define DCHECK_O_EVAL(expr) (void)(expr);
#define DCHECK(expr)
#define NOTREACHED() __builtin_unreachable()
#else
#define DCHECK_O_EVAL(expr) CHECK(expr)
#define DCHECK(expr) CHECK(expr)
#define NOTREACHED() DCHECK(false) << "NOTREACHED"
#endif
