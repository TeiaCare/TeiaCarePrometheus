#pragma once

#include <teiacare/prometheus_client/base_metric_family.hpp>
#include <teiacare/prometheus_client/metric_family.hpp>

#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace tc::prometheus
{
class registry
{
public:
    template <typename MetricT>
    std::shared_ptr<tc::prometheus::metric_family<MetricT>> create_metric(const std::string& name, const std::string& help);

    std::string serialize() const;
    void reset();

private:
    mutable std::mutex _mutex;
    std::unordered_map<std::string, std::shared_ptr<tc::prometheus::base_metric_family>> _families;
};

template <typename MetricT>
std::shared_ptr<tc::prometheus::metric_family<MetricT>> registry::create_metric(const std::string& name, const std::string& help)
{
    std::scoped_lock lock(_mutex);
    if (_families.contains(name))
    {
        throw std::runtime_error("Metric family '" + name + "' already exists");
    }

    auto family = std::make_shared<tc::prometheus::metric_family<MetricT>>(name, help);
    _families.emplace(name, family);
    return family;
}

tc::prometheus::registry& default_registry();

}
