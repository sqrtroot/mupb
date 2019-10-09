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
#include <boost/mpl/set.hpp>

namespace mupb::Types {
    ///Type numbers used in the headerbyte
    enum class WireType {
        /// Used for int32, int64, uint32, uint64, sint32, sint64,
        /// bool, enum
                varint = 0,
        /// Used for fixed64, sfixed64, double
                bit64 = 1,
        /// Used for string, bytes, embedded messages,0000 0001
        /// packed repeated fields
                length_delimited = 2,
        ///\deprecated Used for groups which are now deprecated
                start_group = 3,
        ///\deprecated end_group Used for groups
                end_group = 4,
        /// Used for fixed32, sfixed32, float
                bit32 = 5,
        /// WIRE TYPE NOT DEFINED BY STANDARD
                UNDEFINED = -1,
    };
    /// @cond DEV
    /// Used to do type traits on @see s_tag
    struct s_tag_c {
    };
    /// @endcond

    /// Tag for zig zag encoded int size types
    /// When you want to check for this tag, check for `s_tag_c`
    /// \tparam T Type to apply s_tag to
    template<typename T>
    class s_tag : public s_tag_c {
        /// @cond DEV
        T val;

    public:
        typedef T basetype;

        s_tag() {}

        s_tag(T val) : val(val) {}

        operator T &() { return val; }

        operator T() const { return val; }
        ///@endcond
    };

    /// @cond DEV
    /// Used to do type traits on @see f_tag
    struct f_tag_c {
    };
    /// @endcond

    /// Tag for fixed size types
    /// When you want to check for this tag, check for `f_tag_c`
    /// \tparam T Type to apply f_tag to
    template<typename T>
    class f_tag : public f_tag_c {
        /// @cond DEV
        T val;

    public:
        typedef T basetype;

        f_tag() {}

        f_tag(T val) : val(val) {}

        operator T &() { return val; }

        operator T() const { return val; }
        ///@endcond
    };

    /// 32 bit signed integer
    typedef int32_t int32;
    /// Zig zag encoded 32 bit signed integer
    typedef s_tag<int32_t> sint32;
    /// 32 bit unsigned integer
    typedef uint32_t uint32;

    /// 64 bit signed integer
    typedef int64_t int64;
    /// 64 bit zig zag encoded integer
    typedef s_tag<int64_t> sint64;
    /// 64 bit unsigned integer
    typedef uint64_t uint64;

    // bool
    // enum

    /// Fixed 64 bit encoded as unsigned integer
    typedef f_tag<uint64_t> fixed64;
    /// Fixed 64 bit encoded as signed integer
    typedef f_tag<int64_t> sfixed64;
    // double

    /// Fixed 32 bit encoded as unsigned integer
    typedef f_tag<uint32_t> fixed32;
    /// Fixed 32 bit encoded as signed integer
    typedef f_tag<int32_t> sfixed32;
    // float

    template<typename T>
    constexpr WireType getWireType() {
        if constexpr (boost::mpl::has_key<boost::mpl::set<
                Types::int32,
                Types::int64,
                Types::uint32,
                Types::uint64,
                Types::sint32,
                Types::sint64,
                bool>, T>::value || std::is_enum_v<T>) {
            return WireType::varint;
        } else if constexpr (boost::mpl::has_key<boost::mpl::set<
                Types::fixed64,
                Types::sfixed64,
                double>, T>::value) {
            return WireType::bit64;

        } else if constexpr (boost::mpl::has_key<boost::mpl::set<
                Types::fixed32,
                Types::sfixed32,
                float>, T>::value) {
            return WireType::bit32;
        }
        return WireType::UNDEFINED;
    }


    /// Describes the first byte of an message variable
    /// The first byte contains the number of the field and the type as defined in the protobuf file
    struct FieldDescriptor {
        /// Number of field
        uint8_t number;
        /// Type of field
        WireType wt;

        /// Construct a fielddescriptor from a byte of data
        /// \param data First byte of a field
        FieldDescriptor(uint8_t data)
                : number(data >> 3u), wt(WireType(data & 0b00000111u)) {};

        /// Construct a fielddescriptor from its number and wiretype
        FieldDescriptor(uint8_t number, WireType wt) : number(number), wt(wt) {};

        constexpr bool operator==(const FieldDescriptor &rhs) const {
            return number == rhs.number;
        }

        constexpr bool operator!=(const FieldDescriptor &rhs) const {
            return !(rhs == *this);
        }

    };

    template<uint8_t N, typename T>
    struct Field{

        FieldDescriptor fieldDescriptor{N, getWireType<T>()};

        T val;
        typedef T basetype;

        Field() {}
        Field(T val) : val(val) {}
        operator T &() { return val; }
        operator T() const { return val; }
    };
}  // namespace mupb
