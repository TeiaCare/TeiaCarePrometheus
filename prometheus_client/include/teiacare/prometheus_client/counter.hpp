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
/*!
 * \class counter
 * \brief Counter metric that can only increase.
 *
 * Counters are used for values that only go up (e.g., number of requests, errors).
 * The value can only be incremented and reset to zero.
 *
 * Thread-safety:
 * - All methods are thread-safe and can be called from multiple threads simultaneously
 * - inc() uses atomic operations for lock-free increments
 * - get() and serialize() are safe to call while inc() is being called from other threads
 */
class counter : public tc::prometheus::base_metric
{
public:
    /*!
     * \brief Construct a new counter metric
     * \param name The counter name
     * \param labels Optional labels for the counter
     */
    explicit counter(std::string name, tc::prometheus::labels labels = {});

    /*!
     * \brief Increment the counter by a given amount
     * \param amount The amount to increment (default: 1.0)
     *
     * The amount must be non-negative. The increment is performed atomically.
     */
    void inc(double amount = 1.0);

    /*!
     * \brief Get the current counter value
     * \return The current counter value
     */
    double get() const noexcept;

    /*!
     * \brief Reset the counter to zero
     */
    void reset() override;

    /*!
     * \brief Serialize the counter using the provided serializer
     * \param serializer The serializer to use for output
     */
    void serialize(tc::prometheus::base_metric_serializer& serializer) const override;

private:
    std::atomic<double> _value{0.0};
};

DEFINE_METRIC_TYPE(tc::prometheus::counter, "counter");

}
