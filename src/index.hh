#pragma once

#include <obj/tree.hh>

#include <git2/sys/repository.h>

namespace git {

class Index {
 public:
  inline explicit Index(git_index* raw_index) : _index(raw_index, git_index_free) { CHECK(raw_index); }

  inline void SetRepository(git_repository* raw_repo) { git_repository_set_index(raw_repo, _index.get()); }
  void SetTree(Tree tree);

  bool IsEmpty() const;

  void Clear();

 protected:
  SharedPtr<git_index> _index;
};

}  // namespace git
