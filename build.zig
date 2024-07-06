const std = @import("std");

const examples = [_][]const u8{
    "examples/cells.cpp",
    "examples/colors.cpp",
    "examples/cursor_position.cpp",
    "examples/hello_world.cpp",
    "examples/keys.cpp",
    "examples/raw_escapes.cpp",
    "examples/red_blue_panes.cpp",
};


pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const lib = b.addStaticLibrary(.{
        .name = "rawterm",
        .target = target,
        .optimize = optimize,
    });

    lib.linkLibCpp();
    lib.root_module.addIncludePath(b.path(""));
    lib.addCSourceFiles(.{
        .flags = &.{"-Wall", "-Wextra", "-std=c++20"},
        .files = &.{
            "rawterm/color.cpp",
            "rawterm/core.cpp",
            "rawterm/cursor.cpp",
            "rawterm/extras/extras.cpp",
            "rawterm/screen.cpp",
            "rawterm/text.cpp",
        }
    });

    for (examples) |example| {
        const exec = b.addExecutable(.{
            // .name = "hello-world",
            .name = std.fs.path.stem(example),
            .target = b.host,
        });

        exec.linkLibrary(lib);
        exec.root_module.addIncludePath(b.path(""));
        exec.addCSourceFile(.{
            .file = b.path(example),
            .flags = &.{ "-std=c++20" }
        });

        b.installArtifact(exec);
    }
}
