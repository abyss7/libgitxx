#pragma once

#include <obj/commit.hh>
#include <signature.hh>

namespace git {
namespace repo {

class Bare {
 public:
  // auto-detects .git folder if path to working tree is provided
  explicit Bare(const Path& path);

  // returns path to the root of bare repository (e.g. path to .git)
  inline Path path() const { return git_repository_path(_repo.get()); }

 public:
  Object::Id CreateBlob(const String& content);

  Object::Id CreateCommit(const Signature& author, const Signature& committer, const String& message, const Tree& tree,
                          const String& ref_name = {}, const List<Commit>& parents = {});

  Object GetObject(const Object::Id& id) const;
  bool HasObject(const Object::Id& id, Object::Type type) const;

 public:
  inline const auto* raw_repo() const { return _repo.get(); }

 protected:
  SharedPtr<git_repository> _repo;
};

}  // namespace repo
}  // namespace git
