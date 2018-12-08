#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <list>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>

namespace git {

using ui8 = ::std::uint8_t;
using ui64 = ::std::uint64_t;

using DateTime = ::std::chrono::system_clock::time_point;

template <typename S>
using Fn = ::std::function<S>;

template <typename K, typename V>
using HashMap = ::std::unordered_map<K, V>;

template <typename T>
using List = ::std::list<T>;

template <typename T>
using Optional = ::std::optional<T>;

template <typename F, typename S = F>
using Pair = ::std::pair<F, S>;

using Path = ::std::filesystem::path;

template <typename T>
using Set = ::std::set<T>;

template <typename T>
using SharedPtr = ::std::shared_ptr<T>;

using String = ::std::string;

template <typename T>
using UniquePtr = ::std::unique_ptr<T>;

}  // namespace git
