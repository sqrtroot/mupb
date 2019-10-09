#include <catch2/catch.hpp>
#include <mupb/types>

SCENARIO("The correct wiretype is returned for each type"){
    GIVEN("A data type") {
        THEN("The correct wiretype should be returned") {
            /* Table from https://developers.google.com/protocol-buffers/docs/encoding
             * Type	Meaning	            Used For
             * 0	Varint	            int32, int64, uint32, uint64, sint32, sint64, bool, enum
             * 1	64-bit	            fixed64, sfixed64, double
             * 2	Length-delimited	string, bytes, embedded messages, packed repeated fields
             * 3	Start group	        groups (deprecated)
             * 4	End group	        groups (deprecated)
             * 5	32-bit	            fixed32, sfixed32, float
             */
            using namespace mupb::Types;
            REQUIRE(getWireType<int32>()  == WireType::varint);
            REQUIRE(getWireType<int64>()  == WireType::varint);
            REQUIRE(getWireType<uint32>() == WireType::varint);
            REQUIRE(getWireType<uint64>() == WireType::varint);
            REQUIRE(getWireType<sint32>() == WireType::varint);
            REQUIRE(getWireType<sint64>() == WireType::varint);
            REQUIRE(getWireType<bool>()   == WireType::varint);

            enum       TestEnum{};
            enum class TestEnumClass{};
            REQUIRE(getWireType<TestEnum>()      == WireType::varint);
            REQUIRE(getWireType<TestEnumClass>() == WireType::varint);

            REQUIRE(getWireType<fixed64>() == WireType::bit64);
            REQUIRE(getWireType<sfixed64>() == WireType::bit64);
            REQUIRE(getWireType<double>() == WireType::bit64);

            REQUIRE(getWireType<fixed32>() == WireType::bit32);
            REQUIRE(getWireType<sfixed32>() == WireType::bit32);
            REQUIRE(getWireType<float>() == WireType::bit32);
        }
    }
}
