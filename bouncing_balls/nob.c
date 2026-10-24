#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_EXPERIMENTAL_DELETE_OLD
#define NOB_WARN_DEPRECATED
#include "./olive.c/dev-deps/nob.h"

#define COMMON_CFLAGS                                                          \
  "-Wall", "-Wextra", "-pedantic", "-ggdb", "-I.", "-I./build/", "-I./dev-deps/"

bool build_wasm_demo(Cmd *cmd, Procs *procs, const char *name) {
  cmd_append(
      cmd, "clang", COMMON_CFLAGS, "-O2", "-fno-builtin", "--target=wasm32",
      "--no-standard-libraries", "-Wl,--no-entry", "-Wl,--export=vc_render",
      "-Wl,--export=__heap_base", "-Wl,--allow-undefined", "-o",
      temp_sprintf("./build/demos/%s.wasm", name),
      "-DVC_PLATFORM=VC_WASM_PLATFORM", temp_sprintf("./%s.c", name));
  return cmd_run(cmd, .async = procs);
}

int main(int argc, char **argv) {
  NOB_GO_REBUILD_URSELF(argc, argv);

  Cmd cmd = {0};
  Procs procs = {0};

  const char *name = "test";

  if (!build_wasm_demo(&cmd, &procs, name))
    return 1;
  if (!procs_flush(&procs))
    return 2;

  const char *src_path = temp_sprintf("./build/demos/%s.wasm", name);
  const char *dst_path = temp_sprintf("./wasm/%s.wasm", name);

  if (!copy_file(src_path, dst_path))
    return 3;
  return 0;
}
