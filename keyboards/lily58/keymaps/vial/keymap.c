#include QMK_KEYBOARD_H
#include <stdio.h>
#include "users/holykeebs/holykeebs.h"

#define QK_C_EEPROM QK_CLEAR_EEPROM

enum layer_number {
  _QWERTY = 0,
  _LOWER,
  _RAISE,
  _ADJUST,
};

#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

 [_QWERTY] = LAYOUT(
  KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_GRV,
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_MINS,
  KC_LCTL,  KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_LBRC,  KC_RBRC,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,
                      LOWER, KC_LGUI,  KC_LALT, KC_SPC,   KC_ENT,   KC_BSPC,  KC_RGUI, RAISE
),

[_LOWER] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                        _______, _______, _______,_______, _______, _______,
  _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                        KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______,
  _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                           KC_6,    KC_7,    KC_8,    KC_9,    KC_0, _______,
  _______, HK_D_MODE, KC_BTN4, KC_BTN5, KC_BTN1, KC_BTN2, HK_S_MODE,  _______, KC_PIPE, KC_GRAVE, KC_PLUS, KC_LCBR, KC_RCBR, _______,
                             _______, _______, _______, _______,      _______,  _______, _______, _______
),

[_RAISE] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                     _______, _______, _______, _______, _______, _______,
  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                       KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
  _______, _______, _______, _______, _______, _______,                     XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX,
  _______, _______, _______, _______, _______, _______,   _______, _______, KC_PLUS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS, _______,
                             _______, _______, _______,  _______, _______,  _______, _______, _______
),

  [_ADJUST] = LAYOUT(
    XXXXXXX,      XXXXXXX,    XXXXXXX,    XXXXXXX,      XXXXXXX, XXXXXXX,                        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    QK_BOOT,      HK_DUMP,    HK_SAVE,    HK_RESET,     XXXXXXX, HK_C_SCROLL,                    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,
    QK_C_EEPROM,  HK_P_SET_D, HK_P_SET_S, HK_P_SET_BUF, XXXXXXX, HK_S_MODE_T,                    KC_UP,   KC_DOWN, XXXXXXX, XXXXXXX, XXXXXXX, QK_C_EEPROM,
    KC_LSFT,      XXXXXXX,    XXXXXXX,    XXXXXXX,      XXXXXXX, HK_D_MODE_T, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                                   _______, _______, _______, _______,  _______, _______, _______, _______
  )
};

layer_state_t layer_state_set_user(layer_state_t state) {
  state = update_tri_layer_state(state, _RAISE, _LOWER, _ADJUST);
  return state;
}

#if defined(OLED_ENABLE) && !defined(HK_OLED_ENABLE)

#include "users/holykeebs/pointing.h"

#define IDLE_TIMEOUT 30000

static uint32_t idle_timer = 0;
static uint16_t prev_kc = 0;
static uint8_t  prev_row = 0;
static uint8_t  prev_col = 0;
static bool     was_idle = false;

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (!is_keyboard_master())
        return OLED_ROTATION_270;
    return rotation;
}

// Hamsa Logo - 32x48 raw bitmap, centered (1px pad each side)
static const char PROGMEM hamsa[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0,
    0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xFC, 0xF0, 0xC0, 0x00, 0x00, 0x00, 0xFF,
    0xFF, 0x00, 0x00, 0x00, 0xC0, 0xF0, 0xFC, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x08, 0x1C, 0x38, 0xF0, 0xE0, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF,
    0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xE0, 0xF0, 0x38, 0x1C, 0x08, 0x00,
    0x00, 0x18, 0x38, 0x38, 0x3F, 0xFF, 0xFF, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0x80, 0x00, 0x18, 0x3F,
    0x3F, 0x18, 0x00, 0x80, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0xFF, 0xFF, 0x3F, 0x18, 0x18, 0x18, 0x00,
    0x00, 0x00, 0x00, 0x0C, 0x0E, 0x07, 0x07, 0x0F, 0x9C, 0xF8, 0xF0, 0x71, 0x63, 0x63, 0xE3, 0xE7,
    0xE7, 0xE3, 0x63, 0x63, 0x71, 0xF0, 0xF8, 0x9C, 0x0F, 0x07, 0x07, 0x0E, 0x0C, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
    0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static void render_logo(void) {
    oled_set_cursor(0, 0);
    oled_write_raw_P(hamsa, sizeof(hamsa));
}

// Write text centered on 5-char wide display, padding with spaces
static void oled_write_centered(const char *text, bool invert) {
    char buf[6];
    uint8_t len = strlen(text);
    if (len > 5) len = 5;
    uint8_t left_pad = (5 - len + 1) / 2;
    memset(buf, ' ', 5);
    memcpy(buf + left_pad, text, len);
    buf[5] = '\0';
    oled_write(buf, invert);
}

static const char *get_layer_name(void) {
    switch (get_highest_layer(layer_state)) {
        case _QWERTY: return "Base";
        case _RAISE:  return "Raise";
        case _LOWER:  return "Lower";
        case _ADJUST: return "Adjst";
        default:      return "?????";
    }
}

static void render_info(void) {
    render_logo();

    // Row 6: spacer
    oled_set_cursor(0, 6);
    oled_write_P(PSTR("     "), false);

    // Row 7: layer name (centered)
    oled_set_cursor(0, 7);
    oled_write_centered(get_layer_name(), false);

    // Row 8: currently pressed keys (centered, compact)
    char keys[6];
    uint8_t kpos = 0;
    for (uint8_t i = 0; i < 6 && kpos < 5; i++) {
        char c = g_hk_state.display.pressing_keys[i];
        if (c != '\xB0' && c != '\0') {
            keys[kpos++] = c;
        }
    }
    keys[kpos] = '\0';
    oled_set_cursor(0, 8);
    oled_write_centered(keys, false);

    // Row 9: last key position row:col (centered)
    char buf[16];
    snprintf(buf, sizeof(buf), "%d:%d",
             g_hk_state.display.last_pos.row,
             g_hk_state.display.last_pos.col);
    oled_set_cursor(0, 9);
    oled_write_centered(buf, false);

    // Row 10: spacer
    oled_set_cursor(0, 10);
    oled_write_P(PSTR("     "), false);

    // Row 11-14: pointing device data (format then truncate to 5 chars)
    oled_set_cursor(0, 11);
    snprintf(buf, sizeof(buf), "x%+4d", (int)g_hk_state.display.last_mouse.x);
    buf[5] = '\0';
    oled_write(buf, false);

    oled_set_cursor(0, 12);
    snprintf(buf, sizeof(buf), "y%+4d", (int)g_hk_state.display.last_mouse.y);
    buf[5] = '\0';
    oled_write(buf, false);

    oled_set_cursor(0, 13);
    snprintf(buf, sizeof(buf), "v%+4d", (int)g_hk_state.display.last_mouse.v);
    buf[5] = '\0';
    oled_write(buf, false);

    oled_set_cursor(0, 14);
    snprintf(buf, sizeof(buf), "h%+4d", (int)g_hk_state.display.last_mouse.h);
    buf[5] = '\0';
    oled_write(buf, false);

    // Row 15: cursor mode (centered)
    oled_set_cursor(0, 15);
    if (g_hk_state.main.drag_scroll) {
        oled_write_centered("Drag", false);
    } else if (g_hk_state.main.cursor_mode == CURSOR_MODE_SNIPING) {
        oled_write_centered("Snipe", false);
    } else {
        oled_write_centered("Norm", false);
    }
}

// ===== Sand Dissolve/Reform Animation =====

#define SAND_MAX_GRAINS 32
#define SAND_FRAME_MS   50

enum { SAND_SHOW, SAND_DISSOLVE, SAND_WAIT, SAND_REFORM };

typedef struct {
    int8_t  x;
    uint8_t y;
    uint8_t speed;
    bool    active;
    // Reform: target position in hamsa
    int8_t  target_x;
    uint8_t target_y;
    uint8_t target_idx;
    uint8_t target_bit;
} grain_t;

static uint8_t  sand_hamsa[192];
static uint8_t  sand_reserved[192]; // reservation mask for in-flight reform grains
static uint8_t  sand_pile[32];
static grain_t  sand_grains[SAND_MAX_GRAINS];
static uint8_t  sand_phase;
static uint32_t sand_phase_timer;
static uint32_t sand_frame_timer;
static uint16_t sand_prng;
static uint16_t sand_on;
static uint16_t sand_total;
static bool     sand_ready;

static uint16_t sand_rng(void) {
    sand_prng ^= sand_prng << 7;
    sand_prng ^= sand_prng >> 9;
    sand_prng ^= sand_prng << 8;
    return sand_prng;
}

static void sand_setup(void) {
    memcpy_P(sand_hamsa, hamsa, sizeof(hamsa));
    memset(sand_reserved, 0, sizeof(sand_reserved));
    memset(sand_pile, 0, sizeof(sand_pile));
    memset(sand_grains, 0, sizeof(sand_grains));
    sand_phase = SAND_SHOW;
    sand_phase_timer = timer_read32();
    sand_frame_timer = timer_read32();
    sand_prng = timer_read() | 1;
    sand_total = 0;
    for (uint16_t i = 0; i < 192; i++) {
        uint8_t b = pgm_read_byte(hamsa + i);
        while (b) { sand_total += b & 1; b >>= 1; }
    }
    sand_on = sand_total;
    sand_ready = true;
    oled_clear();
    oled_set_cursor(0, 0);
    oled_write_raw_P(hamsa, sizeof(hamsa));
}

// Remove a random lit pixel from the mutable hamsa
static bool sand_pop(uint8_t *px, uint8_t *py) {
    if (sand_on == 0) return false;
    uint16_t idx = sand_rng() % 192;
    for (uint16_t i = 0; i < 192; i++) {
        uint16_t j = (idx + i) % 192;
        if (!sand_hamsa[j]) continue;
        uint8_t bit = sand_rng() & 7;
        for (uint8_t b = 0; b < 8; b++) {
            uint8_t bi = (bit + b) & 7;
            if (sand_hamsa[j] & (1 << bi)) {
                sand_hamsa[j] &= ~(1 << bi);
                sand_on--;
                *px = j % 32;
                *py = (j / 32) * 8 + bi;
                return true;
            }
        }
    }
    return false;
}

// Reserve a random missing hamsa pixel as a reform target for a grain
static bool sand_reserve(grain_t *g) {
    uint16_t idx = sand_rng() % 192;
    for (uint16_t i = 0; i < 192; i++) {
        uint16_t j = (idx + i) % 192;
        uint8_t orig = pgm_read_byte(hamsa + j);
        uint8_t avail = orig & ~(sand_hamsa[j] | sand_reserved[j]);
        if (!avail) continue;
        uint8_t bit = sand_rng() & 7;
        for (uint8_t b = 0; b < 8; b++) {
            uint8_t bi = (bit + b) & 7;
            if (avail & (1 << bi)) {
                sand_reserved[j] |= (1 << bi);
                g->target_idx = j;
                g->target_bit = bi;
                g->target_x = j % 32;
                g->target_y = (j / 32) * 8 + bi;
                return true;
            }
        }
    }
    return false;
}

static grain_t *sand_free_grain(void) {
    for (uint8_t i = 0; i < SAND_MAX_GRAINS; i++)
        if (!sand_grains[i].active) return &sand_grains[i];
    return NULL;
}

static bool sand_any_active(void) {
    for (uint8_t i = 0; i < SAND_MAX_GRAINS; i++)
        if (sand_grains[i].active) return true;
    return false;
}

static void sand_tick(void) {
    uint32_t now = timer_read32();

    switch (sand_phase) {
    case SAND_SHOW:
        if (timer_elapsed32(sand_phase_timer) > 3000) {
            sand_phase = SAND_DISSOLVE;
            sand_phase_timer = now;
        }
        return;

    case SAND_DISSOLVE:
        // Spawn falling grains from hamsa pixels
        for (uint8_t n = 0; n < 4; n++) {
            grain_t *g = sand_free_grain();
            if (!g) break;
            uint8_t gx, gy;
            if (sand_pop(&gx, &gy)) {
                g->x = gx;
                g->y = gy;
                g->speed = 4 + (sand_rng() % 4);
                g->active = true;
            }
        }
        // Update falling grains
        for (uint8_t i = 0; i < SAND_MAX_GRAINS; i++) {
            grain_t *g = &sand_grains[i];
            if (!g->active) continue;
            g->y += g->speed;
            if ((sand_rng() & 3) == 0) {
                g->x += (sand_rng() & 1) ? 1 : -1;
                if (g->x < 0) g->x = 0;
                if (g->x > 31) g->x = 31;
            }
            uint8_t floor_y = 127 - sand_pile[(uint8_t)g->x];
            if (g->y >= floor_y) {
                if (sand_pile[(uint8_t)g->x] < 64)
                    sand_pile[(uint8_t)g->x]++;
                g->active = false;
            }
        }
        // Check if dissolve is complete
        if (sand_on == 0 && !sand_any_active()) {
            sand_phase = SAND_WAIT;
            sand_phase_timer = now;
        }
        break;

    case SAND_WAIT:
        if (timer_elapsed32(sand_phase_timer) > 2000) {
            sand_phase = SAND_REFORM;
            sand_phase_timer = now;
            memset(sand_reserved, 0, sizeof(sand_reserved));
        }
        return;

    case SAND_REFORM:
        // Spawn grains that rise from pile to their target hamsa pixel
        for (uint8_t n = 0; n < 3; n++) {
            grain_t *g = sand_free_grain();
            if (!g) break;
            // Reserve a target pixel in the hamsa
            if (!sand_reserve(g)) break;
            // Find a pile column to take sand from
            uint8_t src_col = sand_rng() % 32;
            bool found = false;
            for (uint8_t j = 0; j < 32; j++) {
                uint8_t c = (src_col + j) % 32;
                if (sand_pile[c] > 0) {
                    sand_pile[c]--;
                    src_col = c;
                    found = true;
                    break;
                }
            }
            if (!found) {
                // No pile left, undo reservation
                sand_reserved[g->target_idx] &= ~(1 << g->target_bit);
                break;
            }
            g->x = src_col;
            g->y = 127 - sand_pile[src_col];
            g->speed = 3 + (sand_rng() % 3);
            g->active = true;
        }
        // Update rising grains: move toward target
        for (uint8_t i = 0; i < SAND_MAX_GRAINS; i++) {
            grain_t *g = &sand_grains[i];
            if (!g->active) continue;
            // Move y toward target
            if (g->y > g->target_y) {
                uint8_t step = g->speed;
                if (g->y - g->target_y < step) step = g->y - g->target_y;
                g->y -= step;
            }
            // Move x toward target
            if (g->x < g->target_x) g->x++;
            else if (g->x > g->target_x) g->x--;
            // Check if arrived
            if (g->y <= g->target_y && g->x == g->target_x) {
                sand_hamsa[g->target_idx] |= (1 << g->target_bit);
                sand_reserved[g->target_idx] &= ~(1 << g->target_bit);
                sand_on++;
                g->active = false;
            }
        }
        // Transition only when all pixels placed and all grains done
        if (sand_on >= sand_total && !sand_any_active()) {
            memset(sand_pile, 0, sizeof(sand_pile));
            memset(sand_reserved, 0, sizeof(sand_reserved));
            sand_phase = SAND_SHOW;
            sand_phase_timer = now;
            // Final draw matches PROGMEM original exactly
            oled_clear();
            oled_set_cursor(0, 0);
            oled_write_raw_P(hamsa, sizeof(hamsa));
        }
        break;
    }
}

static void sand_draw(void) {
    oled_clear();
    oled_set_cursor(0, 0);
    oled_write_raw((const char *)sand_hamsa, sizeof(sand_hamsa));

    // Draw sand pile from bottom up
    for (uint8_t col = 0; col < 32; col++) {
        for (uint8_t h = 0; h < sand_pile[col]; h++) {
            oled_write_pixel(col, 127 - h, true);
        }
    }

    // Draw active grains
    for (uint8_t i = 0; i < SAND_MAX_GRAINS; i++) {
        grain_t *g = &sand_grains[i];
        if (g->active && g->x >= 0 && g->x < 32 && g->y < 128) {
            oled_write_pixel(g->x, g->y, true);
        }
    }
}

static void render_sand(void) {
    if (!sand_ready) sand_setup();

    if (timer_elapsed32(sand_frame_timer) < SAND_FRAME_MS) return;
    sand_frame_timer = timer_read32();

    sand_tick();

    if (sand_phase == SAND_DISSOLVE || sand_phase == SAND_REFORM) {
        sand_draw();
    }
}

bool oled_task_user(void) {
    if (!is_keyboard_master()) {
        // Detect activity from synced state
        bool activity = false;
        if (g_hk_state.display.last_kc != prev_kc ||
            g_hk_state.display.last_pos.row != prev_row ||
            g_hk_state.display.last_pos.col != prev_col ||
            g_hk_state.display.last_mouse.x != 0 ||
            g_hk_state.display.last_mouse.y != 0) {
            activity = true;
            prev_kc = g_hk_state.display.last_kc;
            prev_row = g_hk_state.display.last_pos.row;
            prev_col = g_hk_state.display.last_pos.col;
        }

        if (activity) {
            idle_timer = timer_read32();
        }

        bool is_idle = timer_elapsed32(idle_timer) > IDLE_TIMEOUT;

        // Reset sand animation when returning to active
        if (was_idle && !is_idle) {
            sand_ready = false;
            oled_clear();
        }
        was_idle = is_idle;

        if (is_idle) {
            render_sand();
        } else {
            render_info();
        }
    }
    return false;
}

#endif // OLED_ENABLE
