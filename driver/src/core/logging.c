//
// Created by Giulio Pimenoff Verdolin on 08/05/25.
//

#include <stdio.h>

#if defined(LOG_LEVEL_DEBUG)
    #define LOG_DEBUG(fmt, ...) printf("[DEBUG] " fmt, ##__VA_ARGS__)
    #define LOG_INFO(fmt, ...) printf("[INFO] " fmt, ##__VA_ARGS__)
    #define LOG_WARNING(fmt, ...) printf("[WARNING] " fmt, ##__VA_ARGS__)
    #define LOG_ERROR(fmt, ...) { \
    printf("[ERROR] " fmt, ##__VA_ARGS__); \
    perror(""); \
    }
#elif defined(LOG_LEVEL_INFO)
    #define LOG_DEBUG(fmt, ...) // No-op
    #define LOG_INFO(fmt, ...) printf("[INFO] " fmt, ##__VA_ARGS__)
    #define LOG_WARNING(fmt, ...) printf("[WARNING] " fmt, ##__VA_ARGS__)
    #define LOG_ERROR(fmt, ...) { \
    printf("[ERROR] " fmt, ##__VA_ARGS__); \
    perror(""); \
    }
#elif defined(LOG_LEVEL_WARNING)
    #define LOG_DEBUG(fmt, ...) // No-op
    #define LOG_INFO(fmt, ...) // No-op
    #define LOG_WARNING(fmt, ...) printf("[WARNING] " fmt, ##__VA_ARGS__)
    #define LOG_ERROR(fmt, ...) { \
    printf("[ERROR] " fmt, ##__VA_ARGS__); \
    perror(""); \
    }
#elif defined(LOG_LEVEL_ERROR)
    #define LOG_DEBUG(fmt, ...) // No-op
    #define LOG_INFO(fmt, ...) // No-op
    #define LOG_WARNING(fmt, ...) // No-op
    #define LOG_ERROR(fmt, ...) { \
    printf("[ERROR] " fmt, ##__VA_ARGS__); \
    perror(""); \
    }
#else
    #define LOG_DEBUG(fmt, ...) // No-op
    #define LOG_INFO(fmt, ...) // No-op
    #define LOG_WARNING(fmt, ...) // No-op
    #define LOG_ERROR(fmt, ...) // No-op
#endif