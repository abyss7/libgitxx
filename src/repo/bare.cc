#include "bare.hh"

#include <base/exception.hh>

namespace git {
namespace repo {

Bare::Bare(const Path& path)
    : _repo(
          [&] {
            git_repository* repo;
            TryGitCall(git_repository_open(&repo, path.c_str()));
            return repo;
          }(),
          git_repository_free) {}

Object::Id Bare::CreateBlob(const String& content) {
  git_oid id;
  TryGitCall(git_blob_create_frombuffer(&id, _repo.get(), content.data(), content.size()));
  return Object::Id(id);
}

Object::Id Bare::CreateCommit(const Signature& author, const Signature& committer, const String& message,
                              const Tree& tree, const String& ref_name, const List<Commit>& parents) {
  using git_commit_ptr = const git_commit*;
  UniquePtr<git_commit_ptr[]> git_parents(new git_commit_ptr[parents.size()]);
  {
    int i = 0;
    for (const auto& parent : parents) {
      git_parents[i++] = parent.raw_commit();
    }
  }

  git_oid id;
  TryGitCall(git_commit_create(&id, _repo.get(), ref_name.empty() ? nullptr : ref_name.c_str(), author.raw_sig(),
                               committer.raw_sig(), nullptr, message.c_str(), tree.raw_tree(), parents.size(),
                               parents.empty() ? nullptr : git_parents.get()));

  return Object::Id(id);
}

Object Bare::GetObject(const Object::Id& id) const {
  git_object* object;
  TryGitCall(git_object_lookup(&object, _repo.get(), id.raw_id(), GIT_OBJ_ANY));
  return Object(object);
}

bool Bare::HasObject(const Object::Id& id, Object::Type type) const {
  git_object* object;
  if (TryGitCall(git_object_lookup(&object, _repo.get(), id.raw_id(), GIT_OBJ_ANY), {GIT_ENOTFOUND}) != GIT_OK) {
    return false;
  }

  return Object(object).type() == type;
}

}  // namespace repo
}  // namespace git
