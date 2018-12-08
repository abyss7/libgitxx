#include "exception.hh"

namespace git {

Exception::Exception(int result) : _error(static_cast<git_error_code>(result)) {
  auto* last_error = giterr_last();

  if (last_error) {
    _class = last_error->klass;
    _message = last_error->message;
  }
}

int TryGitCall(int result, Set<git_error_code> ignore) {
  ignore.insert(GIT_OK);

  if (ignore.count(static_cast<git_error_code>(result)) || result > 0) {
    return result;
  }

  throw Exception(result);
}

}  // namespace git
