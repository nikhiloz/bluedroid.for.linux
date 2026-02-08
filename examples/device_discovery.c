/*
 * Device Discovery Example
 * 
 * Demonstrates how to scan for nearby Bluetooth devices.
 * Compile: gcc -I../include device_discovery.c -o discovery_demo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* 
 * Mock implementation for demonstration
 * In production, link against actual BTIF libraries
 */

typedef struct {
    uint8_t address[6];
    char name[256];
    int rssi;
    int cod;
} bt_device_t;

typedef struct {
    int num_devices;
    bt_device_t devices[20];
} discovery_result_t;

/* Simulated discovery callback */
void on_discovery_complete(discovery_result_t *result) {
    if (!result || result->num_devices == 0) {
        printf("No devices discovered\n");
        return;
    }
    
    printf("\n=== Discovered Devices ===\n");
    printf("Found %d device(s):\n\n", result->num_devices);
    
    for (int i = 0; i < result->num_devices; i++) {
        printf("[%d] Device: %s\n", i, result->devices[i].name);
        printf("    Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
               result->devices[i].address[0],
               result->devices[i].address[1],
               result->devices[i].address[2],
               result->devices[i].address[3],
               result->devices[i].address[4],
               result->devices[i].address[5]);
        printf("    RSSI: %d dBm\n", result->devices[i].rssi);
        printf("    Class of Device: 0x%06x\n\n", result->devices[i].cod);
    }
}

/* Simulated discovery function */
discovery_result_t* simulate_discovery(void) {
    discovery_result_t *result = malloc(sizeof(discovery_result_t));
    
    /* Mock discovered devices */
    result->num_devices = 3;
    
    /* Device 1: Phone */
    strcpy(result->devices[0].name, "Android Phone");
    result->devices[0].address[0] = 0xAA;
    result->devices[0].address[1] = 0xBB;
    result->devices[0].address[2] = 0xCC;
    result->devices[0].address[3] = 0xDD;
    result->devices[0].address[4] = 0xEE;
    result->devices[0].address[5] = 0xFF;
    result->devices[0].rssi = -45;
    result->devices[0].cod = 0x0C010C;  /* Audio/Video device */
    
    /* Device 2: Bluetooth Speaker */
    strcpy(result->devices[1].name, "Portable Speaker");
    result->devices[1].address[0] = 0x11;
    result->devices[1].address[1] = 0x22;
    result->devices[1].address[2] = 0x33;
    result->devices[1].address[3] = 0x44;
    result->devices[1].address[4] = 0x55;
    result->devices[1].address[5] = 0x66;
    result->devices[1].rssi = -62;
    result->devices[1].cod = 0x240404;  /* Audio device (speaker) */
    
    /* Device 3: Wireless Keyboard */
    strcpy(result->devices[2].name, "Wireless Keyboard");
    result->devices[2].address[0] = 0x77;
    result->devices[2].address[1] = 0x88;
    result->devices[2].address[2] = 0x99;
    result->devices[2].address[3] = 0xAA;
    result->devices[2].address[4] = 0xBB;
    result->devices[2].address[5] = 0xCC;
    result->devices[2].rssi = -55;
    result->devices[2].cod = 0x540145;  /* Keyboard peripheral */
    
    return result;
}

int main(int argc, char *argv[]) {
    printf("Bluedroid Device Discovery Example\n");
    printf("===================================\n\n");
    
    printf("Initializing Bluetooth stack...\n");
    // btif_init();  /* Would call real BTIF in production */
    // btif_enable();
    
    printf("Starting device discovery (scan for 12 seconds)...\n");
    printf("Please wait...\n\n");
    
    // In production:
    // btif_dm_register_discovery_callback(on_discovery_complete);
    // start_discovery();
    // sleep(12);
    // cancel_discovery();
    
    /* For demo, simulate discovery */
    discovery_result_t *result = simulate_discovery();
    on_discovery_complete(result);
    
    printf("Discovery complete!\n");
    printf("\nTo connect to a device, use the address shown above.\n");
    printf("Example: create_bond(&device.address);\n");
    
    free(result);
    return 0;
}
