; ModuleID = 'ptest.c'
source_filename = "ptest.c"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx15.0.0"

%union.Data = type { i32, [16 x i8] }

@.str = private unnamed_addr constant [12 x i8] c"data.i: %d\0A\00", align 1
@.str.1 = private unnamed_addr constant [12 x i8] c"data.f: %f\0A\00", align 1
@.str.2 = private unnamed_addr constant [14 x i8] c"Hello, World!\00", align 1
@.str.3 = private unnamed_addr constant [14 x i8] c"data.str: %s\0A\00", align 1

; Function Attrs: noinline nounwind optnone ssp uwtable(sync)
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca %union.Data, align 4
  store i32 0, ptr %1, align 4
  store i32 10, ptr %2, align 4
  %3 = load i32, ptr %2, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %3)
  store float 2.205000e+02, ptr %2, align 4
  %5 = load float, ptr %2, align 4
  %6 = fpext float %5 to double
  %7 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, double noundef %6)
  %8 = getelementptr inbounds [20 x i8], ptr %2, i64 0, i64 0
  %9 = call i32 (ptr, i64, i32, i64, ptr, ...) @__snprintf_chk(ptr noundef %8, i64 noundef 20, i32 noundef 0, i64 noundef 20, ptr noundef @.str.2)
  %10 = getelementptr inbounds [20 x i8], ptr %2, i64 0, i64 0
  %11 = call i32 (ptr, ...) @printf(ptr noundef @.str.3, ptr noundef %10)
  ret i32 0
}

declare i32 @printf(ptr noundef, ...) #1

declare i32 @__snprintf_chk(ptr noundef, i64 noundef, i32 noundef, i64 noundef, ptr noundef, ...) #1

attributes #0 = { noinline nounwind optnone ssp uwtable(sync) "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" }
attributes #1 = { "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 15, i32 2]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 1}
!5 = !{!"Homebrew clang version 19.1.6"}
