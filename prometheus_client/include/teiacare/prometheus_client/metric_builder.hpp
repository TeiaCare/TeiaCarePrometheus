#pragma once

#include <teiacare/prometheus_client/labels.hpp>
#include <teiacare/prometheus_client/metric_family.hpp>
#include <teiacare/prometheus_client/registry.hpp>

#include <string>

namespace tc::prometheus
{
template <typename MetricT>
class metric_builder
{
public:
    explicit metric_builder(std::string name);

    metric_builder& help(std::string help);

    template <StringLike K, StringLike V>
    metric_builder& label(K&& key, V&& value);

    metric_builder& registry(tc::prometheus::registry& registry);

    std::shared_ptr<tc::prometheus::metric_family<MetricT>> build() const;

private:
    std::string _name;
    std::string _help;
    tc::prometheus::labels _labels;
    tc::prometheus::registry* _registry; // = &default_registry();
};

template <typename MetricT>
metric_builder<MetricT>::metric_builder(std::string name)
    : _name(std::move(name))
{
}

template <typename MetricT>
metric_builder<MetricT>& metric_builder<MetricT>::help(std::string help)
{
    _help = std::move(help);
    return *this;
}

template <typename MetricT>
template <StringLike K, StringLike V>
metric_builder<MetricT>& metric_builder<MetricT>::label(K&& key, V&& value)
{
    _labels.add(std::forward<K>(key), std::forward<V>(value));
    return *this;
}

// TODO: generalize this for a map of labels
// template <typename MetricT>
// template <StringLike K, StringLike V>
// metric_builder<MetricT>& metric_builder<MetricT>::label(K&& key, V&& value)
// {
//     _labels.add(std::forward<K>(key), std::forward<V>(value));
//     return *this;
// }

template <typename MetricT>
metric_builder<MetricT>& metric_builder<MetricT>::registry(tc::prometheus::registry& registry)
{
    _registry = &registry;
    return *this;
}

template <typename MetricT>
std::shared_ptr<tc::prometheus::metric_family<MetricT>> metric_builder<MetricT>::build() const
{
    auto family = _registry->create_metric<MetricT>(_name, _help, _labels);
    return family;
}

metric_builder<counter> create_counter(const std::string& name);
metric_builder<gauge> create_gauge(const std::string& name);
metric_builder<histogram> create_histogram(const std::string& name);
metric_builder<summary> create_summary(const std::string& name);

}
