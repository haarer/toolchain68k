# gcc132 Release Notes

## Changes since gcc920

### Toolchain Updates
- **GCC 13.2.0** (up from 9.2.0)
- **Binutils 2.42** (up from 2.41)
- **Newlib 4.4.0.20231231** (up from 4.2.0)
- **GDB 14.2** (up from 10.2)
- **GMP 6.3.0, MPFR 4.2.2, MPC 1.3.1, ISL 0.27** (updated)

### Platform Support
- **Linux x86_64** — glibc-based builds
- **Alpine Linux x86_64** — musl-based builds (new)
- **Windows (MSYS2)** — MinGW64 builds
- Dropped: Raspberry Pi (armv6l/armv7l/armv8l) builds

### CI/CD
- GitHub Actions CI for automated builds on tag pushes and manual dispatch
- GitHub Releases automation via `softprops/action-gh-release@v2`
- Cross-platform artifacts: Linux (glibc), Alpine (musl), Windows (MSYS2)

### Metadata
- `package.json` now includes `"tag"` field for precise release tracking
