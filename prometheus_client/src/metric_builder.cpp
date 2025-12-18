#include <teiacare/prometheus_client/counter.hpp>
#include <teiacare/prometheus_client/gauge.hpp>
#include <teiacare/prometheus_client/histogram.hpp>
#include <teiacare/prometheus_client/metric_builder.hpp>
#include <teiacare/prometheus_client/summary.hpp>

namespace tc::prometheus
{
metric_builder<counter> create_counter(const std::string& name)
{
    return metric_builder<counter>(name);
}

metric_builder<gauge> create_gauge(const std::string& name)
{
    return metric_builder<gauge>(name);
}

metric_builder<histogram> create_histogram(const std::string& name)
{
    return metric_builder<histogram>(name);
}

metric_builder<summary> create_summary(const std::string& name)
{
    return metric_builder<summary>(name);
}

}
