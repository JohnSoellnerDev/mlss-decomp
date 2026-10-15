#!/bin/sh
# Install the toolchain into tools/ (nothing system-wide, so the build runs the same on any
# x86-64 Linux the project is copied to):
#   tools/agbcc/      agbcc, the GCC 2.95 based compiler of Nintendo's GBA SDK (pret/agbcc)
#   tools/binutils/   GNU binutils for arm-none-eabi, statically linked
#   tools/bin/ninja   ninja
#   tools/objdiff-cli objdiff (diffs and the progress report)
#   tools/pylib/      capstone for Python (tools/disasm.py, which generates asm/ from the ROM)
# Needs: a host C compiler, make, curl, git, xz, unzip, sha256sum. Takes a few minutes (binutils).
#
#     tools/setup.sh            # everything that is missing
#     JOBS=8 tools/setup.sh
set -e
cd "$(dirname "$0")/.."
ROOT=$PWD
JOBS=${JOBS:-2}
WORK=${TMPDIR:-/tmp}/mlss-toolchain-$$
BINUTILS_VERSION=2.43.1
NINJA_VERSION=1.12.1
OBJDIFF_VERSION=v3.8.2
AGBCC_REPO=https://github.com/pret/agbcc.git
AGBCC_COMMIT=da598c1
CAPSTONE_WHEEL=https://files.pythonhosted.org/packages/15/3e/3a4f45dd4eda6fc8051f76bf3ed50ead6040d827000371211fbaaf057625/capstone-5.0.7-py3-none-manylinux_2_17_x86_64.manylinux2014_x86_64.whl
CAPSTONE_SHA256=b809a9654844ce0d35099121a851ddd2ab2689df1ff6687037babcedcaae6391
mkdir -p "$WORK" tools/bin
trap 'rm -rf "$WORK"' EXIT

if [ ! -x tools/binutils/bin/arm-none-eabi-as ]; then
    echo "== binutils $BINUTILS_VERSION"
    curl -fsSL "https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.xz" | tar xJ -C "$WORK"
    mkdir "$WORK/binutils-build"
    (cd "$WORK/binutils-build" &&
        "$WORK/binutils-$BINUTILS_VERSION/configure" --target=arm-none-eabi --prefix="$ROOT/tools/binutils" \
            --disable-nls --disable-werror --disable-gdb --disable-gdbserver --disable-sim --disable-gprof \
            --disable-gprofng --disable-plugins --disable-shared --enable-static --without-zstd \
            --without-debuginfod >/dev/null &&
        make -j"$JOBS" MAKEINFO=true configure-host >/dev/null &&
        make -j"$JOBS" MAKEINFO=true LDFLAGS=-all-static >/dev/null &&
        make MAKEINFO=true install-strip >/dev/null)
fi
export PATH="$ROOT/tools/binutils/bin:$PATH"

if [ ! -x tools/bin/ninja ]; then
    echo "== ninja $NINJA_VERSION"
    curl -fsSL -o "$WORK/ninja.zip" "https://github.com/ninja-build/ninja/releases/download/v$NINJA_VERSION/ninja-linux.zip"
    unzip -q -o "$WORK/ninja.zip" -d tools/bin
fi

if [ ! -x tools/objdiff-cli ]; then
    echo "== objdiff-cli $OBJDIFF_VERSION"
    curl -fsSL -o tools/objdiff-cli "https://github.com/encounter/objdiff/releases/download/$OBJDIFF_VERSION/objdiff-cli-linux-x86_64"
    chmod +x tools/objdiff-cli
fi

if [ ! -d tools/pylib/capstone ]; then
    echo "== capstone"
    curl -fsSL -o "$WORK/capstone.whl" "$CAPSTONE_WHEEL"
    echo "$CAPSTONE_SHA256  $WORK/capstone.whl" | sha256sum -c --quiet
    unzip -q -o "$WORK/capstone.whl" -d tools/pylib  # pure Python + its own libcapstone.so
fi

if [ ! -x tools/agbcc/bin/agbcc ]; then
    echo "== agbcc ($AGBCC_COMMIT)"
    git clone -q "$AGBCC_REPO" "$WORK/agbcc"
    (cd "$WORK/agbcc" && git checkout -q "$AGBCC_COMMIT" && sh build.sh >/dev/null 2>&1 && sh install.sh "$ROOT" >/dev/null)
fi

echo "toolchain ready: $(tools/binutils/bin/arm-none-eabi-as --version | head -1), ninja $(tools/bin/ninja --version), $(tools/objdiff-cli --version)"
