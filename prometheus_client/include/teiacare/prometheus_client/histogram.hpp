#pragma once

#include <teiacare/prometheus_client/base_metric.hpp>
#include <teiacare/prometheus_client/base_metric_serializer.hpp>
#include <teiacare/prometheus_client/labels.hpp>

#include <atomic>
#include <string>
#include <vector>

namespace tc::prometheus
{
/**
 * @brief Histogram metric for sampling observations into buckets.
 *
 * Histograms are used to track distributions of values (e.g., request durations, response sizes).
 * Each observation is counted into configured buckets, allowing calculation of quantiles.
 *
 * Thread-safety:
 * - All methods are thread-safe and can be called from multiple threads simultaneously
 * - observe() uses atomic operations for lock-free updates
 * - Serialization is safe to call concurrently with observations
 */
class histogram : public tc::prometheus::base_metric
{
public:
    explicit histogram(std::string name,
                       tc::prometheus::labels labels = {},
                       std::vector<double> bounds = default_buckets());

    void observe(double value);
    const std::vector<double>& bounds() const noexcept;
    const std::vector<std::atomic<uint64_t>>& counts() const noexcept;
    uint64_t count() const noexcept;
    double sum() const noexcept;
    void reset() override;
    void serialize(tc::prometheus::base_metric_serializer& serializer) const override;

private:
    std::vector<double> _bucket_bounds;
    std::vector<std::atomic<uint64_t>> _bucket_counts;
    std::atomic<uint64_t> _total_count{0};
    std::atomic<double> _total_sum{0.0};

    static std::vector<double> default_buckets();
};

DEFINE_METRIC_TYPE(tc::prometheus::histogram, "histogram");

}
