#pragma once

#include "object.hh"

namespace git {

class Tree : public Object {
 public:
  inline explicit Tree(Ptr obj_ptr) : Object(obj_ptr) { CHECK(type() == TREE); }
  inline explicit Tree(git_tree* raw_tree) : Object(reinterpret_cast<git_object*>(raw_tree)) { CHECK(raw_tree); }

  class Entry;
  using EntriesFn = Fn<void(const Entry&)>;
  void ListEntries(EntriesFn callback) const;

  Optional<Entry> GetEntry(const Path& subpath) const;

 public:
  inline const git_tree* raw_tree() const { return reinterpret_cast<const git_tree*>(raw_object()); }
};

class Tree::Entry {
 public:
  Entry(const git_tree_entry* raw_entry, git_repository* raw_repo, bool own = false);

  inline String name() const { return String(git_tree_entry_name(_entry.get())); }
  Object object() const;
  inline int mode() const { return git_tree_entry_filemode(_entry.get()); }

 private:
  SharedPtr<const git_tree_entry> _entry;
  git_repository* const _repo;
};

}  // namespace git
