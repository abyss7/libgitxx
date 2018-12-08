#include "object.hh"

#include <base/exception.hh>

namespace git {

Object::Type Object::type() const {
  switch (git_object_type(raw_object())) {
    case GIT_OBJ_BLOB:
      return BLOB;
    case GIT_OBJ_COMMIT:
      return COMMIT;
    case GIT_OBJ_TREE:
      return TREE;
    default:
      return UNKNOWN;
  }
}

Object::Id::Id(const String& content, Type type) : _id(new git_oid) {
  git_otype git_type;

  switch (type) {
    case BLOB:
      git_type = GIT_OBJ_BLOB;
      break;
    case COMMIT:
      git_type = GIT_OBJ_COMMIT;
      break;
    case TREE:
      git_type = GIT_OBJ_TREE;
      break;
    default:
      git_type = GIT_OBJ_BAD;
  }

  TryGitCall(git_odb_hash(const_cast<git_oid*>(_id.get()), content.data(), content.size(), git_type));
}

}  // namespace git
