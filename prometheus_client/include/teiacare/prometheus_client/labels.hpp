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

#include <teiacare/prometheus_client/metric_type.hpp>

#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace tc::prometheus
{
class labels
{
    struct label
    {
        label(std::string n, std::string v);
        bool operator==(label const& other) const noexcept = default;

        std::string name;
        std::string value;
    };

public:
    labels() = default;

    template <StringLike K, StringLike V>
    labels(std::initializer_list<std::pair<K, V>> key_value_pairs);

    template <StringLike K, StringLike V>
    labels& add(K&& key, V&& value);

    bool empty() const noexcept;
    std::string to_string() const;
    bool operator==(labels const& other) const noexcept;

private:
    std::vector<label> _labels;
    mutable std::optional<std::string> _cached_string;

    static bool is_valid_label_name(std::string_view name);
    static std::string escape_value(std::string const& value);
};

template <StringLike K, StringLike V>
labels::labels(std::initializer_list<std::pair<K, V>> key_value_pairs)
{
    for (const auto& [key, value] : key_value_pairs)
    {
        add(key, value);
    }
}

template <StringLike K, StringLike V>
labels& labels::add(K&& key, V&& value)
{
    std::string name = std::forward<K>(key);
    if (!is_valid_label_name(name))
    {
        throw std::invalid_argument("Invalid label name: " + name);
    }

    _labels.emplace_back(std::move(name), std::string(std::forward<V>(value)));
    _cached_string.reset();
    return *this;
}

}
