#!/bin/bash

set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"

"${DIR}/build.bash" $@

target=$2
build_dir="${DIR}/build"
if [ -n "${target}" ]; then
    cmake_args="${cmake_args} -DCMAKE_TOOLCHAIN_FILE=${DIR}/toolchains/${target}.cmake"
    build_dir="${build_dir}-${target}"
fi

cd "${build_dir}"
ctest --verbose
