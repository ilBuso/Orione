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

// TODO: add param to initialize modifiers_combination
/**
 * @param manager
 * @param rate_limit_ms the minimum time in ms that needs to pass between a profile switch, can be used as debouncer
 */
void profile_manager_init(ProfileManager* manager, unsigned int rate_limit_ms);

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
 * @param x the x-coordinate of the pressed key
 * @param y the y-coordinate of the pressed key
 *
 * @return an int corresponding to the profile number if a profile has been triggered, -1 otherwise
 */
int keypress_has_triggered_profile(ProfileManager* manager, int x, int y);

/**
 * Remember to call this when a key is released, this allows the profile manager to keep in memory the pressed keys
 *
 * @param manager
 * @param x the x-coordinate of the released key
 * @param y the y-coordinate of the released key
 */
void profile_manager_notify_key_release(ProfileManager* manager, int x, int y);
