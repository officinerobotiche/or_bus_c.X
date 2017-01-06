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

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "or_bus/or_message.h"

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

OR_BUS_State_t OR_BUS_header(void *or_bus, unsigned char rxchar);

/******************************************************************************/
/* Communication Functions                                                    */
/******************************************************************************/

void OR_BUS_init(OR_BUS_t *or_bus, 
            unsigned char *buffRx, unsigned char *buffTx, 
            unsigned int rx_size, void *obj, OR_BUS_parser parser) {
    // Initialization RX buffer
    or_bus->rx.buff = buffRx;
    or_bus->rx.length = 0;
    // Initialization TX buffer
    or_bus->tx.buff = buffTx;
    or_bus->tx.length = 0;
    // Initialization callback parser
    or_bus->parser = parser;
    // Initialization header parser
    or_bus->read_decoder = &OR_BUS_header;
    // Index reader
    or_bus->index_data = 0;
    // Initialization header message
    or_bus->tx.buff[0] = OR_BUS_PACKET_HEADER;
    // Pointer to send receiver
    or_bus->obj = obj;
    // Size buffer receiver
    or_bus->rx_size = rx_size;
    // Reset checksum
    or_bus->rx_cks = 0;
}
/**
 * Function to evaluate checksum. Count all bytes in a Buffer and return
 * number for checksum.
 * @param Buffer It's a buffer to sum all bytes
 * @param FirstIndx The number for first element buffer to count all bytes.
 * @param LastIndx The number for last element buffer.
 * @return number evaluated for sum bytes
 */
unsigned char OR_BUS_pkg_checksum(volatile unsigned char* Buffer, int FirstIndx, int LastIndx) {
    unsigned char ChkSum = 0;
    int ChkCnt;
    for (ChkCnt = FirstIndx; ChkCnt < LastIndx; ChkCnt++) {
        ChkSum += Buffer[ChkCnt];
    }
    return ChkSum;
}
/**
 * Store all bytes and run check the checksum. If true launch the parser callback.
 * @param The OR_BUS controller
 * @param rxchar character received from interrupt
 * @return the status of the decoder
 */
inline OR_BUS_State_t OR_BUS_data(void *or_bus, unsigned char rxchar) {
    OR_BUS_t *_or_bus = ((OR_BUS_t*)or_bus);
    if ((_or_bus->index_data + 1) == (_or_bus->rx.length + 1)) {
        // Restart decoder
        _or_bus->read_decoder = &OR_BUS_header;
        // Check the checksum
        if(rxchar == _or_bus->rx_cks) {
              // Call the parser function
            _or_bus->parser(_or_bus->obj, &_or_bus->rx.buff[0], _or_bus->rx.length);
            return OR_BUS_DONE;
        } else {
            return OR_BUS_ERROR_CKS;
        }
    } else {
        // Add the data on checksum
        _or_bus->rx_cks += rxchar;
        // Copy on the buffer the data
        _or_bus->rx.buff[_or_bus->index_data] = rxchar;
        _or_bus->index_data++;
        return OR_BUS_PENDING;
    }
}
/**
 * This function is to able to verify length of packet. 
 * If length (rxchar) is larger than max size return an error, else change 
 * function to call OR_BUS_data and save the length.
 * @param The OR_BUS controller
 * @param rxchar character received from interrupt
 * @return the status of the decoder
 */
inline OR_BUS_State_t OR_BUS_length(void *or_bus, unsigned char rxchar) {
    OR_BUS_t *_or_bus = ((OR_BUS_t*)or_bus);
    if (rxchar > _or_bus->rx_size) {
        // reset decoder
        _or_bus->read_decoder = &OR_BUS_header;
        return OR_BUS_ERROR_LENGTH;
    } else {
        _or_bus->read_decoder = &OR_BUS_data;
        _or_bus->rx.length = rxchar;
        // Initialization in checksum. Reset and add rxchar data
        ((OR_BUS_t*)or_bus)->rx_cks = 0;
        // Reset Index data counter
        _or_bus->index_data = 0;
        return OR_BUS_PENDING;
    }
}
/**
 * First function to decode Header from Serial interrupt
 * Verify if first byte is the header and update the pointer function with the 
 * next function to decode
 * @param The OR_BUS controller
 * @param rxchar character received from interrupt
 * @return the status of the decoder
 */
inline OR_BUS_State_t OR_BUS_header(void *or_bus, unsigned char rxchar) {
    if (rxchar == OR_BUS_PACKET_HEADER) {
        ((OR_BUS_t*)or_bus)->read_decoder = &OR_BUS_length;
        return OR_BUS_PENDING;
    } else {
        return OR_BUS_ERROR_HEADER;
    }
}

OR_BUS_State_t OR_BUS_decoder(OR_BUS_t *or_bus, unsigned char rxchar) {
    return (*or_bus->read_decoder)(or_bus, rxchar);
}

void OR_BUS_build(OR_BUS_t *or_bus, unsigned char *buff, size_t length) {
    // Add the length of the message in the first place
    or_bus->tx.buff[1] = length;
    // Collect all data inside the buffer send
    memcpy(&or_bus->tx.buff[2], buff, length);
    // Evaluating checksum and add in last position
    or_bus->tx.buff[or_bus->tx.length + OR_BUS_LNG_PACKET_HEADER] = 
            OR_BUS_pkg_checksum(buff, 0, length);
}