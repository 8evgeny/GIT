#ifndef MDRMCUDESCRIPTOR_H
#define MDRMCUDESCRIPTOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

namespace ARM {
	enum {
		CPUID_ADDR = 0xe000ed00u, //
	};
	enum CorePartNo {
		CM4 = 0xC24, // Cortex-M4
		CM3 = 0xC23, // Cortex-M3
		CM1 = 0xC21, // Cortex-M1
		CM0 = 0xC20, // Cortex-M0
		CM0P= 0xC60, // Cortex-M0+
		CM33= 0xD21, // Cortex-M33
		CM55= 0xD22, // Cortex-M55
	};
	enum CoreImplementer {
		ARM_Limited = 0x41, // ARM Limited
		DigEquipCrp = 0x44, // Digital Equipment Corporation
		MtrlFrscSem = 0x4D, // Motorola - Freescale Semiconductor Inc.
		MarvellSemi = 0x56, // Marvell Semiconductor Inc.
		AppleCorpor = 0x61, // Apple Inc.
		IntelArmPrt = 0x69, // Intel ARM parts
	};

	struct CPUID
	{
		uint32_t revision:4;  // core patch number
		uint32_t partNo:12;   // CorePartNo
		uint32_t arch:4;      // 0xC = ARMv6-M, ?? = 0xF.
		uint32_t variant:4;   // core revision
		uint32_t implementer:8;//
	};
	typedef struct CPUID CPUID_t;

	static const char* cpuidPartNoDecode(const CPUID_t cpuid) {
		switch (cpuid.partNo) {
			case CorePartNo::CM0: return "Cortex-M0";
			case CorePartNo::CM0P:return "Cortex-M0+";
			case CorePartNo::CM1: return "Cortex-M1";
			case CorePartNo::CM3: return "Cortex-M3";
			case CorePartNo::CM4: return "Cortex-M4";
			default: return "Unknown";
		}
	}

	static const char* cpuidImplementerDecode(const CPUID_t cpuid) {
		switch (cpuid.implementer) {
			case CoreImplementer::ARM_Limited: return "ARM Limited";
			case CoreImplementer::DigEquipCrp: return "Digital Equipment Corporation";
			case CoreImplementer::MtrlFrscSem: return "Motorola - Freescale Semiconductor Inc.";
			case CoreImplementer::MarvellSemi: return "Marvell Semiconductor Inc.";
			case CoreImplementer::AppleCorpor: return "Apple Inc.";
			case CoreImplementer::IntelArmPrt: return "Intel ARM parts";
			default: return "Unknown";
		}
	}
} // namespace ARM


namespace Mcu {
	
	struct AbstractMcu {
		const uint32_t flashAddr;
		const uint32_t flashSize;
		const uint32_t ramAddr;
		const uint32_t ramSize;
		const uint32_t xramAddr;
		const uint32_t xramSize;
		const uint32_t romAddr;
		const uint32_t romSize;
	};
	
	//  flashAdr  flashSize      ramAdr    ramSize     xramAdr   xramSize  romAdr romSize
	const struct AbstractMcu UndefinedMcuMem =
	{          0,         0,          0,         0,          0,         0,      0,      0};
	const struct AbstractMcu MDR1986BE9XMem =
	{ 0x08000000,  128*1024, 0x20000000,   32*1024, 0x20000000,   32*1024,    0x0,   1024};
	const struct AbstractMcu MDR1986BE1Mem =
	{        0x0,  128*1024, 0x20000000,   32*1024, 0x20100000,   16*1024,    0x0,   4096};
	const struct AbstractMcu MDR1986BE3Mem = MDR1986BE1Mem;
	const struct AbstractMcu MDR1986BE4Mem =
	{        0x0,  128*1024, 0x20000000,   16*1024, 0x20000000,   16*1024,    0x0,   4096};
	const struct AbstractMcu MDR1901VC1Mem =
	{ 0x08000000,  128*1024, 0x20000000,   32*1024, 0x20000000,   32*1024,    0x0,   4096};
	
	enum Type {
		UndefinedMcu,
		MDR1986BE1,
		MDR1986BE9X,
		MDR1986BE3,
		MDR1986BE4,
		MDR1901VC1,
		TotalMcuCount
	};
	
	const AbstractMcu mem[6] = {
		UndefinedMcuMem,
		MDR1986BE1Mem,
		MDR1986BE9XMem,
		MDR1986BE3Mem,
		MDR1986BE4Mem,
		MDR1901VC1Mem,
	}; /*! ДЕРЖАТЬ ПОРЯДОК ИНИЦИАЛИЗАЦИИ СТРОГО СИНХРОННО С enum Type !!! */
    static const char* name[6] = {
		"UndefinedMcu",
		"MDR1986BE1",
		"MDR1986BE9X",
		"MDR1986BE3",
		"MDR1986BE4",
		"MDR1901VC1",
	}; /*! ДЕРЖАТЬ ПОРЯДОК ИНИЦИАЛИЗАЦИИ СТРОГО СИНХРОННО С enum Type !!! */
    static const char* shortName[6] = {
		"auto",
		"ve1",
		"ve9x",
		"ve3",
		"ve4",
		"vc1",
	}; /*! ДЕРЖАТЬ ПОРЯДОК ИНИЦИАЛИЗАЦИИ СТРОГО СИНХРОННО С enum Type !!! */

/*
const AbstractMcu mem[] = {
//  flashAdr  flashSize      ramAdr    ramSize     xramAdr   xramSize  romAdr romSize
{          0,         0,          0,         0,          0,         0,      0,      0},
{ 0x08000000,  128*1024, 0x20000000,   32*1024, 0x20000000,   32*1024,    0x0,   1024},//be9x
{        0x0,  128*1024, 0x20000000,   32*1024, 0x20100000,   16*1024,    0x0,   4096},//be1,be3
{        0x0,  128*1024, 0x20000000,   16*1024, 0x20000000,   16*1024,    0x0,   4096},//be4
{ 0x08000000,  128*1024, 0x20000000,   32*1024, 0x20000000,   32*1024,    0x0,   4096},//vc1
};
*/

} // namespace Mcu

#ifdef __cplusplus
}; // extern "C" {
#endif

#endif // MDRMCUDESCRIPTOR_H

