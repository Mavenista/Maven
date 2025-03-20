; ModuleID = 'src/io.mvn'
source_filename = "src/io.mvn"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-darwin24.3.0"

%none_struct = type opaque

@none = internal global %none_struct zeroinitializer
@noneload = internal constant ptr @none
@tmp3 = private unnamed_addr constant [15 x i8] c"example string\00", align 1
@tmp9 = private unnamed_addr constant [15 x i8] c"GenericPointer\00", align 1
@tmp17stralloc = internal global ptr null
@tmp24 = private unnamed_addr constant [3 x i8] c"%c\00", align 1

define internal i32 @"5968854263808077590mainMavenStart"() {
__allocreigon5968854263808077590mainMavenStart:
  %tmp6tmp7 = alloca ptr, align 8
  %tmp6tmp8 = alloca ptr, align 8
  %tmp6tmp10 = alloca ptr, align 8
  %tmp6tmp11 = alloca ptr, align 8
  %tmp6tmp12 = alloca ptr, align 8
  %tmp6tmp13 = alloca ptr, align 8
  %tmp6tmp14 = alloca ptr, align 8
  %tmp6tmp15 = alloca ptr, align 8
  %tmp6tmp16 = alloca ptr, align 8
  %retval5968854263808077590mainMavenStart = alloca i32, align 4
  br label %tmp2

tmp2:                                             ; preds = %__allocreigon5968854263808077590mainMavenStart
  br label %tmp6

tmp6:                                             ; preds = %tmp2
  store i8 0, ptr %tmp6tmp7, align 1
  store i8 0, ptr %tmp6tmp8, align 1
  store i8 65, ptr %tmp6tmp10, align 1
  store i8 65, ptr %tmp6tmp11, align 1
  store i8 65, ptr %tmp6tmp12, align 1
  store i8 65, ptr %tmp6tmp13, align 1
  store i8 65, ptr %tmp6tmp14, align 1
  store i8 65, ptr %tmp6tmp15, align 1
  store i8 0, ptr %tmp6tmp16, align 1
  br label %tmp17

tmp17:                                            ; preds = %tmp6
  store ptr @malloc, ptr @tmp17stralloc, align 8
  store i32 0, ptr %retval5968854263808077590mainMavenStart, align 4
  store i32 0, ptr %retval5968854263808077590mainMavenStart, align 4
  %tmp87 = load i32, ptr %retval5968854263808077590mainMavenStart, align 4
  ret i32 %tmp87
}

define i32 @"5968854263808077590main"() {
tmp0:
  %tmp1 = call i32 @"5968854263808077590mainMavenStart"()
  ret i32 %tmp1
}

declare ptr @"5673998842831842134Abort"(ptr)

define internal ptr @anonfn0() {
__allocreigonanonfn0:
  %retvalanonfn0 = alloca ptr, align 8
  br label %tmp4

tmp4:                                             ; preds = %__allocreigonanonfn0
  store ptr @tmp3, ptr %retvalanonfn0, align 8
  %tmp5 = load ptr, ptr %retvalanonfn0, align 8
  ret ptr %tmp5
}

declare ptr @"5673998842831842134AssertDefer"(i1, ptr)

declare i32 @"5673998842831842134println"(ptr)

declare ptr @"5673998842831842134SubPanic"(ptr, i64)

declare ptr @"5673998842831842134Assert"(i1, ptr)

declare i32 @"5673998842831842134print"(ptr)

declare ptr @"5673998842831842134Address"(ptr)

declare i32 @printf(ptr, ...)

declare i32 @sleep(i32)

declare i8 @getchar()

declare ptr @malloc(i64)

declare ptr @realloc(ptr, i64)

declare ptr @strcpy(ptr, ptr)

declare ptr @strcat(ptr, ptr)

declare i32 @free(ptr)

declare i32 @exit(i32)

declare i32 @sprintf(ptr, ptr, ...)

declare i32 @system(ptr)

define internal ptr @stringify(i8 %0) {
__allocreigonstringify:
  %tmp19ch = alloca i8, align 1
  %tmp19stralloc = alloca ptr, align 8
  %tmp19buff = alloca ptr, align 8
  %retvalstringify = alloca ptr, align 8
  br label %tmp19

tmp19:                                            ; preds = %__allocreigonstringify
  store i8 %0, ptr %tmp19ch, align 1
  store ptr @malloc, ptr %tmp19stralloc, align 8
  %tmp21 = load ptr, ptr %tmp19stralloc, align 8
  %tmp22 = call ptr %tmp21(i64 2)
  store ptr %tmp22, ptr %tmp19buff, align 8
  %tmp23 = load ptr, ptr %tmp19buff, align 8
  %tmp25 = load i8, ptr %tmp19ch, align 1
  %tmp26 = call i32 (ptr, ptr, ...) @sprintf(ptr %tmp23, ptr @tmp24, i8 %tmp25)
  %tmp27 = load ptr, ptr %tmp19buff, align 8
  store ptr %tmp27, ptr %retvalstringify, align 8
  %tmp28 = load ptr, ptr %retvalstringify, align 8
  ret ptr %tmp28
}

define internal ptr @readline() {
__allocreigonreadline:
  %tmp29ch = alloca i8, align 1
  %tmp29stralloc = alloca ptr, align 8
  %tmp29tmp34 = alloca ptr, align 8
  %tmp29buffer = alloca ptr, align 8
  %tmp29buf = alloca ptr, align 8
  %tmp29chars = alloca i64, align 8
  %tmp29strbuf = alloca ptr, align 8
  %tmp44ch = alloca i8, align 1
  %flagtmp44ch = alloca i1, align 1
  store i1 false, ptr %flagtmp44ch, align 1
  %tmp44chars = alloca i64, align 8
  %flagtmp44chars = alloca i1, align 1
  store i1 false, ptr %flagtmp44chars, align 1
  %tmp44buf = alloca ptr, align 8
  %flagtmp44buf = alloca i1, align 1
  store i1 false, ptr %flagtmp44buf, align 1
  %tmp44tmp = alloca ptr, align 8
  %flagtmp44tmp = alloca i1, align 1
  store i1 false, ptr %flagtmp44tmp, align 1
  %tmp44stralloc = alloca ptr, align 8
  %flagtmp44stralloc = alloca i1, align 1
  store i1 false, ptr %flagtmp44stralloc, align 1
  %tmp44strton = alloca ptr, align 8
  %flagtmp44strton = alloca i1, align 1
  store i1 false, ptr %flagtmp44strton, align 1
  %retvalreadline = alloca ptr, align 8
  br label %tmp29

tmp29:                                            ; preds = %__allocreigonreadline
  %tmp30 = call i8 @getchar()
  store i8 %tmp30, ptr %tmp29ch, align 1
  store ptr @malloc, ptr %tmp29stralloc, align 8
  %tmp32 = load ptr, ptr %tmp29stralloc, align 8
  %tmp33 = call ptr %tmp32(i64 2)
  store ptr %tmp33, ptr %tmp29tmp34, align 8
  store ptr %tmp29tmp34, ptr %tmp29buffer, align 8
  %tmp35 = load ptr, ptr %tmp29buffer, align 8
  store ptr %tmp35, ptr %tmp29buf, align 8
  store i64 2, ptr %tmp29chars, align 8
  %tmp36 = load i8, ptr %tmp29ch, align 1
  %tmp37 = call ptr @stringify(i8 %tmp36)
  store ptr %tmp37, ptr %tmp29strbuf, align 8
  %tmp38 = load ptr, ptr %tmp29buf, align 8
  %dereftmp39 = load ptr, ptr %tmp38, align 8
  %tmp40 = load ptr, ptr %tmp29strbuf, align 8
  %tmp41 = call ptr @strcpy(ptr %dereftmp39, ptr %tmp40)
  %tmp42 = load ptr, ptr %tmp29strbuf, align 8
  %tmp43 = call i32 @free(ptr %tmp42)
  %tmp47 = load i8, ptr %tmp29ch, align 1
  %tmp48 = icmp ne i8 %tmp47, 10
  br i1 %tmp48, label %tmp44, label %tmp45

tmp44:                                            ; preds = %tmp44, %tmp29
  %tmp49 = call i8 @getchar()
  store i8 %tmp49, ptr %tmp44ch, align 1
  store i1 true, ptr %flagtmp44ch, align 1
  %tmp50 = load i64, ptr %tmp29chars, align 8
  %0 = load i1, ptr %flagtmp44chars, align 1
  %1 = icmp eq i1 %0, true
  %2 = select i1 %1, ptr %tmp44chars, ptr %tmp29chars
  %tmp501 = load i64, ptr %tmp44chars, align 8
  %3 = select i1 %1, i64 %tmp501, i64 %tmp50
  store i64 %3, ptr %tmp44chars, align 8
  store i1 true, ptr %flagtmp44chars, align 1
  %tmp51 = load i64, ptr %2, align 8
  %tmp52 = add i64 %tmp51, 1
  store i64 %tmp52, ptr %tmp44chars, align 8
  store i1 true, ptr %flagtmp44chars, align 1
  %tmp53 = load ptr, ptr %tmp29buf, align 8
  %4 = load i1, ptr %flagtmp44buf, align 1
  %5 = icmp eq i1 %4, true
  %6 = select i1 %5, ptr %tmp44buf, ptr %tmp29buf
  %tmp532 = load ptr, ptr %tmp44buf, align 8
  %7 = select i1 %5, ptr %tmp532, ptr %tmp53
  store ptr %7, ptr %tmp44buf, align 8
  store i1 true, ptr %flagtmp44buf, align 1
  %tmp54 = load ptr, ptr %6, align 8
  %dereftmp55 = load ptr, ptr %tmp54, align 8
  store ptr %dereftmp55, ptr %tmp44tmp, align 8
  store i1 true, ptr %flagtmp44tmp, align 1
  %tmp56 = load ptr, ptr %tmp44buf, align 8
  %tmp57 = load i64, ptr %tmp44chars, align 8
  %tmp58 = mul i64 %tmp57, 1
  %tmp59 = load ptr, ptr %tmp29stralloc, align 8
  %8 = load i1, ptr %flagtmp44stralloc, align 1
  %9 = icmp eq i1 %8, true
  %10 = select i1 %9, ptr %tmp44stralloc, ptr %tmp29stralloc
  %tmp593 = load ptr, ptr %tmp44stralloc, align 8
  %11 = select i1 %9, ptr %tmp593, ptr %tmp59
  store ptr %11, ptr %tmp44stralloc, align 8
  store i1 true, ptr %flagtmp44stralloc, align 1
  %tmp60 = load ptr, ptr %10, align 8
  %tmp61 = call ptr %tmp60(i64 %tmp58)
  store ptr %tmp61, ptr %tmp56, align 8
  %tmp62 = load ptr, ptr %tmp44buf, align 8
  %dereftmp63 = load ptr, ptr %tmp62, align 8
  %tmp64 = load ptr, ptr %tmp44tmp, align 8
  %tmp65 = call ptr @strcpy(ptr %dereftmp63, ptr %tmp64)
  %tmp66 = load i8, ptr %tmp44ch, align 1
  %tmp67 = call ptr @stringify(i8 %tmp66)
  store ptr %tmp67, ptr %tmp44strton, align 8
  store i1 true, ptr %flagtmp44strton, align 1
  %tmp68 = load ptr, ptr %tmp44buf, align 8
  %dereftmp69 = load ptr, ptr %tmp68, align 8
  %tmp70 = load ptr, ptr %tmp44strton, align 8
  %tmp71 = call ptr @strcat(ptr %dereftmp69, ptr %tmp70)
  %tmp72 = load ptr, ptr %tmp44strton, align 8
  %tmp73 = call i32 @free(ptr %tmp72)
  %tmp74 = load ptr, ptr %tmp44tmp, align 8
  %tmp75 = call i32 @free(ptr %tmp74)
  %tmp76 = load i8, ptr %tmp44ch, align 1
  %tmp77 = icmp ne i8 %tmp76, 10
  br i1 %tmp77, label %tmp44, label %tmp45

tmp45:                                            ; preds = %tmp44, %tmp29
  %tmp78 = load ptr, ptr %tmp29buf, align 8
  %dereftmp79 = load ptr, ptr %tmp78, align 8
  store ptr %dereftmp79, ptr %retvalreadline, align 8
  %tmp80 = load ptr, ptr %retvalreadline, align 8
  ret ptr %tmp80
}

define ptr @"5968854263808077590input"(ptr %0) {
__allocreigon5968854263808077590input:
  %tmp81prompt = alloca ptr, align 8
  %tmp81inp = alloca ptr, align 8
  %retval5968854263808077590input = alloca ptr, align 8
  br label %tmp81

tmp81:                                            ; preds = %__allocreigon5968854263808077590input
  store ptr %0, ptr %tmp81prompt, align 8
  %tmp82 = load ptr, ptr %tmp81prompt, align 8
  %tmp83 = call i32 (ptr, ...) @printf(ptr %tmp82)
  %tmp84 = call ptr @readline()
  store ptr %tmp84, ptr %tmp81inp, align 8
  %tmp85 = load ptr, ptr %tmp81inp, align 8
  store ptr %tmp85, ptr %retval5968854263808077590input, align 8
  %tmp86 = load ptr, ptr %retval5968854263808077590input, align 8
  ret ptr %tmp86
}
