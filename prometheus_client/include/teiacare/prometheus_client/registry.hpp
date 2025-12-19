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
class registry
{
public:
    std::shared_ptr<tc::prometheus::metric_family<tc::prometheus::counter>> create_counter(const std::string& name, const std::string& help);
    std::shared_ptr<tc::prometheus::metric_family<tc::prometheus::gauge>> create_gauge(const std::string& name, const std::string& help);
    std::shared_ptr<tc::prometheus::metric_family<tc::prometheus::histogram>> create_histogram(const std::string& name, const std::string& help, const std::vector<double>& buckets);
    std::shared_ptr<tc::prometheus::metric_family<tc::prometheus::summary>> create_summary(const std::string& name, const std::string& help, const std::vector<double>& quantiles);
    std::string serialize() const;
    void reset();

private:
    mutable std::mutex _mutex;
    std::unordered_map<std::string, std::shared_ptr<tc::prometheus::base_metric_family>> _families;
    void check_metric_family_exists(const std::string& name) const;
};

tc::prometheus::registry& default_registry();

}
