# API Reference

Complete API documentation for bluedroid.for.linux development.

## Core Initialization

### btif_init()
**Header:** `btif/btif_api.h`
```c
bt_status_t btif_init(void);
```
**Description:** Initialize the Bluetooth Interface layer. Must be called before any other BTIF APIs.
**Returns:** `BT_STATUS_SUCCESS` on success, `BT_STATUS_FAIL` otherwise
**Thread Safety:** Not thread-safe, call from main thread only
**Example:**
```c
if (btif_init() != BT_STATUS_SUCCESS) {
    fprintf(stderr, "Failed to initialize BTIF\n");
    return -1;
}
```

### btif_enable()
**Header:** `btif/btif_api.h`
```c
bt_status_t btif_enable(void);
```
**Description:** Enable the Bluetooth adapter. Powers on the Bluetooth controller.
**Returns:** `BT_STATUS_SUCCESS` on success
**Prerequisites:** `btif_init()` must be called first
**Note:** Initialization may take 1-5 seconds
**Example:**
```c
btif_enable();
sleep(2); // Wait for initialization
```

### btif_disable()
**Header:** `btif/btif_api.h`
```c
bt_status_t btif_disable(void);
```
**Description:** Disable the Bluetooth adapter. Powers off the Bluetooth controller.
**Returns:** `BT_STATUS_SUCCESS` on success
**Effect:** Closes all active connections

---

## Device Discovery

### start_discovery()
**Header:** `btif/btif_dm.h`
```c
bt_status_t start_discovery(void);
```
**Description:** Start inquiry scan to discover nearby devices.
**Duration:** Typically 10-15 seconds
**Callback:** Calls registered discovery callback with results
**Returns:** `BT_STATUS_SUCCESS` if inquiry started
**Example:**
```c
bt_status_t status = start_discovery();
// Inquiry will complete and call:
// void discovery_complete_callback(int num_devices, bt_device_t *devices)
```

### cancel_discovery()
**Header:** `btif/btif_dm.h`
```c
bt_status_t cancel_discovery(void);
```
**Description:** Stop ongoing device discovery.
**Returns:** `BT_STATUS_SUCCESS` if inquiry stopped
**Note:** Can be called during active inquiry

---

## Connection Management

### create_bond()
**Header:** `btif/btif_dm.h`
```c
bt_status_t create_bond(bt_bdaddr_t *bd_addr);
```
**Parameters:**
- `bd_addr`: Bluetooth address of remote device (6 bytes)
**Description:** Initiate pairing with remote device.
**Callback:** Calls bonding_complete_callback on completion
**Returns:** `BT_STATUS_SUCCESS` if pairing started
**Example:**
```c
bt_bdaddr_t addr = {{0x11, 0x22, 0x33, 0x44, 0x55, 0x66}};
create_bond(&addr);
```

### disconnect()
**Header:** `btif/btif_dm.h`
```c
bt_status_t disconnect(bt_bdaddr_t *bd_addr);
```
**Description:** Disconnect from remote device.
**Returns:** `BT_STATUS_SUCCESS` if disconnect initiated
**Callback:** Calls connected_callback with state=FALSE

### get_connection_state()
**Header:** `btif/btif_dm.h`
```c
bt_connection_state_t get_connection_state(bt_bdaddr_t *bd_addr);
```
**Returns:** 
- `BT_ACL_STATE_CONNECTED`
- `BT_ACL_STATE_DISCONNECTED`
**Note:** Only reflects ACL connection, not profile-specific states

---

## Profile APIs

### A2DP (Audio Distribution Profile)

#### btif_av_open()
```c
bt_status_t btif_av_open(bt_bdaddr_t *bd_addr);
```
Opens A2DP connection to audio sink/source.

#### btif_av_play()
```c
bt_status_t btif_av_play(void);
```
Start streaming audio.

#### btif_av_pause()
```c
bt_status_t btif_av_pause(void);
```
Pause audio streaming.

#### btif_av_close()
```c
bt_status_t btif_av_close(void);
```
Close A2DP connection.

**Example:**
```c
// Connect and stream
btif_av_open(&remote_addr);
sleep(1);
btif_av_play();
sleep(30); // Stream for 30 seconds
btif_av_pause();
btif_av_close();
```

### HFP (Hands-Free Profile)

#### btif_hf_client_connect()
```c
bt_status_t btif_hf_client_connect(bt_bdaddr_t *bd_addr);
```
Connect to hands-free device (phone).

#### btif_hf_client_make_call()
```c
bt_status_t btif_hf_client_make_call(const char *number);
```
Initiate voice call.

#### btif_hf_client_end_call()
```c
bt_status_t btif_hf_client_end_call(void);
```
End active call.

### HID (Human Interface Device)

#### btif_hh_connect()
```c
bt_status_t btif_hh_connect(bt_bdaddr_t *bd_addr);
```
Connect to HID device (keyboard, mouse).

#### btif_hh_disconnect()
```c
bt_status_t btif_hh_disconnect(bt_bdaddr_t *bd_addr);
```
Disconnect HID device.

### GATT (Generic Attribute Profile)

#### btif_gatt_client_scan_start()
```c
bt_status_t btif_gatt_client_scan_start(void);
```
Start BLE device scanning.

#### btif_gatt_client_scan_stop()
```c
bt_status_t btif_gatt_client_scan_stop(void);
```
Stop BLE scanning.

#### btif_gatt_client_connect()
```c
bt_status_t btif_gatt_client_connect(bt_bdaddr_t *bd_addr, bool is_direct);
```
Connect to GATT server.

---

## Callback Registration

### btif_dm_register_discovery_callback()
**Header:** `btif/btif_dm.h`
```c
void btif_dm_register_discovery_callback(discovery_complete_cb cb);
```
**Callback Signature:**
```c
typedef void (*discovery_complete_cb)(
    int num_devices, 
    bt_device_t *devices
);
```
**Example:**
```c
void my_discovery_cb(int num_devices, bt_device_t *devices) {
    printf("Discovered %d devices:\n", num_devices);
    for (int i = 0; i < num_devices; i++) {
        printf("  %02x:%02x:%02x:%02x:%02x:%02x - %s\n",
            devices[i].address.address[0],
            // ...
            devices[i].name);
    }
}

btif_dm_register_discovery_callback(my_discovery_cb);
```

### btif_dm_register_bonding_callback()
```c
void btif_dm_register_bonding_callback(bonding_complete_cb cb);
```
**Callback Signature:**
```c
typedef void (*bonding_complete_cb)(
    bt_bdaddr_t *bd_addr,
    bt_status_t status
);
```

### btif_dm_register_connection_callback()
```c
void btif_dm_register_connection_callback(connection_state_cb cb);
```
**Callback Signature:**
```c
typedef void (*connection_state_cb)(
    bt_connection_state_t state,
    bt_bdaddr_t *bd_addr
);
```

---

## Data Types

### bt_bdaddr_t
```c
typedef struct {
    uint8_t address[6];  // Little-endian format
} bt_bdaddr_t;
```
Represents a Bluetooth address (MAC address).

### bt_device_t
```c
typedef struct {
    bt_bdaddr_t address;
    char name[249];
    uint32_t cod;        // Class of Device
    int rssi;            // Signal strength
    bool bonded;
} bt_device_t;
```
Represents a discovered Bluetooth device.

### bt_status_t
```c
typedef enum {
    BT_STATUS_SUCCESS = 0,
    BT_STATUS_FAIL = 1,
    BT_STATUS_NOT_READY = 2,
    BT_STATUS_NOMEM = 3,
    BT_STATUS_BUSY = 4,
    // ...
} bt_status_t;
```

### bt_connection_state_t
```c
typedef enum {
    BT_ACL_STATE_CONNECTED = 0,
    BT_ACL_STATE_DISCONNECTED = 1,
} bt_connection_state_t;
```

---

## Error Handling

All APIs return `bt_status_t`. Always check return values:

```c
bt_status_t status = btif_enable();
if (status != BT_STATUS_SUCCESS) {
    switch (status) {
        case BT_STATUS_NOMEM:
            fprintf(stderr, "Out of memory\n");
            break;
        case BT_STATUS_NOT_READY:
            fprintf(stderr, "Bluetooth not ready\n");
            break;
        default:
            fprintf(stderr, "Unknown error: %d\n", status);
    }
    return -1;
}
```

---

## Thread Safety

**Thread-Safe APIs:**
- Profile operations (play, pause, etc.)
- Callback registration

**Not Thread-Safe:**
- `btif_init()`, `btif_enable()`, `btif_disable()`
- Device discovery start/cancel
- Bonding operations

Use synchronization primitives (mutexes) when calling from multiple threads.

---

## Configuration

**Runtime config file:** `conf/bt_config.txt`

```
# Enable/disable profiles
BtaAvDevice=Source
BtaHfpDevice=AG

# Logging
BtuStackDebugBtsnoop=true
BtSnoopFileName=/var/log/bt_snoop.log

# Codec preferences
BtaBtVersion=Bluetooth5_0
```

---

## Common Workflows

### 1. Device Discovery & Pairing
```c
btif_init();
btif_enable();

// Register callbacks
btif_dm_register_discovery_callback(on_discovery_complete);
btif_dm_register_bonding_callback(on_bonding_complete);

// Start discovery
start_discovery();

// In callback:
void on_discovery_complete(...) {
    // User selects device
    create_bond(&selected_device);
}
```

### 2. A2DP Audio Streaming
```c
// After device is bonded and connected
btif_av_open(&device_addr);

// Wait for A2DP profile to connect
btif_av_play();

// Stream audio...

btif_av_pause();
btif_av_close();
```

### 3. GATT BLE Communication
```c
btif_gatt_client_scan_start();

// In scan result callback:
btif_gatt_client_connect(&device_addr, true);

// Wait for GATT connection
// Read/write characteristics via GATT API
```

---

## See Also

- [ARCHITECTURE.md](ARCHITECTURE.md) - System design
- [BUILD.md](BUILD.md) - Compilation instructions
- [DEVELOPMENT.md](DEVELOPMENT.md) - Development setup
- Main [README.md](../README.md) - Overview
