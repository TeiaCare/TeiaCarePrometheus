#pragma once

#include <teiacare/prometheus_client/metric_type.hpp>

#include <memory>
#include <string>
#include <vector>

namespace tc::prometheus
{
class counter;
class gauge;
class histogram;
class summary;
class base_metric;
class base_metric_family;

class base_metric_serializer
{
public:
    virtual ~base_metric_serializer() = default;

    virtual void serialize(const tc::prometheus::base_metric_family& metric_family) = 0;
    virtual void serialize(const tc::prometheus::counter& counter) = 0;
    virtual void serialize(const tc::prometheus::gauge& gauge) = 0;
    virtual void serialize(const tc::prometheus::histogram& histogram) = 0;
    virtual void serialize(const tc::prometheus::summary& summary) = 0;
};

}
