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

#include <cctype>
#include <sstream>
#include <stdexcept>

namespace tc::prometheus
{
labels::label::label(std::string n, std::string v)
    : name(std::move(n))
    , value(std::move(v))
{
}

bool labels::is_valid_label_name(std::string_view name)
{
    if (name.empty() || name.rfind("__", 0) == 0)
        return false;

    char first = name[0];
    if (!(std::isalpha(first) || first == '_' || first == ':'))
        return false;

    for (char c : name)
    {
        if (!(std::isalnum(c) || c == '_' || c == ':'))
            return false;
    }
    return true;
}

std::string labels::escape_value(std::string const& value)
{
    std::string result;
    result.reserve(value.size() * 2);
    for (char c : value)
    {
        switch (c)
        {
        case '\\':
            result += "\\\\";
            break;
        case '\"':
            result += "\\\"";
            break;
        case '\n':
            result += "\\n";
            break;
        case '\t':
            result += "\\t";
            break;
        case '\r':
            result += "\\r";
            break;
        default:
            result += c;
            break;
        }
    }
    return result;
}

bool labels::empty() const noexcept
{
    return _labels.empty();
}

std::string labels::to_string() const
{
    if (!_cached_string)
    {
        if (_labels.empty())
        {
            _cached_string = "";
        }
        else
        {
            std::ostringstream oss;
            oss << '{';
            for (size_t i = 0; i < _labels.size(); ++i)
            {
                if (i)
                    oss << ',';
                oss << _labels[i].name << "=\""
                    << escape_value(_labels[i].value) << "\"";
            }
            oss << '}';
            _cached_string = oss.str();
        }
    }
    return *_cached_string;
}

bool labels::operator==(labels const& other) const noexcept
{
    return _labels == other._labels;
}

}
