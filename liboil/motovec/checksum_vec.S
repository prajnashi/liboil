/*
 * AltiVec versions (*_vec) of equivalent Linux library functions
 * found in /arch/ppc/lib/checksum.S from Linux 2.4.17.  Suggest this
 * file be appended to that one when building a Linux kernel that
 * will employ these functions.
 *
 * Copyright (C) Motorola, Inc. 2003
 *
 * Revision history:
 * 	Rev 0.0	Original                Chuck Corley   5/28/03
 *                                  Contact at risc10@motorola.com
 * Commented source code for Altivec version available at
 * www.motorola.com/altivec
 */

#ifndef TEST_OUTSIDE_LINUX
#include <linux/sys.h>
#include <asm/processor.h>
#include <asm/errno.h>
#include "../kernel/ppc_asm.tmpl"
#if 0
#define v0  vr0
#define v1  vr1
#define v2  vr2
#define v3  vr3
#define v4  vr4
#define v5  vr5
#define v6  vr6
#define v7  vr7
#define v8  vr8
#define v9  vr9
#define v10 vr10
#define v11 vr11
#define v12 vr12
#define v13 vr13
#define v14 vr14
#define v15 vr15
#endif
#else
#define EFAULT 0
#endif

	.text

/* 
 * AltiVec versions of selected functions for use on AltiVec
 * enabled G4 and later microprocessors.
 */ 
#if defined(__GNUC__) || defined(__MWERKS__)  // gcc and codewarrior don't assemble dcba
#define DCBAR4R12	.long 0x7c0465ec
#else
#define DCBAR4R12	dcba	r4,r12
#endif

	.text
	.align	4
#ifndef TEST_OUTSIDE_LINUX
_GLOBAL(csum_partial_copy_generic_vec)
#else
#if __MWERKS__
	.align	16
#else
	.align	4
#endif
	.global	csum_partial_copy_generic_vec     
csum_partial_copy_generic_vec:
#endif
	li	r12,32
	rlwinm	r0,r5,31,1,31
	cmpi	cr7,0,r5,48
	dcbt	r3,r12
	cmpi	cr6,0,r0,0
	addic	r6,r6,0
	addi	r11,r3,-2
	add	r10,r4 ,r5
	bgt	cr7,4f
	andi.	r12,r5,1
	addi	r9,r4,-2
	add	r12,r3,r5
	beq	cr6,2f
	mtctr	r0
1:	lhzu	r0,2(r11)
204:	sthu	r0,2(r9)
	addc	r6,r6,r0
	bdnz	1b
2:	beq	3f
201:	lbz	r0,-1(r12 )
202:	stb	r0,-1(r10)
	rlwinm	r0,r0,8,16,23
	addc	r6,r6,r0
3:	addze	r3,r6
	blr
4:	lvsr	v5,0,r4
	rlwinm	r9,r4,0,28,31
	rlwinm	r12,r3,0,28,31
	lvsr	v7,r4,r5
	subf.	r12,r12,r9
	subf	r12,r3,r4
	lvsr	v6,0,r12
	li	r12,64
	vxor	v0,v0,v0
	dcbt	r3,r12
	cmpi	cr1,0,r9,0
	vnor	v1,v0,v0
	addi	r9,r4,16
	addi	r10,r10,-1
	vperm	v5,v1,v0,v5
	bge	5f
401:	lvx	v2,0,r3
	addi	r3,r3,16
5:	lvx	v3,0,r3
	rlwinm	r9,r9,0,0,27
	vperm	v1,v0,v1,v7
	subf	r11,r9,r10
	vxor	v7,v7,v7
	vxor	v11,v11,v11
	rlwinm	r11,r11,28,4,31
	rlwinm	r0,r10,0,28,31
	li	r12,96
	cmpi	cr5,0,r0,0xF
	subf	r0,r4,r9
	mtctr	r11
	cmpi	cr6,0,r11,4
	mtcrf	0x01,r0
	vperm	v4,v2,v3,v6
	vor	v2,v3,v3
	dcbt	r3,r12
	beq	cr1,9f
	li	r12,0
	vsel	v4,v4,v0,v5
	bns	cr7,6f
502:	stvebx	v4,r4,r12
	addi	r12,r12,1
6:	bne	cr7,7f
602:	stvehx	v4,r4,r12
	addi	r12,r12,2
7:	bng	cr7,8f
702:	stvewx	v4,r4,r12
	addi	r12,r12,4
8:	bnl	cr7,10f
802:	stvewx	v4,r4,r12
	addi	r12,r12,4
804:	stvewx	v4,r4,r12
	b	10f
9:	stvx	v4,0,r4
10:	vxor	v8,v8,v8
	li	r12,16
11:	lvx	v3,r3,r12
	vaddcuw	v9,v4,v8
	vadduwm	v8,v4,v8
	vperm	v4,v2,v3,v6
	vor	v2,v3,v3
112:	stvx	v4,r4,r12
	vadduwm	v11,v9,v11
	addi	r12,r12,16
	bdnzf	25,11b
	add	r9,r4,r12
	addi	r11,r11,-1
	bgt	cr6,19f
12:	add	r10,r4,r5
	add	r11,r3,r5
	bge	13f
	addi	r11,r11,-16
13:	mtcrf	0x01,r10
	addi	r0,r11,-1
131:	lvx	v3,0,r0
	vaddcuw	v9,v4,v8
	vadduwm	v8,v4,v8
	vadduwm	v11,v9,v11
	vperm	v4,v2,v3,v6
	beq	cr5,17f
	vsel	v4,v4,v0,v1
	rlwinm	r10,r10,0,0,27
	li	r9,0
	bnl	cr7,14f
132:	stvewx	v4,r10,r9
	addi	r9,r9,4
134:	stvewx	v4,r10,r9
	addi	r9,r9,4
14:	bng	cr7,15f
142:	stvewx	v4,r10,r9
	addi	r9,r9,4
15:	bne	cr7,16f
152:	stvehx	v4,r10,r9
	addi	r9,r9,2
16:	bns	cr7,18f
162:	stvebx	v4,r10,r9
	b	18f
17:	stvx	v4,r4,r12
18:	vaddcuw	v9,v4,v7
	vadduwm	v12,v4,v7
	vaddcuw	v10,v12,v8
	vadduwm	v8,v12,v8
	vadduwm	v9,v9,v10
500:	vmrglh	v2,v0,v8
	vadduwm	v11,v9,v11
	vmrghh	v3,v0,v8
	rlwinm	r10,r1,0,0,27
	vsumsws	v0,v11,v0
	vadduwm	v8,v2,v3
	li	r12,-16
	vsumsws	v8,v8,v0
182:	stvx	v8,r10,r12
183:	lwz	r3,-4(r10)
	addc	r3,r3,r6
	addze	r3,r3
	blr	
19:	lvx	v3,r3,r12
	addi	r11,r11,-1
	vaddcuw	v9,v4,v8
	vadduwm	v8,v4,v8
	mtcrf	0x02,r9
	addi	r9,r9,16
	addi	r0,r11,-2
	vperm	v4,v2,v3,v6
	vor	v2,v3,v3
192:	stvx	v4,r4,r12
	addi	r12,r12,16
	vadduwm	v11,v9,v11
	bdnzf	27,19b
	mtcrf	0x02,r10
	addi	r11,r3,96
	addi	r9,r12,16
	bns	cr6,20f
	bdnz	20f
20:	lvx	v3,r3,r12
	addi	r11,r11,32
	vaddcuw	v9,v4,v7
201:	lvx	v5,r3,r9
	vadduwm	v12,v4,v7
	dcbt	0,r11
	vaddcuw	v10,v12,v8
	DCBAR4R12
	vadduwm	v8,v12,v8
	vperm	v7,v2,v3,v6
202:	stvx	v7,r4,r12
	vperm	v4,v3,v5,v6
	vadduwm	v9,v9,v10
	bdz	21f
21:	stvx	v4,r4,r9
	vor	v2,v5,v5
	vadduwm	v11,v9,v11
	addi	r12,r9,16
	addi	r9,r12,16
	bdnz	20b
	bso	cr6,22f
	b	12b
22:	lvx	v3,r3,r12
	vaddcuw	v9,v4,v8
	vadduwm	v8,v4,v8
	vadduwm	v11,v9,v11
	vperm	v4,v2,v3,v6
	vor	v2,v3,v3
222:	stvx	v4,r4,r12
	addi	r12,r12,16
	b	12b

/* Intent of this exception table is to store -EFAULT to *src_err or
 * or *dst_err respectively, and (for an error on src) zero the rest
 * of dst.  Return checksum for only those bytes stored before error.
 * (Can't quite figure out how this return value is used since there
 * is no way to restart from the point of error.  So I'll only return 
 * the checksum for actual buffer as stored in memory.  Doesn't look
 * like scalar version adds in bytes loaded but not stored.)
 *    
 * Register useage here:
 *    r3 = src, return checksum
 *    r4 = dst
 *    r5 = (preserve as total byte count til near end)
 *    r6 = entering partial sum; accumulator for scalar result
 *    r7 = src_err
 *    r8 = dst_err
 *    r9 = bytes not copied
 *    r10= dst + byte count
 *    r11= number of quad words (vectors)
 *    r12= Byte Kount index
 */

/* read fault, initial half-word copy */
100:	li	r0,0
	sthu	r0,2(r9)		/* Zero rest of buffer */
	cmpi	0,r7,0
	beq	104f		/* Go return checksum */
	li	r0,-EFAULT
	stw	r0,0(r7)
	b	104f

/* write fault, initial half-word copy */
101:	cmpi	0,r8,0
	beq	104f
	li	r0,-EFAULT
	stw	r0,0(r8)
	b	104f

/* read fault, final single-byte copy */
102:	li	r0,0
	stb	r0,-1(r10)	/* Zero remaining byte */
	cmpi	0,r7,0
	beq	104f
	li	r0,-EFAULT
	stw	r0,0(r7)
	b	104f

/* write fault, final single-byte copy */
103:	cmpi	0,r8,0
	beq	104f
	li	r0,-EFAULT
	stw	r0,0(r8)
104:	addze	r3,r6
	blr

/* read fault, 1st and 2nd vector load */
105:	cmpi	0,r7,0
	beq	155f
	li	r0,-EFAULT
	stw	r0,0(r7)
155:	rlwinm	r0,r5,31,1,31
	andi.	r12,r5,1
	mtctr	r0
	addi	r9,r4,-2
	li	r0,0
106:	sthu	r0,2(r9)
	bdnz	106b
	beq	107f
	stb	r0,2(r9)
107:	addze	r3,r6
	blr

/* write fault, initial vector store(s) (Nothing stored yet) */
108:	cmpi	0,r8,0
	beq	109f
	li	r0,-EFAULT
	stw	r0,0(r8)
109:	addze	r3,r6
	blr
	
/* read fault, load in 16B loop or final load  */
110:	cmpi	0,r7,0
	beq	156f
	li	r0,-EFAULT
	stw	r0,0(r7)
156:	add	r11,r4,r5		/* Last dst byte + 1 */
	add	r4,r4,r12		/* Current dst byte */
	rlwinm  r4,r4,0,0,27	/* Rounded down */
	subf	r5,r4,r11
	rlwinm.	r0,r5,31,1,31
	addi	r9,r4,-2
	cmpi	1,r0,0
	beq	cr1,157f	
	mtctr	r0
	li	r0,0
111:	sthu	r0,2(r9)
	bdnz	111b
157:	andi.	r12,r5,1
	beq	18b
	li	r0,0
	stb	r0,2(r9)
	vaddcuw	v9,v4,v8
	vadduwm	v8,v4,v8
	vxor	v11,v11,v11
	b	500b		/* Go sum across vector checksum */

/* write fault, store in 16B loop  */
1120:	cmpi	0,r8,0
	beq	113f
	li	r0,-EFAULT
	stw	r0,0(r8)
113:	b	500b

/* write fault, final partial store(s)  */

114:	cmpi	0,r8,0
	vxor	v11,v11,v11
	beq	115f
	li	r0,-EFAULT
	stw	r0,0(r8)
115:	b	500b

/* write fault, 1st store in 32B loop  */
116:	cmpi	0,r8,0
	vadduwm	v9,v9,v10
	beq	117f
	li	r0,-EFAULT
	stw	r0,0(r8)
117:	b	500b

/* write fault, 2nd store in 32B loop  */
118:	cmpi	0,r8,0
	vxor	v4,v4,v4
	vadduwm	v11,v9,v11
	beq	119f
	li	r0,-EFAULT
	stw	r0,0(r8)
119:	b	18b

/* read fault, next to final load  */
120:	cmpi	0,r7,0
	beq	121f
	li	r0,-EFAULT
	stw	r0,0(r7)
121:	add	r11,r4,r5
	add	r4,r4,r12
	rlwinm  r4,r4,0,0,27
	subf	r5,r4,r11
	rlwinm.	r0,r5,31,1,31
	addi	r9,r4,-2
	cmpi	1,r0,0
	beq	cr1,123f	
	mtctr	r0
	li	r0,0
122:	sthu	r0,2(r9)
	bdnz	122b
123:	andi.	r12,r5,1
	beq	124f
	li	r0,0
	stb	r0,2(r9)
124:	vaddcuw	v9,v4,v8
	vadduwm	v8,v4,v8
	vadduwm	v11,v9,v11
	vxor	v4,v4,v4
	b	18b

/* write fault, 1st store in 32B loop  */
125:	cmpi	0,r8,0
	vxor	v4,v4,v4
	beq	126f
	li	r0,-EFAULT
	stw	r0,0(r8)
126:	b	18b

/* write or read fault in push/pop from stack. csumcpy complete. */

127:	vxor	v0,v0,v0
	vspltisw v2,1
 	lis	r5,0x8000
	vnor	v1,v0,v0
	vmrglh	v8,v0,v8
 	li	r10,17
	vsldoi	v3,v0,v1,4
 	li	r3,0
	mtctr	r10
	vsumsws	v8,v8,v0
 	vand	v4,v2,v3
128:	vand	v5,v8,v4
	rlwinm	r5,r5,1,0,31
	vcmpequw.	v6,v5,v4
	vsl	v4,v4,v2
	bnl	cr6,129f
	or	r3,r3,r5
129:	bdnz	128b
	addc	r3,r3,r6
	addze	r3,r3
	blr

#ifndef TEST_OUTSIDE_LINUX
	.section __ex_table,"a"	
	.align	2		
	.long	1b,100b
	.long	204b,101b
	.long	201b,102b
	.long	202b,103b
	.long	401b,105b
	.long	5b,105b
	.long	502b,108b
	.long	602b,108b
	.long	702b,108b
	.long	802b,108b
	.long	804b,108b
	.long	9b,108b
	.long	11b,110b
	.long	112b,1120b
	.long	131b,110b
	.long	132b,114b
	.long	134b,114b
	.long	142b,114b
	.long	152b,114b
	.long	162b,114b
	.long	17b,114b
	.long	182b,127b
	.long	183b,127b
	.long	19b,110b
	.long	192b,112b
	.long	20b,110b
	.long	201b,110b
	.long	202b,116b
	.long	21b,118b
	.long	22b,120b
	.long	222b,125b
#endif

	.text
#ifndef TEST_OUTSIDE_LINUX
_GLOBAL(csum_partial_vec)
#else
#if __MWERKS__
	.align	16
#else
	.align	4
#endif
	.global	csum_partial_vec     
csum_partial_vec:
#endif

	li	r12,32
	rlwinm	r0,r4,31,1,31
	cmpi	cr7,0,r4,48
	dcbt	r3,r12
	cmpi	cr6,0,r0,0
	addic	r5,r5,0
	addi	r11,r3,-2
	add	r10,r3,r4
	bgt	cr7,4f
	andi.	r12,r4,1
	beq	cr6,2f
	mtctr	r0
1:	lhzu	r0,2(r11)
	addc	r5,r5,r0
	bdnz	1b
2:	beq	3f
	lbz	r0,-1(r10)
	rlwinm	r0,r0,8,16,23
	addc	r5,r5,r0
3:	addze	r3,r5
	blr	
4:	lvsr	v5,0,r3
	addi	r9,r3,16
	li	r12,64
	lvsr	v7,r3,r4
	rlwinm	r9,r9,0,0,27
	addi	r10,r10,-1
	lvx	v2,0,r3
	subf	r11,r9,r10
	vxor	v0,v0,v0
	dcbt	r3,r12
	rlwinm	r11,r11,28,4,31
	vnor	v1,v0,v0
	mtctr	r11
	vxor	v11,v11,v11
	vperm	v5,v1,v0,v5
	cmpi	cr6,0,r11,4
	vxor	v8,v8,v8
	vperm	v1,v0,v1,v7
	li	r12,16
	vsel	v2,v2,v0,v5
5:	lvx	v3,r3,r12
	vaddcuw	v9,v2,v8
	vadduwm	v8,v2,v8
	vadduwm	v11,v9,v11
	addi	r12,r12,16
	vor	v2,v3,v3
	bdnzf	25,5b
	add	r9,r3,r12
	addi	r11,r11,-1
	bgt	cr6,8f
	vxor	v3,v3,v3
6:	lvx	v5,0,r10
	vaddcuw	v9,v2,v3
	rlwinm	r10,r10,0,28,31
	vadduwm	v12,v2,v3
	cmpi	cr7,0,r10,0xF
	vaddcuw	v10,v12,v8
	vadduwm	v8,v12,v8
	vadduwm	v9,v9,v10
	vadduwm	v11,v9,v11
	beq	cr7, 7f
	vsel	v5,v5,v0,v1
7:	vaddcuw	v9,v5,v8
	vadduwm	v8,v5,v8
	vadduwm	v11,v9,v11
	vmrglh	v2,v0,v8
	vmrghh	v3,v0,v8
	rlwinm	r10,r1,0,0,27
	vsumsws	v0,v11,v0
	vadduwm	v8,v2,v3
	li	r12,-16
	vsumsws	v8,v8,v0
	stvx	v8,r10,r12
	lwz	r3,-4(r10 )
	addc	r3,r3,r5
	addze	r3,r3
	blr	
	.align	4
8:	lvx	v3,r3,r12
	addi	r11,r11,-1
	vaddcuw	v9,v2,v8
	vadduwm	v8,v2,v8
	mtcrf	0x02,r9
	addi	r9,r9,16
	addi	r0,r11,-2
	vor	v2,v3,v3
	addi	r12,r12,16
	vadduwm	v11,v9,v11
	bdnzf	27,8b
	mtcrf	0x02,r10
	addi	r11,r3,96
	vxor	v3,v3,v3
	bns	cr6,9f
	bdnz	9f
9:	lvx	v5,r3,r12
	addi	r12,r12,16
	vaddcuw	v9,v2,v3
	lvx	v6,r3,r12
	addi	r11,r11,32
	vadduwm	v12,v2,v3
	dcbt	0,r11
	addi	r12,r12,16
	vaddcuw	v10,v12,v8
	vadduwm	v8,v12,v8
	vadduwm	v9,v9,v10
	bdz	10f
10:	vadduwm	v11,v9,v11
	vor	v2,v5,v5
	vor	v3,v6,v6
	bdnz	9b
	bso	cr6,11f
	b	6b
11:	lvx	v5,r3,r12
	addi	r12,r12,16
	vaddcuw	v9,v2,v3
	vadduwm	v12,v2,v3
	vaddcuw	v10,v12,v8
	vadduwm	v8,v12,v8
	vadduwm	v9,v9,v10
	vadduwm	v11,v9,v11
	vxor	v3,v3,v3
	vor	v2,v5,v5
	b	6b
