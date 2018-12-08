#include "tree.hh"

#include <base/exception.hh>

namespace git {

void Tree::ListEntries(EntriesFn callback) const {
  int size = git_tree_entrycount(raw_tree());
  for (int i = 0; i < size; ++i) {
    callback(Entry(git_tree_entry_byindex(raw_tree(), i), raw_repo()));
  }
}

Optional<Tree::Entry> Tree::GetEntry(const Path& subpath) const {
  CHECK(subpath.is_relative());
  // FIXME: use CHECK() << "message" syntax

  git_tree_entry* entry;

  // FIXME: handle specific error.
  int result = git_tree_entry_bypath(&entry, raw_tree(), subpath.c_str());
  if (result) {
    return {};
  }

  return Entry(entry, raw_repo(), true);
}

namespace {

auto entry_deleter = [](const git_tree_entry* entry) { git_tree_entry_free(const_cast<git_tree_entry*>(entry)); };

}  // namespace

Tree::Entry::Entry(const git_tree_entry* entry, git_repository* repo, bool own)
    : _entry(entry, own ? entry_deleter : [](const git_tree_entry*) {}), _repo(repo) {
  CHECK(entry);
  CHECK(repo);
}

Object Tree::Entry::object() const {
  git_object* obj;
  TryGitCall(git_tree_entry_to_object(&obj, _repo, _entry.get()));
  return Object(obj);
}

}  // namespace git
