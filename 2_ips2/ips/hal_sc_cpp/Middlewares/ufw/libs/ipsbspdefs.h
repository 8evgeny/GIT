#ifndef IPSBSPDEFS_H
#define IPSBSPDEFS_H
/*
 * IPS boards support package defines
 * A common defines for all "GIT-Comm IPS" boards,
 * "GIT-Comm IPS Configurator" and other support
 * software.
 */
/*
MIT License

Copyright (c) 2020 Dmitry Donskikh

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
 *  Common defines
 */

#define IPS_BSP_EEPROM_MAXFILENAME          12U                     // Maximum length of file name on EEPROM filesystem
#define IPS_BSP_EEPROM_MAXFILES             16U                     // EEPROM filesystem maximum file count


#define IPS_BSP_NW_RESPONSE_SIZE            sizeof(uint32_t)*2      // size of response is 32-bit command + uint32_t type parameter (example Accept(uint32_t size);)

#define IPS_BSP_NW_RESPONSE                 0x0F0F0000              // Response to control functions. Lower half-word must be an response
#define IPS_BSP_NW_RESPONSE_ERROR           0xFFFF0000              // Error response. Lower half-word must be an response


/*
 * Network protocol sender attributes
*/
#define IPS_BSP_NW_SENDER_CONFIGURATOR      0x01010000              // Sender of command is "IPS Configurator" software
#define IPS_BSP_NW_SENDER_OTHER             0x01000000              // Sender os command is another support software

/*
 * File transfer protoclol
 * SendFile(uint32_t file_size,uint8_t drive,uint32_t chunk_size,char name[MAXFILENAME])
 * File transfer start syncronization command.
 * Wait for Accept(uint32_t size)
 * After that client send data with DataSend(uint32_t chunk_size,uint8_t chunk[chunk_size])
 * with accepting every chunk transfer
 * Connection closes by server after recieve EOT() command
 *
 * to get file list send getFileList(uint8_t drive) request.
 * device must return Accept(uint32_t size,char[] filelist) where file list is a null-terminated("C-style") string, with comma separeted file names
*/
#define IPS_BSP_FTRANSFER_CMD_FILESEND		0x01000101              //SendFile(uint32_t file_size,uint8_t drive,uint32_t chunk_size,char name[MAXFILENAME]) - initialize file transfer
#define IPS_BSP_FTRANSFER_CMD_DATASEND		0x01000102              //DataSend(uint32_t chunk_size,uint8_t chunk[chunk_size]) -sends data chunk
#define IPS_BSP_FTRANSFER_CMD_GETLIST       0x01000103              //getFileList(uint8_t drive)
#define IPS_BSP_FTRANSFER_CMD_EOT           0x010001FF              //EOT() -end of transmission


/*
 * Configuration transfer protocol
 * "IPS Configurator" software only
 * Configuration starts with Sync() request to device if device answer with Accept()
 * Configurator starts to transfer
 * LoadSysInfo(uint32_t size) - system information syncronization command after recieve Accept(uint32_t size)
 * software sends data with DataSend(uint32_t chunk_size,uint8_t chunk[chunk_size]) with accepting of transfer
 * LoadIpTable(uint32_t size) - IP table syncronization command after recieve Accept(uint32_t size)
 * software sends data with DataSend(uint32_t chunk_size,uint8_t chunk[chunk_size]) with accepting of transfer
 * LoadKeysTable(uint32_t size) - Board key mapping table syncronization command after recieve Accept(uint32_t size)
 * software sends data with DataSend(uint32_t chunk_size,uint8_t chunk[chunk_size]) with accepting of transfer
 * LoadControllinesTable(uint32_t size) - Control lines mapping table syncronization command after recieve Accept(uint32_t size)
 * software sends data with DataSend(uint32_t chunk_size,uint8_t chunk[chunk_size]) with accepting of transfer
 *
 * send EOT() command to end transmission and server closes TCP connection
 */

#define IPS_BSP_CONF_CMD_SYNC               0x01010200
#define IPS_BSP_CONF_CMD_LOADIPTABLE        0x01010201
#define IPS_BSP_CONF_CMD_LOADSYSINFO        0x01010202
#define IPS_BSP_CONF_CMD_LOADKEYS           0x01010203
#define IPS_BSP_CONF_CMD_LOADCONTROLS       0x01010204
#define IPS_BSP_CONF_CMD_DATASEND           0x01000002
#define IPS_BSP_CONF_CMD_EOT                0x010000FF

/*
 * Protocol response definitions
 *
*/

#define IPS_BSP_RESPONSE_ACCEPT             0x0F0F0001              //Accept(uint32_t size)
#define IPS_BSP_RESPONSE_ER_FILEXISTS		0xFFFF0101              // ERROR: file with given name exists on filesystem
#define IPS_BSP_RESPONSE_ER_DRIVEFULL		0xFFFF0102              // ERROR: given storage device have no free space for transfer
#define IPS_BSP_RESPONSE_ER_WRONGDRIVE		0xFFFF0103              // ERROR: given storage device doesn't exists on board
#define IPS_BSP_RESPONSE_ERROR              0xFFFFFFFF              // ERROR: common error
#define IPS_BSP_RESPONSE_ER_SIZEMISMATCH    0xFFFF0201              // ERROR: mismatch of recieved data type (ex. if server waits for uint8_t[8] and get uint16_t[8] or uint8_t[9])

/*
 * IPS boards storage devices
*/

#define IPS_BSP_FSDEVICE_EEPROM				0x01                    // Storage device: External EEPROM(I2C bus)
#define IPS_BSP_FSDEVICE_INTFLASH			0x02                    // Storage device: uC internal flash memory
#define IPS_BSP_FSDEVICE_SRAM				0x03                    // Storage device: system RAM memory
#define IPS_BSP_FSDEVICE_SDMMC				0x04                    // Storage device: SD/MMC card (octo SPI bus)   (UNAVAILABLE on IPS_BOARDS)

#define FTRANSFER_MAX_CHUNK_SIZE            1024
#define FTRANSFER_MAX_PACKET_SIZE           FTRANSFER_MAX_CHUNK_SIZE+4+4 //4 is a size of uint32_t

#define IPS_RAWPROT_MAGICNUMBER             0xc03efd03              //GIT-COMM IPS raw searchicg protocol magicnumber
/*
 * Raw network searching:
 * HOST ->Broadcast ${RAWPROT_MAGICNUMBER}+<RANDOM>${RNG_SALT[44]};
 *
 * DEVICE ->Broadcast ${RAWPROT_MAGICNUMBER}+(${SERIAL[8]}+${MACADDR[6]}+${TYPE[2]}+${IP[4]}+${MASK[4]}+${GATEWAY[4]}+${NAME[12]}+${CRC32[4]})^RNG_SALT
 * HOST ->Broadcast ${RAWPROT_MAGICNUMBER}+(${CMD[2]+${IP[4]}+${MASK[4]}+${GATEWAY[4]})^RNG_SALT
 * 0x01 <-IP SETTINGS SETUP && REBOOT
*/

#endif // IPSBSPDEFS_H
