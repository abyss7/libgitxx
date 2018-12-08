#pragma once

#include "object.hh"

namespace git {

class Blob : public Object {
 public:
  inline explicit Blob(Ptr obj_ptr) : Object(obj_ptr) { CHECK(type() == BLOB); }

  inline ui64 size() const { return git_blob_rawsize(raw_blob()); }
  inline String content() const {
    return String(reinterpret_cast<const char*>(git_blob_rawcontent(raw_blob())), size());
  }

 public:
  inline const git_blob* raw_blob() const { return reinterpret_cast<const git_blob*>(raw_object()); }
};

}  // namespace git
