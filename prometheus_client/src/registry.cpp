#include <teiacare/prometheus_client/metric_serializer.hpp>
#include <teiacare/prometheus_client/registry.hpp>

namespace tc::prometheus
{
std::string registry::serialize() const
{
    tc::prometheus::metric_serializer serializer;
    std::scoped_lock lock(_mutex);
    for (auto const& [_, family] : _families)
    {
        family->serialize(serializer);
    }

    std::string out = serializer.str();
    if (!out.ends_with("\n"))
        out.push_back('\n');

    out += "# EOF\n";
    return out;
}

void registry::reset()
{
    std::scoped_lock lock(_mutex);
    for (auto const& [_, family] : _families)
    {
        family->reset();
    }
}

registry& default_registry()
{
    static tc::prometheus::registry instance;
    return instance;
}

}
