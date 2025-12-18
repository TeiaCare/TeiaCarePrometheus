#include <teiacare/prometheus_client/counter.hpp>

#include <stdexcept>

namespace tc::prometheus
{
counter::counter(std::string name, tc::prometheus::labels labels)
    : tc::prometheus::base_metric(name, labels)
{
}

void counter::inc(double amount)
{
    if (amount < 0)
    {
        throw std::invalid_argument("Counter increment must be non-negative");
    }

    _value.fetch_add(amount, std::memory_order_relaxed);
}

double counter::get() const noexcept
{
    return _value.load(std::memory_order_relaxed);
}

void counter::reset()
{
    _value.store(0.0, std::memory_order_relaxed);
}

void counter::serialize(tc::prometheus::base_metric_serializer& serializer) const
{
    serializer.serialize(*this);
}

}
