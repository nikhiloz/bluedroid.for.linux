# Bluedroid for Linux

A Linux port of the Bluedroid Bluetooth stack, originally developed for Android. This project brings a modular, feature-rich Bluetooth implementation to Linux systems, supporting both Classic Bluetooth and Bluetooth Low Energy (BLE) protocols.

## Overview

Bluedroid for Linux is a comprehensive Bluetooth stack designed for embedded and desktop Linux systems. It provides a modular architecture with clear separation of concerns, making it suitable for custom Bluetooth applications, system integration, and IoT devices.

**Status:** Legacy codebase (last major update 6 years ago) - Active development/modernization contributions welcome.

## Features

- **Dual-Mode Support:** Classic Bluetooth and BLE
- **Modular Architecture:** Clean separation between profiles, stack, and hardware layers
- **Multiple Profile Support:**
  - A2DP (Advanced Audio Distribution Profile) - Audio streaming
  - AVRCP (Audio/Video Remote Control Protocol) - Media control
  - HFP (Hands-Free Profile) - Voice calls
  - HID (Human Interface Device) - Keyboards, mice, joysticks
  - SPP (Serial Port Profile) - Serial communication
  - GATT (Generic Attribute Profile) - BLE services
  - GAP (Generic Access Profile) - Discovery and connection
  - And more...

- **Multiple Build Systems:** CMake and Makefile support
- **Hardware Abstraction:** Pluggable HCI drivers and platform layers

## Architecture

```
bluedroid.for.linux/
├── stack/              # Core Bluetooth stack (L2CAP, SDP, RFCOMM)
├── bta/                # Bluetooth application layer (profiles)
├── btif/               # Bluetooth interface (bridge between stack and applications)
├── hci/                # Host Controller Interface (hardware communication)
├── hardware/           # Hardware abstraction & drivers
├── udrv/               # Unified driver layer
├── gki/                # Generic Kernel Interface (threading/OS abstraction)
├── audio_a2dp_hw/      # A2DP audio processing
├── embdrv/             # Embedded drivers (codec implementations)
├── include/            # Header files and API definitions
├── conf/               # Configuration files
├── tools/              # Utility tools and diagnostics
├── test/               # Test suites and examples
└── utils/              # Utility functions
```

### Module Hierarchy

1. **Hardware Layer (bottom)**
   - `hardware/` - Device-specific code (UART, USB, etc.)
   - `udrv/` - Unified driver interface
   - `hci/` - HCI packet processing and device communication

2. **Core Stack Layer**
   - `stack/` - L2CAP (Link Layer), SDP (Service Discovery), RFCOMM, etc.
   - `gki/` - OS abstraction and threading model

3. **Application Interface Layer**
   - `btif/` - Bridge between stack and application-level profiles
   - Handles thread management and queue communication

4. **Profile Layer (top)**
   - `bta/` - Implementations of BT profiles (A2DP, AVRCP, HFP, HID, etc.)
   - Profile-specific state machines and logic

5. **Audio & Codec Support**
   - `audio_a2dp_hw/` - A2DP audio sink/source implementation
   - `embdrv/sbc/encoder` - SBC codec for audio compression

## Building

### Prerequisites

- GCC/Clang compiler
- CMake 3.10+ or GNU Make
- Linux headers (for HCI socket interface)
- ALSA development libraries (for audio features)
- pthread library

### Build with CMake (Recommended)

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Build with Makefile

```bash
make -j$(nproc)
```

### Build Specific Components

```bash
# Build with verbose output
make VERBOSE=1

# Build only core stack
make stack

# Build with debug symbols
make DEBUG=1
```

## Testing

```bash
# Run test suite
make test

# Run specific test
./build/test/test_l2cap
./build/test/test_hci

# Manual HCI testing
./tools/hcitool
```

## API Overview

### Core Stack API

#### Device Initialization

```c
// Initialize Bluetooth device
bt_status_t btif_init(void);

// Enable Bluetooth
bt_status_t btif_enable(void);

// Disable Bluetooth
bt_status_t btif_disable(void);
```

#### Device Discovery

```c
// Start inquiry scan
bt_status_t start_discovery(void);

// Stop inquiry scan
bt_status_t cancel_discovery(void);

// Set discoverable mode
bt_status_t set_discoverable(uint8_t discoverable);
```

#### Connection Management

```c
// Remote device connection
bt_status_t create_bond(bt_bdaddr_t *bd_addr);

// Disconnect
bt_status_t disconnect(bt_bdaddr_t *bd_addr);

// Get connection state
bt_connection_state_t get_connection_state(bt_bdaddr_t *bd_addr);
```

#### Profile APIs (Located in `bta/` subdirectories)

- **A2DP:** `bta/av/` - Audio streaming control
- **AVRCP:** `bta/rc/` - Remote control
- **HFP:** `bta/hf_client/` - Hands-free device
- **HID:** `bta/hh/` - Input device handling
- **SPP:** `bta/dm/` - Serial profile

### Configuration

Edit `conf/bt_config.txt` to customize:
```
BtSnoopLogOutput=false
BtuStackDebugBtsnoop=false
EnableBtSnoop=true
BtSnoopFileName=/var/log/bluetooth/btsnoop.log
```

## Known Limitations

1. **Legacy Codebase:** Last synchronized with Android 6.0+ codebase
2. **Build System:** CMake integration may require modernization for recent versions
3. **Deprecated APIs:** Some APIs follow older Bluetooth specifications
4. **Limited Documentation:** Original Android documentation may not apply directly
5. **Platform Support:** Tested primarily on older Linux kernel versions (3.x–4.x)
6. **No Active Upstream:** Not synchronized with latest Android Bluetooth stack changes

## TODO / Future Work

- [ ] Update to modern Bluetooth 5.1+ specifications
- [ ] Migrate deprecated HCI interfaces to modern kernel interfaces
- [ ] Add comprehensive unit tests and CI/CD pipeline
- [ ] Performance optimization and memory profiling
- [ ] BLE Extended Advertisement support
- [ ] Dynamic profile loading
- [ ] Enhanced logging and debugging tools
- [ ] Docker support for isolated testing
- [ ] Example applications and tutorials
- [ ] API documentation (Doxygen)

## Usage Examples

### Example 1: Simple Device Discovery

```c
#include <stdio.h>
#include "btif/btif.h"

int main() {
    // Initialize Bluetooth
    btif_init();
    btif_enable();
    
    // Start discovery
    start_discovery();
    sleep(12); // Scan for 12 seconds
    cancel_discovery();
    
    // Process discovered devices
    // (See btif_dm.c for callbacks)
    
    btif_disable();
    return 0;
}
```

### Example 2: Profile Registration (A2DP)

See `bta/av/` directory for profile state machine examples and callback registration.

## Troubleshooting

### HCI Communication Issues
- Verify Bluetooth hardware: `hciconfig -a`
- Check kernel Bluetooth drivers: `lsmod | grep bluetooth`
- Enable HCI debugging: Set `BtuStackDebugBtsnoop=true` in config

### Compilation Errors
- Ensure all headers in `include/` are accessible
- Check for missing pthread/ALSA libraries: `pkg-config --cflags alsa`
- Verify GCC version supports C99+ features

### Runtime Crashes
- Enable debug symbols: `make DEBUG=1`
- Use valgrind for memory issues: `valgrind ./bt_daemon`
- Check system logs: `dmesg | grep -i bluetooth`

## Contributing

Contributions are welcome! Areas of interest:
- Modernization of build system
- Support for newer Bluetooth specifications
- Bug fixes and performance improvements
- Documentation and examples
- Platform-specific optimizations

Please open an issue or submit a pull request with:
- Clear description of changes
- Test cases for new features
- Updated documentation

## License

[To be specified - check original Android Bluedroid license requirements]

## References

- [Bluetooth SIG Official Specifications](https://www.bluetooth.com/specifications/)
- [Android Open Source Project - Bluedroid](https://android.googlesource.com/platform/system/bt/)
- [Linux Bluetooth Stack (BlueZ)](http://www.bluez.org/)

## Support & Community

- Issues: GitHub Issues tracker
- Discussions: GitHub Discussions
- Related Projects: See [Bluetooth_Notes](https://github.com/nikhiloz/Bluetooth_Notes) for additional documentation

---

**Last Updated:** February 2026  
**Maintainer:** nikhiloz  
**Status:** Legacy - Seeking active contributors for modernization
