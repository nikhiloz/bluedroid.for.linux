---
name: Bug Report
about: Report a bug or issue with BlueLinux
title: "[BUG] "
labels: 'bug'
assignees: ''

---

## Description
**Describe the bug clearly and concisely:**
<!-- What is the problem? -->

## Steps to Reproduce
**How can we reproduce the issue?**
1. Build command:
```bash
# Your build command here
```
2. Device setup:
   - Bluetooth hardware: 
   - Linux distro:
   - Kernel version:
3. Code to trigger the bug:
```c
// Minimal reproducible example
```

## Expected Behavior
**What should happen:**

## Actual Behavior
**What actually happens:**

## Error Messages / Logs
```
Paste relevant error messages, stack traces, or logs here.
Include dmesg output:
dmesg | grep -i bluetooth
```

## Environment
- **OS:** Ubuntu 22.04 / Fedora 37 / etc.
- **Kernel:** 5.15.0 / etc. (check: `uname -r`)
- **Compiler:** GCC 11 / Clang 14 / etc. (check: `gcc --version`)
- **CMake/Make:** CMake 3.20+ or GNU Make 4.3
- **Bluetooth Hardware:** Broadcom / Qualcomm / Intel / etc.

## Attempted Solutions
**What have you already tried?**
- [ ] Clean build (`make clean`)
- [ ] Latest main branch
- [ ] Different compiler
- [ ] Checked existing issues

## Additional Context
**Any other context that might help?**
- Configuration changes
- Custom patches applied
- Related hardware issues

---

**Note:** Before filing, please check:
- Similar issues are already reported
- You're using latest code from `main` branch
- Error occurs consistently
