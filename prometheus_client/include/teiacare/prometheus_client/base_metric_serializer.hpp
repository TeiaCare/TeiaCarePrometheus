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

#include <teiacare/prometheus_client/metric_type.hpp>

#include <memory>
#include <string>
#include <vector>

namespace tc::prometheus
{
class counter;
class gauge;
class histogram;
class summary;
class base_metric;
class base_metric_family;

/*!
 * \class base_metric_serializer
 * \brief Abstract base class for metric serializers.
 *
 * Serializers convert metric data into specific output formats (e.g., Prometheus text format).
 * This class defines the interface for serializing different metric types and metric families.
 */
class base_metric_serializer
{
public:
    virtual ~base_metric_serializer() = default;

    /*!
     * \brief Serialize a metric family
     * \param metric_family The metric family to serialize
     */
    virtual void serialize(const tc::prometheus::base_metric_family& metric_family) = 0;

    /*!
     * \brief Serialize a counter metric
     * \param counter The counter metric to serialize
     */
    virtual void serialize(const tc::prometheus::counter& counter) = 0;

    /*!
     * \brief Serialize a gauge metric
     * \param gauge The gauge metric to serialize
     */
    virtual void serialize(const tc::prometheus::gauge& gauge) = 0;

    /*!
     * \brief Serialize a histogram metric
     * \param histogram The histogram metric to serialize
     */
    virtual void serialize(const tc::prometheus::histogram& histogram) = 0;

    /*!
     * \brief Serialize a summary metric
     * \param summary The summary metric to serialize
     */
    virtual void serialize(const tc::prometheus::summary& summary) = 0;
};

}
