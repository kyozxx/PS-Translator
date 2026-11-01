# pst_chiaki.dll (Remote Play)

chiaki-ng's C core (`ThirdParty/chiaki-ng`, not modified) plus a small C ABI (`pst_chiaki.c`) and
the Windows Media Foundation H.264 decoder. The app calls it through P/Invoke
(`PSTranslate/Core/RemotePlay/ChiakiNative.cs`).

Build (MSYS2, MINGW64 shell):

```sh
pacman -S --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja \
  mingw-w64-x86_64-openssl mingw-w64-x86_64-protobuf mingw-w64-x86_64-python-protobuf \
  mingw-w64-x86_64-json-c mingw-w64-x86_64-miniupnpc mingw-w64-x86_64-libevent mingw-w64-x86_64-pkgconf \
  mingw-w64-x86_64-opus mingw-w64-x86_64-SDL2
sh windows/native/build_chiaki.sh
```

Output goes to `native/out/` (the DLL and the MinGW DLLs it loads); the app project copies it next
to `PSTranslate.exe`. `python windows/native/prepare_psn_login_config.py` makes the ignored
`PSTranslate/PSNLoginConfig.json` for the browser sign-in (values are never printed or committed).

License: chiaki-ng is AGPL-3.0. Any build given to other people must come with this source
(chiaki-ng, `pst_chiaki.c`, the build scripts) under the AGPL.
