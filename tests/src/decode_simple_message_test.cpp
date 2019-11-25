#include <catch2/catch.hpp>
#include <mupb/decode>


SCENARIO("Simple messages can be decoded", "[decode]") {
  typedef mupb::Message<
          mupb::Types::Field<1,mupb::Types::int32>,
          mupb::Types::Field<2,mupb::Types::sint32>,
          mupb::Types::Field<3,mupb::Types::uint32>,
          mupb::Types::Field<4,mupb::Types::int64>,
          mupb::Types::Field<5,mupb::Types::sint64>,
          mupb::Types::Field<6,mupb::Types::uint64>,
          mupb::Types::Field<7,mupb::Types::fixed64>,
          mupb::Types::Field<8,mupb::Types::sfixed64>,
          mupb::Types::Field<9,mupb::Types::fixed32>,
          mupb::Types::Field<10,mupb::Types::sfixed32>,
          mupb::Types::Field<11, float>> TestMessage;

  WHEN("A message is given"){

        uint8_t in[] = {0x08, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x10,
  0x03, 0x18, 0x03, 0x20, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0x01, 0x28, 0x09, 0x30, 0x06, 0x39, 0x07, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x41, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x4d, 0x09, 0x00, 0x00, 0x00, 0x55, 0xf6, 0xff, 0xff, 0xff, 0x5d, 0x8f,
  0xc2, 0x31, 0x41};
        auto f = mupb::decode<TestMessage>(in, sizeof(in));
    THEN("It should set the optional value when decoding is succesfull"){
        REQUIRE(f.has_value());
    }
    THEN("It should contain the correct values after decoding"){
        REQUIRE(*std::get<0>(f->data) == -1);
        REQUIRE(*std::get<1>(f->data) == -2);
        REQUIRE(*std::get<2>(f->data) == 3);
        REQUIRE(*std::get<3>(f->data) == -4);
        REQUIRE(*std::get<4>(f->data) == -5);
        REQUIRE(*std::get<5>(f->data) == 6);
        REQUIRE(*std::get<6>(f->data) == 7);
        REQUIRE(*std::get<7>(f->data) == -8);
        REQUIRE(*std::get<8>(f->data) == 9);
        REQUIRE(*std::get<9>(f->data) == -10);
        REQUIRE(*std::get<10>(f->data) == 11.11f);
    }
  }
}
