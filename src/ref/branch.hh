#pragma once

#include <ref/ref.hh>

namespace git {

class Branch : public Ref {
 public:
  explicit Branch(git_reference* ref);

  inline bool IsHead() const { return TryGitCall(git_branch_is_head(raw_ref())) == 1; }

  String name() const;

  Optional<Ref> GetUpstream() const;
  void SetUpstream(const String& branch_name);
};

}  // namespace git
