#pragma once

#include <base/types.hh>

#include <sstream>

namespace git {

namespace named_levels {

#undef FATAL
#undef ASSERT
#undef ERROR
#undef WARNING
#undef INFO
#undef VERBOSE
#undef TRACE

enum Level : ui8 {
  FATAL = 0u,   // after |LOG(FATAL)| the program terminates with |exit(1)|
  ASSERT = 1u,  // used only inside |CHECK()|

  // Levels above this line can't be filtered out

  ERROR = 10u,
  WARNING = 20u,
  INFO = 30u,
  VERBOSE = 40u,
  TRACE = 50u,
};

}  // namespace named_levels

class Log {
 public:
  using RangeSet = Set<Pair<ui8>>;  // range is represented: [second, first)

  static void Reset(ui8 error_mark, RangeSet&& ranges);  // expects filtered ranges

  explicit Log(named_levels::Level level);
  ~Log();

  Log(Log&&) = default;
  Log(const Log&) = delete;
  Log& operator=(const Log&) = delete;

  template <class T>
  Log& operator<<(const T& message) {
    _stream << message;
    return *this;
  }

  // for |std::endl|
  Log& operator<<(::std::ostream& (*func)(::std::ostream&)) {
    _stream << func;
    return *this;
  }

 private:
  static ui8& error_mark();
  static SharedPtr<RangeSet>& ranges();

  named_levels::Level _level;
  ui8 _error_mark;
  SharedPtr<RangeSet> _ranges;
  ::std::stringstream _stream;
};

using namespace named_levels;

}  // namespace git

#define LOG(level) ::git::Log(level)

#if defined(NDEBUG)
#define DLOG(level) ::std::stringstream()
#else
#define DLOG(level) LOG(level)
#endif
