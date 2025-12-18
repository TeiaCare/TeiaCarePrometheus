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
