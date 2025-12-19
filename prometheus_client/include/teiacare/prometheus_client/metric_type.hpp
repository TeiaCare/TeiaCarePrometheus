// Copyright 2025 TeiaCare
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <concepts>
#include <string>

namespace tc::prometheus
{
template <typename T>
struct metric_type;

template <typename T>
inline constexpr const char* metric_type_t = metric_type<T>::type;

#define DEFINE_METRIC_TYPE(T, type_str)               \
    template <>                                       \
    struct metric_type<T>                             \
    {                                                 \
        static constexpr const char* type = type_str; \
    }

template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template <typename T>
concept StringLike = std::convertible_to<T, std::string>;

}
