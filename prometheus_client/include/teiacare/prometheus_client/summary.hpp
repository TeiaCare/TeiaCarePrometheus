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

#pragma once

#include <teiacare/prometheus_client/base_metric.hpp>
#include <teiacare/prometheus_client/base_metric_serializer.hpp>
#include <teiacare/prometheus_client/labels.hpp>

#include <atomic>
#include <mutex>
#include <string>
#include <vector>

namespace tc::prometheus
{
/**
 * @brief Summary metric for calculating quantiles over a sliding window.
 *
 * Summaries track observations and calculate quantiles (e.g., median, 95th percentile).
 * Unlike histograms, quantiles are calculated on the client side.
 *
 * Thread-safety:
 * - All methods are thread-safe and can be called from multiple threads simultaneously
 * - observe() uses mutex protection for the observation list
 * - Observations are automatically pruned when exceeding max_observations (default 10000)
 *
 * Note: For high-throughput scenarios, consider using histogram instead as it's more efficient.
 */
class summary : public tc::prometheus::base_metric
{
public:
    explicit summary(std::string name,
                     tc::prometheus::labels labels = {},
                     std::vector<double> quantiles = default_quantiles(),
                     size_t max_observations = 10000);

    void observe(double value);
    std::vector<double> sorted_observations() const;
    const std::vector<double>& quantiles_list() const noexcept;
    uint64_t count() const noexcept;
    double sum() const noexcept;
    void reset() override;
    void serialize(tc::prometheus::base_metric_serializer& serializer) const override;

private:
    std::vector<double> _observations;
    std::vector<double> _quantiles;
    size_t _max_observations;
    mutable std::mutex _mutex;
    std::atomic<uint64_t> _total_count{0};
    std::atomic<double> _total_sum{0.0};

    static std::vector<double> default_quantiles();
};

DEFINE_METRIC_TYPE(tc::prometheus::summary, "summary");

}
