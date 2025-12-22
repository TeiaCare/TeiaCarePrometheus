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

#include <teiacare/prometheus_client/base_metric_serializer.hpp>
#include <teiacare/prometheus_client/labels.hpp>
#include <teiacare/prometheus_client/metric_type.hpp>

#include <string>

namespace tc::prometheus
{
/*!
 * \class base_metric
 * \brief Abstract base class for all Prometheus metric types.
 *
 * This class provides the common interface and data members for all metric types
 * (counter, gauge, histogram, summary). Each metric has a name and optional labels
 * that uniquely identify it.
 */
class base_metric
{
public:
    /*!
     * \brief Construct a new base metric
     * \param name The metric name
     * \param labels Optional labels for the metric
     *
     * The metric name should follow Prometheus naming conventions (alphanumeric with underscores).
     */
    explicit base_metric(std::string name, tc::prometheus::labels labels);

    virtual ~base_metric() = default;

    /*!
     * \brief Serialize the metric using the provided serializer
     * \param serializer The serializer to use for output
     *
     * This method is implemented by derived classes to serialize their specific metric data.
     */
    virtual void serialize(tc::prometheus::base_metric_serializer& serializer) const = 0;

    /*!
     * \brief Reset the metric to its initial state
     *
     * This method is implemented by derived classes to reset their specific metric values.
     */
    virtual void reset() = 0;

    /*!
     * \brief Get the metric name
     * \return The metric name
     */
    const std::string& name() const noexcept;

    /*!
     * \brief Get the metric labels
     * \return The metric labels
     */
    const tc::prometheus::labels& labels() const noexcept;

protected:
    const std::string _name;
    const tc::prometheus::labels _labels;
};

}
