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
#include <teiacare/prometheus_client/metric_type.hpp>

#include <gtest/gtest.h>

class counter_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        counter = std::make_unique<tc::prometheus::counter>("counter");
    }

    void TearDown() override
    {
        counter.reset();
    }

    std::unique_ptr<tc::prometheus::counter> counter;
};

TEST_F(counter_test, initial_value_is_zero)
{
    EXPECT_EQ(counter->get(), 0.0);
}

TEST_F(counter_test, increment_increases_value)
{
    counter->inc();
    EXPECT_EQ(counter->get(), 1.0);
}

TEST_F(counter_test, increment_by_amount)
{
    counter->inc(5.0);
    EXPECT_EQ(counter->get(), 5.0);
}

TEST_F(counter_test, multiple_increments)
{
    counter->inc(3.0);
    counter->inc(2.0);
    counter->inc(1.0);
    EXPECT_EQ(counter->get(), 6.0);
}

TEST_F(counter_test, increment_with_negative_throws)
{
    EXPECT_THROW(counter->inc(-1.0), std::invalid_argument);
}

TEST_F(counter_test, reset_sets_value_to_zero)
{
    counter->inc(10.0);
    counter->reset();
    EXPECT_EQ(counter->get(), 0.0);
}

TEST_F(counter_test, metric_type_is_counter)
{
    EXPECT_STREQ(tc::prometheus::metric_type_t<tc::prometheus::counter>, "counter");
}

TEST_F(counter_test, counter_with_labels)
{
    tc::prometheus::labels labels;
    labels.add("method", "GET").add("status", "200");
    auto labeled_counter = std::make_unique<tc::prometheus::counter>("http_requests_total", labels);

    labeled_counter->inc();
    EXPECT_EQ(labeled_counter->get(), 1.0);
}

TEST_F(counter_test, increment_by_zero)
{
    counter->inc(0.0);
    EXPECT_DOUBLE_EQ(counter->get(), 0.0);
}

TEST_F(counter_test, increment_by_large_value)
{
    counter->inc(1e10);
    EXPECT_DOUBLE_EQ(counter->get(), 1e10);
}

TEST_F(counter_test, counter_name_and_labels_access)
{
    tc::prometheus::labels labels;
    labels.add("env", "production");
    tc::prometheus::counter c("access_test", labels);

    EXPECT_EQ(c.name(), "access_test");
    EXPECT_FALSE(c.labels().empty());
    EXPECT_EQ(c.labels().to_string(), labels.to_string());
}
