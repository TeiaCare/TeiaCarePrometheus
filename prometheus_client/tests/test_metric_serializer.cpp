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
#include <teiacare/prometheus_client/metric_serializer.hpp>
#include <teiacare/prometheus_client/registry.hpp>
#include <teiacare/prometheus_client/summary.hpp>

#include <gtest/gtest.h>

class metric_serializer_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        serializer = std::make_unique<tc::prometheus::metric_serializer>();
    }

    void TearDown() override
    {
        serializer.reset();
    }

    std::unique_ptr<tc::prometheus::metric_serializer> serializer;
};

TEST_F(metric_serializer_test, serialize_counter)
{
    tc::prometheus::counter counter("test_counter");
    counter.inc(42);

    serializer->serialize(counter);
    std::string output = serializer->str();

    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("test_counter"), std::string::npos);
    EXPECT_NE(output.find("42"), std::string::npos);
}

TEST_F(metric_serializer_test, serialize_gauge)
{
    tc::prometheus::gauge gauge("test_gauge");
    gauge.set(75.5);

    serializer->serialize(gauge);
    std::string output = serializer->str();

    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("test_gauge"), std::string::npos);
    EXPECT_NE(output.find("75.5"), std::string::npos);
}

TEST_F(metric_serializer_test, serialize_histogram)
{
    std::vector<double> buckets = {0.1, 0.5, 1.0, 5.0};
    tc::prometheus::histogram histogram("test_histogram", tc::prometheus::labels{}, buckets);

    histogram.observe(0.3);
    histogram.observe(0.7);
    histogram.observe(2.0);

    serializer->serialize(histogram);
    std::string output = serializer->str();

    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("test_histogram"), std::string::npos);
}

TEST_F(metric_serializer_test, serialize_summary)
{
    std::vector<double> quantiles = {0.5, 0.9, 0.99};
    tc::prometheus::summary summary("test_summary", tc::prometheus::labels{}, quantiles);

    for (int i = 1; i <= 100; ++i)
    {
        summary.observe(static_cast<double>(i));
    }

    serializer->serialize(summary);
    std::string output = serializer->str();

    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("test_summary"), std::string::npos);
}

TEST_F(metric_serializer_test, serialize_counter_with_labels)
{
    tc::prometheus::labels labels;
    labels.add("method", "GET").add("status", "200");

    tc::prometheus::counter counter("http_requests", labels);
    counter.inc(100);

    serializer->serialize(counter);
    std::string output = serializer->str();

    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("http_requests"), std::string::npos);
    EXPECT_NE(output.find("method"), std::string::npos);
    EXPECT_NE(output.find("GET"), std::string::npos);
    EXPECT_NE(output.find("status"), std::string::npos);
    EXPECT_NE(output.find("200"), std::string::npos);
}

TEST_F(metric_serializer_test, serialize_gauge_with_labels)
{
    tc::prometheus::labels labels;
    labels.add("cpu", "0").add("mode", "user");

    tc::prometheus::gauge gauge("cpu_usage", labels);
    gauge.set(85.2);

    serializer->serialize(gauge);
    std::string output = serializer->str();

    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("cpu_usage"), std::string::npos);
    EXPECT_NE(output.find("cpu"), std::string::npos);
    EXPECT_NE(output.find("mode"), std::string::npos);
}

TEST_F(metric_serializer_test, serialize_metric_family)
{
    tc::prometheus::registry registry;
    auto counter_family = registry.create_counter("family_counter", "Counter family");

    auto counter1 = counter_family->add_metric({{"label", "value1"}});
    auto counter2 = counter_family->add_metric({{"label", "value2"}});

    counter1->inc(10);
    counter2->inc(20);

    serializer->serialize(*counter_family);
    std::string output = serializer->str();

    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("family_counter"), std::string::npos);
}

TEST_F(metric_serializer_test, serialize_multiple_metrics)
{
    tc::prometheus::counter counter("counter_metric");
    tc::prometheus::gauge gauge("gauge_metric");

    counter.inc(5);
    gauge.set(10);

    serializer->serialize(counter);
    serializer->serialize(gauge);

    std::string output = serializer->str();

    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("counter_metric"), std::string::npos);
    EXPECT_NE(output.find("gauge_metric"), std::string::npos);
}

TEST_F(metric_serializer_test, empty_serializer)
{
    std::string output = serializer->str();
    EXPECT_TRUE(output.empty());
}

TEST_F(metric_serializer_test, serialize_counter_with_zero_value)
{
    tc::prometheus::counter counter("zero_counter");
    // Don't increment - should remain at 0

    serializer->serialize(counter);
    std::string output = serializer->str();

    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("zero_counter"), std::string::npos);
    EXPECT_NE(output.find("0"), std::string::npos);
}

TEST_F(metric_serializer_test, serialize_gauge_with_negative_value)
{
    tc::prometheus::gauge gauge("negative_gauge");
    gauge.set(-42.5);

    serializer->serialize(gauge);
    std::string output = serializer->str();

    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("negative_gauge"), std::string::npos);
    EXPECT_NE(output.find("-42.5"), std::string::npos);
}

TEST_F(metric_serializer_test, serialize_histogram_with_observations)
{
    std::vector<double> buckets = {1.0, 2.0, 5.0, 10.0};
    tc::prometheus::histogram histogram("request_duration", tc::prometheus::labels{}, buckets);

    // Add some observations
    histogram.observe(0.5);
    histogram.observe(1.5);
    histogram.observe(3.0);
    histogram.observe(7.0);
    histogram.observe(15.0);

    serializer->serialize(histogram);
    std::string output = serializer->str();

    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("request_duration"), std::string::npos);
    // Should contain sum and count
    EXPECT_NE(output.find("_sum"), std::string::npos);
    EXPECT_NE(output.find("_count"), std::string::npos);
}

TEST_F(metric_serializer_test, registry_serialize)
{
    tc::prometheus::registry registry;

    auto counter_family = registry.create_counter("http_requests_total", "Total HTTP requests");
    auto gauge_family = registry.create_gauge("active_connections", "Active connections");

    auto counter = counter_family->add_metric({{"method", "GET"}});
    auto gauge = gauge_family->add_metric({});

    counter->inc(100);
    gauge->set(50);

    std::string output = registry.serialize();

    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("http_requests_total"), std::string::npos);
    EXPECT_NE(output.find("active_connections"), std::string::npos);
    EXPECT_NE(output.find("100"), std::string::npos);
    EXPECT_NE(output.find("50"), std::string::npos);
}

TEST_F(metric_serializer_test, serialization_includes_help_comment)
{
    tc::prometheus::registry registry;
    auto counter_family = registry.create_counter("test_counter", "This is help text");
    auto counter = counter_family->add_metric({});
    counter->inc(1);

    std::string output = registry.serialize();
    EXPECT_NE(output.find("# HELP test_counter This is help text"), std::string::npos);
}

TEST_F(metric_serializer_test, serialization_includes_type_comment)
{
    tc::prometheus::registry registry;
    auto gauge_family = registry.create_gauge("test_gauge", "Gauge help");
    auto gauge = gauge_family->add_metric({});
    gauge->set(42);

    std::string output = registry.serialize();
    EXPECT_NE(output.find("# TYPE test_gauge gauge"), std::string::npos);
}

TEST_F(metric_serializer_test, histogram_serialization_format)
{
    tc::prometheus::registry registry;
    auto hist_family = registry.create_histogram("request_duration", "Request duration", {0.1, 0.5, 1.0});
    auto hist = hist_family->add_metric({});
    hist->observe(0.3);
    hist->observe(0.7);

    std::string output = registry.serialize();

    // Should include bucket, sum, and count metrics
    EXPECT_NE(output.find("request_duration_bucket"), std::string::npos);
    EXPECT_NE(output.find("request_duration_sum"), std::string::npos);
    EXPECT_NE(output.find("request_duration_count"), std::string::npos);
}

TEST_F(metric_serializer_test, summary_serialization_format)
{
    tc::prometheus::registry registry;
    auto summary_family = registry.create_summary("response_size", "Response size", {0.5, 0.9, 0.99});
    auto summary = summary_family->add_metric({});
    for (int i = 1; i <= 10; ++i)
    {
        summary->observe(i);
    }

    std::string output = registry.serialize();

    // Should include sum and count metrics
    EXPECT_NE(output.find("response_size"), std::string::npos);
    EXPECT_NE(output.find("response_size_sum"), std::string::npos);
    EXPECT_NE(output.find("response_size_count"), std::string::npos);
}
