#include "log.hh"

#include <base/assert.hh>

#include <iostream>

namespace git {

// static
void Log::Reset(ui8 error_mark, RangeSet&& ranges) {
  ui8 prev = 0;
  for (const auto& range : ranges) {
    if ((prev > 0 && range.second <= prev) || range.second > range.first) {
      NOTREACHED();
    }
    prev = range.first;
  }

  Log::error_mark() = error_mark;
  Log::ranges().reset(new RangeSet(std::move(ranges)));
}

Log::Log(Level level) : _level(level), _error_mark(error_mark()) {}

Log::~Log() {
  auto it = _ranges->lower_bound(std::make_pair(_level, 0));
  if ((it != _ranges->end() && _level >= it->second) || _level == ASSERT) {
    _stream << std::endl;

    auto& output_stream = (_level <= _error_mark) ? std::cerr : std::cout;
    output_stream << _stream.str() << std::flush;
  }

  if (_level == FATAL) {
    std::exit(EXIT_FAILURE);
  }

  if (_level == ASSERT) {
    std::abort();
  }
}

// static
ui8& Log::error_mark() {
  static ui8 error_mark = ERROR;
  return error_mark;
}

// static
SharedPtr<Log::RangeSet>& Log::ranges() {
  static SharedPtr<RangeSet> ranges(new RangeSet{std::make_pair(INFO, FATAL)});
  return ranges;
}

}  // namespace git
