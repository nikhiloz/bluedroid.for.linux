# Roadmap

Future direction and planned improvements for BlueLinux.

## Vision

Transform BlueLinux from a functional legacy codebase into a modern, well-maintained Bluetooth stack suitable for production use on contemporary Linux systems and embedded devices.

## Status Overview

- **Current Version:** 2.0.0 (last major update: 6 years ago)
- **Compatibility:** Android 6.0-era Bluetooth stack
- **Maintenance Status:** Legacy codebase seeking modernization
- **Community:** Growing - contributions welcome

---

## Short-Term (Q1-Q2 2026)

### Infrastructure

- [ ] **GitHub Actions CI/CD Pipeline** (CRITICAL)
  - Auto-build on Ubuntu 22.04, Fedora 37, Debian 12
  - Run unit tests automatically
  - Detect regressions early
  - Build status badges

- [ ] **Code Quality Gates**
  - Integration with static analyzers (clang, cppcheck)
  - Memory leak detection in CI
  - Compiler warning elimination
  - Code coverage reporting

### Documentation

- [x] Comprehensive README.md
- [x] Quick Start Guide
- [x] Contributing Guidelines
- [x] Architecture Documentation
- [x] API Reference
- [x] Build Instructions
- [ ] Module-by-module deep dives
  - [ ] L2CAP implementation details
  - [ ] HCI layer communication
  - [ ] Profile state machines
  - [ ] Thread safety model

### Example Applications

- [ ] **Device Discovery Demo**
  - Scan and list nearby devices
  - Clean UI/output
  
- [ ] **A2DP Audio Receiver**
  - Simple audio sink example
  - Stream audio from phone
  - Display metadata

- [ ] **GATT BLE Server**
  - Minimal server implementation
  - Simple characteristic read/write

- [ ] **SPP Serial Bridge**
  - Serial port emulation over Bluetooth

### Build System Modernization

- [ ] Update CMake to 3.20+ standards
- [ ] Remove deprecated CMake features
- [ ] Cross-platform testing (Linux, macOS, Android/Termux)
- [ ] Docker build environment (optional)

---

## Medium-Term (Q3-Q4 2026)

### Bluetooth Specification Updates

- [ ] **Bluetooth 5.1 Support**
  - BLE Extended Advertisements
  - Enhanced Data Rate
  - Improved power management
  - LE Coded PHY

- [ ] **Bluetooth 5.2+ Features** (Phase 2)
  - Enhanced Synchronization Establishment
  - LE Power Control
  - Periodic Advertising with Responses

### Core Stack Improvements

- [ ] **Performance Optimization**
  - Profile memory footprint analysis
  - Buffer management improvements
  - Reduce allocations in hot paths
  - Benchmark key operations

- [ ] **Error Handling & Robustness**
  - Comprehensive error recovery
  - Handle controller resets gracefully
  - Improve timeout mechanisms
  - Add circuit breaker patterns

- [ ] **Security Enhancements**
  - LE Secure Connections improvements
  - Key derivation security audit
  - Encryption validation
  - Secure pairing documentation

### Profile Enhancements

- [ ] **A2DP Improvements**
  - SBC codec optimization
  - AAC codec support
  - aptX/aptX HD support (if license-compatible)
  - LDAC support

- [ ] **BLE/GATT Expansion**
  - Service discovery caching
  - Enhanced property discovery
  - Characteristic batching

- [ ] **HID Enhancements**
  - Better input report handling
  - Device capability detection
  - Output report feedback

### Testing Infrastructure

- [ ] **Unit Test Framework**
  - Mock HCI layer
  - Profile-specific tests
  - State machine verification
  - Thread safety tests

- [ ] **Integration Tests**
  - Real device testing
  - Profile interaction tests
  - Stress testing (many connections)

- [ ] **Regression Suites**
  - Automated regression detection
  - Hardware compatibility matrix

---

## Long-Term (2027+)

### Major Refactoring

- [ ] **Modernize C Codebase**
  - Adopt C11/C17 features safely
  - Replace deprecated APIs
  - Memory safety improvements
  - Use of static analysis

- [ ] **Architecture Evolution**
  - Plugin-based profile system
  - Dynamic feature loading
  - Microkernel-like design (optional)
  - Protocol handler abstraction

### New Features

- [ ] **Bluetooth Mesh Support**
  - Low Energy Mesh (optional component)
  - Node/Relay/Proxy roles

- [ ] **DBus Interface**
  - Socket-based remote access
  - SystemD integration
  - Systemwide daemon mode

- [ ] **Bluetooth Localization**
  - Direction Finding (DF)
  - High-accuracy positioning
  - AoA/AoD support

### Platform Expansion

- [ ] **Windows Support** (experimental)
- [ ] **SystemD Integration**
  - Daemon mode service file
  - Socket activation
  - Journal logging

- [ ] **Container/Kubernetes Support**
  - Docker image
  - K8s manifests
  - Host device passthrough

### Documentation & Community

- [ ] **Video Tutorials**
  - Getting started series
  - Profile-specific guides
  - Debugging walkthroughs

- [ ] **Doxygen API Documentation**
  - Auto-generated API docs
  - Callgraph generation
  - Dependency diagrams

- [ ] **Online Demo/Playground**
  - Browser-based simulator
  - Virtual Bluetooth devices
  - Interactive learning

---

## Known Limitations (To Address)

| Issue | Priority | Target | Status |
|-------|----------|--------|--------|
| No CI/CD pipeline | HIGH | Q2 2026 | Planned |
| Deprecated CMake | HIGH | Q1 2026 | In Progress |
| A2DP codec limited | MEDIUM | Q3 2026 | Planned |
| Memory optimization | MEDIUM | Q3 2026 | Planned |
| BLE ext. ads missing | MEDIUM | Q3 2026 | Planned |
| No stress testing | MEDIUM | Q4 2026 | Planned |
| Limited documentation | LOW | Q2 2026 | In Progress |
| macOS support limited | LOW | Q4 2026 | Planned |

---

## Contribution Opportunities

### High Impact (Quality-of-life)
- [ ] CI/CD setup (GitHub Actions)
- [ ] Example applications
- [ ] Performance profiling tools
- [ ] Better error messages

### Medium Impact (Features)
- [ ] Bluetooth 5.1+ support
- [ ] Additional codecs (AAC, aptX)
- [ ] BLE Extended Advertisements
- [ ] Mesh support

### Low Impact but Valuable
- [ ] Documentation improvements
- [ ] Code cleanup
- [ ] Compiler warning fixes
- [ ] Platform-specific optimizations

**Interested?** See [CONTRIBUTING.md](../CONTRIBUTING.md) to get started!

---

## Release Schedule

### v2.1.0 (Q2 2026)
- CI/CD pipeline
- Modernized build system
- Documentation improvements
- Bug fixes

### v2.2.0 (Q3 2026)
- Bluetooth 5.1 features
- Performance optimizations
- Additional example apps
- Enhanced error handling

### v3.0.0 (2027)
- Major refactoring
- Modern C standards
- Plugin architecture
- Extended platform support

---

## Decision Log

### Why Not Fork BlueZ Instead?

Bluedroid offers:
- Simpler, modular architecture (better for embedded)
- Profile-first design (vs. protocol-centric)
- Smaller memory footprint
- Easier to understand and modify

BlueZ is more complete but heavier. This project targets:
- Embedded systems
- Custom implementations
- Educational purposes
- IoT applications

### Why Continue This Project?

1. **Community Value** - Different niche than BlueZ
2. **Learning Resource** - Educational value is high
3. **Customization** - Easier to modify for specific needs
4. **Simplicity** - Smaller codebase to understand
5. **Independence** - Not tied to Android releases

---

## Goals & Metrics

### Success Criteria

- [ ] 100+ GitHub stars
- [ ] 10+ active contributors
- [ ] Weekly updates (2026)
- [ ] 80%+ code coverage
- [ ] Zero critical vulnerabilities
- [ ] Documentation completeness >90%

### Community Metrics

- [ ] Responsive issue handling (<48 hours)
- [ ] Monthly release cycle
- [ ] Active discussion forum/issues
- [ ] Example projects from community

---

## Questions or Suggestions?

Open a GitHub issue with the label `roadmap` to discuss:
- Feature requests
- Priority adjustments
- Timeline feedback
- Partnership opportunities

---

**Last Updated:** February 2026  
**Next Review:** May 2026
