# Architecture Overview

## System Layers

Bluedroid for Linux is organized in **5 distinct layers**, each with clear responsibilities:

```
┌─────────────────────────────────────────────────────────┐
│              APPLICATION LAYER                          │
│  User applications using Bluetooth functionality        │
└────────────┬────────────────────────────────────────────┘
             │
┌────────────▼────────────────────────────────────────────┐
│              PROFILE LAYER (bta/)                       │
│  A2DP, AVRCP, HFP, HID, SPP, GATT, GAP implementations │
│  State machines, profile-specific logic                 │
└────────────┬────────────────────────────────────────────┘
             │
┌────────────▼────────────────────────────────────────────┐
│         APPLICATION INTERFACE (btif/)                   │
│  Bridge between profiles and core stack                 │
│  Thread management, queue handling, callbacks           │
└────────────┬────────────────────────────────────────────┘
             │
┌────────────▼────────────────────────────────────────────┐
│    CORE BLUETOOTH STACK (stack/)                        │
│  L2CAP, SDP, RFCOMM, ATT, SM                            │
│  Protocol implementations, state management             │
└────────────┬────────────────────────────────────────────┘
             │
┌────────────▼────────────────────────────────────────────┐
│    HARDWARE ABSTRACTION (hci/, udrv/, gki/)            │
│  HCI communication, driver interface, OS abstraction    │
│  UART/USB transport, threading primitives               │
└─────────────────────────────────────────────────────────┘
```

## Layer Details

### Layer 1: Hardware Abstraction Layer (HAL)

**Location:** `hardware/`, `hci/`, `udrv/`, `gki/`

**Responsibility:**
- Communicate with Bluetooth controller via HCI
- Platform-specific UART/USB/SPI drivers
- OS abstraction (threading, memory, timers)
- Device-specific implementations

**Key Components:**
```
hci/
├── hci_layer.c          # HCI packet processing
├── hci_packet_factory.c # Packet creation
└── hci_sequencer.c      # HCI initialization sequence

udrv/
├── ulinux/
│   ├── bt_hwcfg_linux.c  # Linux-specific config
│   ├── bt_vendor_lib.c   # Vendor-specific extensions
│   └── userial.c         # Serial port handling

gki/
├── gki_buffer.c          # Buffer management
├── gki_time.c            # Timer functionality
└── gki_thread.c          # Thread primitives
```

**Example: HCI Flow**
```
User App → BTIF → Stack → HCI Layer → UART Driver → Bluetooth Device
```

### Layer 2: Core Bluetooth Stack

**Location:** `stack/`

**Responsibility:**
- Implement Bluetooth protocols (L2CAP, SDP, RFCOMM, ATT, SM)
- State management for connections
- PDU parsing and generation
- Error handling and recovery

**Key Components:**
```
stack/
├── l2cap/       # Logical Link Control & Adaptation Protocol
├── smp/         # Security Manager Protocol
├── rfcomm/      # Serial emulation over L2CAP
├── sco/         # Synchronous Connection-Oriented
├── hci/         # Host Controller Interface processing
├── pan/         # Personal Area Network
└── gatt/        # Generic Attribute Profile (BLE)
```

**Example: L2CAP Channel Setup**
```c
// From bta/av/ (A2DP profile)
// 1. BTA layer requests channel
bta_av_open_req_evt(...)

// 2. Passes to BTIF
btif_av_handle_evt()

// 3. BTIF calls stack
L2CA_CreateConnectionlessChannel()

// 4. Stack manages channel with HCI layer
l2c_link_send_to_controller()
```

### Layer 3: Application Interface (BTIF)

**Location:** `btif/`

**Responsibility:**
- Bridge between high-level profiles and core stack
- Thread management (dispatch work to dedicated threads)
- Callback handling and event queuing
- Device/connection state tracking

**Key Components:**
```
btif/
├── btif_av.c          # A2DP/AVRCP interface
├── btif_hf_client.c   # Hands-free interface
├── btif_hh.c          # HID host interface
├── btif_gatt.c        # GATT interface
├── btif_dm.c          # Device Management (discovery, pairing)
└── btif_core.c        # Core BTIF initialization
```

**Thread Model:**
```
Main Thread (btif_core_native_thread)
    ├── Receives calls from apps
    ├── Dispatches to profile threads
    └── Returns via callbacks

Profile Threads
    ├── Handles profile-specific state
    ├── Communicates with stack
    └── Invokes callbacks to main thread
```

### Layer 4: Bluetooth Profiles

**Location:** `bta/`

**Responsibility:**
- Implement Bluetooth profiles (A2DP, AVRCP, HFP, HID, etc.)
- Profile-specific state machines
- Service discovery and matching
- Resource management per profile

**Key Profiles:**
```
bta/
├── av/          # A2DP (Audio Distribution), AVRCP (AV Control)
├── hf_client/   # HFP (Hands-Free Protocol)
├── hh/          # HID (Human Interface Device)
├── gattc/       # GATT Client
├── gatts/       # GATT Server
├── dm/          # Device Management (pairing, discovery)
├── pan/         # Personal Area Network
└── sdp/         # Service Discovery Protocol
```

**State Machine Example (A2DP):**
```
IDLE --[open]--> OPENING --[connect]--> OPEN --[close]--> CLOSING --> IDLE
                     |                    |                   |
                     └──[error]─---------→[error state]←─────┘
```

### Layer 5: Application Layer

**Location:** User code / `test/` / `examples/`

**Responsibility:**
- Use BTIF APIs to control Bluetooth
- Handle callbacks and events
- Manage device connections
- Implement application logic

**Example API Usage:**
```c
// Initialize
btif_init();
btif_enable();

// Start discovery
start_discovery();

// Callbacks
void discovery_complete_callback(int num_devices, bt_device_t *devices) {
    // Handle discovered devices
}

// Connect
create_bond(bd_addr);

// Use profile
btif_av_play(bd_addr);
```

## Data Flow Examples

### Example 1: A2DP Audio Streaming

```
User App
    ↓ [btif_av_play()]
BTIF (av)
    ↓ [queue_work_to_profile_thread]
BTA Profile (av/)
    ↓ [State machine: IDLE → OPEN → STREAMING]
Stack (L2CAP, AVDTP)
    ↓ [Create data channels]
HCI Layer
    ↓ [HCI_Create_Connection_cmd]
Hardware Driver (UART)
    ↓ [Serial transmission]
Bluetooth Controller
    ↓ [Sends audio packets]
Remote Device
```

### Example 2: Device Discovery & Pairing

```
User App
    ↓ [start_discovery()]
BTIF (dm)
    ↓ [Send HCI_Inquiry]
Stack (HCI)
    ↓ [Queue to device driver]
Hardware (HCI)
    ↓ [UART transmission]
Bluetooth Controller
    ↓ [Inquiry scan]
Remote Devices
    ↓ [Inquiry responses]
Bluetooth Controller
    ↓ [Report via HCI]
Hardware (UART RX)
    ↓ [Parse packets]
Stack (HCI processing)
    ↓ [Callbacks to BTIF]
BTIF (dm)
    ↓ [Callbacks to app]
User App [devices_found_callback()]
```

## Module Dependencies

```
Application
    ↓
btif/ (all profiles)
    ↓         ↓         ↓
bta/av    bta/hf    bta/hh    ... (profiles)
    ↓         ↓         ↓
    └────→ stack/av ← stack/l2cap
           stack/rfcomm
           stack/smp
           stack/sco
           stack/hci
    ↓
    hci/
    ↓
udrv/
    ↓
gki/
    ↓
Hardware (UART, USB)
```

## Memory Architecture

```
Shared Memory Regions:
├── HCI Rx Buffer (gki_buffer_pool)
│   ├── Small buffers (100-300 bytes)
│   ├── Medium buffers (600-1500 bytes)
│   └── Large buffers (2KB+)
│
├── Stack State (l2c_cb, rfc_cb)
│   ├── Connection states
│   ├── PDU tracking
│   └── Flow control
│
├── Profile State (bta_av_cb, bta_hf_cb)
│   ├── Profile instances
│   ├── Device associations
│   └── Context data
│
└── Application State (btif control)
    ├── Callbacks
    ├── Device tracking
    └── Thread queues
```

## Thread Model

```
Main Process (single-threaded traditionally)
│
├─ Dispatcher Thread
│  └─ Routes work to profile threads
│
├─ HCI Thread
│  └─ Handles device communication
│
├─ A2DP Profile Thread
│  └─ Handles audio streaming logic
│
├─ HFP Profile Thread
│  └─ Handles hands-free logic
│
└─ Other Profile Threads
   └─ Handle respective profile logic

Communication: Thread-safe queues, mutexes, semaphores (gki/)
```

## Integration Points

### 1. Hardware Interface
- **Input:** HCI events from device
- **Output:** HCI commands to device
- **Location:** `hci/`, `udrv/`

### 2. Profile Extensions
- **Input:** App-level profile APIs
- **Output:** Profile state/callbacks
- **Location:** `bta/[profile]/`

### 3. System Integration
- **ALSA (Audio):** `audio_a2dp_hw/`
- **D-Bus:** Potential DBus interface layer (not in current code)
- **Sockets:** Potential socket interface (not in current code)

## Configuration & Customization

**Build-time:**
- CMake flags for enabling/disabling profiles
- Target platform selection
- Feature gates

**Runtime:**
- `conf/bt_config.txt` - Dynamic configuration
- Profile-specific settings
- Logging levels

## Performance Characteristics

| Metric | Typical Value | Notes |
|--------|---|---|
| Memory Footprint | 2-5 MB | Varies with profile count |
| HCI Latency | <10ms | Device communication |
| Profile Latency | 10-50ms | State machine processing |
| Buffer Allocation | Pooled | Pre-allocated for low-latency |

---

For detailed API documentation, see [API_REFERENCE.md](API_REFERENCE.md).  
For development setup, see [DEVELOPMENT.md](DEVELOPMENT.md).
