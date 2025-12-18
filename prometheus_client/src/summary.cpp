#include <teiacare/prometheus_client/summary.hpp>

#include <algorithm>
#include <cmath>
#include <ranges>

namespace tc::prometheus
{

std::vector<double> summary::default_quantiles()
{
    return {0.5, 0.9, 0.99};
}

summary::summary(std::string name,
                 tc::prometheus::labels labels,
                 std::vector<double> quantiles)
    : tc::prometheus::base_metric(name, labels)
    , _quantiles(std::move(quantiles))
{
    std::ranges::sort(_quantiles);
}

void summary::observe(double value)
{
    if (!std::isfinite(value))
        return;

    {
        std::scoped_lock lock(_mutex);
        _observations.push_back(value);
        if (_observations.size() > 10000)
        {
            _observations.erase(_observations.begin(), _observations.begin() + 5000);
        }
    }

    _total_count.fetch_add(1, std::memory_order_relaxed);
    _total_sum.fetch_add(value, std::memory_order_relaxed);
}

std::vector<double> summary::sorted_observations() const
{
    std::vector<double> temp;
    {
        std::scoped_lock lock(_mutex);
        temp = _observations;
    }

    std::ranges::sort(temp);
    return temp;
}

const std::vector<double>& summary::quantiles_list() const noexcept
{
    return _quantiles;
}

uint64_t summary::count() const noexcept
{
    return _total_count.load(std::memory_order_relaxed);
}

double summary::sum() const noexcept
{
    return _total_sum.load(std::memory_order_relaxed);
}

void summary::reset()
{
    std::scoped_lock lock(_mutex);
    _observations.clear();
    _total_count.store(0, std::memory_order_relaxed);
    _total_sum.store(0.0, std::memory_order_relaxed);
}

void summary::serialize(tc::prometheus::base_metric_serializer& serializer) const
{
    serializer.serialize(*this);
}

}
