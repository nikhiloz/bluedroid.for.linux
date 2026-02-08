# Build Instructions

Detailed build guide for various platforms and configurations.

## Table of Contents

- [Quick Build](#quick-build)
- [Ubuntu/Debian](#ubuntudebian)
- [Fedora/RHEL](#fedorarhel)
- [macOS](#macos)
- [Raspberry Pi](#raspberry-pi)
- [Android/Termux](#androidtermux)
- [CMake Options](#cmake-options)
- [Troubleshooting](#troubleshooting)

---

## Quick Build

For the impatient:

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

---

## Ubuntu/Debian

### Installation

```bash
# Ubuntu 22.04 LTS (recommended)
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    libpthread-stubs0-dev \
    libasound2-dev \
    pkg-config

# Verify installations
gcc --version      # Should be 11+
cmake --version    # Should be 3.16+
```

### Build Steps

```bash
cd BlueLinux
mkdir build && cd build

# Default build
cmake ..
make -j$(nproc)

# Optional: Install to system (requires sudo)
sudo make install
```

### Running Tests

```bash
# Within build/
make test

# Or with ctest
ctest -V
```

---

## Fedora/RHEL

### Installation

```bash
# Fedora 37+ / RHEL 9+
sudo dnf update -y
sudo dnf install -y \
    gcc \
    g++ \
    cmake \
    alsa-lib-devel \
    pkgconfig

# Verify
gcc --version
cmake --version
```

### Build Steps

```bash
cd BlueLinux
mkdir build && cd build
cmake ..
make -j$(nproc)
```

---

## macOS

### Installation

```bash
# Install Homebrew if not present
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake

# Optional: For audio support
brew install alsa-lib
```

### Build Steps

```bash
cd BlueLinux
mkdir build && cd build
cmake .. -DCMAKE_OSX_DEPLOYMENT_TARGET=10.15
make -j$(nproc)
```

**Note:** Bluetooth hardware access on macOS requires special system permissions. Full functionality may not be available due to OS restrictions.

---

## Raspberry Pi

### Prerequisites

```bash
# Raspberry Pi OS (Bullseye/Bookworm)
sudo apt-get update
sudo apt-get upgrade -y
sudo apt-get install -y \
    build-essential \
    cmake \
    git \
    libpthread-stubs0-dev \
    libasound2-dev
```

### Build Steps

```bash
git clone https://github.com/nikhiloz/BlueLinux.git
cd BlueLinux
mkdir build && cd build

# Pi optimization (ARMv7/ARMv8)
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_C_FLAGS="-march=native -O3" ..
make -j$(nproc)
```

### Hardware Setup

```bash
# Enable Bluetooth
sudo raspi-config
# Select: Interface Options → Bluetooth → Enable

# Verify hardware
hciconfig -a

# Power on Bluetooth
sudo hciconfig hci0 up

# Auto-start with boot
echo "hciconfig hci0 up" | sudo tee /etc/rc.local
```

### Performance Tips

- Use `-O3` optimization for single-core Pi
- Reduce verbosity for lighter logging
- Consider using `-static` for smaller binaries

---

## Android/Termux

### Termux Environment

```bash
# Install Termux from F-Droid (not Google Play)
pkg update
pkg install clang cmake git

# Optional: For audio
pkg install alsa-lib
```

### Build Steps

```bash
git clone https://github.com/nikhiloz/BlueLinux.git
cd BlueLinux
mkdir build && cd build

cmake -DCMAKE_C_COMPILER=clang ..
make -j$(nproc)
```

**Limitations:** Android's Bluetooth stack integration may limit functionality. This is primarily useful for development and testing.

---

## CMake Options

### Configuration

```bash
mkdir build && cd build

# Standard options
cmake [OPTIONS] ..
make -j$(nproc)
```

### Common Options

| Option | Values | Default | Description |
|--------|--------|---------|-------------|
| `CMAKE_BUILD_TYPE` | Debug, Release, RelWithDebInfo | Debug | Optimization level |
| `ENABLE_AUDIO` | ON, OFF | ON | Enable A2DP audio |
| `ENABLE_GATT` | ON, OFF | ON | Enable BLE/GATT |
| `ENABLE_TESTS` | ON, OFF | ON | Build test suite |
| `ENABLE_SANITIZERS` | ON, OFF | OFF | Memory/UBsan checks |
| `CMAKE_INSTALL_PREFIX` | /path | /usr/local | Installation path |

### Example Configurations

**Minimal Build (embedded):**
```bash
cmake -DCMAKE_BUILD_TYPE=Release \
      -DENABLE_AUDIO=OFF \
      -DENABLE_TESTS=OFF \
      -DCMAKE_C_FLAGS="-Os" ..
```

**Development Build (max debugging):**
```bash
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DENABLE_SANITIZERS=ON \
      -DENABLE_ASAN=ON \
      -DENABLE_UBSAN=ON ..
```

**Optimized Build (performance):**
```bash
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_C_FLAGS="-O3 -march=native -DNDEBUG" ..
```

**Static Linking (portable binary):**
```bash
cmake -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_SHARED_LIBS=OFF \
      -DCMAKE_EXE_LINKER_FLAGS="-static" ..
```

---

## Build Outputs

After successful build:

```
build/
├── bin/                    # Compiled executables
│   ├── hci_test
│   ├── discovery_demo
│   └── ...
├── lib/                    # Libraries (if enabled)
│   ├── libbluedroid.a     # Static library
│   └── libbluedroid.so    # Shared library
├── CMakeFiles/            # Build metadata
└── Makefile
```

### Running Executables

```bash
# From build directory
./bin/discovery_demo

# Or set LD_LIBRARY_PATH for shared libraries
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
./bin/some_test
```

---

## Cleaning & Rebuilding

### Clean Build

```bash
cd build
make clean
make -j$(nproc)
```

### Full Rebuild

```bash
rm -rf build/
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Remove Installation

```bash
sudo make uninstall
```

---

## Cross-Compilation

### ARM (for embedded systems)

```bash
# Create toolchain file: toolchain-arm.cmake
# Then:
cmake -DCMAKE_TOOLCHAIN_FILE=toolchain-arm.cmake ..
```

Example toolchain file:
```cmake
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
# Set sysroot and other flags
```

---

## Troubleshooting

### CMake Not Found

```bash
# Ubuntu/Debian
sudo apt-get install cmake

# Fedora
sudo dnf install cmake

# macOS
brew install cmake

# Verify
cmake --version
```

### GCC/Compiler Issues

```bash
# Check compiler
gcc --version

# Install latest GCC
# Ubuntu
sudo apt-get install gcc-11 g++-11
# Then tell CMake
cmake -DCMAKE_C_COMPILER=gcc-11 -DCMAKE_CXX_COMPILER=g++-11 ..

# Use Clang instead
cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ..
```

### Missing Dependencies

```bash
# Check for missing packages
for pkg in cmake gcc g++ make; do
    which $pkg || echo "Missing: $pkg"
done

# ALSA library not found
pkg-config --cflags alsa

# Install ALSA dev
sudo apt-get install libasound2-dev
```

### Build Fails with "permission denied"

```bash
# Fix file permissions
chmod +x BlueLinux/
cd build

# Rebuild
make clean
make -j$(nproc)
```

### Out of Memory During Build

```bash
# Reduce parallel jobs
make -j2        # Use 2 cores instead of all

# Or disable certain features
cmake -DENABLE_TESTS=OFF ..
```

### HCI Device Not Found

This is not a build issue, but a hardware/kernel issue:

```bash
# Check Bluetooth kernel modules
lsmod | grep bluetooth

# Load them
sudo modprobe bluetooth

# Check device
hciconfig -a

# Power on
sudo hciconfig hci0 up
```

### Tests Fail to Run

```bash
# Check if tests were built
ls -la build/bin/test_*

# Run with verbose output
ctest -V

# Run specific test
ctest -R test_name -VV

# With valgrind
ctest -D ExperimentalMemCheck
```

---

## Next Steps

- See [QUICK_START.md](../QUICK_START.md) for running examples
- See [API_REFERENCE.md](API_REFERENCE.md) for API documentation
- See [DEVELOPMENT.md](DEVELOPMENT.md) for development setup
- Check [ARCHITECTURE.md](ARCHITECTURE.md) for system design

---

**Build successful?** Check out the [Quick Start Guide](../QUICK_START.md)! 🎉
