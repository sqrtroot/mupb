#!/bin/sh
protoc --encode IntegerTypes simple_message.proto < simple_message.in | xxd -i
