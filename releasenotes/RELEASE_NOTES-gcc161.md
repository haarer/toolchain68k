# gcc161 Release Notes

## Changes since gcc152-update1

### Toolchain Updates
- **GCC 16.1.0** (up from 15.2.0)
- **GDB 17.2** (up from 17.1)
- **MPC 1.4.0** (up from 1.3.1)

### ARM Target Improvements
- **Application processors** — `--with-multilib-list` expanded to include `aprofile` alongside `rmprofile`, supporting Cortex-A series in addition to Cortex-M.

### Host Compiler Compatibility
- **GCC 16 fixes** — `CXXFLAGS` uses `-fno-char8_t` to build against GCC 16's new `char8_t` default.

### Build Script Improvements
- **Automatic CPU detection** — `MAKEJOBS` uses `nproc` instead of a hardcoded value.
- **Error handling** — `set -o pipefail` added so pipeline failures are properly caught.

### CI/CD
- **Release notes fix** — Added `actions/checkout` step to the `release` job so `body_path` can find the release notes file.
- **Workflow dispatch** — Added `workflow_dispatch` trigger for manual runs.
