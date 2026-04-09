/*
 disc_io.h
 Interface template for low level disc functions.
 
 Edited 2026 for inclusion in Retro Rewind Channel (c) James "Jacher" Croucher

 Edited 2014 by Alex Chadwick for inclusion in bslug
 Copyright (c) 2006 Michael "Chishm" Chisholm
 Based on code originally written by MightyMax
	
 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.
  3. The name of the author may not be used to endorse or promote products derived
     from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef RRC_OGC_DISC_IO_INCLUDE
#define	RRC_OGC_DISC_IO_INCLUDE

#include <stdbool.h>
#include <stdint.h>
#include <gccore.h>

#define FEATURE_MEDIUM_CANREAD      0x00000001
#define FEATURE_MEDIUM_CANWRITE     0x00000002
#define FEATURE_GAMECUBE_SLOTA      0x00000010
#define FEATURE_GAMECUBE_SLOTB      0x00000020
#define FEATURE_GAMECUBE_DVD        0x00000040
#define FEATURE_WII_SD              0x00000100
#define FEATURE_WII_USB             0x00000200
#define FEATURE_WII_DVD             0x00000400

typedef uint32_t sec_t;

// prefix with rrc to prevent collision
typedef s32 (* RRC_FN_MEDIUM_STARTUP)(void) ;
typedef bool (* RRC_FN_MEDIUM_ISINSERTED)(void) ;
typedef bool (* RRC_FN_MEDIUM_READSECTORS)(sec_t sector, sec_t numSectors, void* buffer) ;
typedef bool (* RRC_FN_MEDIUM_WRITESECTORS)(sec_t sector, sec_t numSectors, const void* buffer) ;
typedef bool (* RRC_FN_MEDIUM_CLEARSTATUS)(void) ;
typedef bool (* RRC_FN_MEDIUM_SHUTDOWN)(void) ;
typedef bool (* RRC_FN_MEDIUM_GETSTATUS)(u32* status) ;

typedef enum {
	SDIO_STARTUP_SUCCESS = 0,
	SDIO_STARTUP_NO_CARD = -1,
	SDIO_STARTUP_INIT_FAILED = -2,
	SDIO_STARTUP_OTHER_ERROR = -3,
	LIBFAT_ERROR_MOUNT_FAILED = -4,
   	LIBFAT_ERROR_WRITE_PROTECTED = -5,
	SDIO_READ_ERROR = -6,
	SDIO_WRITE_ERROR = -7,
} SDIO_RESULT;

// Return a static string describing the error.
const char* sdio_get_error_message(SDIO_RESULT result);

struct RRC_DISC_INTERFACE_STRUCT {
	unsigned long			ioType ;
	unsigned long			features ;
	RRC_FN_MEDIUM_STARTUP		startup ;
	RRC_FN_MEDIUM_ISINSERTED	isInserted ;
	RRC_FN_MEDIUM_READSECTORS	readSectors ;
	RRC_FN_MEDIUM_WRITESECTORS	writeSectors ;
	RRC_FN_MEDIUM_CLEARSTATUS	clearStatus ;
	RRC_FN_MEDIUM_SHUTDOWN		shutdown ;
	RRC_FN_MEDIUM_GETSTATUS		getStatus ;
} ;

typedef struct RRC_DISC_INTERFACE_STRUCT RRC_DISC_INTERFACE ;

#endif	// define RRC_OGC_DISC_IO_INCLUDE
