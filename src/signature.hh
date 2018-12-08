#pragma once

#include <base/assert.hh>
#include <base/types.hh>

#include <git2.h>

namespace git {

class Signature {
 public:
  inline explicit Signature(git_signature* sig) : _sig(sig, git_signature_free) { CHECK(sig); }
  Signature(const String& name, const String& email, const DateTime& date);

 public:
  inline const auto* raw_sig() const { return _sig.get(); }

 private:
  SharedPtr<git_signature> _sig;
};

}  // namespace git
