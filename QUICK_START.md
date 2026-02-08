# Quick Start Guide

Get BlueLinux running in minutes!

## Prerequisites

```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake libpthread-stubs0-dev libasound2-dev

# Fedora/RHEL
sudo dnf install gcc g++ cmake alsa-lib-devel

# macOS
brew install cmake
```

## Build (Choose One)

### Option 1: CMake (Recommended)
```bash
cd BlueLinux
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Option 2: GNU Make
```bash
cd BlueLinux
make -j$(nproc)
```

## Test the Build

```bash
# Run test suite
make test

# Verify HCI binaries compiled
ls -la ./bin/
```

## First Steps

### 1. Check Bluetooth Hardware
```bash
# See connected Bluetooth devices
hciconfig -a
```

### 2. Run Discovery Example
```bash
# Start device scanning (see README.md -> Usage Examples)
./bin/demo_discovery
```

### 3. Enable Bluetooth
```bash
# Power on your Bluetooth adapter
sudo hciconfig hci0 up
```

## Common Issues

**Build fails with "CMake not found"**
```bash
sudo apt-get install cmake
```

**ALSA library warnings**
- Safe to ignore if you don't need audio features
- Install `libasound2-dev` to suppress

**"Permission denied" on /dev/ttyUSB0**
```bash
sudo usermod -a -G dialout $USER
# Log out and log back in
```

**HCI device not found**
```bash
# Check if Bluetooth kernel modules are loaded
lsmod | grep bluetooth

# Load them if missing
sudo modprobe bluetooth
```

## Next Steps

- Read [README.md](README.md) for detailed architecture and API info
- Check [examples/](examples/) for code samples
- See [CONTRIBUTING.md](CONTRIBUTING.md) for development setup

## Need Help?

- Open a GitHub issue: https://github.com/nikhiloz/bluedroid.for.linux/issues
- Review [TROUBLESHOOTING](README.md#troubleshooting) section in README
- Check kernel logs: `dmesg | grep -i bluetooth`

---

**Happy Bluetooth Hacking! 🎵**
