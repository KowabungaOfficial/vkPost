#include "keyboard_input_wayland.hpp"
#include "logger.hpp"

#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>
#include <unordered_map>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

extern wl_display* g_wl_display;

static wl_seat* g_seat = nullptr;
static wl_keyboard* g_keyboard = nullptr;
static xkb_context* g_xkb_context = nullptr;
static xkb_keymap* g_keymap = nullptr;
static xkb_state* g_state = nullptr;
static std::unordered_map<uint32_t, bool> key_states;

static void keyboard_keymap(void *data, wl_keyboard *keyboard, uint32_t format, int32_t fd, uint32_t size) {
    if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1) {
        close(fd);
        return;
    }
    char *keymap_str = static_cast<char*>(mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0));
    if (keymap_str == MAP_FAILED) {
        close(fd);
        return;
    }
    g_keymap = xkb_keymap_new_from_string(g_xkb_context, keymap_str, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
    munmap(keymap_str, size);
    close(fd);
    if (g_keymap) {
        g_state = xkb_state_new(g_keymap);
    }
}

static void keyboard_enter(void *data, wl_keyboard *keyboard, uint32_t serial, wl_surface *surface, wl_array *keys) {
}

static void keyboard_leave(void *data, wl_keyboard *keyboard, uint32_t serial, wl_surface *surface) {
}

static void keyboard_key(void *data, wl_keyboard *keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state_w) {
    if (g_state) {
        xkb_keysym_t keysym = xkb_state_key_get_one_sym(g_state, key + 8);
        key_states[keysym] = (state_w == WL_KEYBOARD_KEY_STATE_PRESSED);
    }
}

static void keyboard_modifiers(void *data, wl_keyboard *keyboard, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group) {
    if (g_state) {
        xkb_state_update_mask(g_state, mods_depressed, mods_latched, mods_locked, 0, 0, group);
    }
}

static const wl_keyboard_listener keyboard_listener = {
    .keymap = keyboard_keymap,
    .enter = keyboard_enter,
    .leave = keyboard_leave,
    .key = keyboard_key,
    .modifiers = keyboard_modifiers,
};

static void registry_handle_global(void *data, wl_registry *registry, uint32_t name, const char *interface, uint32_t version) {
    if (strcmp(interface, wl_seat_interface.name) == 0) {
        g_seat = static_cast<wl_seat*>(wl_registry_bind(registry, name, &wl_seat_interface, 1));
        g_keyboard = wl_seat_get_keyboard(g_seat);
        wl_keyboard_add_listener(g_keyboard, &keyboard_listener, nullptr);
    }
}

static const wl_registry_listener registry_listener = {
    .global = registry_handle_global,
    .global_remove = nullptr,
};

static bool initialized = false;

static void init_wayland_input() {
    if (initialized) return;
    if (!g_wl_display) {
        Logger::debug("No Wayland display available");
        return;
    }
    g_xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    wl_registry* registry = wl_display_get_registry(g_wl_display);
    wl_registry_add_listener(registry, &registry_listener, nullptr);
    wl_display_roundtrip(g_wl_display);
    initialized = true;
}

namespace vkPost {
    uint32_t convertToKeySymWayland(std::string key) {
        uint32_t keysym = xkb_keysym_from_name(key.c_str(), XKB_KEYSYM_NO_FLAGS);
        if (keysym == XKB_KEY_NoSymbol) {
            Logger::err("Invalid key for Wayland: " + key);
        }
        return keysym;
    }

    bool isKeyPressedWayland(uint32_t ks) {
        init_wayland_input();
        auto it = key_states.find(ks);
        if (it != key_states.end()) {
            return it->second;
        }
        return false;
    }
}
