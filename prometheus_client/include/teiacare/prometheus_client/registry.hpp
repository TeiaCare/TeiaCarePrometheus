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
    std::shared_ptr<tc::prometheus::metric_family<tc::prometheus::counter>> create_counter(const std::string& name, const std::string& help);
    std::shared_ptr<tc::prometheus::metric_family<tc::prometheus::gauge>> create_gauge(const std::string& name, const std::string& help);
    std::shared_ptr<tc::prometheus::metric_family<tc::prometheus::histogram>> create_histogram(const std::string& name, const std::string& help, const std::vector<double>& buckets);
    std::shared_ptr<tc::prometheus::metric_family<tc::prometheus::summary>> create_summary(const std::string& name, const std::string& help, const std::vector<double>& quantiles);
    std::string serialize() const;
    void reset();

private:
    mutable std::mutex _mutex;
    std::unordered_map<std::string, std::shared_ptr<tc::prometheus::base_metric_family>> _families;
    void check_metric_family_exists(const std::string& name) const;
};

tc::prometheus::registry& default_registry();

}
