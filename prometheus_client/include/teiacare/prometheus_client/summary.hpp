#pragma once

#include <teiacare/prometheus_client/base_metric.hpp>
#include <teiacare/prometheus_client/base_metric_serializer.hpp>
#include <teiacare/prometheus_client/labels.hpp>

#include <atomic>
#include <mutex>
#include <string>
#include <vector>

namespace tc::prometheus
{
class summary : public tc::prometheus::base_metric
{
public:
    explicit summary(std::string name,
                     tc::prometheus::labels labels = {},
                     std::vector<double> quantiles = default_quantiles());

    void observe(double value);
    std::vector<double> sorted_observations() const;
    const std::vector<double>& quantiles_list() const noexcept;
    uint64_t count() const noexcept;
    double sum() const noexcept;
    void reset() override;
    void serialize(tc::prometheus::base_metric_serializer& serializer) const override;

private:
    std::vector<double> _observations;
    std::vector<double> _quantiles;
    mutable std::mutex _mutex;
    std::atomic<uint64_t> _total_count{0};
    std::atomic<double> _total_sum{0.0};

    static std::vector<double> default_quantiles();
};

DEFINE_METRIC_TYPE(tc::prometheus::summary, "summary");

}
