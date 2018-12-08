#pragma once

#include <base/types.hh>

#include <git2.h>

namespace git {

class Exception : public ::std::exception {
 public:
  explicit Exception(int result);

  inline auto git_class() const { return _class; }
  inline auto git_error() const { return _error; }

  inline const char* what() const noexcept override { return _message.c_str(); }

 private:
  git_error_code _error;
  int _class;
  String _message;
};

int TryGitCall(int result, Set<git_error_code> ignore = {});

}  // namespace git
