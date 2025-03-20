; ModuleID = 'src/Allocator.mvn'
source_filename = "src/Allocator.mvn"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-darwin24.3.0"

%none_struct = type opaque
%tmp16HeapPointer = type { ptr, i64 }

@none = internal global %none_struct zeroinitializer
@noneload = internal constant ptr @none
@tmp3 = private unnamed_addr constant [15 x i8] c"example string\00", align 1
@tmp16ASZ = internal global ptr null
@tmp16ALLOC_SZ = internal global ptr null
@tmp21 = constant %tmp16HeapPointer zeroinitializer
@tmp25 = private unnamed_addr constant [8 x i8] c"Freeing\00", align 1
@tmp42 = private unnamed_addr constant [33 x i8] c"No need to free a direct pointer\00", align 1
@tmp53 = private unnamed_addr constant [26 x i8] c"Failed to allocate memory\00", align 1
@tmp110 = private unnamed_addr constant [29 x i8] c"Failed to re-allocate memory\00", align 1
@tmp130 = private unnamed_addr constant [17 x i8] c"GrowthAlloc: %d\0A\00", align 1

define internal i32 @"11763512213598171213mainMavenStart"() {
__allocreigon11763512213598171213mainMavenStart:
  %tmp6tmp7 = alloca ptr, align 8
  %tmp6tmp8 = alloca ptr, align 8
  %tmp6tmp9 = alloca ptr, align 8
  %tmp6tmp10 = alloca ptr, align 8
  %tmp6tmp11 = alloca ptr, align 8
  %tmp6tmp12 = alloca ptr, align 8
  %tmp6tmp13 = alloca ptr, align 8
  %tmp6tmp14 = alloca ptr, align 8
  %tmp6tmp15 = alloca ptr, align 8
  %tmp16tmp17 = alloca ptr, align 8
  %tmp16tmp19 = alloca ptr, align 8
  %tmp20 = alloca %tmp16HeapPointer, align 8
  %tmp16tmp40 = alloca ptr, align 8
  %retval11763512213598171213mainMavenStart = alloca i32, align 4
  br label %tmp2

tmp2:                                             ; preds = %__allocreigon11763512213598171213mainMavenStart
  br label %tmp6

tmp6:                                             ; preds = %tmp2
  store i8 0, ptr %tmp6tmp7, align 1
  store i8 0, ptr %tmp6tmp8, align 1
  store i8 0, ptr %tmp6tmp9, align 1
  store i8 0, ptr %tmp6tmp10, align 1
  store i8 0, ptr %tmp6tmp11, align 1
  store i8 0, ptr %tmp6tmp12, align 1
  store i8 0, ptr %tmp6tmp13, align 1
  store i8 0, ptr %tmp6tmp14, align 1
  store i8 0, ptr %tmp6tmp15, align 1
  br label %tmp16

tmp16:                                            ; preds = %tmp6
  store i64 0, ptr %tmp16tmp17, align 8
  store ptr %tmp16tmp17, ptr @tmp16ASZ, align 8
  %tmp18 = load ptr, ptr @tmp16ASZ, align 8
  store ptr %tmp18, ptr @tmp16ALLOC_SZ, align 8
  store i8 0, ptr %tmp16tmp19, align 1
  %tmp23 = load %tmp16HeapPointer, ptr %tmp20, align 8
  store i8 0, ptr %tmp16tmp40, align 1
  br label %tmp45

tmp45:                                            ; preds = %tmp16
  %tmp46 = load %tmp16HeapPointer, ptr %tmp20, align 8
  br label %tmp47

tmp47:                                            ; preds = %tmp45
  br label %tmp64

tmp64:                                            ; preds = %tmp47
  %tmp65 = load %tmp16HeapPointer, ptr %tmp20, align 8
  br label %tmp66

tmp66:                                            ; preds = %tmp64
  %tmp76 = load %tmp16HeapPointer, ptr %tmp20, align 8
  %tmp81 = load %tmp16HeapPointer, ptr %tmp20, align 8
  br label %tmp82

tmp82:                                            ; preds = %tmp66
  %tmp83 = load %tmp16HeapPointer, ptr %tmp20, align 8
  br label %tmp84

tmp84:                                            ; preds = %tmp82
  %tmp120 = load %tmp16HeapPointer, ptr %tmp20, align 8
  br label %tmp121

tmp121:                                           ; preds = %tmp84
  %tmp122 = load %tmp16HeapPointer, ptr %tmp20, align 8
  br label %tmp123

tmp123:                                           ; preds = %tmp121
  store i32 0, ptr %retval11763512213598171213mainMavenStart, align 4
  store i32 0, ptr %retval11763512213598171213mainMavenStart, align 4
  %tmp137 = load i32, ptr %retval11763512213598171213mainMavenStart, align 4
  ret i32 %tmp137
}

define i32 @"11763512213598171213main"() {
tmp0:
  %tmp1 = call i32 @"11763512213598171213mainMavenStart"()
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

declare ptr @"5968854263808077590input"(ptr)

declare i32 @printf(ptr, ...)

declare i32 @strcmp(ptr, ptr)

declare i32 @strlen(ptr)

declare ptr @fopen(ptr, ptr)

declare i32 @fprintf(ptr, ptr, ...)

declare ptr @strcpy(ptr, ptr)

declare ptr @malloc(i64)

declare ptr @memcpy(ptr, ptr, i64)

declare ptr @realloc(ptr, i64)

declare i32 @free(ptr)

define internal ptr @Destroy152(%tmp16HeapPointer %0) {
__allocreigontmp22:
  %tmp24tagger = alloca %tmp16HeapPointer, align 8
  %retvaltmp22 = alloca ptr, align 8
  br label %tmp24

tmp24:                                            ; preds = %__allocreigontmp22
  store %tmp16HeapPointer %0, ptr %tmp24tagger, align 8
  %tmp26 = call i32 @"5673998842831842134println"(ptr @tmp25)
  %tmp27 = load ptr, ptr @tmp16ALLOC_SZ, align 8
  %tmp28 = load ptr, ptr @tmp16ALLOC_SZ, align 8
  %dereftmp29 = load i64, ptr %tmp28, align 8
  %tmp31 = getelementptr inbounds { ptr, i64 }, ptr %tmp24tagger, i32 0, i32 1
  %tmp32 = load i64, ptr %tmp31, align 8
  %tmp33 = sub i64 %dereftmp29, %tmp32
  store i64 %tmp33, ptr %tmp27, align 8
  %tmp35 = getelementptr inbounds { ptr, i64 }, ptr %tmp24tagger, i32 0, i32 0
  %tmp36 = load ptr, ptr %tmp35, align 8
  %tmp37 = call i32 @free(ptr %tmp36)
  store ptr @noneload, ptr %retvaltmp22, align 8
  %tmp38 = load ptr, ptr %retvaltmp22, align 8
  ret ptr %tmp38
}

define internal ptr @Destroy135(ptr %0) {
__allocreigontmp39:
  %tmp41tagger = alloca ptr, align 8
  %retvaltmp39 = alloca ptr, align 8
  br label %tmp41

tmp41:                                            ; preds = %__allocreigontmp39
  store ptr %0, ptr %tmp41tagger, align 8
  %tmp43 = call i32 @"5673998842831842134println"(ptr @tmp42)
  store ptr @noneload, ptr %retvaltmp39, align 8
  %tmp44 = load ptr, ptr %retvaltmp39, align 8
  ret ptr %tmp44
}

define internal %tmp16HeapPointer @HeapAlloc(i64 %0) {
__allocreigonHeapAlloc:
  %tmp48sz = alloca i64, align 8
  %tmp48ptr = alloca ptr, align 8
  %tmp58 = alloca %tmp16HeapPointer, align 8, !structee_type !0
  %retvalHeapAlloc = alloca %tmp16HeapPointer, align 8
  br label %tmp48

tmp48:                                            ; preds = %__allocreigonHeapAlloc
  store i64 %0, ptr %tmp48sz, align 8
  %tmp49 = load i64, ptr %tmp48sz, align 8
  %tmp50 = call ptr @malloc(i64 %tmp49)
  store ptr %tmp50, ptr %tmp48ptr, align 8
  %tmp51 = load ptr, ptr %tmp48ptr, align 8
  %tmp52 = icmp ne ptr %tmp51, null
  %tmp54 = call ptr @"5673998842831842134Assert"(i1 %tmp52, ptr @tmp53)
  %tmp55 = load ptr, ptr %tmp48ptr, align 8
  %tmp56 = load i64, ptr %tmp48sz, align 8
  %tmp57 = load %tmp16HeapPointer, ptr @tmp21, align 8
  %tmp59 = getelementptr inbounds %tmp16HeapPointer, ptr %tmp58, i32 0, i32 0
  store ptr %tmp55, ptr %tmp59, align 8
  %tmp60 = getelementptr inbounds %tmp16HeapPointer, ptr %tmp58, i32 0, i32 1
  store i64 %tmp56, ptr %tmp60, align 8
  %tmp62 = load %tmp16HeapPointer, ptr %tmp58, align 8
  store %tmp16HeapPointer %tmp62, ptr %retvalHeapAlloc, align 8
  %tmp63 = load %tmp16HeapPointer, ptr %retvalHeapAlloc, align 8
  ret %tmp16HeapPointer %tmp63
}

define %tmp16HeapPointer @"11763512213598171213Alloc"(i64 %0) {
__allocreigon11763512213598171213Alloc:
  %tmp67sz = alloca i64, align 8
  %retval11763512213598171213Alloc = alloca %tmp16HeapPointer, align 8
  br label %tmp67

tmp67:                                            ; preds = %__allocreigon11763512213598171213Alloc
  store i64 %0, ptr %tmp67sz, align 8
  %tmp68 = load ptr, ptr @tmp16ALLOC_SZ, align 8
  %tmp69 = load ptr, ptr @tmp16ALLOC_SZ, align 8
  %dereftmp70 = load i64, ptr %tmp69, align 8
  %tmp71 = load i64, ptr %tmp67sz, align 8
  %tmp72 = add i64 %dereftmp70, %tmp71
  store i64 %tmp72, ptr %tmp68, align 8
  %tmp73 = load i64, ptr %tmp67sz, align 8
  %tmp74 = call %tmp16HeapPointer @HeapAlloc(i64 %tmp73)
  store %tmp16HeapPointer %tmp74, ptr %retval11763512213598171213Alloc, align 8
  %tmp75 = load %tmp16HeapPointer, ptr %retval11763512213598171213Alloc, align 8
  ret %tmp16HeapPointer %tmp75
}

define ptr @"11763512213598171213Dealloc"(%tmp16HeapPointer %0) {
__allocreigon11763512213598171213Dealloc:
  %tmp77ptr = alloca %tmp16HeapPointer, align 8
  %retval11763512213598171213Dealloc = alloca ptr, align 8
  br label %tmp77

tmp77:                                            ; preds = %__allocreigon11763512213598171213Dealloc
  store %tmp16HeapPointer %0, ptr %tmp77ptr, align 8
  %tmp78 = load %tmp16HeapPointer, ptr %tmp77ptr, align 8
  %tmp79 = call ptr @Destroy152(%tmp16HeapPointer %tmp78)
  store ptr @noneload, ptr %retval11763512213598171213Dealloc, align 8
  %tmp80 = load ptr, ptr %retval11763512213598171213Dealloc, align 8
  ret ptr %tmp80
}

define %tmp16HeapPointer @"11763512213598171213Realloc"(%tmp16HeapPointer %0, i64 %1) {
__allocreigon11763512213598171213Realloc:
  %tmp85ptr = alloca %tmp16HeapPointer, align 8
  %tmp85new_sz = alloca i64, align 8
  %tmp85HeapPtr = alloca %tmp16HeapPointer, align 8
  %tmp85new_ptr = alloca ptr, align 8
  %retval11763512213598171213Realloc = alloca %tmp16HeapPointer, align 8
  br label %tmp85

tmp85:                                            ; preds = %__allocreigon11763512213598171213Realloc
  store %tmp16HeapPointer %0, ptr %tmp85ptr, align 8
  store i64 %1, ptr %tmp85new_sz, align 8
  %tmp86 = load %tmp16HeapPointer, ptr %tmp85ptr, align 8
  store %tmp16HeapPointer %tmp86, ptr %tmp85HeapPtr, align 8
  %tmp88 = getelementptr inbounds { ptr, i64 }, ptr %tmp85ptr, i32 0, i32 0
  %tmp89 = load ptr, ptr %tmp88, align 8
  %tmp90 = load i64, ptr %tmp85new_sz, align 8
  %tmp91 = call ptr @realloc(ptr %tmp89, i64 %tmp90)
  store ptr %tmp91, ptr %tmp85new_ptr, align 8
  %tmp92 = load ptr, ptr @tmp16ALLOC_SZ, align 8
  %tmp93 = load ptr, ptr @tmp16ALLOC_SZ, align 8
  %dereftmp94 = load i64, ptr %tmp93, align 8
  %tmp95 = load i64, ptr %tmp85new_sz, align 8
  %tmp96 = add i64 %dereftmp94, %tmp95
  %tmp98 = getelementptr inbounds { ptr, i64 }, ptr %tmp85ptr, i32 0, i32 1
  %tmp99 = load i64, ptr %tmp98, align 8
  %tmp100 = sub i64 %tmp96, %tmp99
  store i64 %tmp100, ptr %tmp92, align 8
  %tmp104 = load ptr, ptr %tmp85new_ptr, align 8
  %tmp105 = icmp eq ptr %tmp104, null
  br i1 %tmp105, label %tmp102, label %tmp103

tmp102:                                           ; preds = %tmp85
  %tmp107 = getelementptr inbounds { ptr, i64 }, ptr %tmp85ptr, i32 0, i32 0
  %tmp108 = load ptr, ptr %tmp107, align 8
  %tmp109 = call i32 @free(ptr %tmp108)
  %tmp111 = call ptr @"5673998842831842134Abort"(ptr @tmp110)
  br label %tmp103

tmp103:                                           ; preds = %tmp102, %tmp85
  %tmp113 = getelementptr inbounds { ptr, i64 }, ptr %tmp85HeapPtr, i32 0, i32 0
  %tmp114 = load ptr, ptr %tmp85new_ptr, align 8
  store ptr %tmp114, ptr %tmp113, align 8
  %tmp116 = getelementptr inbounds { ptr, i64 }, ptr %tmp85HeapPtr, i32 0, i32 1
  %tmp117 = load i64, ptr %tmp85new_sz, align 8
  store i64 %tmp117, ptr %tmp116, align 8
  %tmp118 = load %tmp16HeapPointer, ptr %tmp85HeapPtr, align 8
  store %tmp16HeapPointer %tmp118, ptr %retval11763512213598171213Realloc, align 8
  %tmp119 = load %tmp16HeapPointer, ptr %retval11763512213598171213Realloc, align 8
  ret %tmp16HeapPointer %tmp119
}

define %tmp16HeapPointer @"11763512213598171213GrowAlloc"(%tmp16HeapPointer %0, i64 %1) {
__allocreigon11763512213598171213GrowAlloc:
  %tmp124ptr = alloca %tmp16HeapPointer, align 8
  %tmp124of_sz = alloca i64, align 8
  %tmp124actual_sz = alloca i64, align 8
  %retval11763512213598171213GrowAlloc = alloca %tmp16HeapPointer, align 8
  br label %tmp124

tmp124:                                           ; preds = %__allocreigon11763512213598171213GrowAlloc
  store %tmp16HeapPointer %0, ptr %tmp124ptr, align 8
  store i64 %1, ptr %tmp124of_sz, align 8
  %tmp126 = getelementptr inbounds { ptr, i64 }, ptr %tmp124ptr, i32 0, i32 1
  %tmp127 = load i64, ptr %tmp126, align 8
  %tmp128 = load i64, ptr %tmp124of_sz, align 8
  %tmp129 = add i64 %tmp127, %tmp128
  store i64 %tmp129, ptr %tmp124actual_sz, align 8
  %tmp131 = load i64, ptr %tmp124actual_sz, align 8
  %tmp132 = call i32 (ptr, ...) @printf(ptr @tmp130, i64 %tmp131)
  %tmp133 = load %tmp16HeapPointer, ptr %tmp124ptr, align 8
  %tmp134 = load i64, ptr %tmp124actual_sz, align 8
  %tmp135 = call %tmp16HeapPointer @"11763512213598171213Realloc"(%tmp16HeapPointer %tmp133, i64 %tmp134)
  store %tmp16HeapPointer %tmp135, ptr %retval11763512213598171213GrowAlloc, align 8
  %tmp136 = load %tmp16HeapPointer, ptr %retval11763512213598171213GrowAlloc, align 8
  ret %tmp16HeapPointer %tmp136
}

!0 = !{!"tmp61"}
