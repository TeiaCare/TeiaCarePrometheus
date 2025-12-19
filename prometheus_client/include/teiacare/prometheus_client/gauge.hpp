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
#include <string>

namespace tc::prometheus
{
/**
 * @brief Gauge metric that can increase or decrease.
 *
 * Gauges are used for values that can go up and down (e.g., memory usage, active connections).
 *
 * Thread-safety:
 * - All methods are thread-safe and can be called from multiple threads simultaneously
 * - set(), inc(), dec() use atomic operations for lock-free updates
 * - get() and serialize() are safe to call while updates are being made from other threads
 */
class gauge : public tc::prometheus::base_metric
{
public:
    explicit gauge(std::string name, tc::prometheus::labels labels = {});

    void set(double v) noexcept;
    void inc(double amount = 1.0) noexcept;
    void dec(double amount = 1.0) noexcept;
    double get() const noexcept;
    void reset() override;
    void serialize(tc::prometheus::base_metric_serializer& serializer) const override;

private:
    std::atomic<double> _value{0.0};
};

DEFINE_METRIC_TYPE(tc::prometheus::gauge, "gauge");

}
