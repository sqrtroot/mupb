.PHONY: all clang_format clang_tidy clang_tidy_fix

FIND = find . -iname *.h -o -iname *.c -o -iname *.cpp -o -iname *.hpp  -exec

all:

clang_format:
	$(FIND) clang-format --verbose --style=file -i {} \;

clang_tidy:
	$(FIND) clang-tidy {} -- $(tr '\n' ' ' < .clang_complete) \;

clang_tidy_fix:
	$(FIND) clang-tidy {} -fix -- $(tr '\n' ' ' < .clang_complete) \;
