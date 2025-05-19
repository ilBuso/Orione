#pragma once

#include <core/set/set.h>
#include <stdint.h>

typedef struct
{
    SimpleSet modifiers_combination;
    SimpleSet pressed_keys;
    uint64_t last_profile_switch_timestamp;
    unsigned int rate_limit_ms;
} ProfileManager;

/**
 * @param manager
 * @param rate_limit_ms the minimum time in ms that needs to pass between a profile switch, can be used as debouncer
 */
void profile_manager_init(ProfileManager* manager, unsigned int rate_limit_ms);


/**
 * Adds a modifier key to the ProfileManager.
 * Modifier keys are used to determine the combination that triggers profile changes.
 *
 * @param manager Pointer to the ProfileManager instance
 * @param modifier_key_code The key code of the modifier to add
 */
void profile_manager_add_modifier(ProfileManager* manager, enum CrossPlatformKeyCode modifier_key_code);

/**
 * Removes a modifier key from the ProfileManager.
 * This stops the key from being considered part of modifier combinations.
 *
 * @param manager Pointer to the ProfileManager instance
 * @param modifier_key_code The key code of the modifier to remove
 */
void profile_manager_remove_modifier(ProfileManager* manager, enum CrossPlatformKeyCode modifier_key_code);

/**
 * Free resources used by the ProfileManager
 *
 * @param limiter
 */
void profile_manager_destroy(ProfileManager* limiter);

/**
 * Notifies the profile manager that a key has been pressed, which detects whether a profile has been triggered
 *
 * @param manager
 * @param keycode
 *
 * @return an int corresponding to the profile number if a profile has been triggered, -1 otherwise
 */
int keypress_has_triggered_profile(ProfileManager* manager, enum CrossPlatformKeyCode keycode);

/**
 * Remember to call this when a key is released, this allows the profile manager to keep in memory the pressed keys
 *
 * @param manager
 * @param keycode
 */
void profile_manager_notify_key_release(ProfileManager* manager, enum CrossPlatformKeyCode keycode);
