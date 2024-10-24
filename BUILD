cc_library(
    name = "libtenno",
    srcs = glob([
        "src/*.cpp",
        "include/tenno/*.hpp"
    ]),
    hdrs = glob([
        "include/tenno/*.hpp"
    ]),
    includes = ["include"],
    copts = [
        "-Iinclude",
        "-std=c++23",
        "-O3",
        "-ffast-math",
        "-march=native",
        "-Wall",
        "-Wextra",
        "-Werror",
        "-Wno-unused-parameter",
        "-Wno-unused-variable",
        "-Wno-unused-function",
    ],
    visibility = ["//visibility:public"],
)
