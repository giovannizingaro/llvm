#include <stdint.h>
/* S0:   3  8 15  1 10  6  5 11 14 13  4  2  7  0  9 12 */

/* depth = 5,7,4,2, Total gates=18 */
#define RND00(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t05, t06, t07, t08, t09, t11, t12, t13, t14, t15, t17, t01;\
	t01 = b   ^ c  ; \
	t02 = a   | d  ; \
	t03 = a   ^ b  ; \
	z   = t02 ^ t01; \
	t05 = c   | z  ; \
	t06 = a   ^ d  ; \
	t07 = b   | c  ; \
	t08 = d   & t05; \
	t09 = t03 & t07; \
	y   = t09 ^ t08; \
	t11 = t09 & y  ; \
	t12 = c   ^ d  ; \
	t13 = t07 ^ t11; \
	t14 = b   & t06; \
	t15 = t06 ^ t13; \
	w   =     ~ t15; \
	t17 = w   ^ t14; \
	x   = t12 ^ t17; }

/* InvS0:  13  3 11  0 10  6  5 12  1 14  4  7 15  9  8  2 */

/* depth = 8,4,3,6, Total gates=19 */
#define InvRND00(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t06, t08, t09, t10, t12, t13, t14, t15, t17, t18, t01;\
	t01 = c   ^ d  ; \
	t02 = a   | b  ; \
	t03 = b   | c  ; \
	t04 = c   & t01; \
	t05 = t02 ^ t01; \
	t06 = a   | t04; \
	y   =     ~ t05; \
	t08 = b   ^ d  ; \
	t09 = t03 & t08; \
	t10 = d   | y  ; \
	x   = t09 ^ t06; \
	t12 = a   | t05; \
	t13 = x   ^ t12; \
	t14 = t03 ^ t10; \
	t15 = a   ^ c  ; \
	z   = t14 ^ t13; \
	t17 = t05 & t13; \
	t18 = t14 | t17; \
	w   = t15 ^ t18; }

/* S1:  15 12  2  7  9  0  5 10  1 11 14  8  6 13  3  4 */

/* depth = 10,7,3,5, Total gates=18 */
#define RND01(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t06, t07, t08, t10, t11, t12, t13, t16, t17, t01;\
	t01 = a   | d  ; \
	t02 = c   ^ d  ; \
	t03 =     ~ b  ; \
	t04 = a   ^ c  ; \
	t05 = a   | t03; \
	t06 = d   & t04; \
	t07 = t01 & t02; \
	t08 = b   | t06; \
	y   = t02 ^ t05; \
	t10 = t07 ^ t08; \
	t11 = t01 ^ t10; \
	t12 = y   ^ t11; \
	t13 = b   & d  ; \
	z   =     ~ t10; \
	x   = t13 ^ t12; \
	t16 = t10 | x  ; \
	t17 = t05 & t16; \
	w   = c   ^ t17; }

/* InvS1:   5  8  2 14 15  6 12  3 11  4  7  9  1 13 10  0 */

/* depth = 7,4,5,3, Total gates=18 */
#define InvRND01(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t06, t07, t08, t09, t10, t11, t14, t15, t17, t01;\
	t01 = a   ^ b  ; \
	t02 = b   | d  ; \
	t03 = a   & c  ; \
	t04 = c   ^ t02; \
	t05 = a   | t04; \
	t06 = t01 & t05; \
	t07 = d   | t03; \
	t08 = b   ^ t06; \
	t09 = t07 ^ t06; \
	t10 = t04 | t03; \
	t11 = d   & t08; \
	y   =     ~ t09; \
	x   = t10 ^ t11; \
	t14 = a   | y  ; \
	t15 = t06 ^ x  ; \
	z   = t01 ^ t04; \
	t17 = c   ^ t15; \
	w   = t14 ^ t17; }

/* S2:   8  6  7  9  3 12 10 15 13  1 14  4  0 11  5  2 */

/* depth = 3,8,11,7, Total gates=16 */
#define RND02(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t05, t06, t07, t08, t09, t10, t12, t13, t14, t01;\
	t01 = a   | c  ; \
	t02 = a   ^ b  ; \
	t03 = d   ^ t01; \
	w   = t02 ^ t03; \
	t05 = c   ^ w  ; \
	t06 = b   ^ t05; \
	t07 = b   | t05; \
	t08 = t01 & t06; \
	t09 = t03 ^ t07; \
	t10 = t02 | t09; \
	x   = t10 ^ t08; \
	t12 = a   | d  ; \
	t13 = t09 ^ x  ; \
	t14 = b   ^ t13; \
	z   =     ~ t09; \
	y   = t12 ^ t14; }

/* InvS2:  12  9 15  4 11 14  1  2  0  3  6 13  5  8 10  7 */

/* depth = 3,6,8,3, Total gates=18 */
#define InvRND02(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t06, t07, t08, t09, t10, t11, t12, t15, t16, t17, t01;\
	t01 = a   ^ d  ; \
	t02 = c   ^ d  ; \
	t03 = a   & c  ; \
	t04 = b   | t02; \
	w   = t01 ^ t04; \
	t06 = a   | c  ; \
	t07 = d   | w  ; \
	t08 =     ~ d  ; \
	t09 = b   & t06; \
	t10 = t08 | t03; \
	t11 = b   & t07; \
	t12 = t06 & t02; \
	z   = t09 ^ t10; \
	x   = t12 ^ t11; \
	t15 = c   & z  ; \
	t16 = w   ^ x  ; \
	t17 = t10 ^ t15; \
	y   = t16 ^ t17; }

/* S3:   0 15 11  8 12  9  6  3 13  1  2  4 10  7  5 14 */

/* depth = 8,3,5,5, Total gates=18 */
#define RND03(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t06, t07, t08, t09, t10, t11, t13, t14, t15, t01;\
	t01 = a   ^ c  ; \
	t02 = a   | d  ; \
	t03 = a   & d  ; \
	t04 = t01 & t02; \
	t05 = b   | t03; \
	t06 = a   & b  ; \
	t07 = d   ^ t04; \
	t08 = c   | t06; \
	t09 = b   ^ t07; \
	t10 = d   & t05; \
	t11 = t02 ^ t10; \
	z   = t08 ^ t09; \
	t13 = d   | z  ; \
	t14 = a   | t07; \
	t15 = b   & t13; \
	y   = t08 ^ t11; \
	w   = t14 ^ t15; \
	x   = t05 ^ t04; }

/* InvS3:   0  9 10  7 11 14  6 13  3  5 12  2  4  8 15  1 */

/* depth = 3,6,4,4, Total gates=17 */
#define InvRND03(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t06, t07, t09, t11, t12, t13, t14, t16, t01;\
	t01 = c   | d  ; \
	t02 = a   | d  ; \
	t03 = c   ^ t02; \
	t04 = b   ^ t02; \
	t05 = a   ^ d  ; \
	t06 = t04 & t03; \
	t07 = b   & t01; \
	y   = t05 ^ t06; \
	t09 = a   ^ t03; \
	w   = t07 ^ t03; \
	t11 = w   | t05; \
	t12 = t09 & t11; \
	t13 = a   & y  ; \
	t14 = t01 ^ t05; \
	x   = b   ^ t12; \
	t16 = b   | t13; \
	z   = t14 ^ t16; }

/* S4:   1 15  8  3 12  0 11  6  2  5  4 10  9 14  7 13 */

/* depth = 6,7,5,3, Total gates=19 */
#define RND04(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t06, t08, t09, t10, t11, t12, t13, t14, t15, t16, t01;\
	t01 = a   | b  ; \
	t02 = b   | c  ; \
	t03 = a   ^ t02; \
	t04 = b   ^ d  ; \
	t05 = d   | t03; \
	t06 = d   & t01; \
	z   = t03 ^ t06; \
	t08 = z   & t04; \
	t09 = t04 & t05; \
	t10 = c   ^ t06; \
	t11 = b   & c  ; \
	t12 = t04 ^ t08; \
	t13 = t11 | t03; \
	t14 = t10 ^ t09; \
	t15 = a   & t05; \
	t16 = t11 | t12; \
	y   = t13 ^ t08; \
	x   = t15 ^ t16; \
	w   =     ~ t14; }

/* InvS4:   5  0  8  3 10  9  7 14  2 12 11  6  4 15 13  1 */

/* depth = 6,4,7,3, Total gates=17 */
#define InvRND04(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t06, t07, t09, t10, t11, t12, t13, t15, t01;\
	t01 = b   | d  ; \
	t02 = c   | d  ; \
	t03 = a   & t01; \
	t04 = b   ^ t02; \
	t05 = c   ^ d  ; \
	t06 =     ~ t03; \
	t07 = a   & t04; \
	x   = t05 ^ t07; \
	t09 = x   | t06; \
	t10 = a   ^ t07; \
	t11 = t01 ^ t09; \
	t12 = d   ^ t04; \
	t13 = c   | t10; \
	z   = t03 ^ t12; \
	t15 = a   ^ t04; \
	y   = t11 ^ t13; \
	w   = t15 ^ t09; }

/* S5:  15  5  2 11  4 10  9 12  0  3 14  8 13  6  7  1 */

/* depth = 4,6,8,6, Total gates=17 */
#define RND05(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t07, t08, t09, t10, t11, t12, t13, t14, t01;\
	t01 = b   ^ d  ; \
	t02 = b   | d  ; \
	t03 = a   & t01; \
	t04 = c   ^ t02; \
	t05 = t03 ^ t04; \
	w   =     ~ t05; \
	t07 = a   ^ t01; \
	t08 = d   | w  ; \
	t09 = b   | t05; \
	t10 = d   ^ t08; \
	t11 = b   | t07; \
	t12 = t03 | w  ; \
	t13 = t07 | t10; \
	t14 = t01 ^ t11; \
	y   = t09 ^ t13; \
	x   = t07 ^ t08; \
	z   = t12 ^ t14; }

/* InvS5:   8 15  2  9  4  1 13 14 11  6  5  3  7 12 10  0 */

/* depth = 4,6,9,7, Total gates=17 */
#define InvRND05(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t07, t08, t09, t10, t12, t13, t15, t16, t01;\
	t01 = a   & d  ; \
	t02 = c   ^ t01; \
	t03 = a   ^ d  ; \
	t04 = b   & t02; \
	t05 = a   & c  ; \
	w   = t03 ^ t04; \
	t07 = a   & w  ; \
	t08 = t01 ^ w  ; \
	t09 = b   | t05; \
	t10 =     ~ b  ; \
	x   = t08 ^ t09; \
	t12 = t10 | t07; \
	t13 = w   | x  ; \
	z   = t02 ^ t12; \
	t15 = t02 ^ t13; \
	t16 = b   ^ d  ; \
	y   = t16 ^ t15; }

/* S6:   7  2 12  5  8  4  6 11 14  9  1 15 13  3 10  0 */

/* depth = 8,3,6,3, Total gates=19 */
#define RND06(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t07, t08, t09, t10, t11, t12, t13, t15, t17, t18, t01;\
	t01 = a   & d  ; \
	t02 = b   ^ c  ; \
	t03 = a   ^ d  ; \
	t04 = t01 ^ t02; \
	t05 = b   | c  ; \
	x   =     ~ t04; \
	t07 = t03 & t05; \
	t08 = b   & x  ; \
	t09 = a   | c  ; \
	t10 = t07 ^ t08; \
	t11 = b   | d  ; \
	t12 = c   ^ t11; \
	t13 = t09 ^ t10; \
	y   =     ~ t13; \
	t15 = x   & t03; \
	z   = t12 ^ t07; \
	t17 = a   ^ b  ; \
	t18 = y   ^ t15; \
	w   = t17 ^ t18; }

/* InvS6:  15 10  1 13  5  3  6  0  4  9 14  7  2 12  8 11 */

/* depth = 5,3,8,6, Total gates=19 */
#define InvRND06(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t06, t07, t08, t09, t12, t13, t14, t15, t16, t17, t01;\
	t01 = a   ^ c  ; \
	t02 =     ~ c  ; \
	t03 = b   & t01; \
	t04 = b   | t02; \
	t05 = d   | t03; \
	t06 = b   ^ d  ; \
	t07 = a   & t04; \
	t08 = a   | t02; \
	t09 = t07 ^ t05; \
	x   = t06 ^ t08; \
	w   =     ~ t09; \
	t12 = b   & w  ; \
	t13 = t01 & t05; \
	t14 = t01 ^ t12; \
	t15 = t07 ^ t13; \
	t16 = d   | t02; \
	t17 = a   ^ x  ; \
	z   = t17 ^ t15; \
	y   = t16 ^ t14; }

/* S7:   1 13 15  0 14  8  2 11  7  4 12 10  9  3  5  6 */

/* depth = 10,7,10,4, Total gates=19 */
#define RND07(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t05, t06, t08, t09, t10, t11, t13, t14, t15, t16, t17, t01;\
	t01 = a   & c  ; \
	t02 =     ~ d  ; \
	t03 = a   & t02; \
	t04 = b   | t01; \
	t05 = a   & b  ; \
	t06 = c   ^ t04; \
	z   = t03 ^ t06; \
	t08 = c   | z  ; \
	t09 = d   | t05; \
	t10 = a   ^ t08; \
	t11 = t04 & z  ; \
	x   = t09 ^ t10; \
	t13 = b   ^ x  ; \
	t14 = t01 ^ x  ; \
	t15 = c   ^ t05; \
	t16 = t11 | t13; \
	t17 = t02 | t14; \
	w   = t15 ^ t17; \
	y   = a   ^ t16; }

/* InvS7:   3  0  6 13  9 14 15  8  5 12 11  7 10  1  4  2 */

/* depth = 9,7,3,3, Total gates=18 */
#define InvRND07(a,b,c,d,w,x,y,z) \
	{ register unsigned long t02, t03, t04, t06, t07, t08, t09, t10, t11, t13, t14, t15, t16, t01;\
	t01 = a   & b  ; \
	t02 = a   | b  ; \
	t03 = c   | t01; \
	t04 = d   & t02; \
	z   = t03 ^ t04; \
	t06 = b   ^ t04; \
	t07 = d   ^ z  ; \
	t08 =     ~ t07; \
	t09 = t06 | t08; \
	t10 = b   ^ d  ; \
	t11 = a   | d  ; \
	x   = a   ^ t09; \
	t13 = c   ^ t06; \
	t14 = c   & t11; \
	t15 = d   | x  ; \
	t16 = t01 | t10; \
	w   = t13 ^ t15; \
	y   = t14 ^ t16; }

#define RND08(a,b,c,d,e,f,g,h) RND00(a,b,c,d,e,f,g,h)
#define RND09(a,b,c,d,e,f,g,h) RND01(a,b,c,d,e,f,g,h)
#define RND10(a,b,c,d,e,f,g,h) RND02(a,b,c,d,e,f,g,h)
#define RND11(a,b,c,d,e,f,g,h) RND03(a,b,c,d,e,f,g,h)
#define RND12(a,b,c,d,e,f,g,h) RND04(a,b,c,d,e,f,g,h)
#define RND13(a,b,c,d,e,f,g,h) RND05(a,b,c,d,e,f,g,h)
#define RND14(a,b,c,d,e,f,g,h) RND06(a,b,c,d,e,f,g,h)
#define RND15(a,b,c,d,e,f,g,h) RND07(a,b,c,d,e,f,g,h)
#define RND16(a,b,c,d,e,f,g,h) RND00(a,b,c,d,e,f,g,h)
#define RND17(a,b,c,d,e,f,g,h) RND01(a,b,c,d,e,f,g,h)
#define RND18(a,b,c,d,e,f,g,h) RND02(a,b,c,d,e,f,g,h)
#define RND19(a,b,c,d,e,f,g,h) RND03(a,b,c,d,e,f,g,h)
#define RND20(a,b,c,d,e,f,g,h) RND04(a,b,c,d,e,f,g,h)
#define RND21(a,b,c,d,e,f,g,h) RND05(a,b,c,d,e,f,g,h)
#define RND22(a,b,c,d,e,f,g,h) RND06(a,b,c,d,e,f,g,h)
#define RND23(a,b,c,d,e,f,g,h) RND07(a,b,c,d,e,f,g,h)
#define RND24(a,b,c,d,e,f,g,h) RND00(a,b,c,d,e,f,g,h)
#define RND25(a,b,c,d,e,f,g,h) RND01(a,b,c,d,e,f,g,h)
#define RND26(a,b,c,d,e,f,g,h) RND02(a,b,c,d,e,f,g,h)
#define RND27(a,b,c,d,e,f,g,h) RND03(a,b,c,d,e,f,g,h)
#define RND28(a,b,c,d,e,f,g,h) RND04(a,b,c,d,e,f,g,h)
#define RND29(a,b,c,d,e,f,g,h) RND05(a,b,c,d,e,f,g,h)
#define RND30(a,b,c,d,e,f,g,h) RND06(a,b,c,d,e,f,g,h)
#define RND31(a,b,c,d,e,f,g,h) RND07(a,b,c,d,e,f,g,h)

#define InvRND08(a,b,c,d,e,f,g,h) InvRND00(a,b,c,d,e,f,g,h)
#define InvRND09(a,b,c,d,e,f,g,h) InvRND01(a,b,c,d,e,f,g,h)
#define InvRND10(a,b,c,d,e,f,g,h) InvRND02(a,b,c,d,e,f,g,h)
#define InvRND11(a,b,c,d,e,f,g,h) InvRND03(a,b,c,d,e,f,g,h)
#define InvRND12(a,b,c,d,e,f,g,h) InvRND04(a,b,c,d,e,f,g,h)
#define InvRND13(a,b,c,d,e,f,g,h) InvRND05(a,b,c,d,e,f,g,h)
#define InvRND14(a,b,c,d,e,f,g,h) InvRND06(a,b,c,d,e,f,g,h)
#define InvRND15(a,b,c,d,e,f,g,h) InvRND07(a,b,c,d,e,f,g,h)
#define InvRND16(a,b,c,d,e,f,g,h) InvRND00(a,b,c,d,e,f,g,h)
#define InvRND17(a,b,c,d,e,f,g,h) InvRND01(a,b,c,d,e,f,g,h)
#define InvRND18(a,b,c,d,e,f,g,h) InvRND02(a,b,c,d,e,f,g,h)
#define InvRND19(a,b,c,d,e,f,g,h) InvRND03(a,b,c,d,e,f,g,h)
#define InvRND20(a,b,c,d,e,f,g,h) InvRND04(a,b,c,d,e,f,g,h)
#define InvRND21(a,b,c,d,e,f,g,h) InvRND05(a,b,c,d,e,f,g,h)
#define InvRND22(a,b,c,d,e,f,g,h) InvRND06(a,b,c,d,e,f,g,h)
#define InvRND23(a,b,c,d,e,f,g,h) InvRND07(a,b,c,d,e,f,g,h)
#define InvRND24(a,b,c,d,e,f,g,h) InvRND00(a,b,c,d,e,f,g,h)
#define InvRND25(a,b,c,d,e,f,g,h) InvRND01(a,b,c,d,e,f,g,h)
#define InvRND26(a,b,c,d,e,f,g,h) InvRND02(a,b,c,d,e,f,g,h)
#define InvRND27(a,b,c,d,e,f,g,h) InvRND03(a,b,c,d,e,f,g,h)
#define InvRND28(a,b,c,d,e,f,g,h) InvRND04(a,b,c,d,e,f,g,h)
#define InvRND29(a,b,c,d,e,f,g,h) InvRND05(a,b,c,d,e,f,g,h)
#define InvRND30(a,b,c,d,e,f,g,h) InvRND06(a,b,c,d,e,f,g,h)
#define InvRND31(a,b,c,d,e,f,g,h) InvRND07(a,b,c,d,e,f,g,h)

/* Linear transformations and key mixing: */

#define ROL(x,n) ((((unsigned long)(x))<<(n))| \
                  (((unsigned long)(x))>>(32-(n))))
#define ROR(x,n) ((((unsigned long)(x))<<(32-(n)))| \
                  (((unsigned long)(x))>>(n)))

#define transform(x0, x1, x2, x3, y0, y1, y2, y3) \
      y0 = ROL(x0, 13); \
      y2 = ROL(x2, 3); \
      y1 = x1 ^ y0 ^ y2; \
      y3 = x3 ^ y2 ^ ((unsigned long)y0)<<3; \
      y1 = ROL(y1, 1); \
      y3 = ROL(y3, 7); \
      y0 = y0 ^ y1 ^ y3; \
      y2 = y2 ^ y3 ^ ((unsigned long)y1<<7); \
      y0 = ROL(y0, 5); \
      y2 = ROL(y2, 22)

#define inv_transform(x0, x1, x2, x3, y0, y1, y2, y3) \
      y2 = ROR(x2, 22);\
      y0 = ROR(x0, 5); \
      y2 = y2 ^ x3 ^ ((unsigned long)x1<<7); \
      y0 = y0 ^ x1 ^ x3; \
      y3 = ROR(x3, 7); \
      y1 = ROR(x1, 1); \
      y3 = y3 ^ y2 ^ ((unsigned long)y0)<<3; \
      y1 = y1 ^ y0 ^ y2; \
      y2 = ROR(y2, 3); \
      y0 = ROR(y0, 13)

#define keying(x0, x1, x2, x3, subkey) \
                         x0^=subkey[0];x1^=subkey[1]; \
                         x2^=subkey[2];x3^=subkey[3]

/* PHI: Constant used in the key schedule */
#define PHI 0x9e3779b9L

/*  Includes:
	Standard include files
*/

#include <stdio.h>

/*  Defines:
	Add any additional defines you need
*/

#define     DIR_ENCRYPT     0    /*  Are we encrpyting?  */
#define     DIR_DECRYPT     1    /*  Are we decrpyting?  */
#define     MODE_ECB        1    /*  Are we ciphering in ECB mode?   */
#define     MODE_CBC        2    /*  Are we ciphering in CBC mode?   */
#define     MODE_CFB1       3    /*  Are we ciphering in 1-bit CFB mode? */
#define     TRUE            1
#define     FALSE           0

/*  Error Codes - CHANGE POSSIBLE: inclusion of additional error codes  */
#define     BAD_KEY_DIR        -1  /*  Key direction is invalid, e;g;,
					unknown value */
#define     BAD_KEY_MAT        -2  /*  Key material not of correct 
					length */
#define     BAD_KEY_INSTANCE   -3  /*  Key passed is not valid  */
#define     BAD_CIPHER_MODE    -4  /*  Params struct passed to 
					cipherInit invalid */
#define     BAD_CIPHER_STATE   -5  /*  Cipher in wrong state (e.g., not 
					initialized) */

/*  CHANGE POSSIBLE:  inclusion of algorithm specific defines  */
#define     MAX_KEY_SIZE	64  /* # of ASCII char's needed to
					represent a key */
#define     MAX_IV_SIZE		32  /* # of ASCII char's needed to
					represent an IV  */

/*  Typedefs:

	Typedef'ed data storage elements.  Add any algorithm specific 
parameters at the bottom of the structs as appropriate.
*/

typedef    unsigned char    BYTE;

#define keyLen 128
serpent_encrypt(uint32_t __attribute__((key))  _key[8],uint32_t __attribute__((plain))  _plaintext[4], 
		uint32_t ciphertext[4])
{
    uint32_t  *key = _key;
    uint32_t  *plaintext = _plaintext;

  register uint32_t x0, x1, x2, x3;
  register uint32_t y0, y1, y2, y3;
  uint32_t i,j;
  uint32_t w[132],k[132];
  
  uint32_t subkeys[33][4];
  int rc;
  i=0;
#define INIT  w[i]=key[i]; i++
    INIT;INIT;INIT;INIT;   //n*volte=keylen /32 
    
    w[i]=(key[i]&((1L<<((keyLen&31)))-1))|(1L<<((keyLen&31)));
  i++;
#define SETZERO  w[i]=0;i++
    SETZERO; SETZERO;SETZERO; SETZERO;
i=8;
#define ITER_1    w[i]=ROL(w[i-8]^w[i-5]^w[i-3]^w[i-1]^PHI^(i-8),11);i++
    ITER_1;ITER_1;ITER_1;ITER_1;ITER_1;ITER_1;ITER_1;ITER_1; //arrivare ad i=16;
#define ITER_2    w[i]=w[i+8];i++    
    i=0;
    ITER_2;ITER_2;ITER_2;ITER_2;ITER_2;ITER_2;ITER_2;ITER_2;

  i=8;
#define ITER_3    w[i]=ROL(w[i-8]^w[i-5]^w[i-3]^w[i-1]^PHI^i,11); i++
    //eseguire 132-8=124 volte
#define ITER_3_10times ITER_3;ITER_3;ITER_3;ITER_3;ITER_3;ITER_3;ITER_3;ITER_3;ITER_3;ITER_3
#define ITER_3_30times ITER_3_10times;ITER_3_10times;ITER_3_10times
 ITER_3_30times;
 ITER_3_30times;
 ITER_3_30times;
 ITER_3_30times;
 ITER_3;ITER_3;ITER_3;ITER_3;

//  for(i=0; i<keyLen/32; i++)
//    w[i]=key[i];
//  if(keyLen<256)
//    w[i]=(key[i]&((1L<<((keyLen&31)))-1))|(1L<<((keyLen&31)));
//  for(i++; i<8; i++)
//    w[i]=0;
//  for(i=8; i<16; i++)
//    w[i]=ROL(w[i-8]^w[i-5]^w[i-3]^w[i-1]^PHI^(i-8),11);
//  for(i=0; i<8; i++)
//    w[i]=w[i+8];
//  for(i=8; i<132; i++)
//    w[i]=ROL(w[i-8]^w[i-5]^w[i-3]^w[i-1]^PHI^i,11);

    
  RND03(w[  0], w[  1], w[  2], w[  3], k[  0], k[  1], k[  2], k[  3]);
  RND02(w[  4], w[  5], w[  6], w[  7], k[  4], k[  5], k[  6], k[  7]);
  RND01(w[  8], w[  9], w[ 10], w[ 11], k[  8], k[  9], k[ 10], k[ 11]);
  RND00(w[ 12], w[ 13], w[ 14], w[ 15], k[ 12], k[ 13], k[ 14], k[ 15]);
  RND31(w[ 16], w[ 17], w[ 18], w[ 19], k[ 16], k[ 17], k[ 18], k[ 19]);
  RND30(w[ 20], w[ 21], w[ 22], w[ 23], k[ 20], k[ 21], k[ 22], k[ 23]);
  RND29(w[ 24], w[ 25], w[ 26], w[ 27], k[ 24], k[ 25], k[ 26], k[ 27]);
  RND28(w[ 28], w[ 29], w[ 30], w[ 31], k[ 28], k[ 29], k[ 30], k[ 31]);
  RND27(w[ 32], w[ 33], w[ 34], w[ 35], k[ 32], k[ 33], k[ 34], k[ 35]);
  RND26(w[ 36], w[ 37], w[ 38], w[ 39], k[ 36], k[ 37], k[ 38], k[ 39]);
  RND25(w[ 40], w[ 41], w[ 42], w[ 43], k[ 40], k[ 41], k[ 42], k[ 43]);
  RND24(w[ 44], w[ 45], w[ 46], w[ 47], k[ 44], k[ 45], k[ 46], k[ 47]);
  RND23(w[ 48], w[ 49], w[ 50], w[ 51], k[ 48], k[ 49], k[ 50], k[ 51]);
  RND22(w[ 52], w[ 53], w[ 54], w[ 55], k[ 52], k[ 53], k[ 54], k[ 55]);
  RND21(w[ 56], w[ 57], w[ 58], w[ 59], k[ 56], k[ 57], k[ 58], k[ 59]);
  RND20(w[ 60], w[ 61], w[ 62], w[ 63], k[ 60], k[ 61], k[ 62], k[ 63]);
  RND19(w[ 64], w[ 65], w[ 66], w[ 67], k[ 64], k[ 65], k[ 66], k[ 67]);
  RND18(w[ 68], w[ 69], w[ 70], w[ 71], k[ 68], k[ 69], k[ 70], k[ 71]);
  RND17(w[ 72], w[ 73], w[ 74], w[ 75], k[ 72], k[ 73], k[ 74], k[ 75]);
  RND16(w[ 76], w[ 77], w[ 78], w[ 79], k[ 76], k[ 77], k[ 78], k[ 79]);
  RND15(w[ 80], w[ 81], w[ 82], w[ 83], k[ 80], k[ 81], k[ 82], k[ 83]);
  RND14(w[ 84], w[ 85], w[ 86], w[ 87], k[ 84], k[ 85], k[ 86], k[ 87]);
  RND13(w[ 88], w[ 89], w[ 90], w[ 91], k[ 88], k[ 89], k[ 90], k[ 91]);
  RND12(w[ 92], w[ 93], w[ 94], w[ 95], k[ 92], k[ 93], k[ 94], k[ 95]);
  RND11(w[ 96], w[ 97], w[ 98], w[ 99], k[ 96], k[ 97], k[ 98], k[ 99]);
  RND10(w[100], w[101], w[102], w[103], k[100], k[101], k[102], k[103]);
  RND09(w[104], w[105], w[106], w[107], k[104], k[105], k[106], k[107]);
  RND08(w[108], w[109], w[110], w[111], k[108], k[109], k[110], k[111]);
  RND07(w[112], w[113], w[114], w[115], k[112], k[113], k[114], k[115]);
  RND06(w[116], w[117], w[118], w[119], k[116], k[117], k[118], k[119]);
  RND05(w[120], w[121], w[122], w[123], k[120], k[121], k[122], k[123]);
  RND04(w[124], w[125], w[126], w[127], k[124], k[125], k[126], k[127]);
  RND03(w[128], w[129], w[130], w[131], k[128], k[129], k[130], k[131]);

      
#define GENSUBKEY(i) subkeys[i][0] = k[4*i];subkeys[i][1] = k[4*i+1];subkeys[i][2] = k[4*i+2];subkeys[i][3] = k[4*i+3]
GENSUBKEY(0);
GENSUBKEY(1);
GENSUBKEY(2);
GENSUBKEY(3);
GENSUBKEY(4);
GENSUBKEY(5);
GENSUBKEY(6);
GENSUBKEY(7);
GENSUBKEY(8);
GENSUBKEY(9);
GENSUBKEY(10);
GENSUBKEY(11);
GENSUBKEY(12);
GENSUBKEY(13);
GENSUBKEY(14);
GENSUBKEY(15);
GENSUBKEY(16);
GENSUBKEY(17);
GENSUBKEY(18);
GENSUBKEY(19);
GENSUBKEY(20);
GENSUBKEY(21);
GENSUBKEY(22);
GENSUBKEY(23);
GENSUBKEY(24);
GENSUBKEY(25);
GENSUBKEY(26);
GENSUBKEY(27);
GENSUBKEY(28);
GENSUBKEY(29);
GENSUBKEY(30);
GENSUBKEY(31);
GENSUBKEY(32);

  
  x0=plaintext[0];
  x1=plaintext[1];
  x2=plaintext[2];
  x3=plaintext[3];

  /* Start to encrypt the plaintext x */
  keying(x0, x1, x2, x3, subkeys[ 0]);
  RND00(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[ 1]);
  RND01(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[ 2]);
  RND02(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[ 3]);
  RND03(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[ 4]);
  RND04(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[ 5]);
  RND05(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[ 6]);
  RND06(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[ 7]);
  RND07(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[ 8]);
  RND08(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[ 9]);
  RND09(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[10]);
  RND10(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[11]);
  RND11(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[12]);
  RND12(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[13]);
  RND13(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[14]);
  RND14(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[15]);
  RND15(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[16]);
  RND16(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[17]);
  RND17(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[18]);
  RND18(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[19]);
  RND19(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[20]);
  RND20(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[21]);
  RND21(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[22]);
  RND22(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[23]);
  RND23(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[24]);
  RND24(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[25]);
  RND25(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[26]);
  RND26(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[27]);
  RND27(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[28]);
  RND28(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[29]);
  RND29(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[30]);
  RND30(x0, x1, x2, x3, y0, y1, y2, y3);
  transform(y0, y1, y2, y3, x0, x1, x2, x3);
  keying(x0, x1, x2, x3, subkeys[31]);
  RND31(x0, x1, x2, x3, y0, y1, y2, y3);
  x0 = y0; x1 = y1; x2 = y2; x3 = y3;
  keying(x0, x1, x2, x3, subkeys[32]);
  /* The ciphertext is now in x */

  ciphertext[0] = x0;
  ciphertext[1] = x1;
  ciphertext[2] = x2;
  ciphertext[3] = x3;
}
#include <stdio.h>
int main(){
uint32_t SK[32];
unsigned char out[16];
unsigned char correct[16] = {0xEA,0x02,0x47,0x14,0xAD,0x5C,0x4D,0x84,0xEA,0x02,0x47,0x14,0xAD,0x5C,0x4D,0x84};
unsigned char plain[16] = {0xBE,0xB6,0xC0,0x69,0x39,0x38,0x22,0xD3,0xBE,0x73,0xFF,0x30,0x52,0x5E,0xC4,0x3E};
unsigned char key[16] = {0x2B,0xD6,0x45,0x9F,0x82,0xC5,0xB3,0x00,0x95,0x2C,0x49,0x10,0x48,0x81,0xFF,0x48};

serpent_encrypt((uint32_t*)key,(uint32_t*)plain,(uint32_t*)out);
if(memcmp(out,correct,16)==0)  printf("OK\n"); else printf("ERROR\n");
for(int i = 0;i<16;i++){
  printf("%x",out[i]);  
}
}
