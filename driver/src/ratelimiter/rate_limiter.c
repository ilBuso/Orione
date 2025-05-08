#include "rate_limiter.h"
#include "coordinates_helper.h"
#include <stdlib.h>
#include <unistd.h>
#include <core/set/set.h>
#include <stdbool.h>



/**
 * Helper function for the rate limiter thread
 */
void* process_keys_thread(void* arg) {
    RateLimiter* limiter = (RateLimiter*)arg;

    while (1) {
        pthread_mutex_lock(&limiter->lock);

        // hanlde thread stop from outside
        if (!limiter->thread_running) {
            pthread_mutex_unlock(&limiter->lock);
            break;
        }

        // seconds since Jan 1 1970
        time_t current_time = time(NULL);

        // we need this for iteration
        // if this makes stuff too slow we can create the array every time we modify the set
        uint64_t size = set_length(&limiter->pressed_keys);
        char** set_array = set_to_array(&limiter->pressed_keys, &size);

        // Process each pressed key
        for (unsigned int i = 0; i < size; i++) {
            const char* coord_str = set_array[i];
            time_t last_sent = hashmap_get(limiter->last_sent_timestamps, coord_str);

            if (current_time - last_sent >= limiter->rate_limit_ms / 1000) {
                int x, y;
                parse_coord_string(coord_str, &x, &y);

                limiter->callback(x, y, true);

                hashmap_set(&limiter->last_sent_timestamps, coord_str, current_time);
            }
        }

        free(set_array);

        pthread_mutex_unlock(&limiter->lock);

        // Sleep for a portion of the rate limit time
        usleep(limiter->rate_limit_ms * 1000 / 10);
    }

    return NULL;
}

/**
 * Check if coordinates are for a modifier key
 */
bool is_modifier_key(int x, int y) {
  	// TODO: fill with actual modifier keys
    if ((x == 0 && y == 0) ||    // Left Ctrl
        (x == 0 && y == 1) ||    // Left Shift
        (x == 0 && y == 2)) {    // Left Alt
        return true;
    }
    return false;
}

/**
 * Initialize the rate limiter
 */
void rate_limiter_init(RateLimiter* limiter, unsigned int rate_limit_ms, KeyEventCallback callback) {
    set_init(&limiter->pressed_keys);
    hashmap_init(&limiter->last_sent_timestamps);
    limiter->thread_running = false;
    limiter->rate_limit_ms = rate_limit_ms;
    limiter->callback = callback;
    pthread_mutex_init(&limiter->lock, NULL);
}

/**
 * Clean up the rate limiter
 */
void rate_limiter_destroy(RateLimiter* limiter) {
    pthread_mutex_lock(&limiter->lock);

    limiter->thread_running = false;

    pthread_mutex_unlock(&limiter->lock);

    // Wait for thread to finish if it's running
    if (limiter->thread_running) {
        pthread_join(limiter->thread_id, NULL);
    }

    set_destroy(&limiter->pressed_keys);
    hashmap_destroy(&limiter->last_sent_timestamps);
    pthread_mutex_destroy(&limiter->lock);
}

/**
 * Handle key press event
 */
void handle_key_press(RateLimiter* limiter, const int x, const int y) {
    char coord_str[32];
    coord_to_string(coord_str, x, y);

    pthread_mutex_lock(&limiter->lock);

    if (set_contains(&limiter->pressed_keys, coord_str) == SET_FALSE) {
        set_add(&limiter->pressed_keys, coord_str);

        // todo: verify whether sending the key immediately is also better
//        if (limiter->callback) {
//            limiter->callback(x, y, true, limiter->user_data);
//        }
//        hashmap_set(&limiter->last_sent_timestamps, coord_str, time(NULL));

        if (!limiter->thread_running) {
            limiter->thread_running = true;
            pthread_create(&limiter->thread_id, NULL, process_keys_thread, limiter);
        }
    }

    pthread_mutex_unlock(&limiter->lock);
}

/**
 * Handle key release event
 */
void handle_key_release(RateLimiter* limiter, int x, int y) {
    char coord_str[32];
    coord_to_string(coord_str, x, y);

    pthread_mutex_lock(&limiter->lock);

    set_remove(&limiter->pressed_keys, coord_str);

    limiter->callback(x, y, false);

    hashmap_set(&limiter->last_sent_timestamps, coord_str, 0);

    if (set_length(&limiter->pressed_keys) == 0) {
        limiter->thread_running = false;
    }

    pthread_mutex_unlock(&limiter->lock);
}