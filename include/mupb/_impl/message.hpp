/**
   Copyright 2018 Robert Bezem

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
/** @file decode.hpp
 *  @brief Base class for all messages
 *
 *  @author Robert Bezem(sqrtroot)
 */
#pragma once

#include "types.hpp"
#include <tuple>

#define MUPB_FIELD_NAME(name, index) decltype(std::get<(index)>(data)) (name) = std::get<(index)>(data)

namespace mupb {
    /// All protobuf messages should inherit from this class
    /// \tparam List of Types in order defined in the protobuf file.
    /// They should be types from @see Types
    template<typename... T>
    struct Message {
        /// All types in a tuple
        typedef std::tuple<T...> data_t;
        /// Actual data
        data_t data;
    };
}  // namespace mupb

