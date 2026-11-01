#!/bin/sh
# Builds pst_chiaki.dll (chiaki-ng's C core + C ABI + H.264 decoder) with MSYS2 MinGW64, using the
# same chiaki options as scripts/build_chiaki_core.sh. Run in an MSYS2 MINGW64 shell.
# Output: windows/native/out/ (the DLL and the MinGW runtime DLLs it needs).
set -eu
here=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
build_dir="$here/build"
out_dir="$here/out"

cmake -S "$here" -B "$build_dir" -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build "$build_dir" --target pst_chiaki --parallel 8

mkdir -p "$out_dir"
cp "$build_dir/pst_chiaki.dll" "$out_dir/"
# Copy the MinGW DLLs it loads, and theirs (not the Windows ones).
copy_deps() {
    for name in $(objdump -p "$1" | sed -n 's/.*DLL Name: //p'); do
        dll="$MINGW_PREFIX/bin/$name"
        if [ -f "$dll" ] && [ ! -f "$out_dir/$name" ]; then
            cp "$dll" "$out_dir/"
            copy_deps "$dll"
        fi
    done
}
copy_deps "$build_dir/pst_chiaki.dll"
ls "$out_dir"
