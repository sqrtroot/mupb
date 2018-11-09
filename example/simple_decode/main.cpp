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
/** @file main.cpp
 *  @brief Example of decoding a simple pb message
 *
 *  This example is created from the following protobuf definition
 *  message TwoInts{
 *    sint32 a = 0;
 *    sint64 b = 1;
 *  }
 *
 *  @author Robert Bezem(sqrtroot)
 */

#include <iostream>
#include <mupb/decode>

#include "TwoInts.hpp"

unsigned char test[] = {0x08, 0xd8, 0x04, 0x10, 0x9f, 0x06};
unsigned int test_len = 6;

int main() {
  if (auto x = mupb::decode<TwoInts>(test, test_len)) {
    std::cout << x->a() << " " << x->b() << "\n";
  }
}
