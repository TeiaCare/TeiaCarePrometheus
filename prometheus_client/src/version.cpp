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

namespace tc::prometheus
{
extern const char* const name = "teiacare_prometheus_client";
extern const char* const version = "0.1.0";

extern const char* const project_description = "TeiaCarePrometheus is a client library to expose Prometheus metrics from C++ applications";
extern const char* const project_url = "https://github.com/TeiaCare/TeiaCarePrometheus";

extern const char* const create_type = "Debug";
extern const char* const compiler_name = "Clang";
extern const char* const compiler_version = "15.0.7";

extern const char* const cxx_flags = "";
extern const char* const cxx_flags_debug = "-g";
extern const char* const cxx_flags_release = "-O3 -DNDEBUG";
extern const char* const cxx_standard = "20";

extern const char* const os_name = "Linux";
extern const char* const os_version = "6.14.0-1011-oem";
extern const char* const os_processor = "x86_64";

}
