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

#include <memory>
#include <sstream>
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
 * \class metric_serializer
 * \brief Concrete implementation of base_metric_serializer for Prometheus text format.
 *
 * This class serializes metrics and metric families into the Prometheus text exposition format.
 * The output can be used directly by Prometheus servers for scraping.
 *
 * Thread-safety:
 * - This class is not thread-safe. Create separate instances for concurrent serialization.
 */
class metric_serializer : public tc::prometheus::base_metric_serializer
{
public:
    /*!
     * \brief Serialize a metric family
     * \param metric_family The metric family to serialize
     *
     * Outputs the metric family name, type, help text, and all metrics within it.
     */
    void serialize(const tc::prometheus::base_metric_family& metric_family) override;

    /*!
     * \brief Serialize a counter metric
     * \param counter The counter metric to serialize
     */
    void serialize(const tc::prometheus::counter& counter) override;

    /*!
     * \brief Serialize a gauge metric
     * \param gauge The gauge metric to serialize
     */
    void serialize(const tc::prometheus::gauge& gauge) override;

    /*!
     * \brief Serialize a histogram metric
     * \param histogram The histogram metric to serialize
     *
     * Outputs all bucket counts, total sum, and total count.
     */
    void serialize(const tc::prometheus::histogram& histogram) override;

    /*!
     * \brief Serialize a summary metric
     * \param summary The summary metric to serialize
     *
     * Outputs all quantile values, total sum, and total count.
     */
    void serialize(const tc::prometheus::summary& summary) override;

    /*!
     * \brief Get the serialized output as a string
     * \return String containing all serialized metrics in Prometheus text format
     */
    std::string str() const;

private:
    std::ostringstream _oss;
};

}
