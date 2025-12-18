#pragma once

#include <teiacare/prometheus_client/base_metric.hpp>
#include <teiacare/prometheus_client/base_metric_serializer.hpp>
#include <teiacare/prometheus_client/labels.hpp>

#include <atomic>
#include <string>

namespace tc::prometheus
{
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
