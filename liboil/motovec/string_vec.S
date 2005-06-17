/*
 * AltiVec versions (*_vec) of equivalent Linux library functions
 * found in /arch/ppc/lib/string.S from Linux 2.4.17.  Suggest this
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
 *
 * AltiVec versions will only deal with L1_CACHE_LINE_SIZE=32
 */


#ifndef TEST_OUTSIDE_LINUX
#include "../kernel/ppc_asm.tmpl"
#include <linux/config.h>
#include <asm/processor.h>
#include <asm/cache.h>
#include <asm/errno.h>
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
#define L1_CACHE_LINE_SIZE 32
#define LG_L1_CACHE_LINE_SIZE 5
#define MAX_L1_COPY_PREFETCH 1
#endif

/* AltiVec versions of selected functions for use on AltiVec
 * enabled G4 and later microprocessors.
 */ 
#if defined(__GNUC__) || defined(__MWERKS__)  /* gcc and codewarrior don't assemble dcba */
#define DCBA_R3R7	.long 0x7c033dec
#define DCBA_R3R9	.long 0x7c034dec
#define DCBA_R0R8	.long 0x7c0045ec
#else
#define DCBA_R3R7	dcba	r4,r7
#define DCBA_R3R9	dcba	r4,r9
#define DCBA_R0R8	dcba	0,r8
#endif

     .text    
     .align   5
     .global  backwards_memcpy_vec
backwards_memcpy_vec:   
     nop      
     .global  memmove_vec
memmove_vec:   
     nop
	.global	cacheable_memcpy_vec
cacheable_memcpy_vec:
     nop     
     .global  memcpy_vec
memcpy_vec:    
	subf.	r7,r4,r3
	cmpi	cr1,0,r5,0
	cmpi	cr7,0,r5,16
	addi	r8,r4,-1
	addi	r9,r3,-1
	add	r10,r4,r5
	beqlr    
	add	r11,r3,r5
	subf	r0,r3,r4
	beqlr	cr1
	bgt	2f
	cmpi	cr5,0,r0,128
	bgt	cr7,23f
	mtctr	r5
1:	lbzu	r0,1(r8)
	stbu	r0,1(r9)
	bdnz	1b
	blr      
2:	cmpi	cr5,0,r7,128
	cmp	cr6,0,r7,r5
	bgt	cr7,4f
	mtctr	r5
3:	lbzu	r0,-1(r10)
	stbu	r0,-1(r11)
	bdnz	3b
	blr      

4:	rlwinm	r8,r4,0,28,31
	rlwinm	r9,r3,0,28,31
	bge	cr6,24f
	lis	r11,0x010c
	subf.	r8,r9,r8
	lvsr	v2,0,r7
	ori	r11,r11,0xffe0
	addi	r11,r10,-1
	bgt	5f
	addi	r8,r8,16
5:	rlwinm	r11,r11,0,0,27
	addi	r7,r5,-1
	subf	r0,r11,r10
	add	r11,r3,r7
	addi	r10,r3,16
	subf.	r8,r0,r8
	rlwinm	r0,r11,0,28,31
	rlwinm	r10,r10,0,0,27
	blt	6f
	lvx	v1,r4,r7
	addi	r4,r4,-16
6:	lvx	v0,r4,r7
	subf	r10,r10,r11
	cmpi	cr7,0,r0,0xF
	cmpi	cr1,0,r9,0
	rlwinm	r10,r10,28,4,31
	add	r0,r3,r5
	cmpi	cr6,0,r10,0
	vperm	v3,v0,v1,v2
	vor	v1,v0,v0
	beq	cr7,10f
	mtcrf	0x01,r0
	rlwinm	r11,r11 ,0,0,27
	li	r9,0
	bnl	cr7,7f
	stvewx	v3,r11,r9
	addi	r9,r9,4
	stvewx	v3,r11,r9
	addi	r9,r9,4
7:	bng	cr7,8f
	stvewx	v3,r11,r9
	addi	r9,r9,4
8:	bne	cr7,9f
	stvehx	v3,r11,r9
	addi	r9,r9,2
9:	bns	cr7,11f
	stvebx	v3,r11,r9
	b	11f
10:	stvx	v3,r3,r7
11:	addi	r7,r7,-16
	ble	cr6,13f
	mtctr	r10
	cmpi	cr6,0,r10,4
12:	lvx	v0,r4,r7
	vperm	v3,v0,v1,v2
	vor	v1,v0,v0
	stvx	v3,r3,r7
	addi	r7,r7,-16
	bdnzf	25,12b
	add	r9,r3,r7
	bgt	cr6,19f
13:	blt	14f
	addi	r4,r4,16
14:	lvx	v0,0,r4
	vperm	v3,v0,v1,v2
	subfic	r9,r3,16
	beq	cr1,18f
	mtcrf	0x01,r9
	li	r9,0
	bns	cr7,15f
	stvebx	v3,r3,r9
	addi	 r9,r9,1
15:	bne	cr7,16f
	stvehx	v3,r3,r9
	addi	r9,r9,2
16:	bng	cr7,17f
	stvewx	v3,r3,r9
	addi	r9,r9,4
17:	bnllr	cr7
	stvewx	v3,r3,r9
	addi	r9,r9,4
	stvewx	v3,r3,r9
	blr
18:	stvx	v3,0,r3    
	blr      
19:	lvx	v0,r4,r7
	mtcrf	0x02,r9
	vperm	v3,v0,v1,v2
	vor	v1,v0,v0
	addi	r9,r9,-16
	stvx	v3,r3,r7
	vor	v7,v0,v0
	addi	r7,r7,-16
	bdnzt	27,19b
	lis	r8,0x102
	mtcrf	0x02,r3
	addi	r9,r7,-16
	ori	r8,r8,0xffe0
	addi	r11,r4,-64
	bso	cr6,20f
	bdnz	20f
20:	lvx	v6,r4,r7
	addi	r11,r11,-32
	lvx	v1,r4,r9
	vperm	v3,v6,v7,v2
	DCBA_R3R9
	vperm	v4,v1,v6,v2
	vor	v7,v1,v1
	bdz	21f
21:	stvx	v3,r3,r7
	addi	r7,r9,-16
	stvx	v4,r3,r9
	addi	r9,r7,-16
	bdnz	20b
	bns	cr6,22f
	b	13b
22:	lvx	v1,r4,r7
	vperm	v4,v1,v7,v2
	stvx	v4,r3,r7
	b	13b

23:	rlwinm	r8,r4,0,28,31
	rlwinm	r9,r3,0,28,31
24:	lis	r10,0x010c
	subf.	r8,r8,r9
	lvsr	v2,0,r7
	ori	r10,r10,32
	dst	r4,r10,0
	addi	r10,r3,16
	addi	r11,r11,-1
	bge	25f
	lvx	v0,0,r4
	addi	r4,r4,16
25:	lvx	v1,0,r4
	rlwinm	r10,r10,0,0,27
	cmpi	cr1,0,r9,0
	subf	r0,r3,r10
	subf	r10,r10,r11
	li	r7,0
	mtcrf	0x01,r0
	rlwinm	r10,r10,28,4,31
	vperm	v3,v0,v1,v2
	vor	v0,v1,v1
	beq	cr1,29f
	bns	cr7,26f
	stvebx	v3,r3,r7
	addi	r7,r7,1
26:	bne	cr7,27f
	stvehx	v3,r3,r7
	addi	r7,r7,2
27:	bng	cr7,28f
	stvewx	v3,r3,r7
	addi	r7,r7,4
28:	bnl	cr7,30f
	stvewx	v3,r3,r7
	addi	r7,r7,4
	stvewx	v3,r3,r7
	b	30f
29:	stvx	v3,0,r3
30:	rlwinm	r0,r11,0,28,31
	cmpi	cr6,0,r10,0
	li	r7,16
	cmpi	cr1,0,r0,0xF
	cmpi	cr7,0,r10,14
	ble	cr6,32f
	mtctr	r10
	cmpi	cr6,0,r10,4
31:	lvx	v1,r4,r7
	vperm	v3,v0,v1,v2
	vor	v0,v1,v1
	stvx	v3,r3,r7
	addi	r7,r7,16
	bdnzf	25,31b
	add	r9,r3,r7
	addi	r10,r10,-1
	bgt	cr6,38f
32:	add	r11,r3,r5
	add	r10,r4,r5
	bge	33f
	addi	r10,r10,-16
33:	mtcrf	0x01,r11
	addi	r11,r11,-1
	addi	r0,r10,-1
	lvx	v1,0,r0
	dss	0
	dss	1
	vperm	v3,v0,v1,v2
	beq	cr1,37f
	rlwinm	r11,r11,0,0,27
	li	r9,0
	bnl	cr7,34f
	stvewx	v3,r11,r9
	addi	r9,r9,4
	stvewx	v3,r11,r9
	addi	r9,r9,4
34:	bng	cr7,35f
	stvewx	v3,r11,r9
	addi	r9,r9,4
35:	bne	cr7,36f
	stvehx	v3,r11,r9
	addi	r9,r9,2
36:	bnslr	cr7
	stvebx	v3,r11,r9
	blr
37:	stvx	v3,r3,r7
	blr 

38:	lvx	v1,r4,r7
	addi	r10,r10,-1
	mtcrf	0x02,r9
	addi	r9,r9,16
	addi	r0,r10,-2
	vperm	v3,v0,v1,v2
	vor	v0,v1,v1
	stvx	v3,r3,r7
	addi	r7,r7,16
	bdnzf	27,38b
	mtcrf	0x02,r11
	lis	r8,0x104
	addi	r9,r7,16
	ori	r8,r8,32
	rlwinm	r11,r0,29,3,31
	rlwinm	r0,r0,0,0,28
	bgt	cr7,43f
39:	addi	r11,r4,256
	xoris	r8,r8,0x6
	bns	cr6,40f
	bdnz	40f
40:	lvx	v1,r4,r7
	addi	r11,r11,32
	lvx	v6,r4,r9
	vperm	v4,v0,v1,v2
	dst	r11,r8,1
	DCBA_R3R7
	vperm	v3,v1,v6,v2
	vor	v0,v6,v6
	bdz	41f
41:	stvx	v4,r3,r7
	addi	r7,r9,16
	stvx	v3,r3,r9
	addi	r9,r7,16
	bdnz	40b
	bso	cr6,42f
	b	32b
42:	lvx	v1,r4,r7
	vperm	v3,v0,v1,v2
	vor	v0,v1,v1
	stvx	v3,r3,r7
	addi	r7,r7,16
	b	32b

43:	subf	r10,r0,r10
	blt	cr5,39b
	mtctr	r11
	addi	r11,r4,256
44:	lvx	v1,r4,r7
	addi	r9,r7,32
	addi	r11,r11,128
	lvx	v7,r4,r9
	addi	r9,r9,32
	lvx	v9,r4,r9
	addi	r9,r9,32
	lvx	v11,r4,r9
	addi	r9,r7,16
	lvx	v6,r4,r9
	addi	r9,r9,32
	lvx	v8,r4,r9
	addi	r9,r9,32
	lvx	v10,r4,r9
	addi	r9,r9,32
	vperm	v3,v0,v1,v2
	lvx	v0,r4,r9
	vperm	v4,v1,v6,v2
	dst	r11,r8,1
	DCBA_R3R7
	stvx	v3,r3,r7
	addi	r7,r7,16
	vperm	v5,v6,v7,v2
	stvx	v4,r3,r7
	addi	r7,r7,16
	vperm	v6,v7,v8,v2
	DCBA_R3R7
	stvx	v5,r3,r7
	addi	r7,r7,16
	vperm	v7,v8,v9,v2
	stvx	v6,r3,r7
	addi	r7,r7,16
	vperm	v8,v9,v10,v2
	DCBA_R3R7
	stvx	v7,r3,r7
	addi	r7,r7,16
	vperm	v9,v10,v11,v2
	stvx	v8,r3,r7
	addi	r7,r7,16
	vperm	v10,v11,v0,v2
	DCBA_R3R7
	stvx	v9,r3,r7
	addi	r7,r7,16
	stvx	v10,r3,r7
	addi	r7,r7,16
	bdnz	44b
	mtctr	r10
	addi	r9,r7,16
	bns	cr6,40b
	bdnz	40b

	.global  bcopy_vec
bcopy_vec:     
	mr	r0,r3
	mr	r3,r4
	mr	r4,r0
	b	memcpy_vec

	.text
	.align	4
	.globl	__clear_user_vec
__clear_user_vec:
	mr	r5,r4
	li	r4,0
	.globl	memset_vec
memset_vec:
	cmpi	cr7,0,r5,16
	cmpi	cr1,0,r5,0
	rlwinm.	r8,r4,28,28,3
	addi	r9,r3,-1
	addi	r10,r3,16
	add	r6,r3,r5
	bgt	cr7,2f
	mtctr	r5
	beqlr	cr1
1:	stbu	r4,1(r9)
	bdnz	1b
	blr              
2:	rlwinm	r10,r10,0,0,27
	addi	r11,r6,-1
	subf	r9,r3,r10
	li	r7,0
	vxor	v0,v0,v0
	subf	r10,r10 ,r11
	cmpi	cr1,0,r9,16
	beq	3f
	lvsl	v0,0,r8
	vspltisb	v1,4
	lvsl	v2,0,r4
	vslb	v0,v0,v1
	vor	v0,v0,v2
	vspltb	v0,v0,0
3:	mtcrf	0x01,r9
	rlwinm	r10,r10,28,4,31
	beq	cr1,7f
	bns	cr7,4f
32:	stvebx	v0,r3,r7
	addi	r7,r7,1
4:	bne	cr7,5f
42:	stvehx	v0,r3,r7
	addi	r7,r7,2
5:	bng	cr7,6f
52:	stvewx	v0,r3,r7
	addi	r7,r7,4
6:	bnl	cr7,8f
62:	stvewx	v0,r3,r7
	addi	r7,r7,4
64:	stvewx	v0,r3,r7
	b	8f
7:	stvx	v0,0,r3
8:	rlwinm	r0,r11,0,28,31
	cmpi	cr6,0,r10,0
	li	r7,16
	cmpi	cr1,0,r0,0xF
	ble	cr6,10f
	mtctr	r10
	cmpi	cr6,0,r10,4
9:	stvx	v0,r3,r7
	addi	r7,r7,16
	bdnzf	25,9b
	add	r9,r3,r7
	addi	r10,r10,-1
	bgt	cr6,16f
10:	mtcrf	0x01,r6
	beq	cr1,14f
	rlwinm	r11,r11,0,0,27
	li	r9,0
	bnl	cr7,11f
102:	stvewx	v0,r11,r9
	addi	r9,r9,4
104:	stvewx	v0,r11,r9
	addi	r9,r9,4
11:	bng	cr7,12f
112:	stvewx	v0,r11,r9
	addi	r9,r9,4
12:	bne	cr7,13f
122:	stvehx	v0,r11,r9
	addi	r9 ,r9 ,2
13:	bnslr	cr7
132:	stvebx	v0,r11,r9
	blr              
14:	stvx	v0,r3,r7
	blr              

16:	addi	r10,r10,-1
	mtcrf	0x02,r9
	addi	r9,r9,16
162:	stvx	v0,r3,r7
	addi	r7,r7,16
	bdnzf	27,16b
	mtcrf	0x02,r11
	bns	cr6,17f
	bdnz	17f                       
17:	stvx	v0,r3,r7
	addi	r7,r7,16
	bdz	18f
18:	stvx	v0,r3,r7
	addi	r7,r7,16
	bdnz	17b
	bso	cr6,19f
	b	10b
19:	stvx	v0,r3,r7
	addi	r7,r7,16
	b	10b

/* Intent of this exception table appears to be to return the byte count */
/* remaining to be cleared when the current store error occurred.  Chuck */
/* Memset doesn't require it but the code is identical to __clear_user   */
/* FIRST FAILURE CHECKED BY RECOMPILATION WITH BRANCHES SUBSTITUTED
 * FOR STORES.    chuckc  030515
*/

91:	mfctr	r3		/* Return byte count remaining */
	blr
92:	subf	r3,r7,r5	/* BC minus bytes already stored */
	blr
93:	mr	r3,r5		/* Nothing stored yet */
	blr
94:	add	r11,r3,r5
	rlwinm	r6,r11,0,28,31	/* Bytes in last vector */
	b	99f
95:	add	r11,r3,r5
	rlwinm	r6,r11,0,28,31	
	subf	r3,r9,r6
	blr
96:	li	r3,16		/* 16 bytes in last vector to be stored. */
	blr
97:	add	r11,r3,r5
	rlwinm	r6,r11,0,27,31
99:	mfctr	r3
	rlwinm	r3,r3,4,0,27
	add	r3,r3,r6
	blr
98:	add	r11,r3,r5	
	rlwinm	r3,r11,0,27,31
	blr

#ifndef TEST_OUTSIDE_LINUX
	.section __ex_table,"a"	
	.align	2		
	.long	1b,91b
	.long	32b,92b
	.long	42b,92b
	.long	52b,92b
	.long	62b,92b
	.long	64b,92b
	.long	7b,93b
	.long	9b,94b
	.long	102b,95b
	.long	104b,95b
	.long	112b,95b
	.long	122b,95b
	.long	132b,95b
	.long	14b,96b
	.long	162b,94b
	.long	17b,97b
	.long	18b,97b
	.long	19b,98b
#endif
	.text
/* Scalar __copy_tofrom_user always copies forward and never checks 
 * for overlap, __copy_tofrom_user_vec will do the same except it will
 * check that overlap is > 128B before entering 128B loop when copying
 * forward. 
 * The scalar version always assumes the destination and source
 * are word aligned.  This routine will assume the same to simplify handling
 * exceptions.    chuckc
 */

	.globl	__copy_tofrom_user_vec
__copy_tofrom_user_vec:
	subf.	r7,r4,r3
	cmpi	cr1,0,r5,0
	cmpi	cr7,0,r5,16
	addi	r8,r4,-1
	addi	r9,r3,-1
	add	r10,r4,r5
	beqlr    
	add	r11,r3,r5
	subf	r0,r3,r4
	beqlr	cr1
	bgt	1f
	cmpi	cr5,0,r0,128	/* Overlap |(DST-SRC)|> 128B? */
	bgt	cr7,23f		/* b to v_memcpy */
1:	cmpi	cr5,0,r7,128	/* Overlap |(DST-SRC)|> 128B? */
	bgt	cr7,23f		/* b to v_memcpy */
	mtctr	r5
2:	lbzu	r0,1(r8)
202:	stbu	r0,1(r9)
	bdnz	2b
	li	r3,0
	blr    

23:	rlwinm	r8,r4,0,28,31
	rlwinm	r9,r3,0,28,31
24:	lis	r10,0x010c
	subf.	r8,r8,r9
	lvsr	v2,0,r7
	ori	r10,r10,32
	dst	r4,r10,0
	addi	r10,r3,16
	addi	r11,r11,-1
	bge	25f
241:	lvx	v0,0,r4
	addi	r4,r4,16
25:	lvx	v1,0,r4
	rlwinm	r10,r10,0,0,27
	cmpi	cr1,0,r9,0
	subf	r0,r3,r10
	subf	r10,r10,r11
	li	r7,0
	mtcrf	0x01,r0
	rlwinm	r10,r10,28,4,31
	vperm	v3,v0,v1,v2
	vor	v0,v1,v1
	beq	cr1,29f
	bns	cr7,26f
252:	stvebx	v3,r3,r7
	addi	r7,r7,1
26:	bne	cr7,27f
262:	stvehx	v3,r3,r7
	addi	r7,r7,2
27:	bng	cr7,28f
272:	stvewx	v3,r3,r7
	addi	r7,r7,4
28:	bnl	cr7,30f
282:	stvewx	v3,r3,r7
	addi	r7,r7,4
284:	stvewx	v3,r3,r7
	b	30f
29:	stvx	v3,0,r3
30:	rlwinm	r0,r11,0,28,31
	cmpi	cr6,0,r10,0
	li	r7,16
	cmpi	cr1,0,r0,0xF
	cmpi	cr7,0,r10,14
	ble	cr6,32f
	mtctr	r10
	cmpi	cr6,0,r10,4
31:	lvx	v1,r4,r7
	vperm	v3,v0,v1,v2
	vor	v0,v1,v1
312:	stvx	v3,r3,r7
	addi	r7,r7,16
	bdnzf	25,31b
	add	r9,r3,r7
	addi	r10,r10,-1
	bgt	cr6,38f
32:	add	r11,r3,r5
	add	r10,r4,r5
	bge	33f
	addi	r10,r10,-16
33:	mtcrf	0x01,r11
	addi	r11,r11,-1
	addi	r0,r10,-1
331:	lvx	v1,0,r0
	dss	0
	dss	1
	vperm	v3,v0,v1,v2
	beq	cr1,37f
	rlwinm	r11,r11,0,0,27
	li	r9,0
	li	r3,0
	bnl	cr7,34f
332:	stvewx	v3,r11,r9
	addi	r9,r9,4
334:	stvewx	v3,r11,r9
	addi	r9,r9,4
34:	bng	cr7,35f
342:	stvewx	v3,r11,r9
	addi	r9,r9,4
35:	bne	cr7,36f
352:	stvehx	v3,r11,r9
	addi	r9,r9,2
36:	bnslr	cr7
362:	stvebx	v3,r11,r9
	blr
37:	stvx	v3,r3,r7
	li	r3,0
	blr 
     
	.align	4
38:	lvx	v1,r4,r7
	addi	r10,r10,-1
	mtcrf	0x02,r9
	addi	r9,r9,16
	addi	r0,r10,-2
	vperm	v3,v0,v1,v2
	vor	v0,v1,v1
382:	stvx	v3,r3,r7
	addi	r7,r7,16
	bdnzf	27,38b
	mtcrf	0x02,r11
	lis	r8,0x104
	addi	r9,r7,16
	ori	r8,r8,32
	rlwinm	r11,r0,29,3,31
	rlwinm	r0,r0,0,0,28
	bgt	cr7,43f
39:	addi	r11,r4,256
	xoris	r8,r8,0x6
	bns	cr6,40f
	bdnz	40f
40:	lvx	v1,r4,r7
	addi	r11,r11,32
401:	lvx	v6,r4,r9
	vperm	v4,v0,v1,v2
	dst	r11,r8,1
	DCBA_R3R7
	vperm	v3,v1,v6,v2
	vor	v0,v6,v6
402:	stvx	v4,r3,r7
	addi	r7,r9,16
	bdz	41f
41:	stvx	v3,r3,r9
	addi	r9,r7,16
	bdnz	40b
	bso	cr6,42f
	b	32b
42:	lvx	v1,r4,r7
	vperm	v3,v0,v1,v2
	vor	v0,v1,v1
422:	stvx	v3,r3,r7
	addi	r7,r7,16
	b	32b

43:	subf	r10,r0,r10
	blt	cr5,39b
	mtctr	r11
	addi	r11,r4,256
44:	lvx	v1,r4,r7
	addi	r9,r7,32
	addi	r11,r11,128
443:	lvx	v7,r4,r9
	addi	r9,r9,32
447:	lvx	v9,r4,r9
	addi	r9,r9,32
451:	lvx	v11,r4,r9
	addi	r9,r7,16
441:	lvx	v6,r4,r9
	addi	r9,r9,32
445:	lvx	v8,r4,r9
	addi	r9,r9,32
449:	lvx	v10,r4,r9
	addi	r9,r9,32
	vperm	v3,v0,v1,v2
453:	lvx	v0,r4,r9
	vperm	v4,v1,v6,v2
	dst	r11,r8,1
	DCBA_R3R7
440:	stvx	v3,r3,r7
	addi	r7,r7,16
	vperm	v5,v6,v7,v2
442:	stvx	v4,r3,r7
	addi	r7,r7,16
	vperm	v6,v7,v8,v2
	DCBA_R3R7
444:	stvx	v5,r3,r7
	addi	r7,r7,16
	vperm	v7,v8,v9,v2
446:	stvx	v6,r3,r7
	addi	r7,r7,16
	vperm	v8,v9,v10,v2
	DCBA_R3R7
448:	stvx	v7,r3,r7
	addi	r7,r7,16
	vperm	v9,v10,v11,v2
450:	stvx	v8,r3,r7
	addi	r7,r7,16
	vperm	v10,v11,v0,v2
	DCBA_R3R7
452:	stvx	v9,r3,r7
	addi	r7,r7,16
454:	stvx	v10,r3,r7
	addi	r7,r7,16
	bdnz	44b
	mtctr	r10
	addi	r9,r7,16
	bns	cr6,40b
	bdnz	40b

/* Intent of this exception table is to return:
 *    r3 = bytes not copied (but preserve dst address in r3 til end)
 *    r4 = 0 on read fault; 1 on write fault
 * Register useage here:
 *    r5 = (preserve as total byte count til near end)
 *    r6 = bytes not copied (move to r3 at end)
 *    r7 = byte count index from memcpy_vec
 *    r9 = alternate byte count index in 128B loop
 *    r10= vectors (QWs remaining) after 128B loop
 *    r11= next destination address (assume word-aligned)
 * For read fault, clear out the destination for bytes remaining
 * starting at r3(dst) + r5(byte count) - r6 (bytes remaining).
 */


/* read fault, initial single-byte copy */
100:	li	r4,0
	mfctr	r3
101:	stbu	r4,1(r9)
	bdnz	101b
	blr

/* write fault, initial single-byte copy */
102:	li	r4,1
	mfctr	r3
	blr

/* read fault, initial vector(s) load */
103:	li	r4,0
	b	91f

/* write fault, initial partial vector store */
104:	li	r4,1
	subf	r5,r7,r5	/* BC minus bytes in 1st vector already stored */
	add	r3,r3,r7	/* dst plus bytes in 1st vector already stored. */
	b	91f

/* write fault, initial full vector store */
105:	li	r4,1
91:	mr	r6,r5
	b	98f

/* read fault in 16B loop(s) and 32B loop (treat as both loads fail)*/
106:	li	r4,0
	b	94f

/* write fault in 16B loop(s), 128B, and first write fault in 32B loop */
107:	li	r4,1
	b	94f

/* second write fault in 32B loop */
108:	li	r4,1
	add	r11,r3,r5	/* Last dst byte + 1 */
	add	r3,r3,r9	/* Current dst byte */
	b	95f

/* read fault in 128B loop (treat as all loads fail)*/
112:	li	r4,0
	mfctr	r0
	slwi	r0,r0,7		/* Convert 128B loop ctr to bytes */
	add	r11,r3,r5
	slwi	r10,r10,4	/* convert QW vectors remaining to bytes */
	add	r3,r3,r7
	rlwinm	r6,r11,0,28,31	/* Bytes in last vector(s) */
	rlwinm  r3,r3,0,0,27
	add	r6,r6,r10
	add	r6,r6,r0
	b	98f

/* read fault, final vector(s) load */
114:	li	r4,0
94:	add	r11,r3,r5
	add	r3,r3,r7
95:	rlwinm  r3,r3,0,0,27
	subf	r6,r3,r11
	b	98f

/* write fault, final partial vector store */
115:	li	r4,1
	add	r11,r3,r5	
	add	r3,r3,r7
	rlwinm  r3,r3,0,0,27	
	subf	r6,r3,r11	
	subf	r6,r9,r6	/* minus bytes already stored */
	b	98f

/* write fault, final full vector store */
116:	li	r4,1
	add	r3,r3,r7
	rlwinm  r3,r3,0,0,27
	li	r6,16
	b	98f

/*
 * At this stage the number of bytes not copied is in r6
 * and r4 is 0 for read or 1 for write.
 * (Like the scalar version, assume dst is word-aligned.)
 */
98:	cmpwi	0,r4,0
	bne	120f
/* for read fault, clear out the destination: r6 bytes remaining 
 */
	srwi.	r0,r6,2
	addi	r3,r3,-4
	subf	r10,r6,r5
	mtctr	r0
	beq	118f
117:	stwu	r4,4(r3)
	bdnz	117b
118:	andi.	r0,r6,3
	mtctr	r0
	beq	120f
119:	stb	r4,4(r3)
	addi	r3,r3,1
	bdnz	119b
120:	mr	r3,r6
	blr

121:	li	r4,1
	mfctr	r3
	rlwinm	r3,r3,2,0,29
	andi.	r0,r6,3
	add	r3,r3,r0
	blr


#ifndef TEST_OUTSIDE_LINUX
	.section __ex_table,"a"	
	.align	2		
	.long	2b,100b
	.long	202b,102b
	.long	241b,103b
	.long	25b,103b
	.long	252b,104b
	.long	262b,104b
	.long	272b,104b
	.long	282b,104b
	.long	284b,104b
	.long	29b,105b
	.long	31b,106b
	.long	312b,107b
	.long	331b,114b
	.long	332b,115b
	.long	334b,115b
	.long	342b,115b
	.long	352b,115b
	.long	362b,115b
	.long	37b,116b
	.long	38b,106b
	.long	382b,107b
	.long	40b,106b
	.long	401b,106b
	.long	402b,107b
	.long	41b,108b
	.long	42b,106b
	.long	422b,107b
	.long	44b,112b
	.long	443b,112b
	.long	447b,112b
	.long	451b,112b
	.long	441b,112b
	.long	445b,112b
	.long	449b,112b
	.long	453b,112b
	.long	440b,107b
	.long	442b,107b
	.long	444b,107b
	.long	446b,107b
	.long	448b,107b
	.long	450b,107b
	.long	452b,107b
	.long	454b,107b
	.long	101b,102b
	.long	117b,121b
	.long	119b,102b
#endif

	.text
	.align 5

	.global strlen_vec
strlen_vec:

	lvxl	v2,0,r3
	vxor	v0,v0,v0
	lvsl	v5,0,r3
	vnor	v1,v0,v0
	rlwinm	r5,r3,0,28,31
	vperm	v2,v2,v1,v5
	mr	r4,r3
	li	r3,16
	vcmpequb.	v4,v0,v2
	vsldoi	v5,v0,v1,8
	bne	cr6,2f
	subf	r3,r5,r3
1:	lvxl	v2,r4,r3
	addi	r3,r3,16
	vcmpequb.	v4,v0,v2
	beq	cr6,1b
2:	vandc	v3,v2,v5
	vsldoi	v7,v0,v1,4
	vcmpequb.	v4,v3,v5
	vsldoi	v8,v0,v1,12
	beq	cr6,10f
	vandc	v3,v2,v8
	vsldoi	v5,v0,v1,10
	vcmpequb.	v4,v3,v8
	vsldoi	v9,v0,v1,14
	beq	cr6,6f
	vandc	v3,v2,v9
	vsldoi	v8,v0,v1,13
	vcmpequb.	v4,v3,v9
	vsldoi	v10,v0,v1,15
	beq	cr6,4f
	vandc	v3,v2,v10
	vcmpequb.	v4,v3,v10
	beq	cr6,3f
	addi	r3,r3,-16
	blr
3:	addi	r3,r3,-15
	blr

4:	vandc	v3,v2,v8
	vcmpequb. v4,v3,v8
	beq	cr6,5f
	addi	r3,r3,-14
	blr
5:	addi	r3,r3,-13
	blr

6:	vandc	v3,v2,v5
	vsldoi	v9,v0,v1,9
	vcmpequb.	v4,v3,v5
	vsldoi	v10,v0,v1,11
	beq	cr6,8f
	vandc	v3,v2,v10
	vcmpequb.	v4,v3,v10
	beq	cr6,7f
	addi	r3,r3,-12
	blr
7:	addi	r3,r3,-11
	blr

8:	vandc	v3,v2,v9
	vcmpequb.	v4,v3,v9
	beq	cr6,9f
	addi	r3,r3,-10
	blr
9:	addi	r3,r3,-9
	blr

10:	vandc	v3,v2,v7
	vsldoi	v5,v0,v1,2
	vcmpequb.	v4,v3,v7
	vsldoi	v10,v0,v1,6
	beq	cr6,14f
	vandc	v3,v2,v10
	vsldoi	v9,v0,v1,5
	vcmpequb.	v4,v3,v10
	vsldoi	v7,v0,v1,7
	beq	cr6,12f
	vandc	v3,v2,v7
	vcmpequb.	v4,v3,v7
	beq	cr6,11f
	addi	r3,r3,-8
	blr
11:	addi	r3,r3,-7
	blr

12:	vandc	v3,v2,v9
	vcmpequb.	v4,v3,v9
	beq	cr6,13f
	addi	r3,r3,-6
	blr
13:	addi	r3,r3,-5
	blr

14:	vandc	v3,v2,v5
	vsldoi	v8,v0,v1,1
	vcmpequb.	v4,v3,v5
	vsldoi	v10,v0,v1,3
	beq	cr6,16f
	vandc	v3,v2,v10
	vcmpequb.	v4,v3,v10
	beq	cr6,15f
	addi	r3,r3,-4
	blr
15:	addi	r3,r3,-3
	blr

16:	vandc	v3,v2,v8
	vcmpequb.	v4,v3,v8
	beq	cr6,17f
	addi	r3,r3,-2
	blr
17:	addi	r3,r3,-1
	blr

	.text
	.align 5

	.global strcmp_vec
strcmp_vec:
	lvxl	v2,0,r3
	vxor	v0,v0,v0
	addi	r7,r4,16
	lvxl	v3,0,r4
	vnor	v1,v0,v0
	xor	r8,r7,r4
	lvsl	v6,0,r3
	vspltisb	v4,8
	cmpi	2,0,r8,0x1000
	lvsl	v10,0,r4
	vspltisb	v12,1
	beq	2,8f
1:	andi.	r8,r3,0xF
	lvxl	v8,0,r7
	vslb	v13,v4,v12
	andi.	r9,r4,0xF
	vperm	v2,v2,v1,v6
	subf.	r0,r8,r9
	addi	r5,r3,16
	vperm	v9,v0,v1,v6
	lvsl 	v6,0,r0
	vor	v7,v3,v3
	vperm	v3,v3,v8,v10
	addi	r4,r7,16
	vslb	v11,v13,v12
	vor	v3,v3,v9
	xor	r3,r3,r3
	vcmpequb.	v10,v2,v3
	vslb	v14,v11,v12
	vnor	v9,v10,v10
	bc	4,6*4+0,3f
	vcmpequb.	v5,v0,v2
	bc	4,6*4+2,7f
	blt	6f
2:	lvxl	v7,0,r4
	addi	r4,r4,16
	lvxl	v2,0,r5
	addi	r5,r5,16
	vperm	v3,v8,v7,v6
	vcmpequb.	v10,v2,v3
	vnor	v9,v10,v10
	bc	12,6*4+0,5f
3:	vcmpequb	v5,v0,v2
	vsum4ubs	v7,v4,v14
	vor	v9,v9,v5
	vsro	v12,v9,v11
	vsrw	v11,v9,v4
	vsro	v6,v9,v14
	vsrw	v14,v9,v13
	vsro	v13,v9,v7
	vor	v9,v12,v6
	vsro	v7,v14,v4
	vor	v9,v9,v13
	vcmpgtuw	v9,v9,v0
	vor	v9,v9,v11
	vor	v9,v9,v14
	vor	v9,v9,v7
	vandc	v11,v10,v9
	vcmpequb.	v14,v11,v9
	vcmpgtub	v7,v3,v2
	bc	12,6*4+2,4f
	vandc	v11,v7,v9
	li	r3,-1
	vcmpequb.	v14,v11,v1
	bc	4,6*4+2,4f
	li	r3,1
4:	blr

5:	vcmpequb.	v5,v0,v2
	bc	4,6*4+2,7f
	lvxl	v8,0,r4
	addi	r4,r4,16
6:	lvxl	v2,0,r5
	addi	r5,r5,16
	vperm	v3,v7,v8,v6
	vcmpequb.	v10,v2,v3
	vnor	v9,v10,v10
	bc	4,6*4+0,3b
	vcmpequb.	v5,v0,v2
	bc	12,6*4+2,2b
7:	blr

8:	vcmpequb.	v5,v0,v2
	bc	13,6*4+2,1b
	vcmpequb.	v10,v2,v3
	bc	4,6*4+0,3b
	blr


             .text            
             .align           5
             .global          memcmp_vec
memcmp_vec:                    
             subf.            r6,r4,r3
             cmpi             cr1,0,r5,0
             cmpi             cr7,0,r5,16
             add              r9,r3,r5
             addi             r7,r4,-1
             addi             r11,r3,16
             beq              2f
             addi             r10,r9,-1
             addi             r8,r3,-1
             rlwinm           r11,r11,0,0,27
             beq              cr1,2f
             subf             r11,r11,r10
             rlwinm           r9,r9,0,28,31
             bgt              cr7,3f
             mtctr            r5
1:           lbzu             r6,1(r7)
             lbzu             r10,1(r8)
             subf.            r3,r6,r10
             bdnzt            2,1b
             blr              

2:           xor              r3,r3,r3
             blr              
3:           rlwinm           r11,r11,28,4,31
             rlwinm           r7,r4,0,28,31
             rlwinm           r8,r3,0,28,31
             cmpi             cr1,0,r11,0
             lvxl             v0,0,r3
             subf.            r7,r7,r8
             li               r7,16
             lvxl             v1,0,r4
             vor              v2,v1,v1
             addi             r5,r5,-1
             bge              4f
             lvxl             v2,r4,r7
             addi             r4,r4,16
             addi             r5,r5,-16
4:           lvsl             v3,0,r3
             vspltisb         v4,8
             vxor             v5,v5,v5
             lvsl             v6,0,r4
             vspltisb         v7,1
             vnor             v8,v5,v5
             lvsr             v10,0,r6
             cmpi             cr5,0,r9,0
             vperm            v11,v5,v8,v3
             lvsr             v12,0,r9
             vperm            v0,v0,v8,v3
             vperm            v1,v1,v2,v6
             vslb             v3,v4,v7
             vor              v1,v1,v11
             vslb             v6,v3,v7
             vcmpequb.        v8,v0,v1
             vslb             v7,v6,v7
             vnor             v13,v8,v8
             bc               4,6*4+0,8f
             ble              cr1,6f
             mtctr            r11
5:           lvxl             v9,r4,r7
             lvxl             v0,r3,r7
             addi             r7,r7,16
             vperm            v1,v2,v9,v10
             vor              v2,v9,v9
             vcmpequb.        v8,v0,v1
             vnor             v13,v8,v8
             bdnzt            24,5b
             bc               4,6*4+0,8f
6:           lvxl             v9,r4,r5
             vperm            v12,v5,v8,v12
             lvxl             v0,r3,r7
             vperm            v1,v2,v9,v10
             beq              cr5,7f
             vor              v1,v1,v12
             vor              v0,v0,v12
7:           vcmpequb.        v8,v0,v1
             vnor             v13,v8,v8
             bc               4,6*4+0,8f
             xor              r3,r3,r3
             blr              
8:           vsum4ubs         v2,v4,v7
             vsro             v9,v13,v6
             vsrw             v6,v13,v4
             vsro             v10,v13,v7
             vsrw             v7,v13,v3
             vsro             v3,v13,v2
             vor              v11,v9,v10
             vsro             v2,v7,v4
             vor              v11,v11,v3
             vcmpgtuw         v11,v11,v5
             vor              v11,v11,v6
             vor              v11,v11,v7
             vor              v11,v11,v2
             vor              v1,v1,v11
             vor              v0,v0,v11
             li               r3,-1
             vcmpgtub.        v8,v1,v0
             bclr             4,6*4+2
	   li               r3,1
             blr              

             .text            
             .align           5
             .global          strcpy_vec
strcpy_vec:                    
             addi             r5,r3,32
             subf.            r6,r4,r3
             subf             r7,r3,r4
             rlwinm           r5,r5,0,0,26
             mr               r8,r3
             beqlr            
             bgt              1f
             mr               r6,r7
1:           subf.            r9,r3,r5
             addi             r5,r8,4096
             cmpi             cr7,0,r6,16
             mtctr            r9
2:           lbzx             r0,0,r4
             addi             r4,r4,1
             cmpi             cr1,0,r0,0
             stbx             r0,0,r8
             addi             r8,r8,1
             bdnzf            6,2b
             beqlr            cr1
             li               r11,4096
             rlwinm           r5,r5,0,0,19
             mr               r10,r4
             ble              cr7,2b                      
             subf.            r5,r8,r5
             rlwinm           r5,r5,28,4,31
             lvsl             v4,0,r4
             vxor             v0,v0,v0
             ble              9f
             mtctr            r5
3:           lvx              v1,0,r10
             addi             r10,r10,16
             bdz              10f
4:           lvx              v2,0,r10
             addi             r10,r10,16
             bdz              11f
5:           lvx              v3,0,r10
             addi             r10,r10,16
             bdz              12f
6:           vperm            v5,v1,v2,v4
             vperm            v6,v2,v3,v4
             vor              v1,v3,v3
             vcmpequb.        v7,v0,v5
             bne              cr6,8f
             addi             r4,r4,16
             vcmpequb.        v7,v0,v6
             bne              cr6,7f
             DCBA_R0R8
             addi             r4,r4,16
             stvx             v5,0,r8
             addi             r8,r8,16
             stvx             v6,0,r8
             addi             r8,r8,16
             b                4b
7:           stvx             v5,0,r8
             addi             r8,r8,16
8:           lbzx             r0,0,r4
             addi             r4,r4,1
             cmpi             cr1,0,r0,0
             stbx             r0,0,r8
             addi             r8,r8,1
             bne              cr1,8b
             blr              

9:           mtctr            r11
             b                3b
10:          vcmpequb.        v7,v0,v1
             bnl              cr6,8b
             mtctr            r11
             b                4b
11:          vcmpequb.        v7,v0,v2
             bnl              cr6,8b
             mtctr            r11
             b                5b
12:          vcmpequb.        v7,v0,v3
             bnl              cr6,8b
             mtctr            r11
             b                6b
