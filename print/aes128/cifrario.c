#define AES_128
#include <stdint.h>
#include <string.h>
#include <stdio.h>
typedef uint32_t u32;
typedef uint8_t u8;
#include "test_vect.h"
//---------------------------------- CONSTANTS ---------------------------------------
#ifdef AES_256
#define N_ROUNDS				14		//The number of rounds used by the cipher
#define KEY_ROUNDS				8		//The number of rounds used by the ks 
#define BLOCK_SIZE              16
#define KEY_SIZE				32
#define BLOCK_SIZE_BITS         128
#endif

#ifdef AES_192
#define N_ROUNDS				12		//The number of rounds used by the cipher
#define KEY_ROUNDS				9		//The number of rounds used by the ks 
#define BLOCK_SIZE              16
#define KEY_SIZE				24
#define BLOCK_SIZE_BITS         128
#endif

#ifdef AES_128
#define N_ROUNDS				10		//The number of rounds used by the cipher
#define KEY_ROUNDS				10		//The number of rounds used by the ks
#define BLOCK_SIZE              16
#define KEY_SIZE				BLOCK_SIZE
#define BLOCK_SIZE_BITS         128
#endif

static const u8 _sBox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
    0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
    0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
    0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
    0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
    0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
    0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
    0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
    0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
    0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
    0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16,
};
#define trascinaMSB(x) ((signed char)((x & 0x80)) >> 7)
#define xTimes(value) (((value) << 1) ^ (0x1B & trascinaMSB(value)   ))
__attribute__((always_inline)) static inline uint8_t fieldmultiply(uint8_t FirstPoly, uint8_t SecondPoly) {
	uint8_t product = 0;
// #define mult(x) product= (SecondPoly & 0x01) ?  product ^ FirstPoly : product;\
//	FirstPoly=(FirstPoly & 0x80) ? ((FirstPoly << 1) ^ 0x1B) : (FirstPoly << 1);\
//	SecondPoly >>= 1;
#define mult(x) product= product ^ (FirstPoly & trascinaMSB(SecondPoly << 7));\
	FirstPoly=xTimes(FirstPoly);\
	SecondPoly >>= 1;

	mult(0);
	mult(1);
	mult(2);
	mult(3);
	mult(4);
	mult(5);
	mult(6);
	mult(7);
#undef mult
	return product;
}

__attribute__((always_inline)) static inline uint8_t fieldsquare(uint8_t input) {
	uint8_t square = 0;
        /* start interleaving the first 4 bits of input*/
	square ^= input & 0x01;
	square ^= (input << 1) & 0x04;
	square ^= (input << 2) & 0x10;
	square ^= (input << 3) & 0x40;
	/* add the i-th bit of the input, in case when squared is 
	 * too high as a degree, directly through adding its reduced
	 * value modulo the AES polynomial */
	/* a^8 = a^4 + a^3 + a + 1 */
	
	square = square ^ (0x1B & trascinaMSB(input << 3));
	/* a^10 = a^6 + a^5 + a^3 + a^2 */
	square = square ^ (0x6C & trascinaMSB(input << 2));
	/* a^12 = a^7 + a^5 + a^3 + a + 1 */
	square = square ^ (0xAB & trascinaMSB(input << 1));
	/* a^14 = a^7 + a^4 + a^3 + a */
	square = square ^ (0x9A & trascinaMSB(input));
	return square;
}

__attribute__((maskedcopy)) uint8_t _sBox_computational(uint8_t input)
{
    uint8_t accu = input,temp,temp2;
    /* We need to exponentiate to 254=0xFE -> all ones except for the last */
    temp2 = fieldsquare(accu); /* x^2 */
    input = fieldmultiply(temp2, input); /* temp = x^2, input = x^3 */
    temp = fieldsquare(input);
    temp = fieldsquare(temp);  /* temp = (x^3)^4 = x^12*/   
    accu = fieldmultiply(temp,input); /* accu = x^12 x^3 = x^15 */
    accu = fieldsquare(accu);
    accu = fieldsquare(accu);
    accu = fieldsquare(accu);
    accu = fieldsquare(accu); /* accu = (x^15)^16 = x^240*/
    accu = fieldmultiply(accu,temp); /* accu = x^240 x^12 = x^252*/
    accu = fieldmultiply(accu,temp2); /* accu = x^252 x^2 = x^254*/
    /* affine transformation post-inversion */
    input = accu;
    accu = (accu << 1) ^ (accu >> 7);
    input = input ^ accu;
    accu = (accu << 1) ^ (accu >> 7);
    input = input ^ accu;
    accu = (accu << 1) ^ (accu >> 7);
    input = input ^ accu;
    accu = (accu << 1) ^ (accu >> 7);
    input = input ^ accu;
    input = input ^ 0x63;
    return input;
}



/** Evolutes the key.
  *
  *        {k[0], k[1], k[2], ... , k[13], k[14], k[15]}
  *         ^									  ^
  * most significant						least significant
  *
  */
//#define xTimes(value) (((value) & 0x80)?(((value) << 1) ^ 0x1B):((value) << 1));
//#define xTimes(value) (((value) << 1) ^ (0x1B & ((signed char)((value) & 0x80)) >> 7)   )

#define K(i,n) k.key[i][n]
#ifdef AES_256
#define createNextKey(i)  K(i,0) = K(i-1,0) ^ sBox[K(i-1,29)] ^ constantsOffSet;\
    K(i,1) = K(i-1,1) ^  sBox[K(i-1,30)];\
    K(i,2) = K(i-1,2) ^  sBox[K(i-1,31)];\
    K(i,3) = K(i-1,3) ^  sBox[K(i-1,28)];\
    K(i,4) = K(i-1,4) ^  K(i,0);\
    K(i,5) = K(i-1,5) ^  K(i,1);\
    K(i,6) = K(i-1,6) ^  K(i,2);\
    K(i,7) = K(i-1,7) ^  K(i,3);\
    K(i,8) = K(i-1,8) ^  K(i,4);\
    K(i,9) = K(i-1,9) ^  K(i,5);\
    K(i,10) = K(i-1,10) ^  K(i,6);\
    K(i,11) = K(i-1,11) ^  K(i,7);\
    K(i,12) = K(i-1,12) ^  K(i,8);\
    K(i,13) = K(i-1,13) ^  K(i,9);\
    K(i,14) = K(i-1,14) ^  K(i,10);\
    K(i,15) = K(i-1,15) ^  K(i,11);\
    K(i,16) = K(i-1,16) ^  sBox[K(i,12)];\
    K(i,17) = K(i-1,17) ^  sBox[K(i,13)];\
    K(i,18) = K(i-1,18) ^  sBox[K(i,14)];\
    K(i,19) = K(i-1,19) ^  sBox[K(i,15)];\
    K(i,20) = K(i-1,20) ^  K(i,16);\
    K(i,21) = K(i-1,21) ^  K(i,17);\
    K(i,22) = K(i-1,22) ^  K(i,18);\
    K(i,23) = K(i-1,23) ^  K(i,19);\
    K(i,24) = K(i-1,24) ^  K(i,20);\
    K(i,25) = K(i-1,25) ^  K(i,21);\
    K(i,26) = K(i-1,26) ^  K(i,22);\
    K(i,27) = K(i-1,27) ^  K(i,23);\
    K(i,28) = K(i-1,28) ^  K(i,24);\
    K(i,29) = K(i-1,29) ^  K(i,25);\
    K(i,30) = K(i-1,30) ^  K(i,26);\
    K(i,31) = K(i-1,31) ^  K(i,27);\
    constantsOffSet = xTimes(constantsOffSet)
#endif

#ifdef AES_192
#define createNextKey(i)  K(i,0) = K(i-1,0) ^ sBox[K(i-1,21)] ^ constantsOffSet;\
    K(i,1) = K(i-1,1) ^  sBox[K(i-1,22)];\
    K(i,2) = K(i-1,2) ^  sBox[K(i-1,23)];\
    K(i,3) = K(i-1,3) ^  sBox[K(i-1,20)];\
    K(i,4) = K(i-1,4) ^  K(i,0);\
    K(i,5) = K(i-1,5) ^  K(i,1);\
    K(i,6) = K(i-1,6) ^  K(i,2);\
    K(i,7) = K(i-1,7) ^  K(i,3);\
    K(i,8) = K(i-1,8) ^  K(i,4);\
    K(i,9) = K(i-1,9) ^  K(i,5);\
    K(i,10) = K(i-1,10) ^  K(i,6);\
    K(i,11) = K(i-1,11) ^  K(i,7);\
    K(i,12) = K(i-1,12) ^  K(i,8);\
    K(i,13) = K(i-1,13) ^  K(i,9);\
    K(i,14) = K(i-1,14) ^  K(i,10);\
    K(i,15) = K(i-1,15) ^  K(i,11);\
    K(i,16) = K(i-1,16) ^  K(i,12);\
    K(i,17) = K(i-1,17) ^  K(i,13);\
    K(i,18) = K(i-1,18) ^  K(i,14);\
    K(i,19) = K(i-1,19) ^  K(i,15);\
    K(i,20) = K(i-1,20) ^  K(i,16);\
    K(i,21) = K(i-1,21) ^  K(i,17);\
    K(i,22) = K(i-1,22) ^  K(i,18);\
    K(i,23) = K(i-1,23) ^  K(i,19);\
    constantsOffSet = xTimes(constantsOffSet)
#endif
#ifdef AES_128
#define createNextKey(i)  K(i,0) = K(i-1,0) ^ sBox[K(i-1,13)] ^ constantsOffSet;\
    K(i,1) = K(i-1,1) ^  sBox[K(i-1,14)];\
    K(i,2) = K(i-1,2) ^  sBox[K(i-1,15)];\
    K(i,3) = K(i-1,3) ^  sBox[K(i-1,12)];\
    K(i,4) = K(i-1,4) ^  K(i,0);\
    K(i,5) = K(i-1,5) ^  K(i,1);\
    K(i,6) = K(i-1,6) ^  K(i,2);\
    K(i,7) = K(i-1,7) ^  K(i,3);\
    K(i,8) = K(i-1,8) ^  K(i,4);\
    K(i,9) = K(i-1,9) ^  K(i,5);\
    K(i,10) = K(i-1,10) ^  K(i,6);\
    K(i,11) = K(i-1,11) ^  K(i,7);\
    K(i,12) = K(i-1,12) ^  K(i,8);\
    K(i,13) = K(i-1,13) ^  K(i,9);\
    K(i,14) = K(i-1,14) ^  K(i,10);\
    K(i,15) = K(i-1,15) ^  K(i,11);\
    constantsOffSet = xTimes(constantsOffSet)
#endif

#define do9Times(x) x(0);\
    x(1);\
    x(2);\
    x(3);\
    x(4);\
    x(5);\
    x(6);\
    x(7);\
    x(8)

#define do16Times(x,...) x(0, ##__VA_ARGS__); \
    x(1, ##__VA_ARGS__);\
    x(2, ##__VA_ARGS__);\
    x(3, ##__VA_ARGS__);\
    x(4, ##__VA_ARGS__);\
    x(5, ##__VA_ARGS__);\
    x(6, ##__VA_ARGS__);\
    x(7, ##__VA_ARGS__);\
    x(8, ##__VA_ARGS__);\
    x(9, ##__VA_ARGS__);\
    x(10, ##__VA_ARGS__);\
    x(11, ##__VA_ARGS__);\
    x(12, ##__VA_ARGS__);\
    x(13, ##__VA_ARGS__);\
    x(14, ##__VA_ARGS__);\
    x(15, ##__VA_ARGS__)
//------- " ROUND FUNCTION "  ---------
union roundkeys{
    u8 key[1+KEY_ROUNDS][KEY_SIZE];
    u8 block[1+N_ROUNDS][BLOCK_SIZE];
    };

/**
 * Encrypts a single data block.
 * @param key The encryption key
 * @param src Pointer to the source block
 * @param dst Pointer to the destination block
 */
void cipherCryptB(u8 __attribute__((key)) *_key, u8 __attribute__((plain)) *_src, u8* _dst){

    u8* key  = _key;
    u8* src  = _src;
    const u8 __attribute__((sbox)) *sBox = _sBox;
#define initializekey(i) k.key[0][i] = key[i];
#define round0pass(i) dst[i]=src[i]^k.block[0][i];

#define ByteSub_lookup(i) dst[i]=sBox[dst[i]];

#define ARK_iter(i,r) dst[i]^=k.block[r][i];

#define shiftrows()         aux1 = dst[1]; dst[1] = dst[5]; dst[5] = dst[9];\
    dst[9] = dst[13]; dst[13] = aux1;\
    aux1 = dst[2]; dst[2] = dst[10]; dst[10] = aux1;\
    aux1 = dst[6]; dst[6] = dst[14]; dst[14] = aux1;\
    aux1 = dst[3]; dst[3] = dst[15]; dst[15] = dst[11];\
    dst[11] = dst[7]; dst[7] = aux1;

#define MixIter() aux2 = dst[i];\
            aux1 = aux2 ^ dst[i+1] ^ dst[i+2] ^ dst[i+3];\
            dst[i] ^= aux1 ^ xTimes(dst[i] ^ dst[i+1]); i++;\
            dst[i] ^= aux1 ^ xTimes(dst[i] ^ dst[i+1]); i++;\
            dst[i] ^= aux1 ^ xTimes(dst[i] ^ dst[i+1]); i++;\
            dst[i] ^= aux1 ^ xTimes(dst[i] ^ aux2);i++

    //holds the round keys
    union roundkeys k;
        u8 dst[BLOCK_SIZE];

    u8 aux1,aux2;
    int i;
    //Offset used by the key schedule
    u8 constantsOffSet=1;
    //Loads the key into the 'k' buffer (so it is not replaced
    //by the key evolution process) and applies the first key

    do16Times(initializekey);
#if N_ROUNDS > 10
    initializekey(16);
    initializekey(17);
    initializekey(18);
    initializekey(19);
    initializekey(20);
    initializekey(21);
    initializekey(22);
    initializekey(23);
#endif
#if N_ROUNDS > 12
    initializekey(24);
    initializekey(25);
    initializekey(26);
    initializekey(27);
    initializekey(28);
    initializekey(29);
    initializekey(30);
    initializekey(31);
#endif
   createNextKey(1);
   createNextKey(2);
   createNextKey(3);
   createNextKey(4);
   createNextKey(5);
   createNextKey(6);
   createNextKey(7);
   createNextKey(8);
#if KEY_ROUNDS > 8
   createNextKey(9);
#if KEY_ROUNDS > 9
   createNextKey(10);
#endif
#endif
    //Initializes offset

    do16Times(round0pass);
#define round(n) do16Times(ByteSub_lookup);\
        shiftrows();\
            i=0;\
            MixIter();\
            MixIter();\
            MixIter();\
            MixIter();\
        do16Times(ARK_iter,n);


//    do9Times(round);
      round(1);
      round(2);
      round(3);
      round(4);
      round(5);
      round(6);
      round(7);
      round(8);
      round(9);
#if N_ROUNDS > 10
      round(10);
      round(11);
#endif
#if N_ROUNDS > 12
      round(12);
      round(13);
#endif
    /*last round*/
            do16Times(ByteSub_lookup);
            shiftrows();
            do16Times(ARK_iter,N_ROUNDS);
#define copy(x) _dst[x]=dst[x];
	    do16Times(copy)

    return;
}



