#include <teiacare/prometheus_client/counter.hpp>
#include <teiacare/prometheus_client/gauge.hpp>
#include <teiacare/prometheus_client/metric_family.hpp>
#include <teiacare/prometheus_client/registry.hpp>

#include <chrono>
#include <iostream>
#include <thread>

int main(int, char**)
{
    using namespace tc::prometheus;

    std::cout << "=== Prometheus Counter Example ===" << std::endl;

    // Create a registry to manage all metrics
    auto registry = std::make_shared<tc::prometheus::registry>();

    // 1. Create counter metric family for HTTP requests
    // This simulates an HTTP server tracking requests
    auto http_requests_family = registry->create_counter(
        "http_requests_total",
        "Total number of HTTP requests received");

    // Create individual counter metrics with different labels
    auto http_requests_get = http_requests_family->add_metric({{"method", "GET"},
                                                               {"endpoint", "/api/users"},
                                                               {"status", "200"}});

    auto http_requests_post = http_requests_family->add_metric({{"method", "POST"},
                                                                {"endpoint", "/api/users"},
                                                                {"status", "201"}});

    auto http_requests_error = http_requests_family->add_metric({{"method", "GET"},
                                                                 {"endpoint", "/api/users"},
                                                                 {"status", "500"}});

    // 2. Create additional metrics to show a complete monitoring setup
    auto database_queries_family = registry->create_counter(
        "database_queries_total",
        "Total number of database queries executed");

    auto database_queries = database_queries_family->add_metric({{"operation", "SELECT"},
                                                                 {"table", "users"}});

    auto cache_hits_family = registry->create_counter(
        "cache_hits_total",
        "Total number of cache hits");

    auto cache_hits = cache_hits_family->add_metric({{"cache_type", "redis"}});

    auto cache_misses_family = registry->create_counter(
        "cache_misses_total",
        "Total number of cache misses");

    auto cache_misses = cache_misses_family->add_metric({{"cache_type", "redis"}});

    // 3. Also create some gauge metrics to show different metric types
    auto active_connections_family = registry->create_gauge(
        "active_connections",
        "Number of currently active connections");

    auto active_connections = active_connections_family->add_metric({});

    std::cout << "Created all metric families and metrics" << std::endl;
    std::cout << std::endl;
    std::cout << "Simulating application traffic..." << std::endl;
    for (int i = 0; i < 100; ++i)
    {
        // Simulate HTTP requests
        if (i % 10 == 0)
        {
            // Simulate error every 10th request
            http_requests_error->inc();
            std::cout << "Handled error request " << (i / 10 + 1) << std::endl;
        }
        else if (i % 3 == 0)
        {
            // Simulate POST requests
            http_requests_post->inc();
            database_queries->inc(2.0); // POST usually involves more DB operations
            std::cout << "Handled POST request " << (i / 3 + 1) << std::endl;
        }
        else
        {
            // Simulate successful GET requests
            http_requests_get->inc();
            database_queries->inc();

            // Simulate cache behavior
            if (i % 4 == 0)
            {
                cache_misses->inc();
                database_queries->inc(); // Cache miss means extra DB query
            }
            else
            {
                cache_hits->inc();
            }

            std::cout << "Handled GET request " << (i + 1) << std::endl;
        }

        // Update gauge metric to simulate varying active connections
        active_connections->set(10 + (i % 20));

        // Small delay to make it more realistic
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    std::cout << std::endl
              << "=== Final Metrics Report ===" << std::endl;

    // 4. Show individual metric values
    std::cout << "HTTP GET 200 requests: " << http_requests_get->get() << std::endl;
    std::cout << "HTTP POST 201 requests: " << http_requests_post->get() << std::endl;
    std::cout << "HTTP GET 500 errors: " << http_requests_error->get() << std::endl;
    std::cout << "Total database queries: " << database_queries->get() << std::endl;
    std::cout << "Cache hits: " << cache_hits->get() << std::endl;
    std::cout << "Cache misses: " << cache_misses->get() << std::endl;
    std::cout << "Active connections: " << active_connections->get() << std::endl;

    std::cout << std::endl
              << "=== Prometheus Exposition Format ===" << std::endl;
    std::cout << "This is the format that Prometheus server would scrape:" << std::endl;
    std::cout << std::endl;

    // 5. Serialize all metrics in Prometheus exposition format
    // This demonstrates how metric families group related metrics together
    std::string prometheus_output = registry->serialize();
    std::cout << prometheus_output << std::endl;

    std::cout << "=== Analysis ===" << std::endl;
    std::cout << "Notice how:" << std::endl;
    std::cout << "1. All HTTP request counters are grouped under 'http_requests_total' family" << std::endl;
    std::cout << "2. Different label combinations create separate time series" << std::endl;
    std::cout << "3. Counters only increase - they track cumulative values" << std::endl;
    std::cout << "4. Labels allow filtering and aggregation in Prometheus queries" << std::endl;
    std::cout << "5. Metric families ensure type consistency across related metrics" << std::endl;

    return 0;
}
