.PHONY: format docs meson-update

format:
	find include src tests benchmarks fuzz -iname "*.cpp" -o -iname "*.hpp" | xargs clang-format -i

docs:
	doxygen doxygen.conf

meson-update:
	meson subprojects update
