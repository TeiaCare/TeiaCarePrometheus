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

#include <teiacare/prometheus_client/base_metric_family.hpp>
#include <teiacare/prometheus_client/metric_family.hpp>

#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace tc::prometheus
{
/*!
 * \class registry
 * \brief Central registry for managing metric families.
 *
 * The registry stores all metric families and provides a central point for creating
 * and managing metrics. It ensures that metric family names are unique and provides
 * serialization of all registered metrics.
 *
 * Thread-safety:
 * - All methods are thread-safe and can be called from multiple threads simultaneously
 * - Metric families can be created or accessed concurrently
 */
class registry
{
public:
    /*!
     * \brief Create a counter metric family
     * \param name The metric family name
     * \param help Help text describing the metric family
     * \return Shared pointer to the newly created counter family
     *
     * Throws std::runtime_error if a metric family with the same name already exists.
     */
    std::shared_ptr<tc::prometheus::metric_family<tc::prometheus::counter>> create_counter(const std::string& name, const std::string& help);

    /*!
     * \brief Create a gauge metric family
     * \param name The metric family name
     * \param help Help text describing the metric family
     * \return Shared pointer to the newly created gauge family
     *
     * Throws std::runtime_error if a metric family with the same name already exists.
     */
    std::shared_ptr<tc::prometheus::metric_family<tc::prometheus::gauge>> create_gauge(const std::string& name, const std::string& help);

    /*!
     * \brief Create a histogram metric family
     * \param name The metric family name
     * \param help Help text describing the metric family
     * \param buckets The bucket boundaries for all histograms in this family
     * \return Shared pointer to the newly created histogram family
     *
     * Throws std::runtime_error if a metric family with the same name already exists.
     */
    std::shared_ptr<tc::prometheus::metric_family<tc::prometheus::histogram>> create_histogram(const std::string& name, const std::string& help, const std::vector<double>& buckets);

    /*!
     * \brief Create a summary metric family
     * \param name The metric family name
     * \param help Help text describing the metric family
     * \param quantiles The quantiles to calculate for all summaries in this family
     * \return Shared pointer to the newly created summary family
     *
     * Throws std::runtime_error if a metric family with the same name already exists.
     */
    std::shared_ptr<tc::prometheus::metric_family<tc::prometheus::summary>> create_summary(const std::string& name, const std::string& help, const std::vector<double>& quantiles);

    /*!
     * \brief Serialize all registered metrics
     * \return String containing all metrics in Prometheus text format
     *
     * This method serializes all metric families and their metrics into a format
     * suitable for consumption by Prometheus servers.
     */
    std::string serialize() const;

    /*!
     * \brief Reset all registered metrics to their initial state
     *
     * This method resets all metrics in all metric families.
     */
    void reset();

private:
    mutable std::mutex _mutex;
    std::unordered_map<std::string, std::shared_ptr<tc::prometheus::base_metric_family>> _families;
    void check_metric_family_exists(const std::string& name) const;
};

/*!
 * \brief Get the global default registry instance
 * \return Reference to the default registry
 *
 * Returns a singleton registry instance that can be used globally.
 */
tc::prometheus::registry& default_registry();

}
