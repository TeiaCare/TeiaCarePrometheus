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
/*!
 * \struct metric_type
 * \brief Template struct for associating metric types with their string names.
 * \tparam T The metric type
 *
 * This struct is specialized for each metric type using the DEFINE_METRIC_TYPE macro.
 */
template <typename T>
struct metric_type;

/*!
 * \brief Template variable for accessing the metric type string.
 * \tparam T The metric type
 *
 * Returns the string representation of the metric type (e.g., "counter", "gauge").
 */
template <typename T>
inline constexpr const char* metric_type_t = metric_type<T>::type;

/*!
 * \def DEFINE_METRIC_TYPE
 * \brief Macro for defining the metric type string for a given type.
 * \param T The metric type (e.g., tc::prometheus::counter)
 * \param type_str The string representation (e.g., "counter")
 *
 * This macro specializes the metric_type template for a specific metric type.
 */
#define DEFINE_METRIC_TYPE(T, type_str)               \
    template <>                                       \
    struct metric_type<T>                             \
    {                                                 \
        static constexpr const char* type = type_str; \
    }

/*!
 * \concept Numeric
 * \brief Concept for numeric types.
 * \tparam T The type to check
 *
 * Satisfied by integral and floating-point types.
 */
template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

/*!
 * \concept StringLike
 * \brief Concept for string-like types.
 * \tparam T The type to check
 *
 * Satisfied by types that can be converted to std::string.
 */
template <typename T>
concept StringLike = std::convertible_to<T, std::string>;

}
