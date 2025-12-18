#pragma once

#include <teiacare/prometheus_client/base_metric_serializer.hpp>
#include <teiacare/prometheus_client/labels.hpp>
#include <teiacare/prometheus_client/metric_type.hpp>

#include <string>

namespace tc::prometheus
{
class base_metric
{
public:
    explicit base_metric(std::string name, tc::prometheus::labels labels);
    virtual ~base_metric() = default;

    virtual void serialize(tc::prometheus::base_metric_serializer& serializer) const = 0;
    virtual void reset() = 0;

    const std::string& name() const noexcept;
    const tc::prometheus::labels& labels() const noexcept;

protected:
    const std::string _name;
    const tc::prometheus::labels _labels;
};

template <typename T>
concept MetricTraits = std::is_base_of_v<tc::prometheus::base_metric, T>;

}
