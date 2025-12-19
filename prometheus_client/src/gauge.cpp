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

#include <teiacare/prometheus_client/gauge.hpp>

namespace tc::prometheus
{
gauge::gauge(std::string name, tc::prometheus::labels labels)
    : tc::prometheus::base_metric(name, labels)
{
}

void gauge::set(double v) noexcept
{
    _value.store(v, std::memory_order_relaxed);
}

void gauge::inc(double amount) noexcept
{
    _value.fetch_add(amount, std::memory_order_relaxed);
}

void gauge::dec(double amount) noexcept
{
    _value.fetch_sub(amount, std::memory_order_relaxed);
}

double gauge::get() const noexcept
{
    return _value.load(std::memory_order_relaxed);
}

void gauge::reset()
{
    _value.store(0.0, std::memory_order_relaxed);
}

void gauge::serialize(tc::prometheus::base_metric_serializer& serializer) const
{
    serializer.serialize(*this);
}

}
