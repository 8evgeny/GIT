#pragma once

/**
 * \mainpage
 * ISLP
 * ==========================
 * \brief     Simple device detection/validation protocol for IPS intercom system.
 *
 * --------------------------
 * ##Contents:##
 * -# @ref protocol "Protocol description."
 *  -# Commion
 *  -# Stations
 *  -# Functions
 *    - Yell
 *    - Station Response
 *    - SetIP
 *    - Accept
 * -# @ref relations "Relations Model"
 * -# @ref realisation "Realisation example"
 *
 * --------------------------
 * \author    Dmitry Donskih
 * \version   0.1
 * \copyright GNU GPL v3
 * \date      2020
*/

/**
 * \page protocol Part 1. Protocol description.
 * \tableofcontents
 * \section common 1. Common description
 *
 * ISLP - Announce devices to get ready for configuration, validate IPS devices,
 * and sets up network settings.
 * __________________________
 *
 * Structure:
 * |Magic Number|Function Code|     PAYLOAD     | CRC32 |
 * |:----------:|:-----------:|:---------------:|:-----:|
 * | 4 bytes    | 4 bytes     | x bytes         |4 bytes|
 * ____________________________
 *
 * Description:
 * - **Magic Number** `0x5e7470cd` -unique number of protocol. Packets that aint valid by magicnumber must be dropped by station.
 * - **Function Code** `uint32_t` -ID code of function. Described in enum Functions{} section.
 * - **Payload** `uint8_t[]` - Function payload.
 * - **CRC32** - `uint32_t` checksum of packet (Magic Number,Function Code and PAYLOAD sections). Polynomial 0x04C11DB7.
 *
 * ______________________________
 * @note
 * - Every message masked by key(fields PAYLOAD & CRC32) except Yell message.
 * - CRC32 generated with standart Ethernet polynomial `0x04C11DB7`
 *
 * ______________________________
 * Functions:
 * - @ref ISLP::yellMsg "Yell()" - function announce devices to get ready for config, sets up protocol validation key,
 * and request to send their settings.
 * - @ref ISLP::setStationMessage "SetIP(intercomStation, Key)" - sets new IP/Mask/Gateway to specified station(by Serial number intercomStation::serial_number)
 * - @ref ISLP::stationResponse "StationInfo(intercomStation)" - station sens their info structure masked with Key sent by Yell message(bytewise XOR operation of payload and crc32 section)
 * - @ref ISLP::stationResponse "Accept(intercomStation)" - responce by station on SetIP Request. Returns new info structure(struct intercomStation)
 *
 * \section stations 2. Stations description
 *
 * Serialized data structure:
 * |Serial number|MAC-address| Type  |   IP   |  Mask  | Gateway |  Name  |
 * |:-----------:|:---------:|:-----:|:------:|:------:|:-------:|:------:|
 * |12 bytes     |6 bytes    |2 bytes|4 bytes |4 bytes |4 bytes  |12 bytes|
 *
 * \section functions 3. Functions description
 * \subsection fn_yell 3.1 Yell(Key)
 *
 * @ref ISLP::yellMsg "Yell()" - function announce devices to get ready for config, sets up protocol validation key,
 * and request to send their settings.
 *
 * Structure:
 * |Magic Number|Function Code|     PAYLOAD     |Key(Magic Number)|
 * |:----------:|:-----------:|:---------------:|:---------------:|
 * | 4 bytes    | 4 bytes     | x bytes         |4 bytes          |
 *
 * Payload and it's length generated randomly, after which the CRC32 checksum (0x04C11DB7) of the entire packet is calculated, the checksum is the session key. The magic number is masked by the received key and written to the end of the packet.
 *
 * \subsection fn_response 3.2 StationInfo(Station)
 *
 * @ref ISLP::stationResponse "StationInfo(intercomStation)" - station sens their info structure masked with Key sent by Yell message(bytewise XOR operation of payload and crc32 section)
 *
 * |Magic Number|Function Code|Serial number|MAC-address| Type  |   IP   |  Mask  | Gateway |  Name  | CRC32 |
 * |:----------:|:-----------:|:-----------:|:---------:|:-----:|:------:|:------:|:-------:|:------:|:-----:|
 * | 4 bytes    | 4 bytes     |12 bytes     |6 bytes    |2 bytes|4 bytes |4 bytes |4 bytes  |12 bytes|4 bytes|
 * |	        |             |M            |M	        |M      |M       |M       |M        |M       |M      |
 *
 * **M** is for masked fields
 *
 * \subsection fn_setip 3.3 SetIP(Station)
 *
 * @ref ISLP::setStationMessage "SetIP(intercomStation, Key)" - sets new IP/Mask/Gateway to specified station(by Serial number intercomStation::serial_number)
 *
 * |Magic Number|Function Code|Serial number|MAC-address| Type  |   IP   |  Mask  | Gateway |  Name  | CRC32 |
 * |:----------:|:-----------:|:-----------:|:---------:|:-----:|:------:|:------:|:-------:|:------:|:-----:|
 * | 4 bytes    | 4 bytes     |12 bytes     |6 bytes    |2 bytes|4 bytes |4 bytes |4 bytes  |12 bytes|4 bytes|
 * |	        |             |M            |M	        |M      |M       |M       |M        |M       |M      |
 *
 * **M** is for masked fields
 *
 * \subsection fn_accept 3.4 Accept(Station)
 *
 * @ref ISLP::stationResponse "Accept(intercomStation)" - responce by station on SetIP Request. Returns new info structure(struct intercomStation)
 *
 * |Magic Number|Function Code|Serial number|MAC-address| Type  |   IP   |  Mask  | Gateway |  Name  | CRC32 |
 * |:----------:|:-----------:|:-----------:|:---------:|:-----:|:------:|:------:|:-------:|:------:|:-----:|
 * | 4 bytes    | 4 bytes     |12 bytes     |6 bytes    |2 bytes|4 bytes |4 bytes |4 bytes  |12 bytes|4 bytes|
 * |	        |             |M            |M	        |M      |M       |M       |M        |M       |M      |
 *
 * **M** is for masked fields
 *
 */

/**
 * \page relations Part 2. RelationsModel.
 *
 * After the IP stack is loaded, each IPS device joins the multicast group 239.168.10.100 and starts listening on port 13120. Upon receiving the Yell packet, the device decodes the session key, checks its validity and, if successful, sends a StationInfo message with its own serialized structure to the same group on port 13121 intercomStation masked by the received key. The session key is retained until a reboot or a new VA is received.
 * Lead package Yell.
 *
 * Upon receipt of the SetIP packet, the device validates it (decodes the packet body with the checksum, verifies the checksums). If successful, it deserializes the resulting structure and compares the serial_number field with its own serial number, if they match, writes the ipaddr, netmask, gateway and name fields to its own structure and updates the record in nonvolatile memory. After that, it sends back an Accept packet with the updated intercomStation structure and reboots.
 *
 */

 /**
 * \page realisation Part 3. Realisation example.
 *
 * @ref ISLP "ISLP"
 *
 */

/**
 * @ref ISLP "ISLP" - Announce devices to get ready for configuration, validate IPS devices,
 * and sets up network settings.
 * __________________________
 *
 * Structure:
 * |Magic Number|Function Code|     PAYLOAD     | CRC32 |
 * |:----------:|:-----------:|:---------------:|:-----:|
 * | 4 bytes    | 4 bytes     | x bytes         |4 bytes|
 * ____________________________
 *
 * Description:
 * - **Magic Number** `0x5e7470cd` -unique number of protocol. Packets that aint valid by magicnumber must be dropped by station.
 * - **Function Code** `uint32_t` -ID code of function. Described in enum Functions{} section.
 * - **Payload** `uint8_t[]` - Function payload.
 * - **CRC32** - `uint32_t` checksum of packet (Magic Number,Function Code and PAYLOAD sections). Polynomial 0x04C11DB7.
 *
 * ______________________________
 * @note
 * - Every message masked by key(fields PAYLOAD & CRC32) except Yell message.
 * - CRC32 generated with standart Ethernet polynomial `0x04C11DB7`
 *
 * ______________________________
 * Functions:
 * - @ref ISLP::yellMsg "Yell()" - function announce devices to get ready for config, sets up protocol validation key,
 * and request to send their settings.
 * - @ref ISLP::setStationMessage "SetIP(intercomStation, Key)" - sets new IP/Mask/Gateway to specified station(by Serial number intercomStation::serial_number)
 * - @ref ISLP::stationResponse "StationInfo(intercomStation)" - station sens their info structure masked with Key sent by Yell message(bytewise XOR operation of payload and crc32 section)
 * - @ref ISLP::stationResponse "Accept(intercomStation)" - responce by station on SetIP Request. Returns new info structure(struct intercomStation)
 */
