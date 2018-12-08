#pragma once

#include <base/exception.hh>

namespace git {

// TODO: refactor this struct to be used where necessary,
//       with respect to allowed multiple calls to |git_libgit2_init()|.

struct Init {
  inline Init() {
    TryGitCall(git_libgit2_init());
    TryGitCall(git_libgit2_opts(GIT_OPT_SET_CACHE_MAX_SIZE, 1 << 20));
    TryGitCall(git_libgit2_opts(GIT_OPT_SET_CACHE_OBJECT_LIMIT, GIT_OBJ_TREE, 1 << 22));
    TryGitCall(git_libgit2_opts(GIT_OPT_SET_CACHE_OBJECT_LIMIT, GIT_OBJ_BLOB, 1 << 10));
    // Caching blobs is useful for write operation.
  }

  inline ~Init() { TryGitCall(git_libgit2_shutdown()); }
};

}  // namespace git
