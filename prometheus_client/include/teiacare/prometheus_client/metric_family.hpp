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
template <typename MetricT>
class metric_family : public tc::prometheus::base_metric_family
{
    static_assert(std::is_base_of_v<tc::prometheus::base_metric, MetricT>);

public:
    explicit metric_family(std::string name, std::string help);

    // template <typename... Args>
    std::shared_ptr<MetricT> add_metric(tc::prometheus::labels labels); // , Args&&... args
    std::shared_ptr<MetricT> get_metric(tc::prometheus::labels labels);

    void serialize(tc::prometheus::base_metric_serializer& serializer) const override;
    void reset() override;
    const std::string& name() const override;
    const std::string& help() const override;
    const std::string& type() const override;
    const std::vector<std::shared_ptr<tc::prometheus::base_metric>>& metrics() const override;

    // Add overload for initializer list syntax
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
    static constexpr std::string type = tc::prometheus::metric_type_t<MetricT>;
    return type;
}

template <typename MetricT>
const std::vector<std::shared_ptr<tc::prometheus::base_metric>>& metric_family<MetricT>::metrics() const
{
    return _metrics;
}

}
