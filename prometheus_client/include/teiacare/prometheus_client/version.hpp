// Copyright 2024 TeiaCare
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

#pragma once

namespace tc::prometheus
{
/*!
 * \brief Library name
 */
extern const char* const name;

/*!
 * \brief Library version string
 */
extern const char* const version;

/*!
 * \brief Project description
 */
extern const char* const project_description;

/*!
 * \brief Project URL
 */
extern const char* const project_url;

/*!
 * \brief Build type (Debug or Release)
 */
extern const char* const build_type;

/*!
 * \brief Compiler name
 */
extern const char* const compiler_name;

/*!
 * \brief Compiler version
 */
extern const char* const compiler_version;

/*!
 * \brief C++ compiler flags
 */
extern const char* const cxx_flags;

/*!
 * \brief C++ compiler flags for Debug builds
 */
extern const char* const cxx_flags_debug;

/*!
 * \brief C++ compiler flags for Release builds
 */
extern const char* const cxx_flags_release;

/*!
 * \brief C++ standard version
 */
extern const char* const cxx_standard;

/*!
 * \brief Operating system name
 */
extern const char* const os_name;

/*!
 * \brief Operating system version
 */
extern const char* const os_version;

/*!
 * \brief Operating system processor architecture
 */
extern const char* const os_processor;
}
