#include "signature.hh"

#include <base/exception.hh>

namespace git {

Signature::Signature(const String& name, const String& email, const DateTime& data) {
  git_signature* sig;
  TryGitCall(git_signature_new(&sig, name.c_str(), email.c_str(), std::chrono::system_clock::to_time_t(data), 0));
  _sig.reset(sig, git_signature_free);
}

}  // namespace git
