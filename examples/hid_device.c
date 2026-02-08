/*
 * HID (Human Interface Device) Example
 * 
 * Demonstrates how to connect to and handle HID devices like keyboards and mice.
 * Compile: gcc -I../include hid_device.c -o hid_demo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* HID device type enum */
typedef enum {
    HID_UNKNOWN = 0,
    HID_KEYBOARD = 1,
    HID_MOUSE = 2,
    HID_JOYSTICK = 3,
    HID_GAMEPAD = 4
} hid_device_type_t;

/* HID input report */
typedef struct {
    uint8_t report_type;
    uint8_t data[8];
    int length;
} hid_input_report_t;

/* HID device info */
typedef struct {
    uint8_t address[6];
    char name[256];
    hid_device_type_t type;
    int battery_level;
    int is_connected;
} hid_device_info_t;

/* HID input report callback */
void on_hid_input_report(hid_device_info_t *device, hid_input_report_t *report) {
    printf("[HID Input Report] Device: %s\n", device->name);
    
    switch (device->type) {
        case HID_KEYBOARD: {
            printf("Key Press: 0x%02x\n", report->data[0]);
            if (report->data[0] >= 0x04 && report->data[0] <= 0x26) {
                printf("(Alphanumeric key)\n");
            }
            break;
        }
        case HID_MOUSE: {
            int dx = (int8_t)report->data[0];
            int dy = (int8_t)report->data[1];
            int buttons = report->data[2];
            printf("Movement: X=%+d, Y=%+d\n", dx, dy);
            printf("Buttons: %s%s%s\n",
                   (buttons & 0x01) ? "Left " : "",
                   (buttons & 0x02) ? "Right " : "",
                   (buttons & 0x04) ? "Middle" : "");
            break;
        }
        case HID_JOYSTICK:
        case HID_GAMEPAD: {
            printf("Button Code: 0x%02x\n", report->data[0]);
            printf("Axis X: %d, Axis Y: %d\n", 
                   report->data[1], report->data[2]);
            break;
        }
        default:
            printf("Raw Data: ");
            for (int i = 0; i < report->length; i++) {
                printf("%02x ", report->data[i]);
            }
            printf("\n");
    }
}

/* HID connection state callback */
void on_hid_connection_state(hid_device_info_t *device, int connected) {
    printf("\n[HID Connection State]\n");
    printf("Device: %s\n", device->name);
    printf("State: %s\n", connected ? "Connected" : "Disconnected");
    printf("Type: ");
    
    switch (device->type) {
        case HID_KEYBOARD:
            printf("Keyboard");
            break;
        case HID_MOUSE:
            printf("Mouse");
            break;
        case HID_JOYSTICK:
            printf("Joystick");
            break;
        case HID_GAMEPAD:
            printf("Game Controller");
            break;
        default:
            printf("Unknown");
    }
    printf("\n");
    
    if (connected && device->battery_level >= 0) {
        printf("Battery Level: %d%%\n", device->battery_level);
    }
}

/* Simulate HID device connection */
void simulate_hid_connection(hid_device_info_t *device) {
    printf("HID Device: %s\n", device->name);
    printf("Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
           device->address[0], device->address[1], device->address[2],
           device->address[3], device->address[4], device->address[5]);
    printf("Type: ");
    
    switch (device->type) {
        case HID_KEYBOARD:
            printf("Keyboard\n");
            break;
        case HID_MOUSE:
            printf("Mouse\n");
            break;
        case HID_JOYSTICK:
            printf("Joystick\n");
            break;
        case HID_GAMEPAD:
            printf("Game Controller\n");
            break;
        default:
            printf("Unknown\n");
    }
    
    printf("\nAttempting connection...\n");
    sleep(1);
    device->is_connected = 1;
    on_hid_connection_state(device, 1);
}

/* Simulate user input */
void simulate_keyboard_input(hid_device_info_t *device) {
    printf("\n--- Keyboard Input Simulation ---\n");
    
    const char *keys[] = {"A (0x04)", "B (0x05)", "C (0x06)", "Enter (0x28)"};
    
    for (int i = 0; i < 4; i++) {
        hid_input_report_t report = {0};
        report.report_type = 1;
        report.data[0] = 0x04 + i;  /* Key code */
        report.length = 8;
        
        printf("\nUser pressed key: %s\n", keys[i]);
        on_hid_input_report(device, &report);
        sleep(1);
    }
}

void simulate_mouse_input(hid_device_info_t *device) {
    printf("\n--- Mouse Movement Simulation ---\n");
    
    struct {
        int dx, dy;
        int buttons;
        const char *action;
    } movements[] = {
        {10, 5, 0x00, "Move right"},
        {-5, 10, 0x00, "Move left-down"},
        {0, 0, 0x01, "Click left button"},
        {-8, -8, 0x00, "Move up-left"},
        {0, 0, 0x02, "Click right button"},
    };
    
    for (int i = 0; i < 5; i++) {
        hid_input_report_t report = {0};
        report.report_type = 1;
        report.data[0] = movements[i].dx;
        report.data[1] = movements[i].dy;
        report.data[2] = movements[i].buttons;
        report.length = 8;
        
        printf("\nUser action: %s\n", movements[i].action);
        on_hid_input_report(device, &report);
        sleep(1);
    }
}

int main(int argc, char *argv[]) {
    printf("Bluedroid HID Device Example\n");
    printf("============================\n\n");
    
    printf("Initializing Bluetooth stack...\n");
    // btif_init();
    // btif_enable();
    
    /* Device 1: Wireless Keyboard */
    printf("\n--- Scenario 1: Wireless Keyboard ---\n");
    hid_device_info_t keyboard = {0};
    keyboard.address[0] = 0xAA; keyboard.address[1] = 0xBB;
    keyboard.address[2] = 0xCC; keyboard.address[3] = 0xDD;
    keyboard.address[4] = 0xEE; keyboard.address[5] = 0xFF;
    strcpy(keyboard.name, "Wireless Keyboard");
    keyboard.type = HID_KEYBOARD;
    keyboard.battery_level = 85;
    
    simulate_hid_connection(&keyboard);
    simulate_keyboard_input(&keyboard);
    
    printf("\n[HID Disconnect] Keyboard disconnected\n");
    keyboard.is_connected = 0;
    on_hid_connection_state(&keyboard, 0);
    
    /* Device 2: Wireless Mouse */
    printf("\n\n--- Scenario 2: Wireless Mouse ---\n");
    hid_device_info_t mouse = {0};
    mouse.address[0] = 0x11; mouse.address[1] = 0x22;
    mouse.address[2] = 0x33; mouse.address[3] = 0x44;
    mouse.address[4] = 0x55; mouse.address[5] = 0x66;
    strcpy(mouse.name, "Wireless Mouse");
    mouse.type = HID_MOUSE;
    mouse.battery_level = 72;
    
    simulate_hid_connection(&mouse);
    simulate_mouse_input(&mouse);
    
    printf("\n[HID Disconnect] Mouse disconnected\n");
    mouse.is_connected = 0;
    on_hid_connection_state(&mouse, 0);
    
    /* Summary */
    printf("\n\n--- Summary ---\n");
    printf("HID device connection workflow completed.\n\n");
    printf("Key Features Demonstrated:\n");
    printf("  • Device connection/disconnection\n");
    printf("  • Device type detection\n");
    printf("  • Battery level reporting\n");
    printf("  • Input report handling\n");
    printf("  • Protocol-specific data parsing\n");
    printf("\nIn production, this would:\n");
    printf("  1. Connect to HID over L2CAP (Control/Interrupt channels)\n");
    printf("  2. Get HID descriptor to understand device capabilities\n");
    printf("  3. Set report mode and idle time\n");
    printf("  4. Process incoming HID reports\n");
    printf("  5. Handle control commands (LED, rumble, etc.)\n");
    printf("  6. Manage battery notifications\n");
    
    return 0;
}
