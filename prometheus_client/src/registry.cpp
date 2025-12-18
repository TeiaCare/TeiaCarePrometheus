#include <teiacare/prometheus_client/counter.hpp>
#include <teiacare/prometheus_client/gauge.hpp>
#include <teiacare/prometheus_client/histogram.hpp>
#include <teiacare/prometheus_client/metric_serializer.hpp>
#include <teiacare/prometheus_client/registry.hpp>
#include <teiacare/prometheus_client/summary.hpp>

namespace tc::prometheus
{
std::shared_ptr<tc::prometheus::metric_family<tc::prometheus::counter>> registry::create_counter(const std::string& name, const std::string& help)
{
    check_metric_family_exists(name);
    auto family = std::make_shared<tc::prometheus::metric_family<tc::prometheus::counter>>(name, help);
    _families.emplace(name, family);
    return family;
}

std::shared_ptr<tc::prometheus::metric_family<tc::prometheus::gauge>> registry::create_gauge(const std::string& name, const std::string& help)
{
    check_metric_family_exists(name);
    auto family = std::make_shared<tc::prometheus::metric_family<tc::prometheus::gauge>>(name, help);
    _families.emplace(name, family);
    return family;
}

std::shared_ptr<tc::prometheus::metric_family<tc::prometheus::histogram>> registry::create_histogram(const std::string& name, const std::string& help, [[maybe_unused]] const std::vector<double>& buckets)
{
    check_metric_family_exists(name);
    auto family = std::make_shared<tc::prometheus::metric_family<tc::prometheus::histogram>>(name, help);
    _families.emplace(name, family);
    return family;
}

std::shared_ptr<tc::prometheus::metric_family<tc::prometheus::summary>> registry::create_summary(const std::string& name, const std::string& help, [[maybe_unused]] const std::vector<double>& quantiles)
{
    check_metric_family_exists(name);
    auto family = std::make_shared<tc::prometheus::metric_family<tc::prometheus::summary>>(name, help);
    _families.emplace(name, family);
    return family;
}

void registry::check_metric_family_exists(const std::string& name) const
{
    std::scoped_lock lock(_mutex);
    if (_families.contains(name))
    {
        throw std::runtime_error("Metric family '" + name + "' already exists");
    }
}

std::string registry::serialize() const
{
    tc::prometheus::metric_serializer serializer;
    std::scoped_lock lock(_mutex);
    for (auto const& [_, family] : _families)
    {
        family->serialize(serializer);
    }

    std::string out = serializer.str();
    if (!out.ends_with("\n"))
        out.push_back('\n');

    out += "# EOF\n";
    return out;
}

void registry::reset()
{
    std::scoped_lock lock(_mutex);
    for (auto const& [_, family] : _families)
    {
        family->reset();
    }
}

registry& default_registry()
{
    static tc::prometheus::registry instance;
    return instance;
}

}
