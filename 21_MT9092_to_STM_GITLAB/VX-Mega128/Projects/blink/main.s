	.file	"main.cpp"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
 ;  GNU C++ (GCC) version 5.4.0 (avr)
 ; 	compiled by GNU C version 9.3.0, GMP version 6.2.0, MPFR version 4.0.2, MPC version 1.1.0
 ;  GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
 ;  переданы ключи:  main.cpp -mn-flash=1
 ;  -auxbase-strip main.s -g -O2 -fverbose-asm -mn-flash=1 -mskip-bug
 ;  -fno-rtti -fno-enforce-eh-specs -fno-exceptions
 ;  активные ключи:  -Wmisspelled-isr
 ;  -faggressive-loop-optimizations -falign-functions -falign-jumps
 ;  -falign-labels -falign-loops -fauto-inc-dec -fbranch-count-reg
 ;  -fchkp-check-incomplete-type -fchkp-check-read -fchkp-check-write
 ;  -fchkp-instrument-calls -fchkp-narrow-bounds -fchkp-optimize
 ;  -fchkp-store-bounds -fchkp-use-static-bounds
 ;  -fchkp-use-static-const-bounds -fchkp-use-wrappers
 ;  -fcombine-stack-adjustments -fcommon -fcompare-elim -fcprop-registers
 ;  -fcrossjumping -fcse-follow-jumps -fdefer-pop -fdevirtualize
 ;  -fdevirtualize-speculatively -fdwarf2-cfi-asm -fearly-inlining
 ;  -feliminate-unused-debug-types -fexpensive-optimizations
 ;  -fforward-propagate -ffunction-cse -fgcse -fgcse-lm -fgnu-runtime
 ;  -fgnu-unique -fguess-branch-probability -fhoist-adjacent-loads -fident
 ;  -fif-conversion -fif-conversion2 -findirect-inlining -finline
 ;  -finline-atomics -finline-functions-called-once
 ;  -finline-small-functions -fipa-cp -fipa-cp-alignment -fipa-icf
 ;  -fipa-icf-functions -fipa-icf-variables -fipa-profile -fipa-pure-const
 ;  -fipa-ra -fipa-reference -fipa-sra -fira-hoist-pressure
 ;  -fira-share-save-slots -fira-share-spill-slots
 ;  -fisolate-erroneous-paths-dereference -fivopts -fkeep-static-consts
 ;  -fleading-underscore -flifetime-dse -flra-remat -flto-odr-type-merging
 ;  -fmath-errno -fmerge-constants -fmerge-debug-strings
 ;  -fmove-loop-invariants -fomit-frame-pointer -foptimize-sibling-calls
 ;  -foptimize-strlen -fpartial-inlining -fpeephole -fpeephole2
 ;  -fprefetch-loop-arrays -freg-struct-return -freorder-blocks
 ;  -freorder-functions -frerun-cse-after-loop
 ;  -fsched-critical-path-heuristic -fsched-dep-count-heuristic
 ;  -fsched-group-heuristic -fsched-interblock -fsched-last-insn-heuristic
 ;  -fsched-rank-heuristic -fsched-spec -fsched-spec-insn-heuristic
 ;  -fsched-stalled-insns-dep -fschedule-fusion -fsemantic-interposition
 ;  -fshow-column -fshrink-wrap -fsigned-zeros -fsplit-ivs-in-unroller
 ;  -fsplit-wide-types -fssa-phiopt -fstdarg-opt -fstrict-aliasing
 ;  -fstrict-overflow -fstrict-volatile-bitfields -fsync-libcalls
 ;  -fthread-jumps -ftoplevel-reorder -ftrapping-math -ftree-bit-ccp
 ;  -ftree-builtin-call-dce -ftree-ccp -ftree-ch -ftree-coalesce-vars
 ;  -ftree-copy-prop -ftree-copyrename -ftree-dce -ftree-dominator-opts
 ;  -ftree-dse -ftree-forwprop -ftree-fre -ftree-loop-if-convert
 ;  -ftree-loop-im -ftree-loop-ivcanon -ftree-loop-optimize
 ;  -ftree-parallelize-loops= -ftree-phiprop -ftree-pre -ftree-pta
 ;  -ftree-reassoc -ftree-scev-cprop -ftree-sink -ftree-slsr -ftree-sra
 ;  -ftree-switch-conversion -ftree-tail-merge -ftree-ter -ftree-vrp
 ;  -funit-at-a-time -fverbose-asm -fzero-initialized-in-bss -mskip-bug

	.stabs	"/home/evg/SOFT/Github/GIT/21_MT9092_to_STM_GITLAB/VX-Mega128/Projects/blink/",100,0,4,.Ltext0
	.stabs	"main.cpp",100,0,4,.Ltext0
	.text
.Ltext0:
	.stabs	"gcc2_compiled.",60,0,0,0
	.stabs	"__uint24:t(0,1)=@s24;r(0,1);0;077777777;",128,0,0,0
	.stabs	"__int24:t(0,2)=@s24;r(0,2);040000000;037777777;",128,0,0,0
	.stabs	"__builtin_va_list:t(0,3)=*(0,4)=(0,4)",128,0,0,0
	.stabs	"complex long double:t(0,5)=R3;8;0;",128,0,0,0
	.stabs	"complex double:t(0,6)=R3;8;0;",128,0,0,0
	.stabs	"complex float:t(0,7)=R3;8;0;",128,0,0,0
	.stabs	"complex int:t(0,8)=s4real:(0,9)=r(0,9);-32768;32767;,0,16;imag:(0,9),16,16;;",128,0,0,0
	.stabs	"long long unsigned int:t(0,10)=@s64;r(0,10);0;01777777777777777777777;",128,0,0,0
	.stabs	"long unsigned int:t(0,11)=@s32;r(0,11);0;037777777777;",128,0,0,0
	.stabs	"unsigned int:t(0,12)=r(0,12);0;0177777;",128,0,0,0
	.stabs	"unsigned char:t(0,13)=@s8;r(0,13);0;255;",128,0,0,0
	.stabs	"long long int:t(0,14)=@s64;r(0,14);01000000000000000000000;00777777777777777777777;",128,0,0,0
	.stabs	"long int:t(0,15)=@s32;r(0,15);020000000000;017777777777;",128,0,0,0
	.stabs	"int:t(0,9)",128,0,0,0
	.stabs	"signed char:t(0,16)=@s8;r(0,16);-128;127;",128,0,0,0
	.stabs	"char:t(0,17)=r(0,17);0;127;",128,0,0,0
	.stabs	"signed:t(0,9)",128,0,0,0
	.stabs	"__int128:t(0,18)=@s128;r(0,18);02000000000000000000000000000000000000000000;01777777777777777777777777777777777777777777;",128,0,0,0
	.stabs	"__int128 unsigned:t(0,19)=@s128;r(0,19);0;03777777777777777777777777777777777777777777;",128,0,0,0
	.stabs	"unsigned long:t(0,11)",128,0,0,0
	.stabs	"long long unsigned:t(0,10)",128,0,0,0
	.stabs	"short int:t(0,20)=r(0,20);-32768;32767;",128,0,0,0
	.stabs	"short unsigned int:t(0,21)=r(0,21);0;0177777;",128,0,0,0
	.stabs	"unsigned short:t(0,21)",128,0,0,0
	.stabs	"float:t(0,22)=r(0,9);4;0;",128,0,0,0
	.stabs	"double:t(0,23)=r(0,9);4;0;",128,0,0,0
	.stabs	"long double:t(0,24)=r(0,9);4;0;",128,0,0,0
	.stabs	"short _Fract:t(0,25)=r(0,9);1;0;",128,0,0,0
	.stabs	"long _Fract:t(0,26)=r(0,9);4;0;",128,0,0,0
	.stabs	"long long _Fract:t(0,27)=r(0,9);8;0;",128,0,0,0
	.stabs	"unsigned short _Fract:t(0,28)=r(0,9);1;0;",128,0,0,0
	.stabs	"unsigned _Fract:t(0,29)=r(0,9);2;0;",128,0,0,0
	.stabs	"unsigned long _Fract:t(0,30)=r(0,9);4;0;",128,0,0,0
	.stabs	"unsigned long long _Fract:t(0,31)=r(0,9);8;0;",128,0,0,0
	.stabs	"_Sat short _Fract:t(0,32)=r(0,9);1;0;",128,0,0,0
	.stabs	"_Sat _Fract:t(0,33)=r(0,9);2;0;",128,0,0,0
	.stabs	"_Sat long _Fract:t(0,34)=r(0,9);4;0;",128,0,0,0
	.stabs	"_Sat long long _Fract:t(0,35)=r(0,9);8;0;",128,0,0,0
	.stabs	"_Sat unsigned short _Fract:t(0,36)=r(0,9);1;0;",128,0,0,0
	.stabs	"_Sat unsigned _Fract:t(0,37)=r(0,9);2;0;",128,0,0,0
	.stabs	"_Sat unsigned long _Fract:t(0,38)=r(0,9);4;0;",128,0,0,0
	.stabs	"_Sat unsigned long long _Fract:t(0,39)=r(0,9);8;0;",128,0,0,0
	.stabs	"short _Accum:t(0,40)=r(0,9);2;0;",128,0,0,0
	.stabs	"long _Accum:t(0,41)=r(0,9);8;0;",128,0,0,0
	.stabs	"long long _Accum:t(0,42)=r(0,9);8;0;",128,0,0,0
	.stabs	"unsigned short _Accum:t(0,43)=r(0,9);2;0;",128,0,0,0
	.stabs	"unsigned _Accum:t(0,44)=r(0,9);4;0;",128,0,0,0
	.stabs	"unsigned long _Accum:t(0,45)=r(0,9);8;0;",128,0,0,0
	.stabs	"unsigned long long _Accum:t(0,46)=r(0,9);8;0;",128,0,0,0
	.stabs	"_Sat short _Accum:t(0,47)=r(0,9);2;0;",128,0,0,0
	.stabs	"_Sat _Accum:t(0,48)=r(0,9);4;0;",128,0,0,0
	.stabs	"_Sat long _Accum:t(0,49)=r(0,9);8;0;",128,0,0,0
	.stabs	"_Sat long long _Accum:t(0,50)=r(0,9);8;0;",128,0,0,0
	.stabs	"_Sat unsigned short _Accum:t(0,51)=r(0,9);2;0;",128,0,0,0
	.stabs	"_Sat unsigned _Accum:t(0,52)=r(0,9);4;0;",128,0,0,0
	.stabs	"_Sat unsigned long _Accum:t(0,53)=r(0,9);8;0;",128,0,0,0
	.stabs	"_Sat unsigned long long _Accum:t(0,54)=r(0,9);8;0;",128,0,0,0
	.stabs	"void:t(0,4)",128,0,0,0
	.stabs	"wchar_t:t(0,55)=r(0,55);-32768;32767;",128,0,0,0
	.stabs	"bool:t(0,56)=@s8;-16;",128,0,0,0
	.stabs	"__vtbl_ptr_type:t(0,57)=*(0,58)=f(0,9)",128,0,0,0
	.stabs	"decltype(nullptr):t(0,59)=(0,59)",128,0,0,0
	.stabs	"int8_t:t(0,60)=(0,16)",128,0,125,0
	.stabs	"uint8_t:t(0,61)=(0,13)",128,0,126,0
	.stabs	"int16_t:t(0,62)=(0,9)",128,0,127,0
	.stabs	"uint16_t:t(0,63)=(0,12)",128,0,128,0
	.stabs	"int32_t:t(0,64)=(0,15)",128,0,129,0
	.stabs	"uint32_t:t(0,65)=(0,11)",128,0,130,0
	.stabs	"int64_t:t(0,66)=(0,14)",128,0,132,0
	.stabs	"uint64_t:t(0,67)=(0,10)",128,0,133,0
	.stabs	"intptr_t:t(0,68)=(0,62)",128,0,146,0
	.stabs	"uintptr_t:t(0,69)=(0,63)",128,0,151,0
	.stabs	"int_least8_t:t(0,70)=(0,60)",128,0,163,0
	.stabs	"uint_least8_t:t(0,71)=(0,61)",128,0,168,0
	.stabs	"int_least16_t:t(0,72)=(0,62)",128,0,173,0
	.stabs	"uint_least16_t:t(0,73)=(0,63)",128,0,178,0
	.stabs	"int_least32_t:t(0,74)=(0,64)",128,0,183,0
	.stabs	"uint_least32_t:t(0,75)=(0,65)",128,0,188,0
	.stabs	"int_least64_t:t(0,76)=(0,66)",128,0,196,0
	.stabs	"uint_least64_t:t(0,77)=(0,67)",128,0,203,0
	.stabs	"int_fast8_t:t(0,78)=(0,60)",128,0,217,0
	.stabs	"uint_fast8_t:t(0,79)=(0,61)",128,0,222,0
	.stabs	"int_fast16_t:t(0,80)=(0,62)",128,0,227,0
	.stabs	"uint_fast16_t:t(0,81)=(0,63)",128,0,232,0
	.stabs	"int_fast32_t:t(0,82)=(0,64)",128,0,237,0
	.stabs	"uint_fast32_t:t(0,83)=(0,65)",128,0,242,0
	.stabs	"int_fast64_t:t(0,84)=(0,66)",128,0,250,0
	.stabs	"uint_fast64_t:t(0,85)=(0,67)",128,0,257,0
	.stabs	"intmax_t:t(0,86)=(0,66)",128,0,277,0
	.stabs	"uintmax_t:t(0,87)=(0,67)",128,0,282,0
	.stabs	"int_farptr_t:t(0,88)=(0,64)",128,0,77,0
	.stabs	"uint_farptr_t:t(0,89)=(0,65)",128,0,81,0
	.section	.text.startup,"ax",@progbits
	.stabs	"main:F(0,9)",36,0,5,main
.global	main
	.type	main, @function
main:
	.stabd	46,0,0
	.stabn	68,0,6,.LM0-.LFBB1
.LM0:
.LFBB1:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.stabn	68,0,7,.LM1-.LFBB1
.LM1:
	sbi 0x17,0	 ; ,
	.stabn	68,0,10,.LM2-.LFBB1
.LM2:
	sbi 0x17,1	 ; ,
	.stabn	68,0,15,.LM3-.LFBB1
.LM3:
	ldi r24,lo8(3)	 ;  tmp54,
.L2:
	.stabn	68,0,15,.LM4-.LFBB1
.LM4:
	out 0x18,r24	 ;  MEM[(volatile uint8_t *)56B], tmp54
.LBB6:
.LBB7:
	.stabs	"/usr/lib/avr/include/util/delay.h",132,0,0,.Ltext1
.Ltext1:
	.stabn	68,0,187,.LM5-.LFBB1
.LM5:
	ldi r30,lo8(-15537)	 ; ,
	ldi r31,hi8(-15537)	 ; ,
1:	sbiw r30,1	 ; 
	brne 1b
	rjmp .
	nop
.LBE7:
.LBE6:
	.stabs	"main.cpp",132,0,0,.Ltext2
.Ltext2:
	.stabn	68,0,17,.LM6-.LFBB1
.LM6:
	out 0x18,__zero_reg__	 ;  MEM[(volatile uint8_t *)56B],
.LBB8:
.LBB9:
	.stabs	"/usr/lib/avr/include/util/delay.h",132,0,0,.Ltext3
.Ltext3:
	.stabn	68,0,187,.LM7-.LFBB1
.LM7:
	ldi r31,lo8(199999)	 ; ,
	ldi r18,hi8(199999)	 ; ,
	ldi r19,hlo8(199999)	 ; ,
1:	subi r31,1	 ; 
	sbci r18,0	 ; 
	sbci r19,0	 ; 
	brne 1b
	rjmp .
	nop
	rjmp .L2	 ; 
.LBE9:
.LBE8:
	.size	main, .-main
.Lscope1:
	.stabs	"",36,0,0,.Lscope1-.LFBB1
	.stabd	78,0,0
	.text
	.stabs	"",100,0,0,.Letext0
.Letext0:
	.ident	"GCC: (GNU) 5.4.0"
