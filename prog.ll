; ModuleID = 'prog.ll'
source_filename = "prog.ll"

@readwrite.str = constant [3 x i8] c"%d\00"
@writeln.str = constant [4 x i8] c"%d\0A\00"
@__literal1 = constant [2 x i32] [i32 5, i32 2]
@max = constant [2 x i32] [i32 200, i32 3300]
@cu = constant i32 3
@arg = global i32 0
@ret = global i32 0
@emre = global [3 x i32] zeroinitializer

declare i32 @printf(ptr, ...)

declare i32 @scanf(ptr, ...)

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #0

define i32 @isOdd(i32 %0) {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %3 = load i32, ptr %2, align 4
  %4 = srem i32 %3, 2
  %5 = icmp eq i32 %4, 1
  br i1 %5, label %6, label %8

6:                                                ; preds = %1
  ret i32 1

7:                                                ; No predecessors!
  br label %10

8:                                                ; preds = %1
  ret i32 0

9:                                                ; No predecessors!
  br label %10

10:                                               ; preds = %9, %7
  ret i32 0
}

define void @isprime() {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 1, ptr @ret, align 4
  store i32 2, ptr %1, align 4
  br label %3

3:                                                ; preds = %18, %0
  %4 = load i32, ptr %1, align 4
  %5 = load i32, ptr @arg, align 4
  %6 = icmp slt i32 %4, %5
  br i1 %6, label %7, label %19

7:                                                ; preds = %3
  %8 = load i32, ptr @arg, align 4
  %9 = load i32, ptr %1, align 4
  %10 = srem i32 %8, %9
  %11 = icmp eq i32 %10, 0
  br i1 %11, label %12, label %14

12:                                               ; preds = %7
  store i32 0, ptr @ret, align 4
  %13 = load i32, ptr @arg, align 4
  store i32 %13, ptr %1, align 4
  br label %18

14:                                               ; preds = %7
  %15 = load i32, ptr %1, align 4
  %16 = add i32 %15, 1
  store i32 %16, ptr %1, align 4
  br label %19

17:                                               ; No predecessors!
  br label %18

18:                                               ; preds = %17, %12
  br label %3

19:                                               ; preds = %14, %3
  store i32 5, ptr %2, align 4
  br label %20

20:                                               ; preds = %23, %19
  %21 = load i32, ptr %2, align 4
  %22 = icmp slt i32 %21, 10
  br i1 %22, label %23, label %28

23:                                               ; preds = %20
  %24 = load i32, ptr %2, align 4
  %25 = call i32 (ptr, ...) @printf(ptr @writeln.str, i32 %24)
  %26 = load i32, ptr %2, align 4
  %27 = add i32 %26, 1
  store i32 %27, ptr %2, align 4
  br label %20

28:                                               ; preds = %20
  ret void
}

define void @primes() {
  %1 = alloca i32, align 4
  %2 = alloca [2 x i32], align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %2, ptr @__literal1, i64 8, i1 false)
  %3 = getelementptr inbounds [2 x i32], ptr %2, i32 0, i32 0
  %4 = load i32, ptr %3, align 4
  store i32 %4, ptr %1, align 4
  %5 = load i32, ptr %1, align 4
  %6 = call i32 (ptr, ...) @printf(ptr @writeln.str, i32 %5)
  store i32 2, ptr @arg, align 4
  br label %7

7:                                                ; preds = %17, %0
  %8 = load i32, ptr @arg, align 4
  %9 = load i32, ptr @cu, align 4
  %10 = icmp slt i32 %8, %9
  br i1 %10, label %11, label %22

11:                                               ; preds = %7
  call void @isprime()
  %12 = load i32, ptr @ret, align 4
  %13 = icmp eq i32 %12, 1
  br i1 %13, label %14, label %17

14:                                               ; preds = %11
  %15 = load i32, ptr @arg, align 4
  %16 = call i32 (ptr, ...) @printf(ptr @writeln.str, i32 %15)
  br label %17

17:                                               ; preds = %14, %11
  %18 = load i32, ptr @arg, align 4
  %19 = add i32 %18, 1
  store i32 %19, ptr @arg, align 4
  %20 = load i32, ptr @arg, align 4
  %21 = call i32 @isOdd(i32 %20)
  store i32 %21, ptr getelementptr inbounds ([3 x i32], ptr @emre, i32 0, i32 1), align 4
  br label %7

22:                                               ; preds = %7
  ret void
}

define i32 @main() {
  call void @primes()
  ret i32 0
}

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
