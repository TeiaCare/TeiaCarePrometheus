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
/*!
 * \class labels
 * \brief Container for metric labels (key-value pairs).
 *
 * Labels are used to differentiate instances of the same metric. For example,
 * an HTTP request counter might have labels for the endpoint and method.
 * Labels must follow Prometheus naming conventions and are serialized in a specific format.
 */
class labels
{
    /*!
     * \struct label
     * \brief Internal representation of a single label.
     */
    struct label
    {
        /*!
         * \brief Construct a new label
         * \param n The label name
         * \param v The label value
         */
        label(std::string n, std::string v);

        bool operator==(label const& other) const noexcept = default;

        std::string name;
        std::string value;
    };

public:
    /*!
     * \brief Default constructor for an empty label set
     */
    labels() = default;

    /*!
     * \brief Construct labels from an initializer list
     * \tparam K Key type (must be convertible to string)
     * \tparam V Value type (must be convertible to string)
     * \param key_value_pairs Initializer list of key-value pairs
     *
     * Example: labels{{"method", "GET"}, {"endpoint", "/api/users"}}
     */
    template <StringLike K, StringLike V>
    labels(std::initializer_list<std::pair<K, V>> key_value_pairs);

    /*!
     * \brief Add a label to the set
     * \tparam K Key type (must be convertible to string)
     * \tparam V Value type (must be convertible to string)
     * \param key The label name
     * \param value The label value
     * \return Reference to this labels object for chaining
     *
     * Label names must follow Prometheus conventions (alphanumeric with underscores).
     * Throws std::invalid_argument if the label name is invalid.
     */
    template <StringLike K, StringLike V>
    labels& add(K&& key, V&& value);

    /*!
     * \brief Check if the label set is empty
     * \return true if there are no labels, false otherwise
     */
    bool empty() const noexcept;

    /*!
     * \brief Convert labels to Prometheus string format
     * \return String representation of labels (e.g., {method="GET",endpoint="/api/users"})
     *
     * The result is cached for performance.
     */
    std::string to_string() const;

    /*!
     * \brief Compare two label sets for equality
     * \param other The other label set to compare
     * \return true if both label sets are equal, false otherwise
     */
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
