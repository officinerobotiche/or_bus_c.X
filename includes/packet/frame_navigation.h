/*
 * Copyright (C) 2014 Officine Robotiche
 * Author: Raffaello Bonghi
 * email:  raffaello.bonghi@officinerobotiche.it
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU Lesser General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
 */

#ifndef FRAMENAVIGATION_H
#define	FRAMENAVIGATION_H

#ifdef	__cplusplus
extern "C" {
#endif
    
/******************************************************************************/
/* Definition commands                                                        */
/******************************************************************************/
//Name for HASHMAP with information about standard messages
#define HASHMAP_NAVIGATION 'N'
//Number association for standard messages
#define SENSOR 0
#define SENSOR_INFRARED 1
#define SENSOR_HUMIDITY 2
#define SENSOR_PARAMETER 3
#define SENSOR_AUTOSEND 4
#define SENSOR_ENABLE 5
/******************************************************************************/
/* Definition messages                                                        */
/******************************************************************************/
#define SENSOR_NUMBER_INFRARED 7
#define SENSOR_BUFFER_AUTOSEND 10

typedef struct _sensor {
    float temperature;
    float voltage;
    float current;
} sensor_t;
#define LNG_SENSOR sizeof(sensor_t)

typedef float sensor_humidity_t;
#define LNG_SENSOR_HUMIDITY sizeof(sensor_humidity_t)

typedef struct _infrared {
    float infrared[SENSOR_NUMBER_INFRARED];
} sensor_infrared_t;
#define LNG_SENSOR_INFRARED sizeof(sensor_infrared_t)

typedef struct _sensor_parameter {
    float gain_sharp;
    float exp_sharp;
    float gain_temperature;
    float gain_voltage;
    float gain_current;
    float gain_humidity;
} sensor_parameter_t;
#define LNG_SENSOR_PARAMETER sizeof(sensor_parameter_t)

typedef struct _autosend {
    int8_t pkgs[SENSOR_BUFFER_AUTOSEND];
} sensor_autosend_t;
#define LNG_SENSOR_AUTOSEND sizeof(sensor_autosend_t)

typedef uint8_t sensor_enable_t;
#define LNG_SENSOR_ENABLE sizeof(sensor_enable_t)
/**
 * List of all navigation messages
 */
typedef union _navigation_frame {
    sensor_t sensor;
    sensor_humidity_t humidity;
    sensor_infrared_t infrared;
    sensor_parameter_t parameter;
    sensor_autosend_t autosend;
    sensor_enable_t enable;
} navigation_frame_u;

#ifdef	__cplusplus
}
#endif

#endif	/* FRAMENAVIGATION_H */