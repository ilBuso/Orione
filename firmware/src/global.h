/**
 * @file global.h
 * @brief Global definitions and constants
 * 
 * System-wide definitions including LED blink interval constants and
 * logic level definitions used throughout the firmware.
 */

#ifndef GLOBAL_H
#define GLOBAL_H

    enum  {
        BLINK_NOT_MOUNTED = 250,
        BLINK_MOUNTED = 1000,
        BLINK_SUSPENDED = 2500,
    };

    #define HIGH 1
    #define LOW 0 
    
    #define CAPS_LOCK_LED 22

#endif