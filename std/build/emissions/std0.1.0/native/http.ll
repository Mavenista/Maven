; ModuleID = 'src/http.mvn'
source_filename = "src/http.mvn"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-darwin24.3.0"

%none_struct = type opaque
%tmp2in_addr = type { i32 }
%tmp2sockaddr = type { i8, i8, [14 x i8] }
%tmp2sockaddr_in = type { i8, i8, i16, %tmp2in_addr, [8 x i8] }
%tmp2hostent = type { ptr, ptr, i32, i32, ptr }
%tmp47 = type { i32, ptr }

@none = internal global %none_struct zeroinitializer
@noneload = internal constant ptr @none
@tmp3 = private unnamed_addr constant [15 x i8] c"example string\00", align 1
@tmp7 = constant %tmp2in_addr zeroinitializer
@tmp11 = constant %tmp2sockaddr zeroinitializer
@tmp16 = constant %tmp2sockaddr_in zeroinitializer
@tmp20 = constant %tmp2hostent zeroinitializer
@tmp2BUFFER_SIZE = internal global i64 0
@tmp45stralloc = internal global ptr null
@tmp71 = private unnamed_addr constant [12 x i8] c"Sockfd: %d\0A\00", align 1
@tmp76 = private unnamed_addr constant [23 x i8] c"Socket creation failed\00", align 1
@tmp83 = private unnamed_addr constant [9 x i8] c"Deferral\00", align 1
@tmp85 = private unnamed_addr constant [39 x i8] c"Failed to get host (check request url)\00", align 1
@tmp87 = private unnamed_addr constant [18 x i8] c"host_addr(p): %p\0A\00", align 1
@tmp99 = private unnamed_addr constant [10 x i8] c"hlen: %d\0A\00", align 1
@tmp122 = private unnamed_addr constant [21 x i8] c"Beginning connection\00", align 1
@tmp128 = private unnamed_addr constant [9 x i8] c"Con: %d\0A\00", align 1
@tmp133 = private unnamed_addr constant [18 x i8] c"Failed connection\00", align 1
@tmp135 = private unnamed_addr constant [46 x i8] c"GET / HTTP/1.1\0D\0AHost: \0D\0AConnection: close\0D\0A\0D\0A\00", align 1
@tmp146 = private unnamed_addr constant [49 x i8] c"GET %s HTTP/1.1\0D\0AHost: %s\0D\0AConnection: close\0D\0A\0D\0A\00", align 1
@tmp147 = private unnamed_addr constant [2 x i8] c"/\00", align 1
@tmp150 = private unnamed_addr constant [15 x i8] c"request: '%s'\0A\00", align 1
@tmp153 = private unnamed_addr constant [19 x i8] c"request_len: '%d'\0A\00", align 1
@tmp164 = private unnamed_addr constant [5 x i8] c"send\00", align 1
@tmp166 = private unnamed_addr constant [10 x i8] c"send: %d\0A\00", align 1
@tmp170 = private unnamed_addr constant [23 x i8] c"Failed to send request\00", align 1
@tmp174 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@tmp181 = private unnamed_addr constant [11 x i8] c"google.com\00", align 1

define internal i32 @mainMavenStart() {
__allocreigonmainMavenStart:
  %tmp6 = alloca %tmp2in_addr, align 8
  %tmp8 = alloca [14 x i8], align 1
  %tmp10 = alloca %tmp2sockaddr, align 8
  %tmp13 = alloca [8 x i8], align 1
  %tmp15 = alloca %tmp2sockaddr_in, align 8
  %tmp2tmp17 = alloca ptr, align 8
  %tmp2tmp18 = alloca ptr, align 8
  %tmp19 = alloca %tmp2hostent, align 8
  %tmp21tmp22 = alloca ptr, align 8
  %tmp21tmp23 = alloca ptr, align 8
  %tmp21tmp24 = alloca ptr, align 8
  %tmp21tmp25 = alloca ptr, align 8
  %tmp21tmp26 = alloca ptr, align 8
  %tmp21tmp27 = alloca ptr, align 8
  %tmp21tmp28 = alloca ptr, align 8
  %tmp21tmp29 = alloca ptr, align 8
  %tmp21tmp30 = alloca ptr, align 8
  %tmp21tmp31 = alloca ptr, align 8
  %tmp21tmp33 = alloca ptr, align 8
  %tmp21tmp34 = alloca ptr, align 8
  %tmp21tmp35 = alloca ptr, align 8
  %tmp21tmp36 = alloca ptr, align 8
  %tmp21tmp37 = alloca ptr, align 8
  %tmp21tmp38 = alloca ptr, align 8
  %tmp21tmp40 = alloca ptr, align 8
  %tmp21tmp42 = alloca ptr, align 8
  %tmp21tmp44 = alloca ptr, align 8
  %tmp48 = alloca %tmp47, align 8
  %retvalmainMavenStart = alloca i32, align 4
  br label %tmp2

tmp2:                                             ; preds = %__allocreigonmainMavenStart
  %0 = getelementptr [14 x i8], ptr %tmp8, i32 0, i32 0
  store i8 65, ptr %0, align 1
  %1 = getelementptr [14 x i8], ptr %tmp8, i32 0, i32 1
  store i8 65, ptr %1, align 1
  %2 = getelementptr [14 x i8], ptr %tmp8, i32 0, i32 2
  store i8 65, ptr %2, align 1
  %3 = getelementptr [14 x i8], ptr %tmp8, i32 0, i32 3
  store i8 65, ptr %3, align 1
  %4 = getelementptr [14 x i8], ptr %tmp8, i32 0, i32 4
  store i8 65, ptr %4, align 1
  %5 = getelementptr [14 x i8], ptr %tmp8, i32 0, i32 5
  store i8 65, ptr %5, align 1
  %6 = getelementptr [14 x i8], ptr %tmp8, i32 0, i32 6
  store i8 65, ptr %6, align 1
  %7 = getelementptr [14 x i8], ptr %tmp8, i32 0, i32 7
  store i8 65, ptr %7, align 1
  %8 = getelementptr [14 x i8], ptr %tmp8, i32 0, i32 8
  store i8 65, ptr %8, align 1
  %9 = getelementptr [14 x i8], ptr %tmp8, i32 0, i32 9
  store i8 65, ptr %9, align 1
  %10 = getelementptr [14 x i8], ptr %tmp8, i32 0, i32 10
  store i8 65, ptr %10, align 1
  %11 = getelementptr [14 x i8], ptr %tmp8, i32 0, i32 11
  store i8 65, ptr %11, align 1
  %12 = getelementptr [14 x i8], ptr %tmp8, i32 0, i32 12
  store i8 65, ptr %12, align 1
  %13 = getelementptr [14 x i8], ptr %tmp8, i32 0, i32 13
  store i8 65, ptr %13, align 1
  %tmp9 = load [14 x i8], ptr %tmp8, align 1
  %tmp12 = load %tmp2in_addr, ptr %tmp6, align 4
  %14 = getelementptr [8 x i8], ptr %tmp13, i32 0, i32 0
  store i8 65, ptr %14, align 1
  %15 = getelementptr [8 x i8], ptr %tmp13, i32 0, i32 1
  store i8 65, ptr %15, align 1
  %16 = getelementptr [8 x i8], ptr %tmp13, i32 0, i32 2
  store i8 65, ptr %16, align 1
  %17 = getelementptr [8 x i8], ptr %tmp13, i32 0, i32 3
  store i8 65, ptr %17, align 1
  %18 = getelementptr [8 x i8], ptr %tmp13, i32 0, i32 4
  store i8 65, ptr %18, align 1
  %19 = getelementptr [8 x i8], ptr %tmp13, i32 0, i32 5
  store i8 65, ptr %19, align 1
  %20 = getelementptr [8 x i8], ptr %tmp13, i32 0, i32 6
  store i8 65, ptr %20, align 1
  %21 = getelementptr [8 x i8], ptr %tmp13, i32 0, i32 7
  store i8 65, ptr %21, align 1
  %tmp14 = load [8 x i8], ptr %tmp13, align 1
  store ptr @tmp3, ptr %tmp2tmp17, align 8
  store ptr @tmp3, ptr %tmp2tmp18, align 8
  store i64 4096, ptr @tmp2BUFFER_SIZE, align 8
  br label %tmp21

tmp21:                                            ; preds = %tmp2
  store i8 0, ptr %tmp21tmp22, align 1
  store i8 0, ptr %tmp21tmp23, align 1
  store i8 0, ptr %tmp21tmp24, align 1
  store i8 0, ptr %tmp21tmp25, align 1
  store i8 0, ptr %tmp21tmp26, align 1
  store i8 0, ptr %tmp21tmp27, align 1
  store i8 0, ptr %tmp21tmp28, align 1
  store i8 0, ptr %tmp21tmp29, align 1
  store i8 0, ptr %tmp21tmp30, align 1
  store i8 0, ptr %tmp21tmp31, align 1
  %tmp32 = load %tmp2hostent, ptr %tmp19, align 8
  store %tmp2hostent %tmp32, ptr %tmp21tmp33, align 8
  store i8 0, ptr %tmp21tmp34, align 1
  store i8 0, ptr %tmp21tmp35, align 1
  store i8 0, ptr %tmp21tmp36, align 1
  store i8 0, ptr %tmp21tmp37, align 1
  store i8 0, ptr %tmp21tmp38, align 1
  %tmp39 = load %tmp2sockaddr, ptr %tmp10, align 1
  store %tmp2sockaddr %tmp39, ptr %tmp21tmp40, align 1
  %tmp41 = load %tmp2in_addr, ptr %tmp6, align 4
  store %tmp2in_addr %tmp41, ptr %tmp21tmp42, align 4
  %tmp43 = load %tmp2in_addr, ptr %tmp6, align 4
  store %tmp2in_addr %tmp43, ptr %tmp21tmp44, align 4
  br label %tmp45

tmp45:                                            ; preds = %tmp21
  store ptr @malloc, ptr @tmp45stralloc, align 8
  %tmp49 = getelementptr inbounds %tmp47, ptr %tmp48, i32 0, i32 0
  store i32 0, ptr %tmp49, align 4
  %tmp50 = getelementptr inbounds %tmp47, ptr %tmp48, i32 0, i32 1
  store ptr @tmp3, ptr %tmp50, align 8
  %tmp51 = load %tmp47, ptr %tmp48, align 8
  %tmp182 = call %tmp47 @request(ptr @tmp181, i32 80, ptr @tmp174)
  store i32 0, ptr %retvalmainMavenStart, align 4
  store i32 0, ptr %retvalmainMavenStart, align 4
  %tmp183 = load i32, ptr %retvalmainMavenStart, align 4
  ret i32 %tmp183
}

define i32 @main() {
tmp0:
  %tmp1 = call i32 @mainMavenStart()
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

declare i32 @perror(ptr)

declare i32 @exit(i32)

declare i64 @strlen(ptr)

declare ptr @memset(ptr, i32, i64)

declare ptr @strcpy(ptr, ptr)

declare ptr @strncpy(ptr, ptr, i64)

declare ptr @memcpy(ptr, ptr, i32)

declare ptr @malloc(i64)

declare i64 @read(i32, ptr, i64)

declare i64 @write(i32, ptr, i64)

declare i32 @close(i32)

declare ptr @gethostbyname(ptr)

declare i32 @socket(i32, i32, i32)

declare i16 @htons(i32)

declare i32 @bind(i32, ptr, i32)

declare i32 @listen(i32, i32)

declare i32 @accept(i32, ptr, ptr)

declare i64 @send(i32, ptr, i64, i32)

declare i64 @recv(i32, ptr, i64, i32)

declare i32 @connect(i32, ptr, i64)

declare ptr @inet_ntoa(ptr)

declare i32 @inet_pton(i32, ptr, ptr)

declare i16 @ntohs(i16)

declare i32 @shutdown(i32, i32)

declare i32 @snprintf(ptr, i64, ptr, ...)

define %tmp47 @request(ptr %0, i32 %1, ptr %2) {
__allocreigonrequest:
  %tmp52site = alloca ptr, align 8
  %tmp52port = alloca i32, align 4
  %tmp52content = alloca ptr, align 8
  %tmp55 = alloca %tmp2in_addr, align 8, !structee_type !0
  %tmp59 = alloca [8 x i8], align 1
  %tmp62 = alloca %tmp2sockaddr_in, align 8, !structee_type !1
  %tmp52site_addr = alloca %tmp2sockaddr_in, align 8
  %tmp52sockfd = alloca i32, align 4
  %tmp52host = alloca ptr, align 8
  %tmp102 = alloca { ptr, ptr, i32, i32, ptr }, align 8
  %tmp111 = alloca { ptr, ptr, i32, i32, ptr }, align 8
  %tmp118 = alloca { ptr, ptr, i32, i32, ptr }, align 8
  %tmp52conn = alloca i32, align 4
  %tmp52request_len = alloca i64, align 8
  %tmp52request = alloca ptr, align 8
  %tmp52sent = alloca i64, align 8
  %tmp176 = alloca %tmp47, align 8
  %retvalrequest = alloca %tmp47, align 8
  br label %tmp52

tmp52:                                            ; preds = %__allocreigonrequest
  store ptr %0, ptr %tmp52site, align 8
  store i32 %1, ptr %tmp52port, align 4
  store ptr %2, ptr %tmp52content, align 8
  %tmp53 = call i16 @htons(i32 80)
  %tmp54 = load %tmp2in_addr, ptr @tmp7, align 4
  %tmp56 = getelementptr inbounds %tmp2in_addr, ptr %tmp55, i32 0, i32 0
  store i32 0, ptr %tmp56, align 4
  %tmp58 = load %tmp2in_addr, ptr %tmp55, align 4
  %3 = getelementptr [8 x i8], ptr %tmp59, i32 0, i32 0
  store i8 65, ptr %3, align 1
  %4 = getelementptr [8 x i8], ptr %tmp59, i32 0, i32 1
  store i8 65, ptr %4, align 1
  %5 = getelementptr [8 x i8], ptr %tmp59, i32 0, i32 2
  store i8 65, ptr %5, align 1
  %6 = getelementptr [8 x i8], ptr %tmp59, i32 0, i32 3
  store i8 65, ptr %6, align 1
  %7 = getelementptr [8 x i8], ptr %tmp59, i32 0, i32 4
  store i8 65, ptr %7, align 1
  %8 = getelementptr [8 x i8], ptr %tmp59, i32 0, i32 5
  store i8 65, ptr %8, align 1
  %9 = getelementptr [8 x i8], ptr %tmp59, i32 0, i32 6
  store i8 65, ptr %9, align 1
  %10 = getelementptr [8 x i8], ptr %tmp59, i32 0, i32 7
  store i8 65, ptr %10, align 1
  %tmp60 = load [8 x i8], ptr %tmp59, align 1
  %tmp61 = load %tmp2sockaddr_in, ptr @tmp16, align 4
  %tmp63 = getelementptr inbounds %tmp2sockaddr_in, ptr %tmp62, i32 0, i32 0
  store i8 0, ptr %tmp63, align 1
  %tmp64 = getelementptr inbounds %tmp2sockaddr_in, ptr %tmp62, i32 0, i32 1
  store i8 0, ptr %tmp64, align 1
  %tmp65 = getelementptr inbounds %tmp2sockaddr_in, ptr %tmp62, i32 0, i32 2
  store i16 %tmp53, ptr %tmp65, align 2
  %tmp66 = getelementptr inbounds %tmp2sockaddr_in, ptr %tmp62, i32 0, i32 3
  store %tmp2in_addr %tmp58, ptr %tmp66, align 4
  %tmp67 = getelementptr inbounds %tmp2sockaddr_in, ptr %tmp62, i32 0, i32 4
  store [8 x i8] %tmp60, ptr %tmp67, align 1
  %tmp69 = load %tmp2sockaddr_in, ptr %tmp62, align 4
  store %tmp2sockaddr_in %tmp69, ptr %tmp52site_addr, align 4
  %tmp70 = call i32 @socket(i32 2, i32 1, i32 0)
  store i32 %tmp70, ptr %tmp52sockfd, align 4
  %tmp72 = load i32, ptr %tmp52sockfd, align 4
  %tmp73 = call i32 (ptr, ...) @printf(ptr @tmp71, i32 %tmp72)
  %tmp74 = load i32, ptr %tmp52sockfd, align 4
  %tmp75 = icmp ne i32 %tmp74, -1
  %tmp77 = call ptr @"5673998842831842134Assert"(i1 %tmp75, ptr @tmp76)
  %tmp78 = load ptr, ptr %tmp52site, align 8
  %tmp79 = call ptr @gethostbyname(ptr %tmp78)
  store ptr %tmp79, ptr %tmp52host, align 8
  %tmp80 = load ptr, ptr %tmp52host, align 8
  %tmp81 = icmp ne ptr %tmp80, null
  %tmp86 = call ptr @"5673998842831842134AssertDefer"(i1 %tmp81, ptr @anonfn1)
  %tmp88 = load ptr, ptr %tmp52host, align 8
  %tmp89 = call i32 (ptr, ...) @printf(ptr @tmp87, ptr %tmp88)
  %tmp91 = load %tmp2sockaddr_in, ptr @tmp16, align 4
  %tmp92 = call ptr @memset(ptr %tmp52site_addr, i32 0, i64 16)
  %tmp94 = getelementptr inbounds { i8, i8, i16, { i32 }, [8 x i8] }, ptr %tmp52site_addr, i32 0, i32 1
  store i8 2, ptr %tmp94, align 1
  %tmp96 = getelementptr inbounds { i8, i8, i16, { i32 }, [8 x i8] }, ptr %tmp52site_addr, i32 0, i32 2
  %tmp97 = load i32, ptr %tmp52port, align 4
  %tmp98 = call i16 @htons(i32 %tmp97)
  store i16 %tmp98, ptr %tmp96, align 2
  %tmp100 = load ptr, ptr %tmp52host, align 8
  %dereftmp101 = load { ptr, ptr, i32, i32, ptr }, ptr %tmp100, align 8
  store { ptr, ptr, i32, i32, ptr } %dereftmp101, ptr %tmp102, align 8
  %tmp103 = getelementptr inbounds { ptr, ptr, i32, i32, ptr }, ptr %tmp102, i32 0, i32 3
  %tmp104 = load i32, ptr %tmp103, align 4
  %tmp105 = call i32 (ptr, ...) @printf(ptr @tmp99, i32 %tmp104)
  %tmp107 = getelementptr inbounds { i8, i8, i16, { i32 }, [8 x i8] }, ptr %tmp52site_addr, i32 0, i32 3
  %tmp108 = getelementptr inbounds { i32 }, ptr %tmp107, i32 0, i32 0
  %tmp109 = load ptr, ptr %tmp52host, align 8
  %dereftmp110 = load { ptr, ptr, i32, i32, ptr }, ptr %tmp109, align 8
  store { ptr, ptr, i32, i32, ptr } %dereftmp110, ptr %tmp111, align 8
  %tmp112 = getelementptr inbounds { ptr, ptr, i32, i32, ptr }, ptr %tmp111, i32 0, i32 4
  %tmp113 = getelementptr ptr, ptr %tmp112, i32 0
  %tmp114 = load ptr, ptr %tmp113, align 8
  %dereftmp115 = load ptr, ptr %tmp114, align 8
  %tmp116 = load ptr, ptr %tmp52host, align 8
  %dereftmp117 = load { ptr, ptr, i32, i32, ptr }, ptr %tmp116, align 8
  store { ptr, ptr, i32, i32, ptr } %dereftmp117, ptr %tmp118, align 8
  %tmp119 = getelementptr inbounds { ptr, ptr, i32, i32, ptr }, ptr %tmp118, i32 0, i32 3
  %tmp120 = load i32, ptr %tmp119, align 4
  %tmp121 = call ptr @memcpy(ptr %tmp108, ptr %dereftmp115, i32 %tmp120)
  %tmp123 = call i32 @"5673998842831842134println"(ptr @tmp122)
  %tmp124 = load i32, ptr %tmp52sockfd, align 4
  %tmp126 = load %tmp2sockaddr_in, ptr %tmp52site_addr, align 4
  %tmp127 = call i32 @connect(i32 %tmp124, ptr %tmp52site_addr, i64 16)
  store i32 %tmp127, ptr %tmp52conn, align 4
  %tmp129 = load i32, ptr %tmp52conn, align 4
  %tmp130 = call i32 (ptr, ...) @printf(ptr @tmp128, i32 %tmp129)
  %tmp131 = load i32, ptr %tmp52conn, align 4
  %tmp132 = icmp eq i32 %tmp131, 0
  %tmp134 = call ptr @"5673998842831842134Assert"(i1 %tmp132, ptr @tmp133)
  %tmp136 = call i64 @strlen(ptr @tmp135)
  %tmp137 = load ptr, ptr %tmp52site, align 8
  %tmp138 = call i64 @strlen(ptr %tmp137)
  %tmp139 = add i64 %tmp136, %tmp138
  store i64 %tmp139, ptr %tmp52request_len, align 8
  %tmp140 = load i64, ptr %tmp52request_len, align 8
  %tmp141 = add i64 %tmp140, 1
  %tmp142 = load ptr, ptr @tmp45stralloc, align 8
  %tmp143 = call ptr %tmp142(i64 %tmp141)
  store ptr %tmp143, ptr %tmp52request, align 8
  %tmp144 = load ptr, ptr %tmp52request, align 8
  %tmp145 = load i64, ptr %tmp52request_len, align 8
  %tmp148 = load ptr, ptr %tmp52site, align 8
  %tmp149 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr %tmp144, i64 %tmp145, ptr @tmp146, ptr @tmp147, ptr %tmp148)
  %tmp151 = load ptr, ptr %tmp52request, align 8
  %tmp152 = call i32 (ptr, ...) @printf(ptr @tmp150, ptr %tmp151)
  %tmp154 = load i64, ptr %tmp52request_len, align 8
  %tmp155 = call i32 (ptr, ...) @printf(ptr @tmp153, i64 %tmp154)
  %tmp156 = load ptr, ptr %tmp52request, align 8
  %tmp157 = call i64 @strlen(ptr %tmp156)
  %tmp158 = call i32 (ptr, ...) @printf(ptr @tmp153, i64 %tmp157)
  %tmp159 = load i32, ptr %tmp52sockfd, align 4
  %tmp160 = load ptr, ptr %tmp52request, align 8
  %tmp161 = load ptr, ptr %tmp52request, align 8
  %tmp162 = call i64 @strlen(ptr %tmp161)
  %tmp163 = call i64 @send(i32 %tmp159, ptr %tmp160, i64 %tmp162, i32 0)
  store i64 %tmp163, ptr %tmp52sent, align 8
  %tmp165 = call i32 @perror(ptr @tmp164)
  %tmp167 = call i32 (ptr, ...) @printf(ptr @tmp166, ptr @send)
  %tmp168 = load i64, ptr %tmp52sent, align 8
  %tmp169 = icmp ne i64 %tmp168, -1
  %tmp171 = call ptr @"5673998842831842134Assert"(i1 %tmp169, ptr @tmp170)
  %tmp172 = load i32, ptr %tmp52sockfd, align 4
  %tmp173 = call i32 @close(i32 %tmp172)
  %tmp177 = getelementptr inbounds %tmp47, ptr %tmp176, i32 0, i32 0
  store i32 0, ptr %tmp177, align 4
  %tmp178 = getelementptr inbounds %tmp47, ptr %tmp176, i32 0, i32 1
  store ptr @tmp174, ptr %tmp178, align 8
  %tmp179 = load %tmp47, ptr %tmp176, align 8
  store %tmp47 %tmp179, ptr %retvalrequest, align 8
  %tmp180 = load %tmp47, ptr %retvalrequest, align 8
  ret %tmp47 %tmp180
}

define internal ptr @anonfn1() {
__allocreigonanonfn1:
  br label %tmp82

tmp82:                                            ; preds = %__allocreigonanonfn1
  %tmp84 = call i32 @"5673998842831842134println"(ptr @tmp83)
  ret ptr @tmp85
}

!0 = !{!"tmp57"}
!1 = !{!"tmp68"}
