; ModuleID = 'src/Builtins.mvn'
source_filename = "src/Builtins.mvn"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-darwin24.3.0"

%none_struct = type opaque

@none = internal global %none_struct zeroinitializer
@noneload = internal constant ptr @none
@tmp4 = private unnamed_addr constant [15 x i8] c"example string\00", align 1
@tmp7 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@tmp11 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@tmp15 = private unnamed_addr constant [16 x i8] c"Panic: %s (%d)\0A\00", align 1
@tmp22 = private unnamed_addr constant [11 x i8] c"Abort: %s\0A\00", align 1
@tmp33 = private unnamed_addr constant [18 x i8] c"Assertion Error: \00", align 1
@tmp52 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1

define internal i32 @"5673998842831842134mainMavenStart"() {
__allocreigon5673998842831842134mainMavenStart:
  %retval5673998842831842134mainMavenStart = alloca i32, align 4
  br label %tmp2

tmp2:                                             ; preds = %__allocreigon5673998842831842134mainMavenStart
  br label %tmp3

tmp3:                                             ; preds = %tmp2
  br label %tmp5

tmp5:                                             ; preds = %tmp3
  store i32 0, ptr %retval5673998842831842134mainMavenStart, align 4
  store i32 0, ptr %retval5673998842831842134mainMavenStart, align 4
  %tmp54 = load i32, ptr %retval5673998842831842134mainMavenStart, align 4
  ret i32 %tmp54
}

define i32 @"5673998842831842134main"() {
tmp0:
  %tmp1 = call i32 @"5673998842831842134mainMavenStart"()
  ret i32 %tmp1
}

declare i32 @printf(ptr, ...)

declare i32 @exit(i32)

define i32 @"5673998842831842134print"(ptr %0) {
__allocreigon5673998842831842134print:
  %tmp6str = alloca ptr, align 8
  br label %tmp6

tmp6:                                             ; preds = %__allocreigon5673998842831842134print
  store ptr %0, ptr %tmp6str, align 8
  %tmp8 = load ptr, ptr %tmp6str, align 8
  %tmp9 = call i32 (ptr, ...) @printf(ptr @tmp7, ptr %tmp8)
  ret i32 0
}

define i32 @"5673998842831842134println"(ptr %0) {
__allocreigon5673998842831842134println:
  %tmp10str = alloca ptr, align 8
  br label %tmp10

tmp10:                                            ; preds = %__allocreigon5673998842831842134println
  store ptr %0, ptr %tmp10str, align 8
  %tmp12 = load ptr, ptr %tmp10str, align 8
  %tmp13 = call i32 (ptr, ...) @printf(ptr @tmp11, ptr %tmp12)
  ret i32 0
}

define ptr @"5673998842831842134SubPanic"(ptr %0, i64 %1) {
__allocreigon5673998842831842134SubPanic:
  %tmp14SStr = alloca ptr, align 8
  %tmp14SS = alloca i64, align 8
  %retval5673998842831842134SubPanic = alloca ptr, align 8
  br label %tmp14

tmp14:                                            ; preds = %__allocreigon5673998842831842134SubPanic
  store ptr %0, ptr %tmp14SStr, align 8
  store i64 %1, ptr %tmp14SS, align 8
  %tmp16 = load ptr, ptr %tmp14SStr, align 8
  %tmp17 = load i64, ptr %tmp14SS, align 8
  %tmp18 = call i32 (ptr, ...) @printf(ptr @tmp15, ptr %tmp16, i64 %tmp17)
  %tmp19 = load i64, ptr %tmp14SS, align 8
  call void asm sideeffect "\0A        mov x1, $0\0A        add sp, sp, x1\0A        mov x0, #1\0A        bl _exit\0A        ", "r"(i64 %tmp19)
  store ptr @noneload, ptr %retval5673998842831842134SubPanic, align 8
  %tmp20 = load ptr, ptr %retval5673998842831842134SubPanic, align 8
  ret ptr %tmp20
}

define ptr @"5673998842831842134Abort"(ptr %0) {
__allocreigon5673998842831842134Abort:
  %tmp21Str = alloca ptr, align 8
  %retval5673998842831842134Abort = alloca ptr, align 8
  br label %tmp21

tmp21:                                            ; preds = %__allocreigon5673998842831842134Abort
  store ptr %0, ptr %tmp21Str, align 8
  %tmp23 = load ptr, ptr %tmp21Str, align 8
  %tmp24 = call i32 (ptr, ...) @printf(ptr @tmp22, ptr %tmp23)
  %tmp25 = call i32 @exit(i32 1)
  store ptr @noneload, ptr %retval5673998842831842134Abort, align 8
  %tmp26 = load ptr, ptr %retval5673998842831842134Abort, align 8
  ret ptr %tmp26
}

define ptr @"5673998842831842134Assert"(i1 %0, ptr %1) {
__allocreigon5673998842831842134Assert:
  %tmp27condition = alloca i1, align 1
  %tmp27reason = alloca ptr, align 8
  %retval5673998842831842134Assert = alloca ptr, align 8
  br label %tmp27

tmp27:                                            ; preds = %__allocreigon5673998842831842134Assert
  store i1 %0, ptr %tmp27condition, align 1
  store ptr %1, ptr %tmp27reason, align 8
  %tmp31 = load i1, ptr %tmp27condition, align 1
  %tmp32 = icmp ne i1 %tmp31, true
  br i1 %tmp32, label %tmp29, label %tmp30

tmp29:                                            ; preds = %tmp27
  %tmp34 = call i32 (ptr, ...) @printf(ptr @tmp33)
  %tmp35 = load ptr, ptr %tmp27reason, align 8
  %tmp36 = call ptr @"5673998842831842134Abort"(ptr %tmp35)
  br label %tmp30

tmp30:                                            ; preds = %tmp29, %tmp27
  store ptr @noneload, ptr %retval5673998842831842134Assert, align 8
  %tmp37 = load ptr, ptr %retval5673998842831842134Assert, align 8
  ret ptr %tmp37
}

define internal ptr @anonfn0() {
__allocreigonanonfn0:
  %retvalanonfn0 = alloca ptr, align 8
  br label %tmp38

tmp38:                                            ; preds = %__allocreigonanonfn0
  store ptr @tmp4, ptr %retvalanonfn0, align 8
  %tmp39 = load ptr, ptr %retvalanonfn0, align 8
  ret ptr %tmp39
}

define ptr @"5673998842831842134AssertDefer"(i1 %0, ptr %1) {
__allocreigon5673998842831842134AssertDefer:
  %tmp40condition = alloca i1, align 1
  %tmp40cb = alloca ptr, align 8
  %retval5673998842831842134AssertDefer = alloca ptr, align 8
  br label %tmp40

tmp40:                                            ; preds = %__allocreigon5673998842831842134AssertDefer
  store i1 %0, ptr %tmp40condition, align 1
  store ptr %1, ptr %tmp40cb, align 8
  %tmp44 = load i1, ptr %tmp40condition, align 1
  %tmp45 = icmp ne i1 %tmp44, true
  br i1 %tmp45, label %tmp42, label %tmp43

tmp42:                                            ; preds = %tmp40
  %tmp46 = call i32 (ptr, ...) @printf(ptr @tmp33)
  %tmp47 = load ptr, ptr %tmp40cb, align 8
  %tmp48 = call ptr %tmp47()
  %tmp49 = call ptr @"5673998842831842134Abort"(ptr %tmp48)
  br label %tmp43

tmp43:                                            ; preds = %tmp42, %tmp40
  store ptr @noneload, ptr %retval5673998842831842134AssertDefer, align 8
  %tmp50 = load ptr, ptr %retval5673998842831842134AssertDefer, align 8
  ret ptr %tmp50
}

define ptr @"5673998842831842134Address"(ptr %0) {
__allocreigon5673998842831842134Address:
  %tmp51ptr = alloca ptr, align 8
  %retval5673998842831842134Address = alloca ptr, align 8
  br label %tmp51

tmp51:                                            ; preds = %__allocreigon5673998842831842134Address
  store ptr %0, ptr %tmp51ptr, align 8
  store ptr @tmp52, ptr %retval5673998842831842134Address, align 8
  %tmp53 = load ptr, ptr %retval5673998842831842134Address, align 8
  ret ptr %tmp53
}
