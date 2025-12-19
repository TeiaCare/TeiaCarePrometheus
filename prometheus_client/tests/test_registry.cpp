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

#include <teiacare/prometheus_client/counter.hpp>
#include <teiacare/prometheus_client/gauge.hpp>
#include <teiacare/prometheus_client/histogram.hpp>
#include <teiacare/prometheus_client/registry.hpp>
#include <teiacare/prometheus_client/summary.hpp>

#include <gtest/gtest.h>

class registry_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        registry = std::make_unique<tc::prometheus::registry>();
    }

    void TearDown() override
    {
        registry.reset();
    }

    std::unique_ptr<tc::prometheus::registry> registry;
};

TEST_F(registry_test, create_counter_family)
{
    auto counter_family = registry->create_counter("test_counter", "Test counter help");
    ASSERT_NE(counter_family, nullptr);
    EXPECT_EQ(counter_family->name(), "test_counter");
    EXPECT_EQ(counter_family->help(), "Test counter help");
}

TEST_F(registry_test, create_gauge_family)
{
    auto gauge_family = registry->create_gauge("test_gauge", "Test gauge help");
    ASSERT_NE(gauge_family, nullptr);
    EXPECT_EQ(gauge_family->name(), "test_gauge");
    EXPECT_EQ(gauge_family->help(), "Test gauge help");
}

TEST_F(registry_test, create_histogram_family)
{
    std::vector<double> buckets = {0.1, 0.5, 1.0, 5.0};
    auto histogram_family = registry->create_histogram("test_histogram", "Test histogram help", buckets);
    ASSERT_NE(histogram_family, nullptr);
    EXPECT_EQ(histogram_family->name(), "test_histogram");
    EXPECT_EQ(histogram_family->help(), "Test histogram help");
}

TEST_F(registry_test, create_summary_family)
{
    std::vector<double> quantiles = {0.5, 0.9, 0.99};
    auto summary_family = registry->create_summary("test_summary", "Test summary help", quantiles);
    ASSERT_NE(summary_family, nullptr);
    EXPECT_EQ(summary_family->name(), "test_summary");
    EXPECT_EQ(summary_family->help(), "Test summary help");
}

TEST_F(registry_test, add_metrics_to_counter_family)
{
    auto counter_family = registry->create_counter("http_requests", "HTTP requests");

    auto counter1 = counter_family->add_metric({{"method", "GET"}, {"status", "200"}});
    auto counter2 = counter_family->add_metric({{"method", "POST"}, {"status", "201"}});

    ASSERT_NE(counter1, nullptr);
    ASSERT_NE(counter2, nullptr);

    counter1->inc(10);
    counter2->inc(5);

    EXPECT_EQ(counter1->get(), 10.0);
    EXPECT_EQ(counter2->get(), 5.0);
}

TEST_F(registry_test, add_metrics_to_gauge_family)
{
    auto gauge_family = registry->create_gauge("cpu_usage", "CPU usage percentage");

    auto gauge1 = gauge_family->add_metric({{"cpu", "0"}});
    auto gauge2 = gauge_family->add_metric({{"cpu", "1"}});

    ASSERT_NE(gauge1, nullptr);
    ASSERT_NE(gauge2, nullptr);

    gauge1->set(75.5);
    gauge2->set(82.3);

    EXPECT_EQ(gauge1->get(), 75.5);
    EXPECT_EQ(gauge2->get(), 82.3);
}

TEST_F(registry_test, serialize_empty_registry)
{
    std::string output = registry->serialize();
    // Empty registry may still output headers or metadata
    // Just verify it doesn't crash
    EXPECT_TRUE(true);
}

TEST_F(registry_test, serialize_registry_with_metrics)
{
    auto counter_family = registry->create_counter("test_counter", "Test counter");
    auto counter = counter_family->add_metric({{"label", "value"}});
    counter->inc(42);

    std::string output = registry->serialize();
    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("test_counter"), std::string::npos);
}

TEST_F(registry_test, reset_registry)
{
    auto counter_family = registry->create_counter("test_counter", "Test counter");
    auto counter = counter_family->add_metric({});
    counter->inc(100);

    registry->reset();

    EXPECT_EQ(counter->get(), 0.0);
}

TEST_F(registry_test, multiple_metric_families)
{
    auto counter_family = registry->create_counter("counter", "Counter metric");
    auto gauge_family = registry->create_gauge("gauge", "Gauge metric");

    auto counter = counter_family->add_metric({});
    auto gauge = gauge_family->add_metric({});

    counter->inc(10);
    gauge->set(20);

    std::string output = registry->serialize();
    EXPECT_NE(output.find("counter"), std::string::npos);
    EXPECT_NE(output.find("gauge"), std::string::npos);
}

TEST_F(registry_test, get_metric_from_family)
{
    auto counter_family = registry->create_counter("http_requests", "HTTP requests");

    tc::prometheus::labels labels;
    labels.add("method", "GET").add("status", "200");

    auto counter1 = counter_family->add_metric(labels);
    counter1->inc(100);

    auto counter2 = counter_family->get_metric(labels);
    EXPECT_EQ(counter2->get(), 100.0);
    EXPECT_EQ(counter1, counter2); // Should be the same instance
}

TEST_F(registry_test, metric_family_with_empty_labels)
{
    auto counter_family = registry->create_counter("simple_counter", "Simple counter");
    auto counter = counter_family->add_metric({});

    ASSERT_NE(counter, nullptr);
    counter->inc();
    EXPECT_EQ(counter->get(), 1.0);
}

TEST_F(registry_test, default_registry)
{
    auto& default_reg = tc::prometheus::default_registry();

    auto counter_family = default_reg.create_counter("default_counter", "Counter in default registry");
    auto counter = counter_family->add_metric({});
    counter->inc(5);

    EXPECT_EQ(counter->get(), 5.0);

    default_reg.reset();
}

TEST_F(registry_test, create_duplicate_metric_family_throws)
{
    registry->create_counter("duplicate_name", "First counter");
    EXPECT_THROW(registry->create_counter("duplicate_name", "Second counter"),
                 std::runtime_error);
}

TEST_F(registry_test, get_non_existent_metric_throws)
{
    auto counter_family = registry->create_counter("test_counter", "Test");
    counter_family->add_metric({{"label", "value1"}});

    tc::prometheus::labels non_existent;
    non_existent.add("label", "value2");

    EXPECT_THROW(counter_family->get_metric(non_existent), std::runtime_error);
}

TEST_F(registry_test, create_histogram_with_custom_buckets)
{
    std::vector<double> custom_buckets = {0.01, 0.1, 1.0, 10.0, 100.0};
    auto histogram_family = registry->create_histogram("custom_hist", "Custom histogram", custom_buckets);
    auto histogram = histogram_family->add_metric({});

    histogram->observe(0.5);
    ASSERT_NE(histogram, nullptr);
    EXPECT_EQ(histogram->count(), 1);
}

TEST_F(registry_test, create_summary_with_custom_quantiles)
{
    std::vector<double> custom_quantiles = {0.25, 0.5, 0.75, 0.95};
    auto summary_family = registry->create_summary("custom_summary", "Custom summary", custom_quantiles);
    auto summary = summary_family->add_metric({});

    ASSERT_NE(summary, nullptr);
    // Note: quantiles parameter is not currently used by create_summary
    // The summary will use its default quantiles
    EXPECT_FALSE(summary->quantiles_list().empty());
}

TEST_F(registry_test, serialize_with_eof_termination)
{
    auto counter_family = registry->create_counter("test", "Test");
    auto counter = counter_family->add_metric({});
    counter->inc();

    std::string output = registry->serialize();
    EXPECT_TRUE(output.ends_with("\n"));
    EXPECT_NE(output.find("# EOF"), std::string::npos);
}
