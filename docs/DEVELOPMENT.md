# Development Guide

Instructions for setting up your development environment and contributing.

## Development Environment Setup

### Prerequisites

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    git \
    valgrind \
    gdb \
    libpthread-stubs0-dev \
    libasound2-dev \
    pkg-config

# Fedora/RHEL
sudo dnf install -y \
    gcc \
    g++ \
    cmake \
    git \
    valgrind \
    gdb \
    alsa-lib-devel \
    pkgconfig

# macOS
brew install cmake valgrind lldb
```

### Clone & Setup

```bash
# Clone repository
git clone https://github.com/nikhiloz/bluedroid.for.linux.git
cd bluedroid.for.linux

# Create development branch
git checkout -b feature/my-feature

# Create build directory
mkdir build && cd build
cmake ..
```

## Building

### CMake Build (Recommended)

```bash
# Build with default options
cmake ..
make -j$(nproc)

# Build with debugging symbols
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)

# Build with optimization
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

# Verbose build (see actual compiler commands)
make VERBOSE=1
```

### GNU Make Build

```bash
cd ..  # Back to root
make clean
make DEBUG=1 VERBOSE=1 -j$(nproc)
```

### Build Options

**CMake Variables:**
```bash
# Enable/disable components
cmake -DBT_CLASSIC=ON -DBT_BLE=ON -DAUDIO_A2DP=ON ..

# Custom compiler
cmake -DCMAKE_C_COMPILER=gcc-11 -DCMAKE_CXX_COMPILER=g++-11 ..

# Optimization level
cmake -DCMAKE_OPT_LEVEL=-O3 ..

# Sanitizers (detect memory issues)
cmake -DENABLE_ASAN=ON -DENABLE_UBSAN=ON ..
```

## Testing

### Unit Tests

```bash
# Run all tests
cd build
make test

# Run with verbose output
ctest -V

# Run specific test
ctest -R test_hci -V

# Run with valgrind (if enabled)
ctest -D ExperimentalMemCheck
```

### Manual Testing

```bash
# Check Bluetooth hardware
hciconfig -a

# Run a demo
./bin/discovery_demo

# Monitor HCI packets
sudo btmon

# Check for memory leaks
valgrind --leak-check=full ./bin/hci_test

# Debug with GDB
gdb -ex run -ex where --args ./bin/hci_test
```

## Code Style

### C Coding Standards

**Indentation & Formatting:**
- 4 spaces per indentation level (no tabs)
- Follow Linux kernel coding style (K&R variant)
- Max line length: 100 characters

**Example:**
```c
static bt_status_t btif_av_handle_play_req(bt_av_context_t *context,
                                            int start_offset)
{
    int result;

    if (!context) {
        LOG_ERROR("Invalid context");
        return BT_STATUS_FAIL;
    }

    result = start_streaming(context, start_offset);
    if (result != 0) {
        LOG_ERROR("Failed to start streaming: %d", result);
        return BT_STATUS_FAIL;
    }

    context->state = BT_AV_STATE_STREAMING;
    return BT_STATUS_SUCCESS;
}
```

**Naming Conventions:**
```c
// Functions: lowercase_with_underscores
bt_status_t btif_av_open(bt_bdaddr_t *addr);

// Constants: UPPERCASE_WITH_UNDERSCORES
#define HCI_MAX_ACL_SIZE 1024
#define DEFAULT_TIMEOUT_MS 5000

// Typedef structs: suffix _t
typedef struct {
    uint8_t address[6];
} bt_bdaddr_t;

// Local variables: lowercase_with_underscores
int num_devices = 0;
bt_device_t *devices = NULL;
```

**Comments:**
```c
// For single-line comments

/*
 * For multi-line comments
 * explaining complex logic
 * or file-level documentation
 */

// IMPORTANT: This is critical
// TODO: Needs optimization
// FIXME: Known issue, needs fix
// XXX: Hack, temporary solution
```

**File Headers:**
```c
/*
 * Copyright (c) 2026 [Your Name/Organization]
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * See LICENSE file for details.
 */

#ifndef __BT_AV_H__
#define __BT_AV_H__

// File content

#endif
```

## Code Quality

### Static Analysis

```bash
# Clang static analyzer
scan-build make

# Cppcheck
cppcheck --enable=all src/

# Shellcheck (for scripts)
shellcheck *.sh
```

### Dynamic Analysis

```bash
# Memory leak detection
valgrind --leak-check=full --show-leak-kinds=all \
    ./bin/test_hci

# Address sanitizer (compile-time)
cmake -DENABLE_ASAN=ON ..
make

# UBSan (undefined behavior)
cmake -DENABLE_UBSAN=ON ..
make
```

### Compiling Without Warnings

Always compile with strict flags:

```bash
cmake -DCMAKE_C_FLAGS="-Wall -Wextra -Wshadow -Wpedantic -Werror" ..
make
```

## Debugging

### GDB Debugging

```bash
# Start debugger
gdb ./bin/hci_test

# (gdb) commands:
run                          # Run program
break btif_av_play           # Set breakpoint
continue                     # Continue execution
step                         # Step into function
next                         # Step over function
print variable_name          # Print variable
backtrace                    # Show call stack
info locals                  # Show local variables
quit                         # Exit GDB
```

### Remote Debugging

```bash
# On target device
gdbserver localhost:2345 ./bin/hci_test

# On development machine
gdb ./bin/hci_test
(gdb) target remote 192.168.1.100:2345
```

### Log-based Debugging

Enable debug logging:
```c
// In source file
#define ENABLE_DEBUG_LOGGING 1

// Check for debug level
if (BT_DEBUG_LEVEL >= 3) {
    fprintf(stderr, "Debug info: %s\n", variable);
}
```

Config file:
```
BtuStackDebugBtsnoop=true
BtSnoopLogOutput=true
BtSnoopFileName=/tmp/btsnoop.log
```

## Performance Profiling

### With Perf

```bash
# Compile with debug symbols
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Profile with perf
perf record -g ./bin/hci_test
perf report

# Flame graph
perf script | stackcollapse-perf.pl | flamegraph.pl > profile.svg
```

### Memory Profiling

```bash
# Massif (memory profiler)
valgrind --tool=massif ./bin/hci_test
ms_print massif.out.* | less
```

## Documentation

### Doxygen

```bash
# Generate API docs
doxygen Doxyfile

# View docs
firefox docs/html/index.html
```

### Updating Documentation

- Update [docs/API_REFERENCE.md](API_REFERENCE.md) for API changes
- Update [docs/ARCHITECTURE.md](ARCHITECTURE.md) for design changes
- Add inline code comments for complex logic
- Update README.md for user-facing changes

## Git Workflow

### Branch Strategy

```bash
# Create feature branch
git checkout -b feature/descriptive-name

# Make commits
git commit -m "fix: description of change"

# Push to fork
git push origin feature/descriptive-name

# Create pull request on GitHub
# Link related issues: Fixes #123
```

### Commit Best Practices

```bash
# Good commit messages
git commit -m "feat: Add GATT client scan support"
git commit -m "fix: Handle HCI timeout in uart driver"
git commit -m "docs: Update API reference"
git commit -m "refactor: Simplify L2CAP state machine"

# Avoid
git commit -m "fix bug"
git commit -m "WIP"
git commit -m "asdf"
```

### Keeping Fork Updated

```bash
# Add upstream
git remote add upstream https://github.com/nikhiloz/bluedroid.for.linux.git

# Fetch latest
git fetch upstream

# Rebase on main
git rebase upstream/main

# Force push (only on your branch!)
git push origin feature/my-feature --force-with-lease
```

## Debugging Common Issues

### Build Fails

```bash
# Clean build
rm -rf build
mkdir build && cd build
cmake ..
make clean
make VERBOSE=1

# Check compiler
gcc --version

# Missing dependencies
pkg-config --cflags alsa
```

### Test Failures

```bash
# Run failed test individually
ctest -R test_name -V

# With gdb
gdb --args ./bin/test_name

# With valgrind
valgrind --leak-check=full ./bin/test_name
```

### HCI Communication Issues

```bash
# Show kernel logs
dmesg | grep -i bluetooth

# Monitor HCI packets
sudo btmon

# Check device
hciconfig -a

# Reset Bluetooth
sudo hciconfig hci0 down
sudo hciconfig hci0 up
```

## IDE Integration

### Visual Studio Code

Create `.vscode/settings.json`:
```json
{
    "C_Cpp.intelliSenseEngine": "Tag Parser",
    "C_Cpp.default.includePath": [
        "${workspaceFolder}/include",
        "${workspaceFolder}/stack"
    ],
    "C_Cpp.default.defines": [
        "BT_DEBUG=1"
    ],
    "files.exclude": {
        "build": true,
        "bin": true,
        "**/*.o": true
    }
}
```

### CLion

- File → Open → Select project root
- Check "CMake" in toolchain
- Tools → CMake → Reload CMake Project

## Performance Tips

1. **Use Release builds** for profiling: `cmake -DCMAKE_BUILD_TYPE=Release`
2. **Minimize allocations** in hot paths
3. **Profile before optimizing** - use perf/valgrind
4. **Watch for callback latency** - BTIF threads can become bottleneck
5. **Buffer management** - reuse buffers, avoid fragmentation

## Contributing Back

See [CONTRIBUTING.md](../CONTRIBUTING.md) for:
- Code style requirements
- Testing expectations
- Pull request process
- Recognition policy

---

**Happy hacking! Feel free to reach out if you get stuck.** 🎯
