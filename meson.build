project('libgit++',
  'cpp',
  default_options : ['cpp_std=c++17']
)

dep_git2 = dependency('libgit2')

includes = include_directories('src')

library('git++',
  [
    'src/base/assert.cc',
    'src/base/assert.hh',
    'src/base/exception.cc',
    'src/base/exception.hh',
    'src/base/log.cc',
    'src/base/log.hh',
    'src/base/types.hh',
    'src/obj/blob.cc',
    'src/obj/blob.hh',
    'src/obj/commit.cc',
    'src/obj/commit.hh',
    'src/obj/object.cc',
    'src/obj/object.hh',
    'src/obj/tree.cc',
    'src/obj/tree.hh',
    'src/ref/ref.cc',
    'src/ref/ref.hh',
    'src/repo/bare.cc',
    'src/repo/bare.hh',
    'src/buffer.hh',
    'src/index.cc',
    'src/index.hh',
    'src/init.hh',
    'src/signature.cc',
    'src/signature.hh',
  ],
  dependencies: dep_git2,
  include_directories: includes,
  link_args: ['-lstdc++fs'],
)
