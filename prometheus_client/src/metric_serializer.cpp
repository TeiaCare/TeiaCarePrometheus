#include <teiacare/prometheus_client/base_metric_family.hpp>
#include <teiacare/prometheus_client/counter.hpp>
#include <teiacare/prometheus_client/gauge.hpp>
#include <teiacare/prometheus_client/histogram.hpp>
#include <teiacare/prometheus_client/metric_serializer.hpp>
#include <teiacare/prometheus_client/summary.hpp>

namespace tc::prometheus
{
void metric_serializer::serialize(const tc::prometheus::base_metric_family& metric_family)
{
    _oss << "# HELP " << metric_family.name() << " " << metric_family.help() << "\n";
    _oss << "# TYPE " << metric_family.name() << " " << metric_family.type() << "\n";

    for (const auto& metric : metric_family.metrics())
    {
        metric->serialize(*this);
    }
}

void metric_serializer::serialize(const tc::prometheus::counter& counter)
{
    _oss << counter.name() << counter.labels().to_string() << " " << counter.get() << "\n";
}

void metric_serializer::serialize(const tc::prometheus::gauge& gauge)
{
    _oss << gauge.name() << gauge.labels().to_string() << " " << gauge.get() << "\n";
}

void metric_serializer::serialize(const tc::prometheus::histogram& histogram)
{
    const auto& bounds = histogram.bounds();
    const auto& counts = histogram.counts();
    for (size_t i = 0; i < bounds.size(); ++i)
    {
        tc::prometheus::labels lb = histogram.labels();
        if (bounds[i] == std::numeric_limits<double>::infinity())
        {
            lb.add("le", "+Inf");
        }
        else
        {
            lb.add("le", std::to_string(bounds[i]));
        }
        _oss << histogram.name() << "_bucket" << lb.to_string() << " " << counts[i].load(std::memory_order_relaxed) << "\n";
    }

    _oss << histogram.name() << "_sum" << histogram.labels().to_string() << " " << histogram.sum() << "\n";
    _oss << histogram.name() << "_count" << histogram.labels().to_string() << " " << histogram.count() << "\n";
}

void metric_serializer::serialize(const tc::prometheus::summary& summary)
{
    auto sorted = summary.sorted_observations();
    if (!sorted.empty())
    {
        const auto& quantiles = summary.quantiles_list();
        for (double quantile : quantiles)
        {
            size_t idx = static_cast<size_t>(quantile * (sorted.size() - 1));
            idx = std::min(idx, sorted.size() - 1);
            tc::prometheus::labels lb = summary.labels();
            lb.add("quantile", std::to_string(quantile));
            _oss << summary.name() << lb.to_string() << " " << sorted[idx] << "\n";
        }
    }

    _oss << summary.name() << "_sum" << summary.labels().to_string() << " " << summary.sum() << "\n";
    _oss << summary.name() << "_count" << summary.labels().to_string() << " " << summary.count() << "\n";
}

std::string metric_serializer::str() const
{
    return _oss.str();
}

}
