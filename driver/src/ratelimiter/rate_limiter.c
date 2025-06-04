#include "rate_limiter.h"
#include "../core/coordinates_helper.h"
#include <stdlib.h>
#include <unistd.h>
#include "../core/set/set.h"
#include <stdbool.h>
#include <string.h>

void* process_keys_thread(void* arg) {
    RateLimiter* limiter = arg;

    while (1) {
        pthread_mutex_lock(&limiter->lock);

        // handle thread stop from outside
        if (!limiter->thread_running) {
            pthread_mutex_unlock(&limiter->lock);
            break;
        }

        // seconds since Jan 1 1970
        const time_t current_time = time(NULL);

        // we need this for iteration
        // if this makes stuff too slow we can create the array every time we modify the set
        uint64_t size = set_length(&limiter->pressed_keys);
        char** set_array = set_to_array(&limiter->pressed_keys, &size);

        // Process each pressed key
        for (unsigned int i = 0; i < size; i++) {
            const char* coord_str = set_array[i];

            // Create temporary key for lookup
            struct coord_time_pair lookup = { .coord_str = coord_str };

            // Get the entry to check timestamp
            struct coord_time_pair* entry = hashmap_get(limiter->last_sent_timestamps, &lookup);
            const time_t last_sent = entry ? entry->timestamp : 0;

            if (current_time - last_sent >= limiter->rate_limit_ms / 1000) {
                int x, y;
                parse_coord_string(coord_str, &x, &y);

                limiter->callback(x, y, true);

                // Update timestamp if entry exists, otherwise create new entry
                if (entry) {
                    entry->timestamp = current_time;
                } else {
                    // This shouldn't happen in normal operation but handle it just in case
                    struct coord_time_pair new_entry = {
                        .coord_str = strdup(coord_str),
                        .timestamp = current_time
                    };
                    hashmap_set(limiter->last_sent_timestamps, &new_entry);
                }
            }
        }

        free(set_array);

        pthread_mutex_unlock(&limiter->lock);

        // Sleep for a portion of the rate limit time
        usleep(limiter->rate_limit_ms * 1000 / 10);
    }

    return NULL;
}

uint64_t coord_str_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const struct coord_time_pair *pair = item;
    if (!pair->coord_str) {
        return 0;
    }

    return hashmap_sip(pair->coord_str, strlen(pair->coord_str), seed0, seed1);
}

int coord_str_compare(const void *a, const void *b, void *udata) {
    const struct coord_time_pair *pair_a = a;
    const struct coord_time_pair *pair_b = b;
    return strcmp(pair_a->coord_str, pair_b->coord_str);
}

void rate_limiter_init(RateLimiter* limiter, unsigned int rate_limit_ms, KeyEventCallback callback) {
    set_init(&limiter->pressed_keys);

    // Initialize hashmap with proper size and functions
    limiter->last_sent_timestamps = hashmap_new(sizeof(struct coord_time_pair), 0, 0, 0,
                                               coord_str_hash, coord_str_compare, NULL, NULL);

    limiter->thread_running = false;
    limiter->rate_limit_ms = rate_limit_ms;
    limiter->callback = callback;
    pthread_mutex_init(&limiter->lock, NULL);
}

void rate_limiter_destroy(RateLimiter* limiter) {
    pthread_mutex_lock(&limiter->lock);
    limiter->thread_running = false;
    pthread_mutex_unlock(&limiter->lock);

    // Wait for thread to finish if it's running
    if (limiter->thread_running) {
        pthread_join(limiter->thread_id, NULL);
    }

    // Free all allocated strings in hashmap
    size_t iter = 0;
    void *item;
    while (hashmap_iter(limiter->last_sent_timestamps, &iter, &item)) {
        struct coord_time_pair *pair = item;
        free((void*)pair->coord_str);
    }

    set_destroy(&limiter->pressed_keys);
    hashmap_free(limiter->last_sent_timestamps);
    pthread_mutex_destroy(&limiter->lock);
}

void handle_key_press(RateLimiter* limiter, const int x, const int y) {
    char coord_str[32];
    coord_to_string(coord_str, x, y);

    pthread_mutex_lock(&limiter->lock);

    if (set_contains(&limiter->pressed_keys, coord_str) == SET_FALSE) {
        set_add(&limiter->pressed_keys, coord_str);

        // Create a new timestamp entry with current time
        struct coord_time_pair new_entry = {
            .coord_str = strdup(coord_str),  // Create persistent copy
            .timestamp = time(NULL)
        };
        hashmap_set(limiter->last_sent_timestamps, &new_entry);

        if (!limiter->thread_running) {
            limiter->thread_running = true;
            pthread_create(&limiter->thread_id, NULL, process_keys_thread, limiter);
        }
    }

    pthread_mutex_unlock(&limiter->lock);
}

void handle_key_release(RateLimiter* limiter, const int x, const int y) {
    char coord_str[32];
    coord_to_string(coord_str, x, y);

    pthread_mutex_lock(&limiter->lock);

    set_remove(&limiter->pressed_keys, coord_str);

    limiter->callback(x, y, false);

    // Create a temporary key for lookup
    const struct coord_time_pair lookup = { .coord_str = coord_str };

    // Get the entry to free the string memory
    const struct coord_time_pair *entry = hashmap_get(limiter->last_sent_timestamps, &lookup);
    if (entry) {
        free((void*)entry->coord_str); // Free the allocated string
        hashmap_delete(limiter->last_sent_timestamps, &lookup);
    }

    if (set_length(&limiter->pressed_keys) == 0) {
        limiter->thread_running = false;
    }

    pthread_mutex_unlock(&limiter->lock);
}