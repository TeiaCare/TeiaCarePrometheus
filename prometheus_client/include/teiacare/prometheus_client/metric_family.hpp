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

#include <teiacare/prometheus_client/base_metric.hpp>
#include <teiacare/prometheus_client/base_metric_family.hpp>
#include <teiacare/prometheus_client/labels.hpp>
#include <teiacare/prometheus_client/metric_serializer.hpp>
#include <teiacare/prometheus_client/metric_type.hpp>

#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace tc::prometheus
{
/*!
 * \class metric_family
 * \brief Template class for managing a family of metrics of the same type.
 * \tparam MetricT The metric type (counter, gauge, histogram, or summary)
 *
 * A metric family groups related metrics that share the same name and type but
 * differ in their label values. This class provides thread-safe operations for
 * adding and retrieving metrics within the family.
 *
 * Thread-safety:
 * - All methods are thread-safe and can be called from multiple threads simultaneously
 * - Metrics can be added or retrieved concurrently
 */
template <typename MetricT>
class metric_family : public tc::prometheus::base_metric_family
{
    static_assert(std::is_base_of_v<tc::prometheus::base_metric, MetricT>);

public:
    /*!
     * \brief Construct a new metric family
     * \param name The metric family name
     * \param help Help text describing the metric family
     */
    explicit metric_family(std::string name, std::string help);

    /*!
     * \brief Add a new metric to the family
     * \param labels The labels for the new metric
     * \return Shared pointer to the newly created metric
     *
     * Creates a new metric instance with the given labels and adds it to the family.
     */
    std::shared_ptr<MetricT> add_metric(tc::prometheus::labels labels);

    /*!
     * \brief Get an existing metric from the family
     * \param labels The labels of the metric to retrieve
     * \return Shared pointer to the metric
     *
     * Throws std::runtime_error if no metric with the given labels exists.
     */
    std::shared_ptr<MetricT> get_metric(tc::prometheus::labels labels);

    /*!
     * \brief Serialize all metrics in the family
     * \param serializer The serializer to use for output
     */
    void serialize(tc::prometheus::base_metric_serializer& serializer) const override;

    /*!
     * \brief Reset all metrics in the family to their initial state
     */
    void reset() override;

    /*!
     * \brief Get the metric family name
     * \return The metric family name
     */
    const std::string& name() const override;

    /*!
     * \brief Get the metric family help text
     * \return The help text describing the metric family
     */
    const std::string& help() const override;

    /*!
     * \brief Get the metric type as a string
     * \return The metric type (e.g., "counter", "gauge", "histogram", "summary")
     */
    const std::string& type() const override;

    /*!
     * \brief Get all metrics in the family
     * \return Vector of shared pointers to all metrics in the family
     */
    const std::vector<std::shared_ptr<tc::prometheus::base_metric>>& metrics() const override;

    /*!
     * \brief Add a new metric using initializer list syntax
     * \param label_pairs Initializer list of label key-value pairs
     * \return Shared pointer to the newly created metric
     *
     * Example: family.add_metric({{"method", "GET"}, {"endpoint", "/api/users"}})
     */
    std::shared_ptr<MetricT> add_metric(std::initializer_list<std::pair<std::string, std::string>> label_pairs)
    {
        tc::prometheus::labels ls;
        for (const auto& [key, value] : label_pairs)
        {
            ls.add(key, value);
        }
        return add_metric(ls);
    }

private:
    const std::string _name;
    const std::string _help;
    mutable std::mutex _mutex;
    std::vector<std::shared_ptr<tc::prometheus::base_metric>> _metrics;
};

template <typename MetricT>
metric_family<MetricT>::metric_family(std::string name, std::string help)
    : _name(std::move(name))
    , _help(std::move(help))
// , _labels(std::move(labels))
{
}

// template <typename... Args>
template <typename MetricT>
std::shared_ptr<MetricT> metric_family<MetricT>::add_metric(tc::prometheus::labels labels) // , Args&&... args
{
    // auto metric = std::make_shared<MetricT>(_name, std::move(labels));
    auto metric = std::make_shared<MetricT>(_name, labels);
    {
        std::scoped_lock lock(_mutex);
        _metrics.emplace_back(metric);
    }

    return metric;
}

template <typename MetricT>
std::shared_ptr<MetricT> metric_family<MetricT>::get_metric(tc::prometheus::labels labels)
{
    std::string metric_key = labels.to_string();
    std::scoped_lock lock(_mutex);
    for (const auto& metric : _metrics)
    {
        if (metric->labels().to_string() == metric_key)
            return std::static_pointer_cast<MetricT>(metric);
    }

    throw std::runtime_error("Metric with labels " + metric_key + " not found in family: " + _name);
}

template <typename MetricT>
void metric_family<MetricT>::serialize(tc::prometheus::base_metric_serializer& serializer) const
{
    serializer.serialize(*this);
}

template <typename MetricT>
void metric_family<MetricT>::reset()
{
    std::scoped_lock lock(_mutex);
    for (const auto& m : _metrics)
    {
        m->reset();
    }
}

template <typename MetricT>
const std::string& metric_family<MetricT>::name() const
{
    return _name;
}

template <typename MetricT>
const std::string& metric_family<MetricT>::help() const
{
    return _help;
}

template <typename MetricT>
const std::string& metric_family<MetricT>::type() const
{
    static const std::string type = tc::prometheus::metric_type_t<MetricT>;
    return type;
}

template <typename MetricT>
const std::vector<std::shared_ptr<tc::prometheus::base_metric>>& metric_family<MetricT>::metrics() const
{
    return _metrics;
}

}
