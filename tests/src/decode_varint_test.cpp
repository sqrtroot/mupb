#include <csignal>
#include <catch2/catch.hpp>
#include <mupb/decode>
#include <mupb/types>

SCENARIO("varint's can be decoded", "[decode]") {
    GIVEN("A Unsigned Varint") {
        WHEN("It is decoded") {
            uint8_t compressed1[] = {0b00000001};

            auto decoded1 = mupb::typedecode::decode_varint<mupb::Types::uint32>(compressed1, 1);

            THEN("A value should be set"){
                REQUIRE(decoded1.has_value());
            }AND_THEN("The correct amount of bytes should be decoded"){
                REQUIRE(decoded1->second == 1);
            }AND_THEN("The correct value should be set"){
                REQUIRE(decoded1->first == 1);
            }

            uint8_t compressed2[] = {0b10101100, 0b00000010};
            auto decoded2 = mupb::typedecode::decode_varint<mupb::Types::uint32>(compressed2, 2);
            THEN("A value should be set"){
                REQUIRE(decoded2.has_value());
            }AND_THEN("The correct amount of bytes should be decoded"){
                REQUIRE(decoded2->second == 2);
            }AND_THEN("The correct value should be set"){
                REQUIRE(decoded2->first == 300);
            }
        }
    }
    GIVEN("A negative zigzag encoded varint") {
        WHEN("It is decoded") {
            uint8_t compressed[] = {0xF1, 0xC0, 0x01, 0x00, 0x00, 0x00};
            auto decoded1 = mupb::typedecode::decode_varint<mupb::Types::sint32>(compressed, 6);
            THEN("A value should be set"){
                REQUIRE(decoded1.has_value());
            }AND_THEN("The correct amount of bytes should be decoded"){
                REQUIRE(decoded1->second == 3);
            }AND_THEN("The correct value should be set"){
                REQUIRE(decoded1->first == -12345);
            }
        }
    }

    GIVEN("A positive zigzag encoded varint") {
        WHEN("It is decoded") {
            uint8_t compressed[] = {0xF2, 0xC0, 0x01, 0x00, 0x00, 0x00};
            auto decoded1 = mupb::typedecode::decode_varint<mupb::Types::sint32>(compressed, 6);
            THEN("A value should be set"){
                REQUIRE(decoded1.has_value());
            }AND_THEN("The correct amount of bytes should be decoded"){
                REQUIRE(decoded1->second == 3);
            }AND_THEN("The correct value should be set"){
                REQUIRE(decoded1->first == 12345);
            }
        }
    }
    GIVEN("A signed varint") {
        WHEN("It is decoded") {
            uint8_t compressed[] = {0xae, 0xf6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01};
            auto decoded1 = mupb::typedecode::decode_varint<mupb::Types::int32>(compressed, 13);
            THEN("A value should be set"){
                REQUIRE(decoded1.has_value());
            }AND_THEN("The correct amount of bytes should be decoded"){
                REQUIRE(decoded1->second == 10);
            }AND_THEN("The correct value should be set"){
                REQUIRE(decoded1->first == -1234);
            }
        }
    }
}
