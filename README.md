MuPB
----

![Mupb logo](https://raw.githubusercontent.com/sqrtroot/mupb/master/mupb.svg?sanitize=true "Logo")

MuPB is a modern c++ implementation of googles [protcol buffers](https://developers.google.com/protocol-buffers/)
Messages are defined as child classes of the message class.
This allows generating the decode and encode functions at compile time.

MuPB is a work in progress
--------------------------
things still to do
==================

- [ ] Writing all the decode type functions
- [ ] Writing the encoding template function
      Probably looks a lot like the decode on
- [ ] Create protobuf compiler extension
- [ ] Write unit tests
- [ ] Write regression tests for:
      - [ ] Generating classes
      - [ ] Decoding. Probably best if generated by one of the official protoc compiling languages
      - [ ] Encoding
            Best compared with the official protoc implementation
- [ ] Set up automatic testing

