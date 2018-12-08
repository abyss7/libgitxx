#pragma once

#include <base/assert.hh>
#include <base/types.hh>

#include <git2.h>

namespace git {

class Object {
 public:
  enum Type {
    BLOB,
    COMMIT,
    TREE,
    UNKNOWN,
  };

  class Id {
   public:
    Id(const String& content, Type type);

    inline explicit Id(const git_oid* id) : _id(id, [](const git_oid*) {}) { CHECK(_id); }

    inline explicit Id(const git_oid& id) : _id(new git_oid) { git_oid_cpy(const_cast<git_oid*>(_id.get()), &id); }

    inline explicit Id(const String& str) : _id(new git_oid) {
      git_oid_fromstrp(const_cast<git_oid*>(_id.get()), str.c_str());
    }

    inline operator String() const { return git_oid_tostr_s(_id.get()); }

   public:
    inline const git_oid* raw_id() const { return _id.get(); }

   private:
    SharedPtr<const git_oid> _id;
  };

  inline explicit Object(git_object* obj) : _obj(obj, git_object_free) { CHECK(obj); }

  Type type() const;

  inline Id id() const { return Id(raw_id()); }

  template <class Derived>
  inline Derived as() const {
    STATIC_CHECK(::std::is_base_of_v<Object, Derived>);
    return Derived(_obj);
  }

  inline bool operator==(const Object& other) const { return git_oid_equal(raw_id(), other.raw_id()); }

  inline bool operator!=(const Object& other) const { return !this->operator==(other); }

 protected:
  using Ptr = SharedPtr<git_object>;

  inline explicit Object(Ptr obj_ptr) : _obj(obj_ptr) { CHECK(obj_ptr); }

  inline const git_object* raw_object() const { return _obj.get(); }
  inline const git_oid* raw_id() const { return git_object_id(raw_object()); }
  inline git_repository* raw_repo() const { return git_object_owner(raw_object()); }

 private:
  Ptr _obj;
};

}  // namespace git
