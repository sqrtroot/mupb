/*
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
/** @file types.hpp
 *  @brief Declare all types and implement type helping functions
 *
 *  @author Robert Bezem(sqrtroot)
 */
#pragma once
#include <cstdint>
#include <type_traits>

namespace mupb {
  namespace Types {
    enum WireType {
      varint = 0,  /// Used for int32, int64, uint32, uint64, sint32, sint64,
                   /// bool, enum
      bit64 = 1,   /// Used for fixed64, sfixed64, double
      length_delimited = 2,  /// Used for string, bytes, embedded messages,
                             /// packed repeated fields
      start_group = 3,       ///@deprecated Used for groups
      end_group = 4,         ///@deprecated Used for groups
      bit32 = 5,             /// Used for fixed32, sfixed32, float
    };

    struct s_tag_c {};
    template <typename T>
    class s_tag : public s_tag_c {
      T val;

     public:
      typedef T basetype;
      s_tag() {}
      s_tag(T val) : val(val) {}
      operator T&() { return val; }
      operator T() const { return val; }
    };

    struct f_tag_c {};
    template <typename T>
    class f_tag : public f_tag_c {
      T val;

     public:
      typedef T basetype;
      f_tag() {}
      f_tag(T val) : val(val) {}
      operator T&() { return val; }
      operator T() const { return val; }
    };

    typedef int32_t int32;
    typedef s_tag<int32_t> sint32;
    typedef uint32_t uint32;

    typedef int64_t int64;
    typedef s_tag<int64_t> sint64;
    typedef uint64_t uint64;

    // bool
    // enum

    typedef f_tag<uint64_t> fixed64;
    typedef f_tag<int64_t> sfixed64;
    // double

    typedef f_tag<uint32_t> fixed32;
    typedef f_tag<uint32_t> sfixed32;
    // float

    template <typename T>
    bool checkType(WireType wt) {
      switch (wt) {
        case WireType::varint:
          if constexpr (std::is_same<T, Types::int32>::value) return true;
          if constexpr (std::is_same<T, Types::int64>::value) return true;
          if constexpr (std::is_same<T, Types::uint32>::value) return true;
          if constexpr (std::is_same<T, Types::uint64>::value) return true;
          if constexpr (std::is_same<T, Types::sint32>::value) return true;
          if constexpr (std::is_same<T, Types::sint64>::value) return true;

          if constexpr (std::is_same<T, bool>::value) return true;
          if constexpr (std::is_enum<T>::value) return true;

          return false;
        case WireType::bit64:
          if constexpr (std::is_same<T, Types::fixed64>::value) return true;
          if constexpr (std::is_same<T, Types::sfixed64>::value) return true;
          if constexpr (std::is_same<T, double>::value) return true;
          return false;
        case WireType::length_delimited:
          return false;
        case WireType::start_group:
          return false;
        case WireType::end_group:
          return false;
        case WireType::bit32:
          if constexpr (std::is_same<T, Types::fixed32>::value) return true;
          if constexpr (std::is_same<T, Types::sfixed32>::value) return true;
          if constexpr (std::is_same<T, float>::value) return true;
          return false;
      }
      return false;
    }
    struct FieldDescriptor {
      uint8_t number;
      WireType wt;
      FieldDescriptor(uint8_t data)
          : number(data >> 3), wt(WireType(data & 0b00000111)) {}
    };
  }  // namespace Types
}  // namespace mupb
