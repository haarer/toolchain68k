# gcc152 Release Notes

## Changes since gcc132

### Toolchain Updates
- **GCC 15.2.0** (up from 13.2.0)
- **Binutils 2.46.0** (up from 2.42)
- **Newlib 4.6.0.20260123** (up from 4.4.0.20231231)
- **GDB 17.1** (up from 14.2)
- **MPFR 4.2.2**, **ISL 0.27** (updated)

### GMP GCC 15 Compatibility
- Added `gmp-gcc-15.patch` to fix GMP build under GCC 15 (C23 inline semantics).

### Build Script Fixes
- `CXXFLAGS` set to `-std=gnu++17` for GCC 15 host compiler compatibility
- `--disable-werror` added to binutils configure
- `-Wno-error=implicit-function-declaration` added to target CFLAGS to handle newer GCC warnings
- `set -o pipefail` and proper error logging added to all build stages (configure, make, install)
- `MAKEJOBS` bumped from 16 to 24
- `EXECUTEABLESUFFIX` dropped (no more `.exe` handling)

### Platform Support
- **Dropped: Windows (MSYS2)** — Windows build job and all MSYS-related code removed
- **Dropped: PlatformIO examples from Alpine job**
- **Alpine Linux** — New CI build job using `jirutka/setup-alpine` chroot on Ubuntu runner, producing `toolchain-*-alpine-gcc-15.2.0.tar.gz` artifacts
- Linux build job artifact names now prefixed with `linux-` (e.g. `linux-m68k-elf`)

### CI/CD
- GitHub Releases automation — added `release` job using `softprops/action-gh-release@v2` triggered on tags
- `actions/upload-artifact` upgraded from v2 to v4
- Fixed artifact naming collisions between Linux and Alpine builds
- Fixed shell syntax bug in Alpine job setup
