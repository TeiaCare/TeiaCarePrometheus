#pragma once

#include <teiacare/prometheus_client/base_metric_serializer.hpp>

#include <memory>
#include <sstream>
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

class metric_serializer : public tc::prometheus::base_metric_serializer
{
public:
    void serialize(const tc::prometheus::base_metric_family& metric_family) override;
    void serialize(const tc::prometheus::counter& counter) override;
    void serialize(const tc::prometheus::gauge& gauge) override;
    void serialize(const tc::prometheus::histogram& histogram) override;
    void serialize(const tc::prometheus::summary& summary) override;

    std::string str() const;

private:
    std::ostringstream _oss;
};

}
