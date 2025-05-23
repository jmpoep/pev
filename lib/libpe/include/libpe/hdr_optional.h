/*
    libpe - the PE library

    Copyright (C) 2010 - 2017 libpe authors

    This file is part of libpe.

    libpe is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libpe is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with libpe.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIBPE_HDR_OPTIONAL_H
#define LIBPE_HDR_OPTIONAL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// REFERENCE: http://msdn.microsoft.com/en-us/library/windows/desktop/ms680339(v=vs.85).aspx
typedef enum {
	// Unknown subsystem
	IMAGE_SUBSYSTEM_UNKNOWN						= 0,
	// No subsystem required (device drivers and native system processes)
	IMAGE_SUBSYSTEM_NATIVE						= 1,
	// Windows graphical user interface (GUI) subsystem
	IMAGE_SUBSYSTEM_WINDOWS_GUI					= 2,
	// Windows character-mode user interface (CUI) subsystem
	IMAGE_SUBSYSTEM_WINDOWS_CUI					= 3,
	// Old Windows CE subsystem
	IMAGE_SUBSYSTEM_WINDOWS_OLD_CE_GUI				= 4,
	// OS/2 CUI subsystem
	IMAGE_SUBSYSTEM_OS2_CUI						= 5,
	// POSIX CUI subsystem
	IMAGE_SUBSYSTEM_POSIX_CUI					= 7,
	// MMOSA/Native Win32E
	IMAGE_SUBSYSTEM_MMOSA					= 8,
	// Windows CE system
	IMAGE_SUBSYSTEM_WINDOWS_CE_GUI				= 9,
	// Extensible Firmware Interface (EFI) application
	IMAGE_SUBSYSTEM_EFI_APPLICATION				= 10,
	// EFI driver with boot services
	IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER 	= 11,
	// EFI driver with run-time services
	IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER			= 12,
	// EFI ROM image
	IMAGE_SUBSYSTEM_EFI_ROM						= 13,
	// Xbox system
	IMAGE_SUBSYSTEM_XBOX						= 14,
	// Boot application
	IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION	= 16,
	// XBOX Code Catalog
	IMAGE_SUBSYSTEM_XBOX_CODE_CATALOG		= 17
} WindowsSubsystem;

// REFERENCE: http://msdn.microsoft.com/en-us/library/windows/desktop/ms680339(v=vs.85).aspx
typedef enum {
	// IMAGE_LIBRARY_* defined in PECOFF 4.0 (https://bytepointer.com/resources/pecoff_v4.0.htm)
	// DLL initialization function called just after process initialization.
	IMAGE_LIBRARY_PROCESS_INIT			= 0x0001,
	// DLL initialization function called just before process termination.
	IMAGE_LIBRARY_PROCESS_TERM			= 0x0002,
	// DLL initialization function called just after thread initialization.
	// This does not apply to the first thread, which is allocated during process initialization.
	IMAGE_LIBRARY_THREAD_INIT			= 0x0004,
	// DLL initialization function called just before thread initialization.
	// This does not apply to the first thread allocated.
	IMAGE_LIBRARY_THREAD_TERM			= 0x0008,
	// IMAGE_DLLCHARACTERISTICS_RESERVED_10			= 0x0010,
	// ASLR with 64 bit address space.
	// Image can be loaded at address above 4GB.
	IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA		= 0x0020,
	// The DLL can be relocated at load time.
	IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE			= 0x0040,
	// Code integrity checks are forced.
	IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY		= 0x0080,
	// The image is compatible with data execution prevention (DEP).
	// Prevents code execution on the stack, in the PE header and sections without IMAGE_SCN_MEM_EXECUTE.
	IMAGE_DLLCHARACTERISTICS_NX_COMPAT				= 0x0100,
	// The image is isolation aware, but should not be isolated.
	// Prevents loading of manifest file (from embedded resource or external file .manifest)
	// and prevents setting of default activation context.
	// https://learn.microsoft.com/en-us/windows/win32/sbscs/manifest-files-reference
	IMAGE_DLLCHARACTERISTICS_NO_ISOLATION			= 0x0200,
	// The image does not use structured exception handling (SEH).
	// No handlers can be called in this image.
	// Vectored Exception Handler still work.
	IMAGE_DLLCHARACTERISTICS_NO_SEH					= 0x0400,
	// Do not bind the image.
	IMAGE_DLLCHARACTERISTICS_NO_BIND				= 0x0800,
	// Image is a Wx86 Thunk DLL.
	// Valid only for non-x86 (risc) DLL files.
	// Can be generated by undocumented MSVC5+ linker flag /dllchar:x86thunk.
	IMAGE_DLLCHARACTERISTICS_X86_THUNK			= 0x1000,
	// Image should execute in an AppContainer (Metro Apps in Windows 8).
	// Valid only for EXE files.
	IMAGE_DLLCHARACTERISTICS_APPCONTAINER			= 0x1000,
	// A WDM driver.
	IMAGE_DLLCHARACTERISTICS_WDM_DRIVER				= 0x2000,
	// Image supports Control Flow Guard
	IMAGE_DLLCHARACTERISTICS_GUARD_CF			= 0x4000,
	// The image is terminal server (Remote Desktop Services) aware.
	// https://learn.microsoft.com/en-us/windows/win32/termserv/application-compatibility-layer
	IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE	= 0x8000
} ImageDllCharacteristics;

// PECOFF 4.0 (https://bytepointer.com/resources/pecoff_v4.0.htm) and WDK's ntimage.h
typedef enum {
	// DLL initialization function: Halt prior to executing first instruction.
	IMAGE_LOADER_FLAGS_BREAK_ON_LOAD	= 0x00000001,
	// DLL initialization function: Break prior to executing first instruction; effect is similar to a breakpoint.
	IMAGE_LOADER_FLAGS_DEBUG_ON_LOAD	= 0x00000002,
	// COM+ image executable; obsolete and automatically set by loader when IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR is present.
	IMAGE_LOADER_FLAGS_COMPLUS		= 0x00000001,
	// Global subsections apply across TS sessions.
	IMAGE_LOADER_FLAGS_SYSTEM_GLOBAL	= 0x01000000
} ImageLoaderFlags;

typedef enum {
	MAGIC_PE32_0	= 0x000,
	MAGIC_ROM	= 0x107,
	MAGIC_PE32	= 0x10b,
	MAGIC_PE64	= 0x20b // PE32+
} opt_type_e;

#pragma pack(push, 1)

typedef struct {
	uint16_t Magic;
	uint8_t MajorLinkerVersion;
	uint8_t MinorLinkerVersion;
	uint32_t SizeOfCode;
	uint32_t SizeOfInitializedData;
	uint32_t SizeOfUninitializedData;
	uint32_t AddressOfEntryPoint;
	uint32_t BaseOfCode;
	uint32_t BaseOfData;
	uint32_t BaseOfBss;
	uint32_t GprMask;
	uint32_t CprMask[4];
	uint32_t GpValue;
} IMAGE_ROM_OPTIONAL_HEADER;

// REFERENCE: http://msdn.microsoft.com/en-us/library/windows/desktop/ms680339(v=vs.85).aspx
typedef struct {
	uint16_t Magic;
	uint8_t MajorLinkerVersion;
	uint8_t MinorLinkerVersion;
	uint32_t SizeOfCode;
	uint32_t SizeOfInitializedData;
	uint32_t SizeOfUninitializedData;
	uint32_t AddressOfEntryPoint;
	uint32_t BaseOfCode;
	uint32_t BaseOfData; // only in PE32
	uint32_t ImageBase;
	uint32_t SectionAlignment;
	uint32_t FileAlignment;
	uint16_t MajorOperatingSystemVersion;
	uint16_t MinorOperatingSystemVersion;
	uint16_t MajorImageVersion;
	uint16_t MinorImageVersion;
	uint16_t MajorSubsystemVersion;
	uint16_t MinorSubsystemVersion;
	uint32_t Win32VersionValue;
	uint32_t SizeOfImage;
	uint32_t SizeOfHeaders;
	uint32_t CheckSum;
	uint16_t Subsystem; // WindowsSubsystem
	uint16_t DllCharacteristics;
	uint32_t SizeOfStackReserve;
	uint32_t SizeOfStackCommit;
	uint32_t SizeOfHeapReserve;
	uint32_t SizeOfHeapCommit;
	uint32_t LoaderFlags;
	uint32_t NumberOfRvaAndSizes;
	// IMAGE_DATA_DIRECTORY DataDirectory[MAX_DIRECTORIES];
} IMAGE_OPTIONAL_HEADER_32;

// REFERENCE: http://msdn.microsoft.com/en-us/library/windows/desktop/ms680339(v=vs.85).aspx
typedef struct {
	uint16_t Magic;
	uint8_t MajorLinkerVersion;
	uint8_t MinorLinkerVersion;
	uint32_t SizeOfCode;
	uint32_t SizeOfInitializedData;
	uint32_t SizeOfUninitializedData;
	uint32_t AddressOfEntryPoint;
	uint32_t BaseOfCode;
	uint64_t ImageBase;
	uint32_t SectionAlignment;
	uint32_t FileAlignment;
	uint16_t MajorOperatingSystemVersion;
	uint16_t MinorOperatingSystemVersion;
	uint16_t MajorImageVersion;
	uint16_t MinorImageVersion;
	uint16_t MajorSubsystemVersion;
	uint16_t MinorSubsystemVersion;
	uint32_t Win32VersionValue;
	uint32_t SizeOfImage;
	uint32_t SizeOfHeaders;
	uint32_t CheckSum;
	uint16_t Subsystem; // WindowsSubsystem
	uint16_t DllCharacteristics;
	uint64_t SizeOfStackReserve;
	uint64_t SizeOfStackCommit;
	uint64_t SizeOfHeapReserve;
	uint64_t SizeOfHeapCommit;
	uint32_t LoaderFlags;
	uint32_t NumberOfRvaAndSizes;
	// IMAGE_DATA_DIRECTORY DataDirectory[MAX_DIRECTORIES];
} IMAGE_OPTIONAL_HEADER_64;

typedef struct {
	uint16_t type; // opt_type_e
	size_t length;
	IMAGE_OPTIONAL_HEADER_32 *_32;
	IMAGE_OPTIONAL_HEADER_64 *_64;
	IMAGE_ROM_OPTIONAL_HEADER *_rom;
} IMAGE_OPTIONAL_HEADER;

#pragma pack(pop)

#ifdef __cplusplus
} // extern "C"
#endif

#endif
