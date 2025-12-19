// Copyright 2025 TeiaCare
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
                 std::vector<double> quantiles,
                 size_t max_observations)
    : tc::prometheus::base_metric(name, labels)
    , _quantiles(std::move(quantiles))
    , _max_observations(max_observations)
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
        if (_observations.size() > _max_observations)
        {
            // Remove oldest half of observations when limit is reached
            _observations.erase(_observations.begin(), _observations.begin() + (_max_observations / 2));
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
