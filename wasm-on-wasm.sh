#! /bin/sh

set -e

MAJOR=${WASI_SDK_MAJOR:-16}
MINOR=${WASI_SDK_MINOR:-0}
WASI_SDK_DIR=.wasi-sdk-${MAJOR}.${MINOR}
DIST_DIR=.dist

mkdir -p ${DIST_DIR}

fetch_wasi_sdk()
{
    UNAME=$(uname -s)
    case ${UNAME} in
    Darwin)
        PLATFORM=macos
        ;;
    Linux)
        PLATFORM=linux
        ;;
    *)
        echo "Unknown uname ${UNAME}"
        exit 1
        ;;
    esac
    TAR=wasi-sdk-${MAJOR}.${MINOR}-${PLATFORM}.tar.gz
    if [ ! -f ${DIST_DIR}/${TAR} ]; then
        URL=https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-${MAJOR}/${TAR}
        curl -L -o ${DIST_DIR}/${TAR} ${URL}
    fi
    pax -rz \
    -f ${DIST_DIR}/${TAR} \
    -s"!^wasi-sdk-${MAJOR}\.${MINOR}!${WASI_SDK_DIR}!"
}

test -d "${WASI_SDK_DIR}" || fetch_wasi_sdk

# see also: https://github.com/WebAssembly/tail-call
cmake \
-B build.wasm \
-DWASI_SDK_PREFIX=$(pwd)/${WASI_SDK_DIR} \
-DCMAKE_TOOLCHAIN_FILE=${WASI_SDK_DIR}/share/cmake/wasi-sdk.cmake \
-DBUILD_TESTING=OFF \
-DTOYWASM_USE_TAILCALL=OFF \
.
cmake --build build.wasm

cmake \
-B build.native \
-DBUILD_TESTING=OFF \
.
cmake --build build.native

EXTRA_OPTIONS=--print-stats

echo "=== native ==="
time \
./build.native/toywasm --version

echo "=== wasm ==="
time \
./build.native/toywasm ${EXTRA_OPTIONS} --wasi --wasi-dir . -- \
./build.wasm/toywasm --version

echo "=== wasm on wasm ==="
time \
./build.native/toywasm ${EXTRA_OPTIONS} --wasi --wasi-dir . -- \
./build.wasm/toywasm ${EXTRA_OPTIONS} --wasi --wasi-dir . -- \
./build.wasm/toywasm --version

echo "=== wasm on wasm on wasm ==="
time \
./build.native/toywasm ${EXTRA_OPTIONS} --wasi --wasi-dir . -- \
./build.wasm/toywasm ${EXTRA_OPTIONS} --wasi --wasi-dir . -- \
./build.wasm/toywasm ${EXTRA_OPTIONS} --wasi --wasi-dir . -- \
./build.wasm/toywasm --version
