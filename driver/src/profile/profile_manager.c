#include "profile_manager.h"
#include "coordinates_helper.h"
#include <set.h>
#include <stdbool.h>
#include <stdlib.h>

bool is_number(const char* pressed_key_coord) {
  int x, y;
  parse_coord_string(pressed_key_coord, &x, &y);

  // todo: evaluate whether it's a number

  return false
}

void profile_manager_init(ProfileManager* manager, unsigned int rate_limit_ms) {
  set_init(&manager->modifiers_combination);
  set_init(&manager->pressed_keys);
  manager->rate_limit_ms = rate_limit_ms;
  manager->last_profile_switch_timestamp = 0;
}

void profile_manager_destroy(ProfileManager* limiter) {
  set_destroy(&limiter->modifiers_combination);
}

unsigned int profile_triggered(ProfileManager* manager, const char* pressed_key_coord) {
  if (set_contains(&manager->modifiers_combination, pressed_key_coord) || is_number(pressed_key_coord)) {
    if (!set_contains(&manager->pressed_keys, pressed_key_coord)) {
      set_add(&manager->pressed_keys, pressed_key_coord);

      uint64_t size = set_length(&manager->modifiers_combination);
      char** array = set_to_array(&manager->modifiers_combination, size);

      bool valid = true;
      int modifiers_count = 0;
      int profile_number = -1;
      for (int i = 0; i < size; i++) {
        if (set_contains(&manager->pressed_keys, array[i])) {
          modifiers_count++;
        } else if (is_number(array[i])) {
          // todo: convert string coordinates to number for this to work
          profile_number = array[i];
        }
      }

      free(array);

      if (modifiers_count != size || profile_number == -1) {
        return NULL;
      }

      time_t current_time = time(NULL);

      if (current_time - manager->last_profile_switch_timestamp >= manager->rate_limit_ms / 1000) {
        return profile_number;
      } else {
        return NULL;
      }
    } else {
      return NULL;
    }
  } else {
    return NULL;
  }
}