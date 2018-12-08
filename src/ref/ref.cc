#include "ref.hh"

namespace git {

Optional<Ref> Ref::GetTarget() const {
  if (!IsSymbolic()) {
    return {};
  }

  const char* target_name = git_reference_symbolic_target(_ref.get());
  git_repository* repo = git_reference_owner(_ref.get());
  git_reference* target;

  TryGitCall(git_reference_lookup(&target, repo, target_name));

  return Ref(target);
}

void Ref::Set(const Commit& commit) {
  git_reference* new_ref;
  TryGitCall(git_reference_set_target(&new_ref, _ref.get(), commit.id().raw_id(), nullptr));
  _ref.reset(new_ref, git_reference_free);
}

}  // namespace git
