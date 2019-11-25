.PHONY: all clang_format clang_tidy clang_tidy_fix doxygen

FIND = find include -iname *.h -o -iname *.c -o -iname *.cpp -o -iname *.hpp  -exec
CLANG_OPTIONS= $(shell cat .clang_complete | sed "s/^\(.*\)$$/-extra-arg=\"\0\" /")

clang_format:
	$(FIND) clang-format --verbose --style=file -i {} \;

clang_tidy:
	$(FIND) clang-tidy {} $(CLANG_OPTIONS) \;

clang_tidy_fix:
	$(FIND) clang-tidy {} -fix $(CLANG_OPTIONS) \;

doxygen:
	convert -resize 200 mupb.svg docs/build/mupb.png
	doxygen docs/Doxyfile
