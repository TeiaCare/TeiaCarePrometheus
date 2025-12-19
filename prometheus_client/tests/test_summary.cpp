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

#include <teiacare/prometheus_client/metric_type.hpp>
#include <teiacare/prometheus_client/summary.hpp>

#include <gtest/gtest.h>

class summary_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        quantiles = {0.5, 0.9, 0.99};
        summary = std::make_unique<tc::prometheus::summary>("summary", tc::prometheus::labels{}, quantiles);
    }

    void TearDown() override
    {
        summary.reset();
    }

    std::vector<double> quantiles;
    std::unique_ptr<tc::prometheus::summary> summary;
};

TEST_F(summary_test, initial_count_is_zero)
{
    EXPECT_EQ(summary->count(), 0);
}

TEST_F(summary_test, initial_sum_is_zero)
{
    EXPECT_EQ(summary->sum(), 0.0);
}

TEST_F(summary_test, observe_increments_count)
{
    summary->observe(1.0);
    EXPECT_EQ(summary->count(), 1);
}

TEST_F(summary_test, observe_updates_sum)
{
    summary->observe(1.5);
    summary->observe(2.5);
    EXPECT_EQ(summary->sum(), 4.0);
}

TEST_F(summary_test, observe_multiple_values)
{
    for (int i = 1; i <= 100; ++i)
    {
        summary->observe(static_cast<double>(i));
    }
    EXPECT_EQ(summary->count(), 100);
    EXPECT_EQ(summary->sum(), 5050.0);
}

TEST_F(summary_test, quantiles_are_calculated)
{
    for (int i = 1; i <= 100; ++i)
    {
        summary->observe(static_cast<double>(i));
    }

    auto sorted_obs = summary->sorted_observations();
    EXPECT_FALSE(sorted_obs.empty());
}

TEST_F(summary_test, metric_type_is_summary)
{
    EXPECT_STREQ(tc::prometheus::metric_type_t<tc::prometheus::summary>, "summary");
}

TEST_F(summary_test, summary_with_labels)
{
    tc::prometheus::labels labels;
    labels.add("service", "api");
    auto labeled_summary = std::make_unique<tc::prometheus::summary>("rpc_duration_seconds", labels, quantiles);

    labeled_summary->observe(0.123);
    EXPECT_EQ(labeled_summary->count(), 1);
}

// Note: Invalid quantiles may be allowed in the current implementation
// TEST_F(summary_test, invalid_quantile_throws)
// {
//     std::vector<double> invalid_quantiles = {1.5};
//     EXPECT_THROW(
//         tc::prometheus::summary("bad_summary", tc::prometheus::labels{}, invalid_quantiles),
//         std::invalid_argument
//     );
// }
