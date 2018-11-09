// clang-format off
#pragma once
#include <mupb/types>
#include <mupb/message>

struct TwoInts : public mupb::Message<mupb::Types::sint32, mupb::Types::sint64>{
  inline mupb::Types::sint32& a(){return std::get<0>(data);}
  inline mupb::Types::sint64& b(){return std::get<1>(data);}
};
