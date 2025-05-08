#include "profile_manager.h"
#include "core/coordinates_helper.h"
#include <core/set/set.h>
#include "matrix/matrix.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

/**
 * Returns the number represented by a key at the given keyboard matrix coordinates.
 *
 * This function checks the key located at position (x, y) in the `keyboard_matrix` and,
 * if the key corresponds to a digit key (0–9), it returns the associated integer.
 * If the key is not a digit key, the function returns -1.
 *
 * @param x The row index in the keyboard matrix.
 * @param y The column index in the keyboard matrix.
 * @return
 *   - An integer from 0 to 9 if the key at the given coordinates is a digit key.
 *   - -1 if the key does not represent a digit.
 */
bool get_coordinate_number(const int x, const int y)
{
  const enum CrossPlatformKeyCode key = keyboard_matrix[x][y];

  switch (key) {
  case KEY_1:
    return 1;
  case KEY_2:
    return 2;
  case KEY_3:
    return 3;
  case KEY_4:
    return 4;
  case KEY_5:
    return 5;
  case KEY_6:
    return 6;
  case KEY_7:
    return 7;
  case KEY_8:
    return 8;
  case KEY_9:
    return 9;
  case KEY_0:
    return 0;
  default:
    return -1;
  }
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

int keypress_has_triggered_profile(ProfileManager* manager, const int x, const int y) {
  char* coords_str = "";
  coord_to_string(coords_str, x, y);
  const int number = get_coordinate_number(x, y);

  if (set_contains(&manager->modifiers_combination, coords_str) || number != -1) {
    if (!set_contains(&manager->pressed_keys, coords_str)) {
      set_add(&manager->pressed_keys, coords_str);

      uint64_t size = set_length(&manager->modifiers_combination);
      char** array = set_to_array(&manager->modifiers_combination, size);

      bool valid = true;
      int modifiers_count = 0;
      int profile_number = -1;
      for (int i = 0; i < size; i++) {
        int curr_x, curr_y = 0;
        parse_coord_string(array[i], &curr_x, &curr_y);

        const int curr_array_element_number = get_coordinate_number(x, y);

        if (set_contains(&manager->pressed_keys, array[i])) {
          modifiers_count++;
        } else if (curr_array_element_number != -1) {
          profile_number = curr_array_element_number;
        }
      }

      free(array);

      if (modifiers_count != size || profile_number == -1) {
        return -1;
      }

      time_t current_time = time(NULL);

      if (current_time - manager->last_profile_switch_timestamp >= manager->rate_limit_ms / 1000) {
        return profile_number;
      } else {
        return -1;
      }
    } else {
      return -1;
    }
  } else {
    return -1;
  }
}

// todo: function to handle key release