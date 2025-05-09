#pragma once

#include <pthread.h>
#include <time.h>
#include <core/set/set.h>
#include <core/hashmap/hashmap.h>

/**
 * Callback type for key events.
 *
 * @param x The x-coordinate of the key.
 * @param y The y-coordinate of the key.
 * @param is_pressed True if the key is pressed, false if released.
 */
typedef void (*KeyEventCallback)(int x, int y, bool is_pressed);

/**
 * Structure representing a coordinate string and the last timestamp it was sent.
 */
struct coord_time_pair {
    const char *coord_str;  // Coordinate string key
    time_t timestamp;       // Last sent timestamp
};

typedef struct {
    simple_set pressed_keys;                // Set of currently pressed keys
    struct hashmap *last_sent_timestamps; // Hashmap of when keys were last sent
    pthread_t thread_id;               // Thread ID for the rate limiting thread
    bool thread_running;               // Flag to indicate if thread is running
    pthread_mutex_t lock;              // Mutex for thread safety
    unsigned int rate_limit_ms;        // Rate limit in milliseconds
    KeyEventCallback callback;         // Callback for key events
} RateLimiter;

/**
 * Initializes the rate limiter.
 *
 * @param limiter Pointer to the RateLimiter structure.
 * @param rate_limit_ms The rate limit in milliseconds.
 * @param callback Callback function to be called on key events.
 */
void rate_limiter_init(RateLimiter* limiter, unsigned int rate_limit_ms, KeyEventCallback callback);

/**
 * Cleans up and destroys the rate limiter, releasing any allocated resources.
 *
 * @param limiter Pointer to the RateLimiter structure.
 */
void rate_limiter_destroy(RateLimiter* limiter);

/**
 * Handles a key press event.
 *
 * @param limiter Pointer to the RateLimiter structure.
 * @param x The x-coordinate of the key.
 * @param y The y-coordinate of the key.
 */
void handle_key_press(RateLimiter* limiter, const int x, const int y);

/**
 * Handles a key release event.
 *
 * @param limiter Pointer to the RateLimiter structure.
 * @param x The x-coordinate of the key.
 * @param y The y-coordinate of the key.
 */
void handle_key_release(RateLimiter* limiter, int x, int y);