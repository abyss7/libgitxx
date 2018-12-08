#pragma once

#include <base/types.hh>

#include <git2.h>

namespace git {

class Buffer {
 public:
  inline Buffer() : _buf(GIT_BUF_INIT_CONST(nullptr, 0)) {}
  inline ~Buffer() { git_buf_free(raw_buf()); }

  inline operator String() const { return String(_buf.ptr, _buf.size); }

 public:
  inline git_buf* raw_buf() { return &_buf; }

 private:
  git_buf _buf;
};

}  // namespace git
