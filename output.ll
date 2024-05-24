; ModuleID = 'calculator'
declare i32 @printf(i8*, ...)
declare i32 @scanf(i8*, ...)
@read.str = constant [3 x i8] c"%d\00"
@print.str = constant [4 x i8] c"%d\0A\00"

@cu = constant i32 3
@arg = global i32 0
@ret = global i32 0
@emre = global [3 x i32] zeroinitializer

define i32 @isOdd(number) {
  br label %if1

if1:
  %1 = load i32, i32* 
  %2 = srem i32 %1, 2
  br i1 %2, label %if2, label %if3

if2:
  ret i32 1
  br label %if4

if3:
  ret i32 0
  br label %if4

if4:


  ret i32 0
}

define void @isprime() {
  %1 = alloca i32
  %2 = alloca i32
  %3 = alloca [2 x i32]
  %4 = load i32, i32* @ret
  store i32 1, i32* 
  %5 = load i32, i32* %1
  store i32 2, i32* 
  br label %while5

while5:
  %6 = load i32, i32* %1
  %7 = load i32, i32* @arg
  %8 = icmp slt i32 %6, %7
  br i1 %8, label %while6, label %while7

while6:
  br label %if1

if1:
  %9 = load i32, i32* @arg
  %10 = load i32, i32* %1
  %11 = srem i32 %9, %10
  %12 = icmp eq i32 %11, 0
  br i1 %12, label %if2, label %if3

if2:
  %13 = getelementptr inbounds[3 x i32], i32* @emre, i32 2
  store i32 0, i32* 
  %14 = getelementptr inbounds[2 x i32], i32* %3, i32 0
  store i32 2, i32* 
  %15 = load i32, i32* @ret
  store i32 0, i32* 
  %16 = load i32, i32* %1
  %17 = load i32, i32* @arg
  store i32 %17, i32* 
  br label %if4

if3:
  %18 = load i32, i32* %1
  %19 = load i32, i32* %1
  %20 = add i32 %19, 1
  store i32 %20, i32* 
  br label %if4

if4:

  br label %while5

while7:


  ret void
}

define void @primes() {
  %1 = load i32, i32* @arg
  store i32 2, i32* 
  br label %while4

while4:
  %2 = load i32, i32* @arg
  %3 = load i32, i32* @cu
  %4 = icmp slt i32 %2, %3
  br i1 %4, label %while5, label %while6

while5:
  call void @isprime()
  br label %if1

if1:
  %5 = load i32, i32* @ret
  %6 = icmp eq i32 %5, 1
  br i1 %6, label %if2, label %if3

if2:
  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @read.str, i32 0, i32 0), i32* )
  br label %if3

if3:

  %7 = load i32, i32* @arg
  %8 = load i32, i32* @arg
  %9 = load i32, i32* @arg
  %10 = add i32 %9, 1
  store i32 %10, i32* 
  %11 = getelementptr inbounds[3 x i32], i32* @emre, i32 1
  %12 = load i32, i32* @arg
  store i32 isOdd, i32* 
  br label %while4

while6:


  ret void
}

define i32 @main() {
  call void @primes()

  ret i32 0
}
