#include "assert.hh"

#include <cxxabi.h>
#include <execinfo.h>

namespace git {

// TODO: implement for platforms other then linux

namespace {

String Demangle(const char* frame) {
  const String string = frame;

  auto begin_name = string.find('(');
  if (begin_name == String::npos) {
    return string;
  }
  begin_name++;

  const auto end_name = string.find('+', begin_name);
  if (end_name == String::npos) {
    return string;
  }

  const String mangled_name = string.substr(begin_name, end_name - begin_name);
  size_t size = 256;
  int status;
  char* demangled_name = abi::__cxa_demangle(mangled_name.c_str(), nullptr, &size, &status);
  if (status == 0) {
    auto result = String(demangled_name);
    free(demangled_name);
    return result;
  } else {
    if (demangled_name) {
      free(demangled_name);
    }
    return mangled_name;
  }
}

}  // namespace

void GetStackTrace(List<String>& frames, ui8 max_depth) noexcept {
  using void_ptr = void*;
  UniquePtr<void_ptr[]> buffer(new void_ptr[max_depth + 1]);

  auto size = backtrace(buffer.get(), max_depth + 1);
  auto symbols = backtrace_symbols(buffer.get(), size);

  // Two last frames always look like:
  //     backtrace
  //     git::GetStackTrace(…)
  //
  // We don't need them.
  for (int i = 2; i < size; ++i) {
    frames.push_back(Demangle(symbols[i]));
    // TODO: implement prettifiers here.
  }

  free(symbols);
}

}  // namespace git
