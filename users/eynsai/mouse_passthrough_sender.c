#include QMK_KEYBOARD_H
#include "raw_hid.h"
#include "mouse_passthrough_common.h"
#include "mouse_passthrough_sender.h"

uint8_t state;
uint8_t device_id_self;
uint8_t device_id_others[MAX_REGISTERED_DEVICES - 1];
uint8_t device_id_remote;
uint32_t last_connection_attempt_time;
uint32_t last_connection_success_time;
uint8_t message_queue[QMK_RAW_HID_REPORT_SIZE * MAX_QUEUED_MESSAGES];
uint8_t message_queue_next_empty_offset = 0;

static uint8_t current_buttons_state = 0;
static uint8_t last_buttons_sent = 0;
static uint8_t lingering_buttons_host = 0;
static uint8_t lingering_buttons_remote = 0;
static bool block_buttons_on = false;
static bool block_pointer_on = false;
static bool block_wheel_on = false;
static bool passthrough_on = false;

void mouse_passthrough_sender_matrix_scan_task(void) {

    // we can only send one raw hid message per matrix scan, anything after the first message gets garbled for some reason
    if (message_queue_next_empty_offset != 0) {
        message_queue_next_empty_offset -= QMK_RAW_HID_REPORT_SIZE;
        raw_hid_send(message_queue + message_queue_next_empty_offset, QMK_RAW_HID_REPORT_SIZE);
    }

    if (timer_elapsed32(last_connection_success_time) > HUB_CONNECTION_EXPIRY_INTERVAL) {
        state = MOUSE_PASSTHROUGH_DISCONNECTED;
    }

    if (timer_elapsed32(last_connection_attempt_time) > HUB_CONNECTION_ATTEMPT_INTERVAL) {
        last_connection_attempt_time = timer_read32();
        // send a registration report
        if (message_queue_next_empty_offset < sizeof(message_queue)) {
            memset(message_queue + message_queue_next_empty_offset, 0, QMK_RAW_HID_REPORT_SIZE);
            message_queue[message_queue_next_empty_offset + REPORT_OFFSET_COMMAND_ID] = RAW_HID_HUB_COMMAND_ID;
            message_queue[message_queue_next_empty_offset + REPORT_OFFSET_DEVICE_ID] = 0xFF;
            message_queue[message_queue_next_empty_offset + REPORT_OFFSET_REGISTRATION] = 0x01;
            message_queue_next_empty_offset += QMK_RAW_HID_REPORT_SIZE;
        }
        
        if (state == MOUSE_PASSTHROUGH_HUB_CONNECTED) {
            // handshake step 1/4: mouse broadcasts to all devices
            for (int i = 0; i < MAX_REGISTERED_DEVICES - 1; i++) {
                if (device_id_others[i] != DEVICE_ID_UNASSIGNED) {
                    if (message_queue_next_empty_offset < sizeof(message_queue)) {
                        memset(message_queue + message_queue_next_empty_offset, 0, QMK_RAW_HID_REPORT_SIZE);
                        message_queue[message_queue_next_empty_offset + REPORT_OFFSET_COMMAND_ID] = RAW_HID_HUB_COMMAND_ID;
                        message_queue[message_queue_next_empty_offset + REPORT_OFFSET_DEVICE_ID] = device_id_others[i];
                        message_queue[message_queue_next_empty_offset + REPORT_OFFSET_HANDSHAKE] = 13;
                        message_queue_next_empty_offset += QMK_RAW_HID_REPORT_SIZE;
                    }
                }
            }
        }
    }
}

bool mouse_passthrough_sender_raw_hid_receive_task(uint8_t* data) {

    if (data[REPORT_OFFSET_COMMAND_ID] != RAW_HID_HUB_COMMAND_ID) {
        return true;
    }

    last_connection_success_time = timer_read32();
    if (state == MOUSE_PASSTHROUGH_DISCONNECTED) {
        state = MOUSE_PASSTHROUGH_HUB_CONNECTED;
    }

    if (state == MOUSE_PASSTHROUGH_REMOTE_CONNECTED && data[REPORT_OFFSET_DEVICE_ID] == device_id_remote) {
        // unpack control payload
        if (!block_buttons_on && data[REPORT_OFFSET_CONTROL_BLOCK_BUTTONS]) {
            lingering_buttons_host = current_buttons_state;
        }
        if (passthrough_on && !data[REPORT_OFFSET_CONTROL_PASSTHROUGH]) {
            lingering_buttons_remote = current_buttons_state;
        }
        block_buttons_on = data[REPORT_OFFSET_CONTROL_BLOCK_BUTTONS];
        block_pointer_on = data[REPORT_OFFSET_CONTROL_BLOCK_POINTER];
        block_wheel_on = data[REPORT_OFFSET_CONTROL_BLOCK_WHEEL];
        passthrough_on = data[REPORT_OFFSET_CONTROL_PASSTHROUGH];

    } else if (data[REPORT_OFFSET_DEVICE_ID] == DEVICE_ID_HUB) {
        if (data[REPORT_OFFSET_DEVICE_ID_SELF] == DEVICE_ID_UNASSIGNED) {
            // hub has shutdown
            state = MOUSE_PASSTHROUGH_DISCONNECTED;
        } else {
            device_id_self = data[REPORT_OFFSET_DEVICE_ID_SELF];
            memcpy(device_id_others, data + REPORT_OFFSET_DEVICE_ID_OTHERS, MAX_REGISTERED_DEVICES - 1);
            if (state == MOUSE_PASSTHROUGH_REMOTE_CONNECTED) {
                // see if we're still connected
                bool found = false;
                for (int i = REPORT_OFFSET_DEVICE_ID_OTHERS; i < QMK_RAW_HID_REPORT_SIZE; i++) {
                    if (data[i] == device_id_remote) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    state = MOUSE_PASSTHROUGH_HUB_CONNECTED;
                }
            }
        }
            
    } else if (state == MOUSE_PASSTHROUGH_HUB_CONNECTED && data[REPORT_OFFSET_HANDSHAKE] == 26) {
        // handshake step 3/4: mouse responds to first keyboard it hears from
        if (message_queue_next_empty_offset < sizeof(message_queue)) {
            state = MOUSE_PASSTHROUGH_REMOTE_CONNECTED;
            device_id_remote = data[REPORT_OFFSET_DEVICE_ID];
            memset(message_queue + message_queue_next_empty_offset, 0, QMK_RAW_HID_REPORT_SIZE);
            message_queue[message_queue_next_empty_offset + REPORT_OFFSET_COMMAND_ID] = RAW_HID_HUB_COMMAND_ID;
            message_queue[message_queue_next_empty_offset + REPORT_OFFSET_DEVICE_ID] = device_id_remote;
            message_queue[message_queue_next_empty_offset + REPORT_OFFSET_HANDSHAKE] = 39;
            message_queue_next_empty_offset += QMK_RAW_HID_REPORT_SIZE;
            block_buttons_on = false;
            block_pointer_on = false;
            block_wheel_on = false;
            passthrough_on = false;
        }
    }

    return false;
}

report_mouse_t mouse_passthrough_sender_pointing_device_task(report_mouse_t mouse) {
    current_buttons_state = mouse.buttons;
    
    if (state != MOUSE_PASSTHROUGH_REMOTE_CONNECTED) {
        return mouse;
    }

    if (passthrough_on) {
        if (((mouse.buttons != last_buttons_sent) || (mouse.x != 0) || (mouse.y != 0) || (mouse.v != 0) || (mouse.h != 0)) && (message_queue_next_empty_offset < sizeof(message_queue))) {
            // send data payload
            memset(message_queue + message_queue_next_empty_offset, 0, QMK_RAW_HID_REPORT_SIZE);
            message_queue[message_queue_next_empty_offset + REPORT_OFFSET_COMMAND_ID] = RAW_HID_HUB_COMMAND_ID;
            message_queue[message_queue_next_empty_offset + REPORT_OFFSET_DEVICE_ID] = device_id_remote;
            message_queue[message_queue_next_empty_offset + REPORT_OFFSET_DATA_BUTTONS] = mouse.buttons;
            message_queue[message_queue_next_empty_offset + REPORT_OFFSET_DATA_X_MSB] = (mouse.x >> 8) & 0xFF;
            message_queue[message_queue_next_empty_offset + REPORT_OFFSET_DATA_X_LSB] = mouse.x & 0xFF;
            message_queue[message_queue_next_empty_offset + REPORT_OFFSET_DATA_Y_MSB] = (mouse.y >> 8) & 0xFF;
            message_queue[message_queue_next_empty_offset + REPORT_OFFSET_DATA_Y_LSB] = mouse.y & 0xFF;
            message_queue[message_queue_next_empty_offset + REPORT_OFFSET_DATA_V_MSB] = (mouse.v >> 8) & 0xFF;
            message_queue[message_queue_next_empty_offset + REPORT_OFFSET_DATA_V_LSB] = mouse.v & 0xFF;
            message_queue[message_queue_next_empty_offset + REPORT_OFFSET_DATA_H_MSB] = (mouse.h >> 8) & 0xFF;
            message_queue[message_queue_next_empty_offset + REPORT_OFFSET_DATA_H_LSB] = mouse.h & 0xFF;
            message_queue_next_empty_offset += QMK_RAW_HID_REPORT_SIZE;
            last_buttons_sent = mouse.buttons;
        }
    } else {
        lingering_buttons_remote &= mouse.buttons;
        if ((lingering_buttons_remote != last_buttons_sent) && (message_queue_next_empty_offset < sizeof(message_queue))) {
            memset(message_queue + message_queue_next_empty_offset, 0, QMK_RAW_HID_REPORT_SIZE);
            message_queue[message_queue_next_empty_offset + REPORT_OFFSET_COMMAND_ID] = RAW_HID_HUB_COMMAND_ID;
            message_queue[message_queue_next_empty_offset + REPORT_OFFSET_DEVICE_ID] = device_id_remote;
            message_queue[message_queue_next_empty_offset + REPORT_OFFSET_DATA_BUTTONS] = lingering_buttons_remote;
            message_queue_next_empty_offset += QMK_RAW_HID_REPORT_SIZE;
            last_buttons_sent = lingering_buttons_remote;
        }
    }

    // block inputs
    if (block_buttons_on) {
        lingering_buttons_host &= mouse.buttons;
        mouse.buttons = lingering_buttons_host;
    }
    if (block_pointer_on) {
        mouse.x = 0;
        mouse.y = 0;
    }
    if (block_wheel_on) {
        mouse.v = 0;
        mouse.h = 0;
    }
    return mouse;
}

bool is_mouse_passthrough_connected(void) {
    return state == MOUSE_PASSTHROUGH_REMOTE_CONNECTED;
}
