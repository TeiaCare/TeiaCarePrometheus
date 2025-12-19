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
#include <teiacare/prometheus_client/metric_type.hpp>

#include <chrono>
#include <gtest/gtest.h>

class gauge_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        gauge = std::make_unique<tc::prometheus::gauge>("gauge");
    }

    void TearDown() override
    {
        gauge.reset();
    }

    std::unique_ptr<tc::prometheus::gauge> gauge;
};

TEST_F(gauge_test, initial_value_is_zero)
{
    EXPECT_EQ(gauge->get(), 0.0);
}

TEST_F(gauge_test, set_value)
{
    gauge->set(42.5);
    EXPECT_EQ(gauge->get(), 42.5);
}

TEST_F(gauge_test, increment_increases_value)
{
    gauge->inc();
    EXPECT_EQ(gauge->get(), 1.0);
}

TEST_F(gauge_test, increment_by_amount)
{
    gauge->inc(5.5);
    EXPECT_EQ(gauge->get(), 5.5);
}

TEST_F(gauge_test, decrement_decreases_value)
{
    gauge->set(10.0);
    gauge->dec();
    EXPECT_EQ(gauge->get(), 9.0);
}

TEST_F(gauge_test, decrement_by_amount)
{
    gauge->set(10.0);
    gauge->dec(3.5);
    EXPECT_EQ(gauge->get(), 6.5);
}

TEST_F(gauge_test, set_to_current_time)
{
    auto now = std::chrono::duration_cast<std::chrono::seconds>(
                   std::chrono::system_clock::now().time_since_epoch())
                   .count();
    gauge->set(static_cast<double>(now));
    EXPECT_GT(gauge->get(), 0.0);
}

TEST_F(gauge_test, gauge_can_be_negative)
{
    gauge->set(-10.5);
    EXPECT_EQ(gauge->get(), -10.5);
}

TEST_F(gauge_test, metric_type_is_gauge)
{
    EXPECT_STREQ(tc::prometheus::metric_type_t<tc::prometheus::gauge>, "gauge");
}

TEST_F(gauge_test, gauge_with_labels)
{
    tc::prometheus::labels labels;
    labels.add("cpu", "0").add("mode", "user");
    auto labeled_gauge = std::make_unique<tc::prometheus::gauge>("cpu_usage", labels);

    labeled_gauge->set(75.5);
    EXPECT_EQ(labeled_gauge->get(), 75.5);
}
