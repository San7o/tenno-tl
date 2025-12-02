.PHONY: format docs meson-update check

format:
	find include src tests benchmarks fuzz -iname "*.cpp" -o -iname "*.hpp" | xargs clang-format -i

docs:
	doxygen docs/doxygen.conf

meson-update:
	meson subprojects update

check:
	cppcheck --enable=all include/tenno/* --suppress=unusedFunction -I include  --suppress=missingIncludeSystem --quiet --error-exitcode=1
