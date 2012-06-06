#ifndef QR_Encode_h
#define QR_Encode_h

/*
 * QR_Encode.h
 *
 *  Created on: Jan 18, 2012
 *      Author: swex
 *      Updated by: jonasgulle
 */

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// Constant

// Error correction level
#define QR_LEVEL_L	0
#define QR_LEVEL_M	1
#define QR_LEVEL_Q	2
#define QR_LEVEL_H	3

// Data Mode
#define QR_MODE_NUMERAL		0
#define QR_MODE_ALPHABET	1
#define QR_MODE_8BIT		2
#define QR_MODE_KANJI		3

// Group version (model number)
// Version of the code 1 - 40 resolution, etc. 0 = auto-detection based on the size.
#define QR_VERSION_S	0 // 1 ~ 9
#define QR_VERSION_M	1 // 10 ~ 26
#define QR_VERSION_L	2 // 27 ~ 40

#define MAX_ALLCODEWORD	 3706 // The maximum total number of code words
#define MAX_DATACODEWORD 2956 // Maximum data word code (version 40-L)

#define MAX_CODEBLOCK	  153 // (Including RS code word) the maximum number of block data code word
#define MAX_MODULESIZE	  177 // Maximum number of modules in a side
#define MAX_BITDATA		 3917 // finfile data

/////////////////////////////////////////////////////////////////////////////

typedef struct tagRS_BLOCKINFO
{
	uint8_t ncRSBlock;		// RS block number
	uint8_t ncAllCodeWord;	// The number of codewords in the block
	uint8_t ncDataCodeWord;	// The number of data code words (the number of code words - the number of RS code word)
} RS_BLOCKINFO, *LPRS_BLOCKINFO;

/////////////////////////////////////////////////////////////////////////////
// Version code-related information (model number)
typedef struct tagQR_VERSIONINFO
{
	uint8_t nVersionNo;
	uint16_t ncAllCodeWord;

	// Error correction levels (0 = L, 1 = M, 2 = Q, 3 = H)
	uint16_t ncDataCodeWord[4];	// data len

	uint8_t ncAlignPoint;	// position
	uint8_t nAlignPoint[6];	// numberof

	RS_BLOCKINFO RS_BlockInfo1[4]; // EC pos
	RS_BLOCKINFO RS_BlockInfo2[4]; // EC pos
} QR_VERSIONINFO, *LPQR_VERSIONINFO;

typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef BYTE* LPBYTE;
typedef const char* LPCSTR;

/*
 * nLevel      Level of error correction.
 * nVersion    Version of the code 1 - 40, 0 = auto-detection based on size.
 * lpsSource   Data to encode
 * sourcelen   Length of data. A zero will use strlen to determine the size.
 * QR_m_data   Array to write in bits.
 *
 * Returns the square size.
 */
int QR_EncodeData(int nLevel, int nVersion, LPCSTR lpsSource, int sourcelen, unsigned char QR_m_data[]);

#endif

