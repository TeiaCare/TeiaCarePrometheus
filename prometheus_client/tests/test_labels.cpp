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

#include <teiacare/prometheus_client/labels.hpp>

#include <gtest/gtest.h>

class labels_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(labels_test, default_constructor_creates_empty_labels)
{
    tc::prometheus::labels labels;
    EXPECT_TRUE(labels.empty());
}

TEST_F(labels_test, initializer_list_constructor)
{
    tc::prometheus::labels labels;
    labels.add("key1", "value1").add("key2", "value2");
    EXPECT_FALSE(labels.empty());
}

TEST_F(labels_test, add_label)
{
    tc::prometheus::labels labels;
    labels.add("key", "value");
    EXPECT_FALSE(labels.empty());
}

TEST_F(labels_test, to_string)
{
    tc::prometheus::labels labels;
    labels.add("key", "value");
    std::string result = labels.to_string();
    EXPECT_FALSE(result.empty());
}

TEST_F(labels_test, add_invalid_label_name_throws)
{
    tc::prometheus::labels labels;
    EXPECT_THROW(labels.add("", "value"), std::invalid_argument);
}

TEST_F(labels_test, equality_operator)
{
    tc::prometheus::labels labels1;
    labels1.add("key1", "value1");
    tc::prometheus::labels labels2;
    labels2.add("key1", "value1");
    EXPECT_EQ(labels1, labels2);
}

TEST_F(labels_test, inequality_operator)
{
    tc::prometheus::labels labels1;
    labels1.add("key1", "value1");
    tc::prometheus::labels labels2;
    labels2.add("key2", "value2");
    EXPECT_NE(labels1, labels2);
}

TEST_F(labels_test, labels_with_multiple_values)
{
    tc::prometheus::labels labels;
    labels.add("method", "GET");
    labels.add("status", "200");
    labels.add("path", "/api/v1/users");

    std::string result = labels.to_string();
    EXPECT_FALSE(result.empty());
}
