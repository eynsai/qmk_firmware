#pragma once

#include "quantum.h"

#define INTERCEPT_DEFINE(_index, _function) [_index] = { .function = _function, .active = false }

typedef struct intercept_t {   
    bool (*function)(uint16_t, bool);  // returns whether or not to interrupt
    bool auto_off;
    bool active;
} intercept_t;

extern intercept_t intercepts[];
extern const size_t n_intercepts;

bool intercept_process_record_task(uint16_t keycode, bool pressed);
void intercept_on(size_t index);
void intercept_off(size_t index);
bool intercept_is_on(size_t index);
