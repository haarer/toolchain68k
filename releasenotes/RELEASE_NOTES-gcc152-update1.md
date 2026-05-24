# gcc152-update1 Release Notes

## Changes since gcc152

### New Features
- **Install-to-/opt** — New `install` action copies the built toolchain to `/opt/<name>` with an appropriate `PATH` hint. Install path simplified (drops redundant arch prefix).
- **Automatic CPU detection** — `MAKEJOBS` now uses `nproc` instead of a hardcoded 24, adapting to the host's available cores.

### ARM Target Improvements
- **Multilib support** — ARM target switched from hardcoded `--with-cpu`/`--with-fpu` flags to `--with-multilib-list=rmprofile`, enabling proper multilib builds for the Cortex-M family.

### Host Compiler Compatibility
- **GCC 16 fixes** — `CXXFLAGS` changed to `-fno-char8_t` (from `-std=gnu++17`) to build against GCC 16's new `char8_t` default.
- **Dynamic GCC version detection** — Hardcoded `13.2.0` path replaced with `$GCC_SHORTVER`, correctly supporting the GCC 15.2.0 in use.

### Robustness
- **Error handling** — Added `set -o pipefail` so pipeline failures are caught. All critical steps (clone, download, configure, build, install) now print descriptive error messages and exit on failure.
- **Cleanup** — Log removal uses `-f` to avoid errors on missing files.
