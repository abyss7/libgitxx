#pragma once

#include "object.hh"
#include "tree.hh"

#include <index.hh>

namespace git {

class Commit : public Object {
 public:
  inline explicit Commit(Ptr obj_ptr) : Object(obj_ptr) { CHECK(type() == COMMIT); }
  inline explicit Commit(git_commit* raw_commit) : Object(reinterpret_cast<git_object*>(raw_commit)) {
    CHECK(raw_commit);
  }

  Optional<String> header(const String& name) const;

  inline String message() const { return git_commit_message(raw_commit()); }
  Optional<Commit> parent(ui8 num = 0) const;
  Tree tree() const;

  bool IsAncestorOf(const Commit& commit) const;

  Index Merge(const Commit& their) const;

 public:
  inline const git_commit* raw_commit() const { return reinterpret_cast<const git_commit*>(raw_object()); }
};

}  // namespace git
