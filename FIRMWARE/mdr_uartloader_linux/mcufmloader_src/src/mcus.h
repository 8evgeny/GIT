/*===========================================================================
 *
 *  MCU firmware loader
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2016
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File mcus.h: MCU definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef _MCUS_H
#define _MCUS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bastypes.h"

//MCU model names
#define MCUNM_1986BE9X                  "1986be9x"
#define MCUNM_1986BE1X                  "1986be1x"
#define MCUNM_1901BC1X                  "1901bc1x"

//MCU model ids
#define MCU_1986BE9X                    198690
#define MCU_1986BE1X                    198610
#define MCU_1901BC1X                    190110

/** */
typedef struct tag_MCUParams
{
	/** MCU model name */
	const char *name;	

	/** MCU model id */
	s32 id;

	/** FLASH base address */
	u32 fmBaseAddr;

	/** RAM base address */
	u32 ramBaseAddr;

	/** FLASH size, bytes */
	u32 fmSize;

	/** RAM size, bytes */
	u32 ramSize;

	/** Initial speed to connect loader */
	s32 ldInitSpeed;

	/** Loader ID string */
	const char *ldIdStr;

	/** Pointer to loader executable code */
	const u8 *ldCode;

	/** Start address of loader (relative ramBaseAddr) */
	u32 ldStartAddr;

	/** Size of loader code */
	u32 ldCodeSize;

} MCUParams;

/** List of available mcus */
extern const MCUParams *mcus[];

/** */
const MCUParams *GetMCUByName(const char *name);

/** */
const MCUParams *GetMCUByID(s32 id);

#ifdef __cplusplus
}
#endif

#endif /* !_MCUS_H */

/*===========================================================================
 * End of file mcus.cpp
 *===========================================================================*/
