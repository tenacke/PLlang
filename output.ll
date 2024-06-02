; ModuleID = 'mypl'
declare i32 @printf(i8*, ...)
declare i32 @scanf(i8*, ...)
@readwrite.str = constant [3 x i8] c"%d\00"
@writeln.str = constant [4 x i8] c"%d\0A\00"

declare void @llvm.memcpy.p0.p0.i64(ptr, ptr, i64, i1)

@__literal1 = constant [2 x i32] [i32 5, i32 2]
@max = constant [2 x i32] [i32 200, i32 3300]
@cu = constant i32 3
@arg = global i32 0
@ret = global i32 0
@emre = global [3 x i32] zeroinitializer

define i32 @isOdd(i32 %0) {
   %2 = alloca i32
   store i32 %0, i32* %2
   %3 = load i32, i32* %2
   %4 = srem i32 %3, 2
   %5 = icmp eq i32 %4, 1
   br i1 %5, label %6, label %8
6:
   ret i32 1
   br label %10
8:
   ret i32 0
   br label %10
10:
   ret i32 0
}

define void @isprime() {
   %1 = alloca i32
   %2 = alloca i32
   store i32 1, i32* @ret
   store i32 2, i32* %1
   br label %3
3:
   %4 = load i32, i32* %1
   %5 = load i32, i32* @arg
   %6 = icmp slt i32 %4, %5
   br i1 %6, label %7, label %19
7:
   %8 = load i32, i32* @arg
   %9 = load i32, i32* %1
   %10 = srem i32 %8, %9
   %11 = icmp eq i32 %10, 0
   br i1 %11, label %12, label %14
12:
   store i32 0, i32* @ret
   %13 = load i32, i32* @arg
   store i32 %13, i32* %1
   br label %18
14:
   %15 = load i32, i32* %1
   %16 = add i32 %15, 1
   store i32 %16, i32* %1
   br label %19
   br label %18
18:
   br label %3
19:
   store i32 5, i32* %2
   br label %20
20:
   %21 = load i32, i32* %2
   %22 = icmp slt i32 %21, 10
   br i1 %22, label %23, label %28
23:
   %24 = load i32, i32* %2
   %25 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @writeln.str, i32 0, i32 0), i32 %24)
   %26 = load i32, i32* %2
   %27 = add i32 %26, 1
   store i32 %27, i32* %2
   br label %20
28:
   ret void
}

define void @primes() {
   %1 = alloca i32
   %2 = alloca [2 x i32]
   call void @llvm.memcpy.p0.p0.i64(ptr %2, ptr @__literal1, i64 8, i1 false)
   %3 = getelementptr inbounds [2 x i32], [2 x i32]* %2, i32 0, i32 0
   %4 = load i32, i32* %3
   store i32 %4, i32* %1
   %5 = load i32, i32* %1
   %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @writeln.str, i32 0, i32 0), i32 %5)
   store i32 2, i32* @arg
   br label %7
7:
   %8 = load i32, i32* @arg
   %9 = load i32, i32* @cu
   %10 = icmp slt i32 %8, %9
   br i1 %10, label %11, label %22
11:
   call void @isprime()
   %12 = load i32, i32* @ret
   %13 = icmp eq i32 %12, 1
   br i1 %13, label %14, label %17
14:
   %15 = load i32, i32* @arg
   %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @writeln.str, i32 0, i32 0), i32 %15)
   br label %17
17:
   %18 = load i32, i32* @arg
   %19 = add i32 %18, 1
   store i32 %19, i32* @arg
   %20 = load i32, i32* @arg
   %21 = call i32 @isOdd(i32 %20)
   store i32 %21, i32* getelementptr inbounds ([3 x i32], [3 x i32]* @emre, i32 0, i32 1)
   br label %7
22:
   ret void
}

define i32 @main() {
   call void @primes()
   ret i32 0
}
