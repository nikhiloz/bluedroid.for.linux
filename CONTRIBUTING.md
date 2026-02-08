# Contributing to bluedroid.for.linux

Thank you for your interest in contributing! This document provides guidelines and instructions.

## Code of Conduct

- Be respectful and constructive
- Welcome all levels of experience
- No harassment, discrimination, or hostile behavior
- Report issues to: [Create GitHub issue](https://github.com/nikhiloz/bluedroid.for.linux/issues)

## How to Contribute

### 1. Report Bugs
**Found a bug?** Open an issue with:
- **Title:** Clear, concise description
- **Description:** Steps to reproduce, expected vs actual behavior
- **Environment:** Linux distro, kernel version, Bluetooth hardware
- **Logs:** `dmesg` output, compiler errors, debug logs

### 2. Suggest Features
**Have an idea?** Open an issue with:
- **Title:** Feature request - [brief description]
- **Description:** Problem solved, use case, implementation thoughts
- **Acceptance Criteria:** What "done" looks like

### 3. Submit Code

#### Setup Development Environment

```bash
# Clone and prep
git clone https://github.com/nikhiloz/bluedroid.for.linux.git
cd bluedroid.for.linux
git checkout -b feature/your-feature-name

# Build
mkdir build && cd build
cmake ..
make -j$(nproc)

# Test
make test
```

#### Coding Standards

**C Code Style:**
```c
// Use 4-space indentation
// Follow K&R/Linux kernel style

// Good naming
uint8_t device_state;
bt_status_t connect_device(bt_bdaddr_t *addr);

// Comment complex logic
// Check if vendor-specific HCI command is supported
if (hci_vendor_cmd_supported & HCI_VENDOR_CUSTOM_BIT) {
    // Send vendor command
}
```

**File Headers:**
```c
/*
 * Copyright (c) 2026 [Your Name/Organization]
 * 
 * Permission is hereby granted under Apache 2.0 License
 * See LICENSE file for details
 */
```

**Commit Messages:**
```
type: Brief description (50 chars max)

Longer explanation if needed (wrap at 72 chars).
- Bullet points are okay
- Reference issues: Fixes #123

Examples:
- fix: Correct HCI timeout handling in uart driver
- feat: Add BLE extended advertising support
- docs: Update API reference for GATT profiles
- refactor: Modernize CMakeLists.txt for CMake 3.20+
```

#### Testing

```bash
# Compile your changes
make clean && make -j$(nproc)

# Run tests
make test

# Check for memory leaks (if valgrind available)
valgrind ./bin/test_hci

# Build documentation
make doc
```

#### Pull Request Process

1. **Fork** the repository
2. **Create branch** from main: `git checkout -b feature/my-feature`
3. **Commit** with clear messages
4. **Push** to your fork: `git push origin feature/my-feature`
5. **Open PR** against `main` with:
   - Description of changes
   - Link to related issues
   - Testing performed
   - Any breaking changes noted

6. **Address feedback** - maintainers may request changes
7. **Merge** once approved by maintainers

### 4. Improve Documentation

Documentation is code! Help with:
- README.md clarifications
- API documentation
- Tutorials and examples
- Fix typos/grammar

```bash
# Documentation changes don't need code review, just:
git checkout -b docs/fix-typo
# Make changes
git commit -m "docs: Fix typo in README"
git push origin docs/fix-typo
# Open PR
```

## Development Areas

### High Priority (Help Wanted!)
- [ ] Modernize CMake build system (3.20+ compatibility)
- [ ] Update to Bluetooth 5.1+ specifications
- [ ] Optimize memory usage
- [ ] Add unit test suite
- [ ] CI/CD pipeline (GitHub Actions)

### Medium Priority
- [ ] Performance profiling tools
- [ ] Extended BLE advertising
- [ ] Additional profile implementations
- [ ] Better error handling

### Nice to Have
- [ ] Docker build environment
- [ ] Example applications
- [ ] Protocol analysis tools
- [ ] Doxygen API documentation

## Project Structure

```
bluedroid.for.linux/
├── stack/           # Core Bluetooth stack (careful here - deep logic)
├── bta/             # Profiles (good for targeted features)
├── hci/             # HCI layer (needs modernization)
├── include/         # Headers (update when APIs change)
├── tools/           # Utilities (easy contributions)
└── test/            # Tests (always welcome)
```

**Best areas for first-time contributors:**
1. Documentation
2. Test improvements
3. Tools and utilities
4. Build system enhancements

## Review & Merge

- **Code Review:** 1+ maintainer approval required
- **CI/CD:** Must pass automated tests (when available)
- **Conflicts:** Resolve before merge
- **Timeline:** Reviews typically within 1-2 weeks

## Recognition

Contributors are recognized via:
- GitHub contributors list
- CONTRIBUTORS.md file (coming soon)
- Release notes

## Questions?

- Check existing issues and pull requests
- Open a discussion: GitHub Discussions
- Email maintainer for sensitive topics

## License

By contributing, you agree that your contributions are licensed under Apache License 2.0.
All contributions must be compatible with this license.

---

**Thank you for making bluedroid.for.linux better! 🚀**

See [README.md](README.md) for more project information.
