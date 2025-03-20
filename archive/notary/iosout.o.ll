; ModuleID = 'iosout'
source_filename = "iosout"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-darwin"

%entryenumT = type { i32, ptr, i32, i32 }

@0 = private unnamed_addr constant [7 x i8] c"hallo\0A\00", align 1
@1 = private unnamed_addr constant [6 x i8] c"so %s\00", align 1
@2 = private unnamed_addr constant [5 x i8] c"hola\00", align 1
@3 = private unnamed_addr constant [10 x i8] c"EKVIN: %d\00", align 1
@4 = private unnamed_addr constant [5 x i8] c"hola\00", align 1
@5 = private unnamed_addr constant [10 x i8] c"Something\00", align 1
@Tcvaluesmth = internal global ptr @5
@6 = private unnamed_addr constant [8 x i8] c"CV: %s\0A\00", align 1
@7 = private unnamed_addr constant [19 x i8] c"Something the 2nd!\00", align 1
@8 = private unnamed_addr constant [8 x i8] c"CV: %s\0A\00", align 1
@9 = private unnamed_addr constant [10 x i8] c"Ekid: %d\0A\00", align 1
@10 = private unnamed_addr constant [12 x i8] c"Ptable: %d\0A\00", align 1
@11 = private unnamed_addr constant [26 x i8] c"\0AHello world from ptable\0A\00", align 1

declare i32 @printf(ptr, ...)

declare i32 @strcmp(ptr, ptr)

define i32 @main(i32 %0, ptr %1) {
MavenStartblk:
  %2 = call i32 @MavenStart(i32 %0, ptr %1)
  ret i32 %2
}

define internal i32 @MavenStart(i32 %0, ptr %1) {
entry0:
  br label %__allocreigonMavenStart

__allocreigonMavenStart:                          ; preds = %entry0
  %entryvarenumkinst0 = alloca %entryenumT, align 8
  %flagentryvarenumkinst0 = alloca i1, align 1
  store i1 false, ptr %flagentryvarenumkinst0, align 1
  %entryvarenumkinst5 = alloca %entryenumT, align 8
  %flagentryvarenumkinst5 = alloca i1, align 1
  store i1 false, ptr %flagentryvarenumkinst5, align 1
  %entryvarenumkinst8 = alloca %entryenumT, align 8
  %flagentryvarenumkinst8 = alloca i1, align 1
  store i1 false, ptr %flagentryvarenumkinst8, align 1
  %entryvarEKA = alloca %entryenumT, align 8
  %flagentryvarEKA = alloca i1, align 1
  store i1 false, ptr %flagentryvarEKA, align 1
  %entryvarenumkinst37 = alloca %entryenumT, align 8
  %flagentryvarenumkinst37 = alloca i1, align 1
  store i1 false, ptr %flagentryvarenumkinst37, align 1
  %entryvaridk = alloca %entryenumT, align 8
  %flagentryvaridk = alloca i1, align 1
  store i1 false, ptr %flagentryvaridk, align 1
  br label %entry

entry:                                            ; preds = %__allocreigonMavenStart
  %structUentryenumT = alloca %entryenumT, align 8
  %enumfieldofentryvarenumkinst00 = getelementptr inbounds %entryenumT, ptr %entryvarenumkinst0, i32 0, i32 2
  store i32 10, ptr %enumfieldofentryvarenumkinst00, align 4
  %enumfieldofentryvarenumkinst01 = getelementptr inbounds %entryenumT, ptr %entryvarenumkinst0, i32 0, i32 3
  store i32 19, ptr %enumfieldofentryvarenumkinst01, align 4
  %discrimenumfieldofentryvarenumkinst0 = getelementptr inbounds %entryenumT, ptr %entryvarenumkinst0, i32 0, i32 0
  store i32 1, ptr %discrimenumfieldofentryvarenumkinst0, align 4
  %lenumk4 = load %entryenumT, ptr %entryvarenumkinst0, align 8
  %discrimenumfieldofentryvarenumkinst5 = getelementptr inbounds %entryenumT, ptr %entryvarenumkinst5, i32 0, i32 0
  store i32 3, ptr %discrimenumfieldofentryvarenumkinst5, align 4
  %lenumk7 = load %entryenumT, ptr %entryvarenumkinst5, align 8
  %enumfieldofentryvarenumkinst80 = getelementptr inbounds %entryenumT, ptr %entryvarenumkinst8, i32 0, i32 1
  store ptr @0, ptr %enumfieldofentryvarenumkinst80, align 8
  %enumfieldofentryvarenumkinst81 = getelementptr inbounds %entryenumT, ptr %entryvarenumkinst8, i32 0, i32 2
  store i32 11, ptr %enumfieldofentryvarenumkinst81, align 4
  %discrimenumfieldofentryvarenumkinst8 = getelementptr inbounds %entryenumT, ptr %entryvarenumkinst8, i32 0, i32 0
  store i32 2, ptr %discrimenumfieldofentryvarenumkinst8, align 4
  %lenumk12 = load %entryenumT, ptr %entryvarenumkinst8, align 8
  store %entryenumT %lenumk12, ptr %entryvarEKA, align 8
  store i1 true, ptr %flagentryvarEKA, align 1
  %discrimenumfieldofentryvarenumkinst37 = getelementptr inbounds %entryenumT, ptr %entryvarenumkinst37, i32 0, i32 0
  store i32 3, ptr %discrimenumfieldofentryvarenumkinst37, align 4
  %lenumk39 = load %entryenumT, ptr %entryvarenumkinst37, align 8
  store %entryenumT %lenumk39, ptr %entryvaridk, align 8
  store i1 true, ptr %flagentryvaridk, align 1
  call void @TcmethodEnumTest(ptr %entryvaridk, ptr @4)
  %lidkcvaluesmth = load ptr, ptr @Tcvaluesmth, align 8
  %2 = call i32 (ptr, ...) @printf(ptr @6, ptr %lidkcvaluesmth)
  store ptr @7, ptr @Tcvaluesmth, align 8
  %lidkcvaluesmth1 = load ptr, ptr @Tcvaluesmth, align 8
  %3 = call i32 (ptr, ...) @printf(ptr @8, ptr %lidkcvaluesmth1)
  %4 = call i32 (ptr, ...) @printf(ptr @9, ptr %entryvaridk)
  %5 = call i32 @idkcmethodPrintable(ptr @11)
  %6 = call i32 (ptr, ...) @printf(ptr @10, i32 %5)
  ret i32 0
}

define void @TcmethodEnumTest(ptr %0, ptr %1) {
entry1:
  br label %__allocreigonEnumTest

__allocreigonEnumTest:                            ; preds = %entry1
  %b1ifb7varenumkinst21 = alloca %entryenumT, align 8
  %flagb1ifb7varenumkinst21 = alloca i1, align 1
  store i1 false, ptr %flagb1ifb7varenumkinst21, align 1
  %b1ifb7varEK102 = alloca %entryenumT, align 8
  %flagb1ifb7varEK102 = alloca i1, align 1
  store i1 false, ptr %flagb1ifb7varEK102, align 1
  %b1elseb8varenumkinst32 = alloca %entryenumT, align 8
  %flagb1elseb8varenumkinst32 = alloca i1, align 1
  store i1 false, ptr %flagb1elseb8varenumkinst32, align 1
  %b1elseb8varEKZ2 = alloca %entryenumT, align 8
  %flagb1elseb8varEKZ2 = alloca i1, align 1
  store i1 false, ptr %flagb1elseb8varEKZ2, align 1
  br label %b1

b1:                                               ; preds = %__allocreigonEnumTest
  %2 = call i32 (ptr, ...) @printf(ptr @1, ptr %1)
  %3 = call i32 @strcmp(ptr %1, ptr @2)
  %4 = icmp eq i32 %3, 0
  br label %b1ifintemediary3

b1ifmerge2:                                       ; preds = %b1elseb8, %b1ifb7
  br label %b14

b1ifintemediary3:                                 ; preds = %b1
  br i1 %4, label %b1ifb7, label %b1elseb8

b1ifb7:                                           ; preds = %b1ifintemediary3
  %enumfieldofb1ifb7varenumkinst210 = getelementptr inbounds %entryenumT, ptr %b1ifb7varenumkinst21, i32 0, i32 1
  store ptr %1, ptr %enumfieldofb1ifb7varenumkinst210, align 8
  %enumfieldofb1ifb7varenumkinst211 = getelementptr inbounds %entryenumT, ptr %b1ifb7varenumkinst21, i32 0, i32 2
  store i32 10, ptr %enumfieldofb1ifb7varenumkinst211, align 4
  %discrimenumfieldofb1ifb7varenumkinst21 = getelementptr inbounds %entryenumT, ptr %b1ifb7varenumkinst21, i32 0, i32 0
  store i32 2, ptr %discrimenumfieldofb1ifb7varenumkinst21, align 4
  %lenumk25 = load %entryenumT, ptr %b1ifb7varenumkinst21, align 8
  store %entryenumT %lenumk25, ptr %b1ifb7varEK102, align 8
  store i1 true, ptr %flagb1ifb7varEK102, align 1
  %lb1ifb7varEK102 = load %entryenumT, ptr %b1ifb7varEK102, align 8
  store %entryenumT %lb1ifb7varEK102, ptr %0, align 8
  %aEKT30 = getelementptr inbounds %entryenumT, ptr %b1ifb7varEK102, i32 0, i32 0
  %laEKT30 = load i32, ptr %aEKT30, align 4
  %5 = call i32 (ptr, ...) @printf(ptr @3, i32 %laEKT30)
  br label %b1ifmerge2

b1elseb8:                                         ; preds = %b1ifintemediary3
  %discrimenumfieldofb1elseb8varenumkinst32 = getelementptr inbounds %entryenumT, ptr %b1elseb8varenumkinst32, i32 0, i32 0
  store i32 3, ptr %discrimenumfieldofb1elseb8varenumkinst32, align 4
  %lenumk34 = load %entryenumT, ptr %b1elseb8varenumkinst32, align 8
  store %entryenumT %lenumk34, ptr %b1elseb8varEKZ2, align 8
  store i1 true, ptr %flagb1elseb8varEKZ2, align 1
  %lb1elseb8varEKZ2 = load %entryenumT, ptr %b1elseb8varEKZ2, align 8
  store %entryenumT %lb1elseb8varEKZ2, ptr %0, align 8
  br label %b1ifmerge2

b14:                                              ; preds = %b1ifmerge2
  ret void
}

define internal i32 @idkcmethodPrintable(ptr %0) {
entry4:
  br label %__allocreigonPrintable

__allocreigonPrintable:                           ; preds = %entry4
  br label %b2939

b2939:                                            ; preds = %__allocreigonPrintable
  %1 = call i32 (ptr, ...) @printf(ptr %0)
  ret i32 %1
}
