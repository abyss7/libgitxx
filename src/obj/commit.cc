#include "commit.hh"

#include <base/exception.hh>
#include <buffer.hh>

namespace git {

Optional<String> Commit::header(const String& name) const {
  Buffer header;

  if (TryGitCall(git_commit_header_field(header.raw_buf(), raw_commit(), name.c_str()), {GIT_ENOTFOUND}) == GIT_OK) {
    return String(header);
  }

  return {};
}

Optional<Commit> Commit::parent(ui8 num) const {
  if (num >= git_commit_parentcount(raw_commit())) {
    return {};
  }

  git_commit* parent;
  TryGitCall(git_commit_parent(&parent, raw_commit(), num));
  return Commit(parent);
}

Tree Commit::tree() const {
  git_tree* tree;
  TryGitCall(git_commit_tree(&tree, raw_commit()));
  return Tree(tree);
}

bool Commit::IsAncestorOf(const Commit& commit) const {
  git_oidarray ids;

  if (TryGitCall(git_merge_bases(&ids, raw_repo(), raw_id(), commit.raw_id()), {GIT_ENOTFOUND}) == GIT_ENOTFOUND) {
    return false;
  }

  for (auto i = 0u; i < ids.count; ++i) {
    if (git_oid_equal(raw_id(), &ids.ids[i])) {
      git_oidarray_free(&ids);
      return true;
    }
  }

  git_oidarray_free(&ids);
  return false;
}

Index Commit::Merge(const Commit& their) const {
  git_index* index;
  git_merge_options merge_opts;

  TryGitCall(git_merge_init_options(&merge_opts, GIT_MERGE_OPTIONS_VERSION));
  TryGitCall(git_merge_commits(&index, raw_repo(), raw_commit(), their.raw_commit(), &merge_opts));

  return Index(index);
}

}  // namespace git
