#ifndef KEYBOARD_INPUT_WAYLAND_HPP
#define KEYBOARD_INPUT_WAYLAND_HPP

#include <cstdint>
#include <string>

uint32_t convertToKeySymWayland(std::string key);
bool isKeyPressedWayland(uint32_t ks);

#endif // KEYBOARD_INPUT_WAYLAND_HPP
