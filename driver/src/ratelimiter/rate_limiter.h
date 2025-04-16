#ifndef RATE_LIMITER_H
#define RATE_LIMITER_H

#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <set.h>
#include <hashmap.h>

/**
 * Callback function type for key events
 *
 * @param x The x-coordinate of the key
 * @param y The y-coordinate of the key
 * @param is_press true for press events, false for release events
 */
typedef void (*KeyEventCallback)(int x, int y, bool is_press);


/**
 * Rate limiter structure and functions
 */
typedef struct {
    SimpleSet pressed_keys;
    struct hashmap last_sent_timestamps*;
    pthread_t thread_id;
    bool thread_running;
    pthread_mutex_t lock;
    unsigned int rate_limit_ms;
    KeyEventCallback callback;
} RateLimiter;

// todo: for macOS, create a new struct that contains the pressed modifier keys (which are (un)set when receiving a key press/release), and pass that to the callback
// ideally instead, let this platform specific aspect be handled outside the rate limiter

/**
 * Initialize a rate limiter with the specified rate limit in milliseconds
 *
 * @param limiter The rate limiter to initialize
 * @param rate_limit_ms The minimum time between key repeats in milliseconds
 * @param callback Function to call when sending key events
 */
void rate_limiter_init(RateLimiter* limiter, unsigned int rate_limit_ms,
                       KeyEventCallback callback, char** profile_keys_combination, ProfileEventCallback profile_callback);

/**
 * Clean up and destroy a rate limiter
 *
 * @param limiter The rate limiter to destroy
 */
void rate_limiter_destroy(RateLimiter* limiter);

/**
 * Handle a key press event
 *
 * @param limiter The rate limiter
 * @param x The x-coordinate of the key
 * @param y The y-coordinate of the key
 */
void handle_key_press(RateLimiter* limiter, int x, int y);

/**
 * Handle a key release event
 *
 * @param limiter The rate limiter
 * @param x The x-coordinate of the key
 * @param y The y-coordinate of the key
 */
void handle_key_release(RateLimiter* limiter, int x, int y);

/**
 * Check if the given coordinates correspond to a modifier key
 *
 * @param x The x-coordinate of the key
 * @param y The y-coordinate of the key
 * @return true if it's a modifier key, false otherwise
 */
bool is_modifier_key(int x, int y);

#endif /* RATE_LIMITER_H */