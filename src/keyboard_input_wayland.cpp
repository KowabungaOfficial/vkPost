#include "keyboard_input_wayland.hpp"
#include "logger.hpp"
#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <vector>

namespace vkPost
{
    static struct wl_display* wl_display_ptr = nullptr;
    static struct wl_seat* seat = nullptr;
    static struct wl_keyboard* keyboard = nullptr;
    static struct xkb_context* context_xkb = nullptr;
    static struct xkb_keymap* keymap_xkb = nullptr;
    static struct xkb_state* state_xkb = nullptr;
    static struct wl_event_queue* queue = nullptr;
    static std::vector<xkb_keysym_t> wl_pressed_keys;

    // Function prototypes for Wayland listeners
    static void seat_handle_capabilities(void *data, wl_seat *seat, uint32_t caps);
    static void registry_handle_global(void *data, struct wl_registry* registry, uint32_t name, const char *interface, uint32_t version);
    static void wl_keyboard_keymap(void *data, struct wl_keyboard *wl_keyboard, uint32_t format, int32_t fd, uint32_t size);
    static void wl_keyboard_key(void *data, struct wl_keyboard *wl_keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state);
    static void wl_keyboard_leave(void *data, struct wl_keyboard *wl_keyboard, uint32_t serial, struct wl_surface *surface);

    // Listener structures
    static const struct wl_seat_listener seat_listener = {
        .capabilities = seat_handle_capabilities,
        .name = nullptr,
    };

    static const struct wl_registry_listener registry_listener = {
        .global = registry_handle_global,
        .global_remove = nullptr,
    };

    static const struct wl_keyboard_listener keyboard_listener = {
        .keymap = wl_keyboard_keymap,
        .enter = nullptr,
        .leave = wl_keyboard_leave,
        .key = wl_keyboard_key,
        .modifiers = nullptr,
        .repeat_info = nullptr,
    };

    static void seat_handle_capabilities(void *data, wl_seat *seat, uint32_t caps)
    {
        if (caps & WL_SEAT_CAPABILITY_KEYBOARD && !keyboard) {
            keyboard = wl_seat_get_keyboard(seat);
            wl_keyboard_add_listener(keyboard, &keyboard_listener, nullptr);
        }
    }

    static void registry_handle_global(void *data, struct wl_registry* registry, uint32_t name, const char *interface, uint32_t version)
    {
        if (strcmp(interface, wl_seat_interface.name) == 0 && !seat) {
            seat = (struct wl_seat*)wl_registry_bind(registry, name, &wl_seat_interface, 5);
            wl_seat_add_listener(seat, &seat_listener, nullptr);
        }
    }

    static void wl_keyboard_keymap(void *data, struct wl_keyboard *wl_keyboard, uint32_t format, int32_t fd, uint32_t size)
    {
        char* map_shm = (char*)mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (!context_xkb)
            context_xkb = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
        if (keymap_xkb && state_xkb) {
            xkb_keymap_unref(keymap_xkb);
            xkb_state_unref(state_xkb);
        }
        keymap_xkb = xkb_keymap_new_from_string(context_xkb, map_shm, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
        state_xkb = xkb_state_new(keymap_xkb);
        munmap(map_shm, size);
        close(fd);
    }

    static void wl_keyboard_key(void *data, struct wl_keyboard *wl_keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state)
    {
        xkb_keycode_t keycode = key + 8;
        xkb_keysym_t keysym = xkb_state_key_get_one_sym(state_xkb, keycode);
        if (state) {
            wl_pressed_keys.push_back(keysym);
        } else {
            auto it = std::find(wl_pressed_keys.begin(), wl_pressed_keys.end(), keysym);
            if (it != wl_pressed_keys.end())
                wl_pressed_keys.erase(it);
        }
    }

    static void wl_keyboard_leave(void *data, struct wl_keyboard *wl_keyboard, uint32_t serial, struct wl_surface *surface)
    {
        wl_pressed_keys.clear();
    }

    void init_wayland_data()
    {
        if (!wl_display_ptr) {
            wl_display_ptr = wl_display_connect(nullptr);
            if (!wl_display_ptr) {
                Logger::err("Failed to connect to Wayland display");
                return;
            }
        }
        queue = wl_display_create_queue(wl_display_ptr);
        struct wl_display *display_wrapped = (struct wl_display*)wl_proxy_create_wrapper(wl_display_ptr);
        wl_proxy_set_queue((struct wl_proxy*)display_wrapped, queue);
        wl_registry *registry = wl_display_get_registry(display_wrapped);
        wl_proxy_wrapper_destroy(display_wrapped);
        wl_registry_add_listener(registry, &registry_listener, nullptr);
        wl_display_roundtrip_queue(wl_display_ptr, queue);
        wl_display_roundtrip_queue(wl_display_ptr, queue);
    }

    uint32_t convertToKeySymWayland(std::string key)
    {
        xkb_keysym_t keysym = xkb_keysym_from_name(key.c_str(), XKB_KEYSYM_NO_FLAGS);
        if (keysym == XKB_KEY_NoSymbol) {
            Logger::err("Invalid key: " + key);
            return 0u;
        }
        return static_cast<uint32_t>(keysym);
    }

    bool isKeyPressedWayland(uint32_t ks)
    {
        wl_display_dispatch_queue_pending(wl_display_ptr, queue);
        xkb_keysym_t keysym = static_cast<xkb_keysym_t>(ks);
        return std::find(wl_pressed_keys.begin(), wl_pressed_keys.end(), keysym) != wl_pressed_keys.end();
    }
} // namespace vkPost
