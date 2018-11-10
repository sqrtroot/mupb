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
 *  This uses a lot of unicorn magic. The important parts are the implementation
 * in decode::type and the main function at the bottom
 *
 *  @author Robert Bezem(sqrtroot)
 */
#pragma once
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
      bool inline has_next(uint8_t data) { return data & 0b10000000; }

      /// Remove next_bit
      /// \param data byte to remove nextbit from
      /// \return byte without nextbit set
      uint8_t inline remove_next_bit(uint8_t data) { return data & 0b01111111; }
    }  // namespace tools

    /// Decode varint types
    /// \tparam T type to decode to
    /// \param data pointer to data \sideeffect will increment data pointer
    /// \param size size of data
    template <typename T>
    T decode_varint(uint8_t*& data, size_t size) {
      T rval = 0;
      std::size_t i = 0;
      do {
        rval += tools::remove_next_bit(*data) << i * 7;
      } while (++i < size && tools::has_next(*data++));

      if constexpr (std::is_base_of<Types::s_tag_c, T>::value) {
        if (!(rval & 0x1)) {
          return rval >> 1;
        }
        return (rval >> 1) ^ (~0);
        // rval = (rval << 1) ^ (rval >> 31);
      }
      return rval;
    }

  }  // namespace type
  namespace impl {
    ///Decode a field of message
    template <typename X, size_t N, size_t... seq>
    bool decode(X& x, uint8_t*& buffer, size_t buffer_size) {
      auto fd = Types::FieldDescriptor(*buffer++);
      if (!Types::checkType<typename std::tuple_element<N, X>::type>(fd.wt))
        return false;
      switch (fd.wt) {
        case Types::WireType::varint:
          std::get<N>(x) =
              typedecode::decode_varint<typename std::tuple_element<N, X>::type>(
                  buffer, buffer_size);
          break;
        case Types::WireType::bit32:
          break;
        case Types::WireType::bit64:
          break;
        case Types::WireType::start_group:
          return false;
        case Types::WireType::end_group:
          return false;
        case Types::WireType::length_delimited:
          break;
      }

      if constexpr (sizeof...(seq) != 0)
        return decode<X, seq...>(x, buffer, buffer_size);

      return true;
    };
    /// Forward decoding to @see decode
    template <typename X, size_t... seq>
    bool decode(X& x, uint8_t*& buffer, size_t buffer_size,
                std::index_sequence<seq...>) {
      return decode<X, seq...>(x, buffer, buffer_size);
    }
  }  // namespace impl

  /// Decode an Message
  /// \tparam T @see Message childclass
  /// \param buffer Pointer to begin of buffer containing T packaet
  /// \param size Size of afore mentioned buffer
  template <typename T,  ///@cond DEV don't document internal type param
            typename Indices = std::make_index_sequence<
                std::tuple_size<typename T::data_t>::value>  ///@endcond
            >
  std::optional<T> decode(uint8_t* buffer, size_t size) {
    T retval;
    if (impl::decode(retval.data, buffer, size, Indices{})) return retval;
    return std::nullopt;
  }
}  // namespace mupb
