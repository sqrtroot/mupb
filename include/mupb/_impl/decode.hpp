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
 *  @brief Implement the decoder for protobuf
 *
 *  This uses a bit of unicorn magic. The important parts are the implementation
 * in decode::type and the main function at the bottom
 *
 *  @author Robert Bezem(sqrtroot)
 */
#pragma once

#include <iostream>
#include <csignal>
#include <boost/endian/conversion.hpp>
#include <cstring>
#include <limits>
#include <optional>
#include <tuple>
#include <type_traits>
#include "./message.hpp"
#include "./types.hpp"

/// Main namespace
namespace mupb {
    /// Decode functions for different types
    namespace typedecode {
        /// Tools to help with type decoding
        namespace tools {
            /// Check if byte has next bit set
            /// \param data Byte to check
            bool inline has_next(const uint8_t data) { return data & 0b10000000u; }

            /// Remove next_bit
            /// \param data byte to remove nextbit from
            /// \return byte without nextbit set
            uint8_t inline remove_next_bit(const uint8_t data) { return data & 0b01111111u; }
        }  // namespace tools

        /// Decode varint types
        /// \tparam T type to decode to
        /// \param size size of data
        template<typename T>
        std::optional<std::pair<T, size_t>> decode_varint(const uint8_t *data, size_t size) {
            uint64_t rval = 0;
            std::size_t i = 0;
            do {
                if (i * 7 < sizeof(T) * 8) {
                    rval += (uint64_t) tools::remove_next_bit(*data) << (i * 7);
                }
            } while (++i < size && tools::has_next(*data++));

            if constexpr (std::is_base_of<Types::s_tag_c, T>::value) {
                if (rval & 0x1u) { // first bit set aka uneven means negative number
                    rval = (rval / 2 + 1) * -1;
                } else { //otherwise positive number
                    rval = (rval / 2);
                }
            }
            boost::endian::little_to_native_inplace(rval);
            return std::pair((T) rval, i);
        }

        /// Decode fixed size bit types
        /// \tparam T type to decode to
        /// \param size size of data
        /// \param bytes amount of bytes to decode usualy 4 or 8
        template<typename T>
        std::optional<std::pair<T, size_t>> decode_bit(const uint8_t *data, size_t size, size_t bytes) {
            if (sizeof(T) > size || sizeof(T) != bytes) {
                return std::nullopt;
            }
            auto rval = *(T *) (data);
            return std::pair(boost::endian::little_to_native(rval), bytes);
        }
    }  // namespace typedecode

    template<typename T, size_t N , size_t... seq>
    bool _decode(T &t, const uint8_t *buffer, size_t buffer_size) {
        using namespace mupb::Types;
        using namespace typedecode;

        if(buffer_size == 0){
            return false;
        }


        auto& original_field = std::get<N>(t.data);

        auto new_field_descriptor = FieldDescriptor(*buffer);
        buffer++; buffer_size--;

        if (original_field.fieldDescriptor != new_field_descriptor){
            if constexpr (sizeof...(seq) != 0){
                return true && _decode<T, seq...>(t, buffer, buffer_size);
            }else {
                return true;
            }
        }

        const auto wt = original_field.fieldDescriptor.wt;

        if(wt == WireType::varint){
            auto decoded = decode_varint<typename std::remove_reference_t<decltype(original_field)>::value_type>(buffer, buffer_size);
            if(decoded){
               auto [val, decoded_size] = *decoded;
               *original_field = val;
               buffer_size -= decoded_size;
               buffer += decoded_size;
            }else{
                return false;
            }
        }else if(wt == WireType::bit64 || wt == WireType::bit32){
            const auto bits = wt == WireType::bit64 ? 8 : 4;
            auto decoded = decode_bit<typename std::remove_reference_t<decltype(original_field)>::value_type>(buffer, buffer_size, bits);
            if(decoded){
               auto [val, decoded_size] = *decoded;
               *original_field = val;
               buffer_size -= decoded_size;
               buffer += decoded_size;
            }else{
                return false;
            }
        }else if(wt == WireType::length_delimited){
            return false;
        }else{
            return false;
        }

        if constexpr (sizeof...(seq) != 0){
            return true && _decode<T, seq...>(t, buffer, buffer_size);
        }else {
            return true;
        }

    }

    template<typename T, size_t... seq>
    bool __decode(T& t, const uint8_t *buffer, size_t buffer_size, 
                    std::index_sequence<seq...>) {
        return _decode<T, seq...>(t, buffer, buffer_size);
    }

    /// Decode an Message
    /// \tparam T @see Message childclass
    /// \param buffer Pointer to begin of buffer containing T packet
    /// \param size Size of afore mentioned buffer
    template<typename T,
            ///@cond DEV don't document internal type param
            typename Indices = std::make_index_sequence<
                    std::tuple_size<typename T::data_t>::value>
            ///@endcond
    >
    std::optional<T> decode(const uint8_t *buffer, size_t size) {
        T t;
        if(__decode(t, buffer, size, Indices{})){
            return t;
        }
        return std::nullopt;
    }
}  // namespace mupb
