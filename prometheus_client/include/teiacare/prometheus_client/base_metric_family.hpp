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
#include <teiacare/prometheus_client/labels.hpp>
#include <teiacare/prometheus_client/metric_type.hpp>

#include <memory>
#include <string>
#include <vector>

namespace tc::prometheus
{
class base_metric_serializer;

/*!
 * \class base_metric_family
 * \brief Abstract base class for metric families.
 *
 * A metric family is a collection of related metrics that share the same name and type,
 * but differ in their label values. For example, an HTTP request counter might have
 * different instances for different endpoints and methods.
 */
class base_metric_family
{
public:
    virtual ~base_metric_family() = default;

    /*!
     * \brief Serialize all metrics in the family
     * \param serializer The serializer to use for output
     */
    virtual void serialize(tc::prometheus::base_metric_serializer& serializer) const = 0;

    /*!
     * \brief Reset all metrics in the family to their initial state
     */
    virtual void reset() = 0;

    /*!
     * \brief Get the metric family name
     * \return The metric family name
     */
    virtual const std::string& name() const = 0;

    /*!
     * \brief Get the metric family help text
     * \return The help text describing the metric family
     */
    virtual const std::string& help() const = 0;

    /*!
     * \brief Get the metric type as a string
     * \return The metric type (e.g., "counter", "gauge", "histogram", "summary")
     */
    virtual const std::string& type() const = 0;

    /*!
     * \brief Get all metrics in the family
     * \return Vector of shared pointers to all metrics in the family
     */
    virtual const std::vector<std::shared_ptr<tc::prometheus::base_metric>>& metrics() const = 0;
};

}
