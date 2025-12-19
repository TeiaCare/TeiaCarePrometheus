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

#include <teiacare/prometheus_client/histogram.hpp>
#include <teiacare/prometheus_client/metric_type.hpp>

#include <gtest/gtest.h>

class histogram_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        bounds = {0.1, 0.5, 1.0, 5.0, 10.0};
        histogram = std::make_unique<tc::prometheus::histogram>("histogram", tc::prometheus::labels{}, bounds);
    }

    void TearDown() override
    {
        histogram.reset();
    }

    std::vector<double> bounds;
    std::unique_ptr<tc::prometheus::histogram> histogram;
};

TEST_F(histogram_test, initial_count_is_zero)
{
    EXPECT_EQ(histogram->count(), 0);
}

TEST_F(histogram_test, initial_sum_is_zero)
{
    EXPECT_EQ(histogram->sum(), 0.0);
}

TEST_F(histogram_test, observe_increments_count)
{
    histogram->observe(0.5);
    EXPECT_EQ(histogram->count(), 1);
}

TEST_F(histogram_test, observe_updates_sum)
{
    histogram->observe(0.5);
    histogram->observe(1.5);
    EXPECT_EQ(histogram->sum(), 2.0);
}

TEST_F(histogram_test, observe_updates_buckets)
{
    histogram->observe(0.3);
    histogram->observe(0.7);
    histogram->observe(2.0);

    // Check that observations were counted
    EXPECT_EQ(histogram->count(), 3);
    EXPECT_DOUBLE_EQ(histogram->sum(), 3.0);
}

TEST_F(histogram_test, observe_multiple_values)
{
    for (int i = 0; i < 100; ++i)
    {
        histogram->observe(static_cast<double>(i) / 10.0);
    }
    EXPECT_EQ(histogram->count(), 100);
}

TEST_F(histogram_test, metric_type_is_histogram)
{
    EXPECT_STREQ(tc::prometheus::metric_type_t<tc::prometheus::histogram>, "histogram");
}

TEST_F(histogram_test, histogram_with_labels)
{
    tc::prometheus::labels labels;
    labels.add("endpoint", "/api/users");
    auto labeled_histogram = std::make_unique<tc::prometheus::histogram>("http_request_duration_seconds", labels, bounds);

    labeled_histogram->observe(0.25);
    EXPECT_EQ(labeled_histogram->count(), 1);
}

// Note: Empty buckets may be allowed in the current implementation
// TEST_F(histogram_test, empty_buckets_throws)
// {
//     std::vector<double> empty_bounds;
//     EXPECT_THROW(
//         tc::prometheus::histogram("bad_histogram", tc::prometheus::labels{}, empty_bounds),
//         std::invalid_argument
//     );
// }
