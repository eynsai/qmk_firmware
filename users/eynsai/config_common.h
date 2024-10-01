#pragma once

// ============================================================================
// DISABLE FEATURES
// ============================================================================

#define NO_ACTION_TAPPING
#define NO_ACTION_ONESHOT

// ============================================================================
// MOUSE PASSTHROUGH
// ============================================================================

#define MOUSE_EXTENDED_REPORT
#define WHEEL_EXTENDED_REPORT

#define POINTING_DEVICE_HIRES_SCROLL_ENABLE
#define POINTING_DEVICE_HIRES_SCROLL_MULTIPLIER 120
#define POINTING_DEVICE_HIRES_SCROLL_EXPONENT 1

#define POINTING_DEVICE_DRAGSCROLL_ENABLE
#define DRAGSCROLL_APPLIED_AFTER_POINTING_DEVICE_TASK_KB
#define DRAGSCROLL_THROTTLE_MS 16
#define DRAGSCROLL_TIMEOUT_MS 500
#define DRAGSCROLL_MULTIPLIER_H 0.03
#define DRAGSCROLL_MULTIPLIER_V -0.03
#define DRAGSCROLL_SMOOTHING 5
#define DRAGSCROLL_AXIS_SNAPPING_THRESHOLD 25
#define DRAGSCROLL_AXIS_SNAPPING_RATIO 2.0
#define DRAGSCROLL_ACCELERATION
#define DRAGSCROLL_ACCELERATION_BLEND 0.872116
#define DRAGSCROLL_ACCELERATION_SCALE 500.0

#define MOUSE_DELAY_DURATION 20

// ============================================================================
// TAPPING TERMS
// ============================================================================

#define DEFAULT_TAP_TERM 175
#define DEFAULT_MULTITAP_TERM 175

// ============================================================================
// RGB ANIMATIONS
// ============================================================================

#define RGB_MAX_ANIMATION_SIZE 5
#define RGB_UPDATE_INTERVAL 20
#define RGB_MAX_VAL 100

#define RGB_BREATHING_SPEED 2000
#define RGB_BREATHING_HUE_AMPLITUDE 10

#define RGB_STATIC_DUMMY_DURATION 1000

#define RGB_BASE_FROM_WORKMAN_START_DURATION 50
#define RGB_BASE_FROM_WORKMAN_HOLD_DURATION 50
#define RGB_BASE_FROM_WORKMAN_FADE_DURATION 500
#define RGB_BASE_TO_WORKMAN_START_DURATION 50
#define RGB_BASE_TO_WORKMAN_FADE_DURATION 100

#define RGB_ONESHOT_ON_FIRST_START_DURATION 50
#define RGB_ONESHOT_ON_FIRST_HOLD_DURATION 50
#define RGB_ONESHOT_ON_FIRST_FADE_DURATION 500
#define RGB_ONESHOT_ON_EXTRA_START_DURATION 200
#define RGB_ONESHOT_ON_EXTRA_HOLD_DURATION 300
#define RGB_ONESHOT_ON_EXTRA_FADE_DURATION 500
#define RGB_ONESHOT_OFF_START_DURATION 50
#define RGB_ONESHOT_OFF_FADE_DURATION 100

// ============================================================================
// DISABLE BUILT-IN RGB ANIMATIONS
// ============================================================================

#undef RGBLIGHT_EFFECT_BREATHING
#undef RGBLIGHT_EFFECT_RAINBOW_MOOD
#undef RGBLIGHT_EFFECT_RAINBOW_SWIRL
#undef RGBLIGHT_EFFECT_SNAKE
#undef RGBLIGHT_EFFECT_KNIGHT
#undef RGBLIGHT_EFFECT_CHRISTMAS
#undef RGBLIGHT_EFFECT_STATIC_GRADIENT
#undef RGBLIGHT_EFFECT_RGB_TEST
#undef RGBLIGHT_EFFECT_ALTERNATING
#undef RGBLIGHT_EFFECT_TWINKLE

#undef ENABLE_RGB_MATRIX_ALPHAS_MODS
#undef ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#undef ENABLE_RGB_MATRIX_BREATHING
#undef ENABLE_RGB_MATRIX_CYCLE_ALL
#undef ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#undef ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
#undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN
#undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
#undef ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#undef ENABLE_RGB_MATRIX_DUAL_BEACON
#undef ENABLE_RGB_MATRIX_RAINBOW_BEACON
#undef ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#undef ENABLE_RGB_MATRIX_RAINDROPS
#undef ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
#undef ENABLE_RGB_MATRIX_TYPING_HEATMAP
#undef ENABLE_RGB_MATRIX_DIGITAL_RAIN
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#undef ENABLE_RGB_MATRIX_SPLASH
#undef ENABLE_RGB_MATRIX_MULTISPLASH
#undef ENABLE_RGB_MATRIX_SOLID_SPLASH
#undef ENABLE_RGB_MATRIX_SOLID_MULTISPLASH