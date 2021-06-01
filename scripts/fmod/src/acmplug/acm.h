#ifndef __ACM_H__
#define __ACM_H__



//  # of bits |     Value           |  Description
// -----------+---------------------+-----------------
//     24     | 0x97 0x28 0x03      | ACM-signature1
//      8     | 0x01                | ACM-signature2
//     32     | count of samples    | a sample is 16 bit
//     16     | # of channels       | 1-mono, 2-stereo (actually not used)
//     16     | frequency (bitrate) | 22050 in all known ACMs (actually not used)
//      4     | packAttrs           | (see below)
//     12     | packAttrs2          |


struct AcmHeader
{
	unsigned int 	signature;
	unsigned int 	sampleCount;
	unsigned short  channelCount;
	unsigned short  bitrate;
	unsigned int    packAttributeLow : 4;
	unsigned short  packAttribteHigh : 12; 
}

#endif // __ACM_H__