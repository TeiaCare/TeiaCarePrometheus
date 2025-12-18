#include <teiacare/prometheus_client/base_metric.hpp>

namespace tc::prometheus
{
base_metric::base_metric(std::string name, tc::prometheus::labels labels)
    : _name(std::move(name))
    , _labels(std::move(labels))
{
}

const std::string& base_metric::name() const noexcept
{
    return _name;
}

const tc::prometheus::labels& base_metric::labels() const noexcept
{
    return _labels;
}

}
