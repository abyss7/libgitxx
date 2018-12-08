#pragma once

#include <base/exception.hh>
#include <obj/commit.hh>

namespace git {

class Ref {
 public:
  inline explicit Ref(git_reference* ref) : _ref(ref, git_reference_free) { CHECK(ref); }

  inline String name() const { return git_reference_name(_ref.get()); }

  inline bool IsRemote() const { return !!git_reference_is_remote(_ref.get()); }
  inline bool IsSymbolic() const { return git_reference_type(_ref.get()) == GIT_REF_SYMBOLIC; }

  Optional<Ref> GetTarget() const;

  inline void Delete() { TryGitCall(git_reference_delete(_ref.get())); }
  void Set(const Commit& commit);

 public:
  inline const git_reference* raw_ref() const { return _ref.get(); }

 private:
  SharedPtr<git_reference> _ref;
};

}  // namespace git
