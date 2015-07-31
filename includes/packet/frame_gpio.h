/* 
 * File:   frame_gpio.h
 * Author: Raffaello
 *
 * Created on July 31, 2015, 2:11 PM
 */

#ifndef FRAME_GPIO_H
#define	FRAME_GPIO_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>

//Name for HASHMAP with information about standard messages
#define HASHMAP_GPIO          'P'

/**
 * Configuration GPIO
 * - [#]     Number GPIO to set
 * - [0 - 2] Configuration GPIO [1 Read, 2 Write, 3 Analog (if available)]
 */
typedef struct _gpio_set {
    uint8_t number;
    int8_t type;
} gpio_set_t;
#define LNG_GPIO_SET sizeof(gpio_set_t)    

/**
 * List of all system messages
 */
typedef union _gpio_frame {
    gpio_set_t set;
} gpio_frame_u;
    
//Number association for standard messages
#define GPIO_SET            0
#define GPIO_READ_ALL       1
#define GPIO_WRITE_ALL      2
#define GPIO_READ           3
#define GPIO_WRITE          4

    
#ifdef	__cplusplus
}
#endif

#endif	/* FRAME_GPIO_H */

