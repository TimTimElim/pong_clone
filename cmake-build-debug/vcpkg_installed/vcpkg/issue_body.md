Package: freetype[brotli,bzip2,core,png,zlib]:x64-windows@2.13.3

**Host Environment**

- Host: x64-windows
- Compiler: MSVC 19.43.34808.0
-    vcpkg-tool version: 2025-02-11-bec4296bf5289dc9ce83b4f5095943e44162f9c2
    vcpkg-scripts version: 4110d43398 2025-03-04 (10 hours ago)

**To Reproduce**

`vcpkg install `

**Failure logs**

```
Downloading https://gitlab.freedesktop.org//freetype/freetype/-/archive/VER-2-13-3/freetype-VER-2-13-3.tar.gz -> freetype-freetype-VER-2-13-3.tar.gz
warning: Download https://gitlab.freedesktop.org//freetype/freetype/-/archive/VER-2-13-3/freetype-VER-2-13-3.tar.gz failed -- retrying after 1000ms
warning: Download https://gitlab.freedesktop.org//freetype/freetype/-/archive/VER-2-13-3/freetype-VER-2-13-3.tar.gz failed -- retrying after 2000ms
warning: Download https://gitlab.freedesktop.org//freetype/freetype/-/archive/VER-2-13-3/freetype-VER-2-13-3.tar.gz failed -- retrying after 4000ms
error: https://gitlab.freedesktop.org//freetype/freetype/-/archive/VER-2-13-3/freetype-VER-2-13-3.tar.gz: WinHttpSendRequest failed with exit code 12030. The connection with the server was terminated abnormally
note: If you are using a proxy, please ensure your proxy settings are correct.
Possible causes are:
1. You are actually using an HTTP proxy, but setting HTTPS_PROXY variable to `https//address:port`.
This is not correct, because `https://` prefix claims the proxy is an HTTPS proxy, while your proxy (v2ray, shadowsocksr, etc...) is an HTTP proxy.
Try setting `http://address:port` to both HTTP_PROXY and HTTPS_PROXY instead.
2. If you are using Windows, vcpkg will automatically use your Windows IE Proxy Settings set by your proxy software. See: https://github.com/microsoft/vcpkg-tool/pull/77
The value set by your proxy might be wrong, or have same `https://` prefix issue.
3. Your proxy's remote server is our of service.
If you believe this is not a temporary download server failure and vcpkg needs to be changed to download this file from a different location, please submit an issue to https://github.com/Microsoft/vcpkg/issues
CMake Error at scripts/cmake/vcpkg_download_distfile.cmake:124 (message):
  Download failed, halting portfile.
Call Stack (most recent call first):
  scripts/cmake/vcpkg_from_gitlab.cmake:113 (vcpkg_download_distfile)
  buildtrees/versioning_/versions/freetype/8a2c633dcc14eaabdb31cf4637242f4e3c2f3fa2/portfile.cmake:7 (vcpkg_from_gitlab)
  scripts/ports.cmake:196 (include)



```

**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "name": "pong",
  "version-string": "1.0.0",
  "builtin-baseline": "4110d43398232f486c3fd0b74e2dc9e4ca5e2a59",
  "dependencies": [
    {
      "name": "sfml",
      "version>=": "3.0.0"
    }
  ]
}

```
</details>
