#pragma once

#include <teiacare/prometheus_client/base_metric.hpp>
#include <teiacare/prometheus_client/labels.hpp>
#include <teiacare/prometheus_client/metric_type.hpp>

#include <memory>
#include <string>
#include <vector>

namespace tc::prometheus
{
class base_metric_serializer;

class base_metric_family
{
public:
    virtual ~base_metric_family() = default;
    virtual void serialize(tc::prometheus::base_metric_serializer& serializer) const = 0;
    virtual void reset() = 0;

    virtual const std::string& name() const = 0;
    virtual const std::string& help() const = 0;
    virtual const std::string& type() const = 0;
    virtual const std::vector<std::shared_ptr<tc::prometheus::base_metric>>& metrics() const = 0;
};

}
