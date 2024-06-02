	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 14, 0
	.globl	_isOdd                          ; -- Begin function isOdd
	.p2align	2
_isOdd:                                 ; @isOdd
	.cfi_startproc
; %bb.0:                                ; %common.ret
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	mov	w8, w0
	and	w9, w0, #0x1
	cmp	w0, #0
	cneg	w9, w9, lt
	cmp	w9, #1
	str	w8, [sp, #12]
	cset	w0, eq
	add	sp, sp, #16
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_isprime                        ; -- Begin function isprime
	.p2align	2
_isprime:                               ; @isprime
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #48
	stp	x20, x19, [sp, #16]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 48
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	adrp	x8, _ret@PAGE
	mov	w10, #1                         ; =0x1
	mov	w11, #2                         ; =0x2
	adrp	x9, _arg@PAGE
	str	w10, [x8, _ret@PAGEOFF]
	str	w11, [sp, #12]
LBB1_1:                                 ; =>This Inner Loop Header: Depth=1
	ldr	w10, [sp, #12]
	ldr	w11, [x9, _arg@PAGEOFF]
	cmp	w10, w11
	b.ge	LBB1_5
; %bb.2:                                ;   in Loop: Header=BB1_1 Depth=1
	ldr	w10, [x9, _arg@PAGEOFF]
	ldr	w11, [sp, #12]
	sdiv	w12, w10, w11
	msub	w10, w12, w11, w10
	cbnz	w10, LBB1_4
; %bb.3:                                ;   in Loop: Header=BB1_1 Depth=1
	ldr	w10, [x9, _arg@PAGEOFF]
	str	wzr, [x8, _ret@PAGEOFF]
	str	w10, [sp, #12]
	b	LBB1_1
LBB1_4:
	ldr	w8, [sp, #12]
	add	w8, w8, #1
	str	w8, [sp, #12]
LBB1_5:
	mov	w8, #5                          ; =0x5
Lloh0:
	adrp	x19, _writeln.str@PAGE
Lloh1:
	add	x19, x19, _writeln.str@PAGEOFF
	str	w8, [sp, #8]
LBB1_6:                                 ; =>This Inner Loop Header: Depth=1
	ldr	w8, [sp, #8]
	cmp	w8, #9
	b.gt	LBB1_8
; %bb.7:                                ;   in Loop: Header=BB1_6 Depth=1
	ldr	w8, [sp, #8]
	mov	x0, x19
	str	x8, [sp]
	bl	_printf
	ldr	w8, [sp, #8]
	add	w8, w8, #1
	str	w8, [sp, #8]
	b	LBB1_6
LBB1_8:
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #48
	ret
	.loh AdrpAdd	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.globl	_primes                         ; -- Begin function primes
	.p2align	2
_primes:                                ; @primes
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #96
	stp	x24, x23, [sp, #32]             ; 16-byte Folded Spill
	stp	x22, x21, [sp, #48]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #64]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #80]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 96
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	mov	x8, #5                          ; =0x5
Lloh2:
	adrp	x19, _writeln.str@PAGE
Lloh3:
	add	x19, x19, _writeln.str@PAGEOFF
	movk	x8, #2, lsl #32
	mov	w9, #5                          ; =0x5
	mov	x0, x19
	str	x8, [sp, #16]
	str	w9, [sp, #28]
	str	x9, [sp]
	bl	_printf
Lloh4:
	adrp	x8, _cu@PAGE
	adrp	x20, _arg@PAGE
	mov	w9, #2                          ; =0x2
	adrp	x22, _ret@PAGE
	adrp	x23, _emre@PAGE+4
Lloh5:
	ldr	w21, [x8, _cu@PAGEOFF]
	str	w9, [x20, _arg@PAGEOFF]
	b	LBB2_2
LBB2_1:                                 ;   in Loop: Header=BB2_2 Depth=1
	ldr	w8, [x20, _arg@PAGEOFF]
	add	w0, w8, #1
	str	w0, [x20, _arg@PAGEOFF]
	bl	_isOdd
	str	w0, [x23, _emre@PAGEOFF+4]
LBB2_2:                                 ; =>This Inner Loop Header: Depth=1
	ldr	w8, [x20, _arg@PAGEOFF]
	cmp	w8, w21
	b.ge	LBB2_5
; %bb.3:                                ;   in Loop: Header=BB2_2 Depth=1
	bl	_isprime
	ldr	w8, [x22, _ret@PAGEOFF]
	cmp	w8, #1
	b.ne	LBB2_1
; %bb.4:                                ;   in Loop: Header=BB2_2 Depth=1
	ldr	w8, [x20, _arg@PAGEOFF]
	mov	x0, x19
	str	x8, [sp]
	bl	_printf
	b	LBB2_1
LBB2_5:
	ldp	x29, x30, [sp, #80]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #64]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #48]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #32]             ; 16-byte Folded Reload
	add	sp, sp, #96
	ret
	.loh AdrpLdr	Lloh4, Lloh5
	.loh AdrpAdd	Lloh2, Lloh3
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	.cfi_def_cfa_offset 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	bl	_primes
	mov	w0, wzr
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__const
	.globl	_readwrite.str                  ; @readwrite.str
_readwrite.str:
	.asciz	"%d"

	.globl	_writeln.str                    ; @writeln.str
_writeln.str:
	.asciz	"%d\n"

	.globl	___literal1                     ; @__literal1
	.p2align	2, 0x0
___literal1:
	.long	5                               ; 0x5
	.long	2                               ; 0x2

	.globl	_max                            ; @max
	.p2align	2, 0x0
_max:
	.long	200                             ; 0xc8
	.long	3300                            ; 0xce4

	.globl	_cu                             ; @cu
	.p2align	2, 0x0
_cu:
	.long	3                               ; 0x3

	.globl	_arg                            ; @arg
.zerofill __DATA,__common,_arg,4,2
	.globl	_ret                            ; @ret
.zerofill __DATA,__common,_ret,4,2
	.globl	_emre                           ; @emre
.zerofill __DATA,__common,_emre,12,2
.subsections_via_symbols
