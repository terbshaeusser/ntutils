#!/bin/bash

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"

mode=$1
if [ -z "${mode}" ]; then
    mode=Debug
fi
cmake_args="-DCMAKE_BUILD_TYPE=${mode}"

target=$2
build_dir="${DIR}/build"
if [ -n "${target}" ]; then
    cmake_args="${cmake_args} -DCMAKE_TOOLCHAIN_FILE=${DIR}/toolchains/${target}.cmake"
    build_dir="${build_dir}-${target}"
fi

mkdir -p "${build_dir}"
cd "${build_dir}"
cmake ${cmake_args} "${DIR}"
make -j4
