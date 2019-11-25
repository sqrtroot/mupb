#include <catch2/catch.hpp>

#include <mupb/decode>
#include <mupb/types>

SCENARIO("fixed size types can be decoded", "[decode]") {
  WHEN("A incorrect sized buffer is given"){
    THEN("It should return a nullopt"){
      uint8_t in[] = {0xA7,0xFE,0xFF, 0xFF};
      auto out = mupb::typedecode::decode_bit<mupb::Types::sfixed32>(in, 2, 4);
      REQUIRE(!out.has_value());
    }
  }

  WHEN("A correct sized buffer is given"){
    THEN("It should decode to the correct integer"){
      uint8_t in[] = {0xA7,0xFE,0xFF, 0xFF};
      auto out = mupb::typedecode::decode_bit<mupb::Types::sfixed32>(in, 4, 4);
      REQUIRE(out.has_value());
      REQUIRE(out->first ==-345);
    }
  }
}
