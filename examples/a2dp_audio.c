/*
 * A2DP Audio Streaming Example
 * 
 * Demonstrates how to establish A2DP connection and control audio playback.
 * Compile: gcc -I../include a2dp_audio.c -o a2dp_demo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Type definitions */
typedef enum {
    BT_AV_STATE_IDLE = 0,
    BT_AV_STATE_OPENING = 1,
    BT_AV_STATE_OPEN = 2,
    BT_AV_STATE_STARTED = 3,
    BT_AV_STATE_PAUSED = 4
} bt_av_state_t;

typedef struct {
    uint8_t address[6];
    char name[256];
    bt_av_state_t state;
} bt_audio_device_t;

/* Global state */
bt_audio_device_t current_device = {0};

/* Connection state callback */
void on_av_connection_state_changed(int state, const char *device_name) {
    printf("\n[AV Connection State Change]\n");
    printf("Device: %s\n", device_name);
    
    switch (state) {
        case BT_AV_STATE_IDLE:
            printf("State: Disconnected\n");
            break;
        case BT_AV_STATE_OPENING:
            printf("State: Connecting...\n");
            break;
        case BT_AV_STATE_OPEN:
            printf("State: Connected\n");
            break;
        case BT_AV_STATE_STARTED:
            printf("State: Playing\n");
            break;
        case BT_AV_STATE_PAUSED:
            printf("State: Paused\n");
            break;
        default:
            printf("State: Unknown (%d)\n", state);
    }
}

/* Audio codec callback */
void on_audio_codec_config(const char *codec_name, int sample_rate, int channels) {
    printf("\n[Audio Configuration]\n");
    printf("Codec: %s\n", codec_name);
    printf("Sample Rate: %d Hz\n", sample_rate);
    printf("Channels: %d\n", channels);
}

/* Simulated A2DP operations */
int simulate_a2dp_connect(const uint8_t *bd_addr, const char *device_name) {
    printf("\nConnecting to A2DP device: %s\n", device_name);
    printf("Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
           bd_addr[0], bd_addr[1], bd_addr[2],
           bd_addr[3], bd_addr[4], bd_addr[5]);
    
    /* Simulate connection delay */
    sleep(1);
    
    printf("Device connected!\n");
    on_av_connection_state_changed(BT_AV_STATE_OPEN, device_name);
    
    memcpy(current_device.address, bd_addr, 6);
    strcpy(current_device.name, device_name);
    current_device.state = BT_AV_STATE_OPEN;
    
    return 0;
}

int simulate_a2dp_play(void) {
    printf("\nStarting audio playback...\n");
    
    /* Simulate codec negotiation */
    on_audio_codec_config("SBC", 44100, 2);
    
    sleep(1);
    printf("Playback started\n");
    on_av_connection_state_changed(BT_AV_STATE_STARTED, current_device.name);
    current_device.state = BT_AV_STATE_STARTED;
    
    return 0;
}

int simulate_a2dp_pause(void) {
    printf("\nPausing audio playback...\n");
    sleep(1);
    printf("Playback paused\n");
    on_av_connection_state_changed(BT_AV_STATE_PAUSED, current_device.name);
    current_device.state = BT_AV_STATE_PAUSED;
    
    return 0;
}

int simulate_a2dp_disconnect(void) {
    printf("\nDisconnecting from device: %s\n", current_device.name);
    sleep(1);
    printf("Device disconnected\n");
    on_av_connection_state_changed(BT_AV_STATE_IDLE, current_device.name);
    memset(&current_device, 0, sizeof(current_device));
    
    return 0;
}

int main(int argc, char *argv[]) {
    uint8_t speaker_address[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
    
    printf("Bluedroid A2DP Audio Streaming Example\n");
    printf("======================================\n\n");
    
    printf("Initializing Bluetooth stack...\n");
    // btif_init();
    // btif_enable();
    
    printf("Registering callbacks...\n");
    // btif_av_register_connection_callback(on_av_connection_state_changed);
    // btif_av_register_audio_config_callback(on_audio_codec_config);
    
    /* Connect to audio device */
    printf("\n--- Scenario: Connect to Bluetooth Speaker ---\n");
    simulate_a2dp_connect(speaker_address, "My Bluetooth Speaker");
    
    /* Display device info */
    printf("\nDevice Information:\n");
    printf("Name: %s\n", current_device.name);
    printf("Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
           current_device.address[0], current_device.address[1],
           current_device.address[2], current_device.address[3],
           current_device.address[4], current_device.address[5]);
    
    /* Playback control */
    printf("\n--- Scenario: Audio Playback Control ---\n");
    simulate_a2dp_play();
    
    printf("\nPlaying audio stream for 5 seconds...\n");
    sleep(5);
    
    simulate_a2dp_pause();
    
    printf("\nPaused for 3 seconds...\n");
    sleep(3);
    
    /* Resume playback */
    printf("\nResuming playback...\n");
    simulate_a2dp_play();
    
    printf("\nPlaying for 3 more seconds...\n");
    sleep(3);
    
    /* Disconnect */
    simulate_a2dp_disconnect();
    
    printf("\n--- Summary ---\n");
    printf("A2DP connection workflow completed successfully.\n");
    printf("\nIn production, this would:\n");
    printf("  1. Connect to remote A2DP device\n");
    printf("  2. Negotiate audio codec (SBC, AAC, aptX, etc.)\n");
    printf("  3. Setup L2CAP data channel\n");
    printf("  4. Stream audio data via AVDTP protocol\n");
    printf("  5. Respond to play/pause/stop commands\n");
    printf("  6. Handle disconnection gracefully\n");
    
    return 0;
}
