#pragma once

#include <teiacare/prometheus_client/base_metric.hpp>
#include <teiacare/prometheus_client/base_metric_serializer.hpp>
#include <teiacare/prometheus_client/labels.hpp>

#include <atomic>
#include <string>

namespace tc::prometheus
{
/**
 * @brief Gauge metric that can increase or decrease.
 *
 * Gauges are used for values that can go up and down (e.g., memory usage, active connections).
 *
 * Thread-safety:
 * - All methods are thread-safe and can be called from multiple threads simultaneously
 * - set(), inc(), dec() use atomic operations for lock-free updates
 * - get() and serialize() are safe to call while updates are being made from other threads
 */
class gauge : public tc::prometheus::base_metric
{
public:
    explicit gauge(std::string name, tc::prometheus::labels labels = {});

    void set(double v) noexcept;
    void inc(double amount = 1.0) noexcept;
    void dec(double amount = 1.0) noexcept;
    double get() const noexcept;
    void reset() override;
    void serialize(tc::prometheus::base_metric_serializer& serializer) const override;

private:
    std::atomic<double> _value{0.0};
};

DEFINE_METRIC_TYPE(tc::prometheus::gauge, "gauge");

}
