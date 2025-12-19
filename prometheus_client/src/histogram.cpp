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

#include <teiacare/prometheus_client/histogram.hpp>

#include <algorithm>
#include <cmath>
#include <limits>
#include <ranges>

namespace tc::prometheus
{
std::vector<double> histogram::default_buckets()
{
    static const std::vector<double> defaults = {
        0.005, 0.01, 0.025, 0.05, 0.075,
        0.1, 0.25, 0.5, 0.75, 1.0,
        2.5, 5.0, 7.5, 10.0,
        std::numeric_limits<double>::infinity()};
    return defaults;
}

histogram::histogram(std::string name,
                     tc::prometheus::labels labels,
                     std::vector<double> bounds)
    : tc::prometheus::base_metric(name, labels)
{
    std::ranges::sort(bounds);
    if (bounds.empty() || bounds.back() != std::numeric_limits<double>::infinity())
    {
        bounds.push_back(std::numeric_limits<double>::infinity());
    }
    _bucket_bounds = std::move(bounds);

    _bucket_counts = std::vector<std::atomic<uint64_t>>(_bucket_bounds.size());
}

void histogram::observe(double value)
{
    if (!std::isfinite(value))
        return;

    _total_count.fetch_add(1, std::memory_order_relaxed);
    _total_sum.fetch_add(value, std::memory_order_relaxed);
    for (size_t i = 0; i < _bucket_bounds.size(); ++i)
    {
        if (value <= _bucket_bounds[i])
        {
            _bucket_counts[i].fetch_add(1, std::memory_order_relaxed);
        }
    }
}

const std::vector<double>& histogram::bounds() const noexcept
{
    return _bucket_bounds;
}

const std::vector<std::atomic<uint64_t>>& histogram::counts() const noexcept
{
    return _bucket_counts;
}

uint64_t histogram::count() const noexcept
{
    return _total_count.load(std::memory_order_relaxed);
}

double histogram::sum() const noexcept
{
    return _total_sum.load(std::memory_order_relaxed);
}

void histogram::reset()
{
    _total_count.store(0, std::memory_order_relaxed);
    _total_sum.store(0.0, std::memory_order_relaxed);
    for (auto& c : _bucket_counts)
    {
        c.store(0, std::memory_order_relaxed);
    }
}

void histogram::serialize(tc::prometheus::base_metric_serializer& serializer) const
{
    serializer.serialize(*this);
}

}
