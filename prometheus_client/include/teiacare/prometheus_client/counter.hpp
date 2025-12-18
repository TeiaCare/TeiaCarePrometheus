#pragma once

#include <teiacare/prometheus_client/base_metric.hpp>
#include <teiacare/prometheus_client/base_metric_serializer.hpp>
#include <teiacare/prometheus_client/labels.hpp>

#include <atomic>
#include <string>

namespace tc::prometheus
{
/**
 * @brief Counter metric that can only increase.
 *
 * Counters are used for values that only go up (e.g., number of requests, errors).
 *
 * Thread-safety:
 * - All methods are thread-safe and can be called from multiple threads simultaneously
 * - inc() uses atomic operations for lock-free increments
 * - get() and serialize() are safe to call while inc() is being called from other threads
 */
class counter : public tc::prometheus::base_metric
{
public:
    explicit counter(std::string name, tc::prometheus::labels labels = {});

    void inc(double amount = 1.0);
    double get() const noexcept;
    void reset() override;
    void serialize(tc::prometheus::base_metric_serializer& serializer) const override;

private:
    std::atomic<double> _value{0.0};
};

DEFINE_METRIC_TYPE(tc::prometheus::counter, "counter");

}
