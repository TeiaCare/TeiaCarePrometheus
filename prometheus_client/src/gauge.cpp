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
