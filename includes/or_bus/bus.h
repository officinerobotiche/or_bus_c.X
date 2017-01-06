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

#ifndef OR_MESSAGE_H
#define	OR_MESSAGE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>        /* Includes uint16_t definition   */
#include <stdbool.h>       /* Includes true/false definition */
#include <string.h>

    /******************************************************************************/
    /* Prototype              													  */
    /******************************************************************************/

    /// Header packet
#define OR_BUS_PACKET_HEADER '#'
    /// Length header
#define OR_BUS_LNG_PACKET_HEADER 2

    typedef enum {
        OR_BUS_ERROR_CKS = -3,
        OR_BUS_ERROR_LENGTH = -2,
        OR_BUS_ERROR_HEADER = -1,
        OR_BUS_PENDING = 0,
        OR_BUS_DONE = 1
    } OR_BUS_State_t;

    typedef struct _over_buffer {
        unsigned char *buff;
        unsigned int length;
    } OR_BUS_buffer_t;
#define OR_BUS_BUFFER_INIT(buff) {buff, 0}

    /*! Pointer to function, initialized for pkg_header */
    typedef OR_BUS_State_t(*OR_BUS_read_decoder)(void *or_bus, unsigned char inchar);

    typedef void (*OR_BUS_parser)(void* obj, unsigned char *buff, size_t size);

    typedef struct _Overbus {
        OR_BUS_buffer_t rx;
        OR_BUS_buffer_t tx;
        OR_BUS_read_decoder read_decoder;
        OR_BUS_parser parser;
        void *obj;
        unsigned int rx_size;
        unsigned int index_data;
        unsigned char rx_cks;
    } OR_BUS_t;

    /*************************************************************************/
    /* System Function Prototypes                                            */
    /*************************************************************************/
    /**
     * @brief Initialization OS_BUS controller
     * @param or_bus The OR_BUS controller
     * @param buffTx The transmitter buffer
     * @param buffRx The receiver buffer 
     * @param rx_size the size of received buffer
     * @param obj The object to recall after a complete decode
     * @param parser The callback to recall
     */
    void OR_BUS_init(OR_BUS_t *or_bus,
            unsigned char *buffTx, unsigned char *buffRx,
            unsigned int rx_size, void *obj, OR_BUS_parser parser);
    /**
     * @brief Decode a OR_BUS message
     * @param The OR_BUS controller
     * @param rxchar character received from interrupt
     * @return the status of the decoder
     */
    OR_BUS_State_t OR_BUS_decoder(OR_BUS_t *or_bus, unsigned char rxchar);
    /**
     * @brief Build a OR_BUS message from a starting buffer
     * Data structure:
     * ------------------------------------------------
     * | HEADER | LENGTH |       DATA           | CKS |
     * ------------------------------------------------
     *     1        2             3 -> n          n+1
     *
     * Only element of packet have a relative function to decode
     * 1) Header -> pkg_header
     * 2) Length -> pkg_length
     * 3 to n+1) Data -> pkg_data
     * @param or_bus The OR_BUS controller
     * @param buff The buffer to send
     * @param length The length of the buffer
     */
    void OR_BUS_build(OR_BUS_t *or_bus, unsigned char *buff, size_t length);

#ifdef	__cplusplus
}
#endif

#endif	/* OR_MESSAGE_H */
