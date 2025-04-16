#ifndef PROFILE_MANAGER_H
#define PROFILE_MANAGER_H

#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <set.h>
#include <hashmap.h>

typedef struct {
    SimpleSet modifiers_combination;
    SimpleSet pressed_keys;
    uint64_t last_profile_switch_timestamp;
    unsigned int rate_limit_ms;
} ProfileManager;

/**
* @param rate_limit_ms the minimum time in ms that needs to pass between a profile switch, can be used as debouncer
*/
void profile_manager_init(ProfileManager* manager, unsigned int rate_limit_ms);

void profile_manager_destroy(ProfileManager* limiter);

/**
* @return NULL if no profile has been triggered, an int corresponding to the profile number if a profile has been triggered instead
*/
unsigned int profile_triggered(ProfileManager* manager, const char* pressed_key_coord);

#endif /* PROFILE_MANAGER_H */