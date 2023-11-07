; ModuleID = 'loops.c'
source_filename = "loops.c"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.37.32822"

$printf = comdat any

$__local_stdio_printf_options = comdat any

$"??_C@_0BE@IBJOJFBB@A1?9A2?5mismatch?3?5?$CFd?6?$AA@" = comdat any

$"??_C@_0BE@IMKAICCJ@A2?9A3?5mismatch?3?5?$CFd?6?$AA@" = comdat any

$"??_C@_0BE@NMOCCHBC@A3?9A1?5mismatch?3?5?$CFd?6?$AA@" = comdat any

$"??_C@_0O@ECOBCFJO@?5?3error?5found?$AA@" = comdat any

$"??_C@_0M@JFAEHDGC@?5?3result?5OK?$AA@" = comdat any

$"??_C@_0BK@JEPGKCBD@loops?4c?5check?5complete?$CFs?6?$AA@" = comdat any

@"??_C@_0BE@IBJOJFBB@A1?9A2?5mismatch?3?5?$CFd?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [20 x i8] c"A1-A2 mismatch: %d\0A\00", comdat, align 1
@"??_C@_0BE@IMKAICCJ@A2?9A3?5mismatch?3?5?$CFd?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [20 x i8] c"A2-A3 mismatch: %d\0A\00", comdat, align 1
@"??_C@_0BE@NMOCCHBC@A3?9A1?5mismatch?3?5?$CFd?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [20 x i8] c"A3-A1 mismatch: %d\0A\00", comdat, align 1
@"??_C@_0O@ECOBCFJO@?5?3error?5found?$AA@" = linkonce_odr dso_local unnamed_addr constant [14 x i8] c" :error found\00", comdat, align 1
@"??_C@_0M@JFAEHDGC@?5?3result?5OK?$AA@" = linkonce_odr dso_local unnamed_addr constant [12 x i8] c" :result OK\00", comdat, align 1
@"??_C@_0BK@JEPGKCBD@loops?4c?5check?5complete?$CFs?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [26 x i8] c"loops.c check complete%s\0A\00", comdat, align 1
@__local_stdio_printf_options._OptionsStorage = internal global i64 0, align 8
@str = private unnamed_addr constant [14 x i8] c"loops.c start\00", align 1
@str.1 = private unnamed_addr constant [33 x i8] c"func_orig _complete result is A1\00", align 1
@str.2 = private unnamed_addr constant [34 x i8] c"func_pragma complete result is A2\00", align 1
@str.3 = private unnamed_addr constant [31 x i8] c"func_pipe_by_hand result is A3\00", align 1

; Function Attrs: nofree noinline norecurse nosync nounwind memory(argmem: readwrite) uwtable
define dso_local void @func_parallel(i64 noundef %0, ptr noalias nocapture noundef %1, ptr noalias nocapture noundef readonly %2, ptr noalias nocapture noundef readonly %3, ptr noalias nocapture noundef readonly %4) local_unnamed_addr #0 {
  %6 = icmp eq i64 %0, 0
  br i1 %6, label %41, label %7

7:                                                ; preds = %5
  %8 = icmp ult i64 %0, 8
  br i1 %8, label %39, label %9

9:                                                ; preds = %7
  %10 = and i64 %0, -8
  br label %11

11:                                               ; preds = %11, %9
  %12 = phi i64 [ 0, %9 ], [ %35, %11 ]
  %13 = getelementptr inbounds float, ptr %1, i64 %12
  %14 = load <4 x float>, ptr %13, align 4, !tbaa !4
  %15 = getelementptr inbounds float, ptr %13, i64 4
  %16 = load <4 x float>, ptr %15, align 4, !tbaa !4
  %17 = getelementptr inbounds float, ptr %2, i64 %12
  %18 = load <4 x float>, ptr %17, align 4, !tbaa !4
  %19 = getelementptr inbounds float, ptr %17, i64 4
  %20 = load <4 x float>, ptr %19, align 4, !tbaa !4
  %21 = fadd <4 x float> %14, %18
  %22 = fadd <4 x float> %16, %20
  %23 = getelementptr inbounds float, ptr %3, i64 %12
  %24 = load <4 x float>, ptr %23, align 4, !tbaa !4
  %25 = getelementptr inbounds float, ptr %23, i64 4
  %26 = load <4 x float>, ptr %25, align 4, !tbaa !4
  %27 = fadd <4 x float> %21, %24
  %28 = fadd <4 x float> %22, %26
  %29 = getelementptr inbounds float, ptr %4, i64 %12
  %30 = load <4 x float>, ptr %29, align 4, !tbaa !4
  %31 = getelementptr inbounds float, ptr %29, i64 4
  %32 = load <4 x float>, ptr %31, align 4, !tbaa !4
  %33 = fadd <4 x float> %27, %30
  %34 = fadd <4 x float> %28, %32
  store <4 x float> %33, ptr %13, align 4, !tbaa !4
  store <4 x float> %34, ptr %15, align 4, !tbaa !4
  %35 = add nuw i64 %12, 8
  %36 = icmp eq i64 %35, %10
  br i1 %36, label %37, label %11, !llvm.loop !8

37:                                               ; preds = %11
  %38 = icmp eq i64 %10, %0
  br i1 %38, label %41, label %39

39:                                               ; preds = %7, %37
  %40 = phi i64 [ 0, %7 ], [ %10, %37 ]
  br label %42

41:                                               ; preds = %42, %37, %5
  ret void

42:                                               ; preds = %39, %42
  %43 = phi i64 [ %55, %42 ], [ %40, %39 ]
  %44 = getelementptr inbounds float, ptr %1, i64 %43
  %45 = load float, ptr %44, align 4, !tbaa !4
  %46 = getelementptr inbounds float, ptr %2, i64 %43
  %47 = load float, ptr %46, align 4, !tbaa !4
  %48 = fadd float %45, %47
  %49 = getelementptr inbounds float, ptr %3, i64 %43
  %50 = load float, ptr %49, align 4, !tbaa !4
  %51 = fadd float %48, %50
  %52 = getelementptr inbounds float, ptr %4, i64 %43
  %53 = load float, ptr %52, align 4, !tbaa !4
  %54 = fadd float %51, %53
  store float %54, ptr %44, align 4, !tbaa !4
  %55 = add nuw i64 %43, 1
  %56 = icmp eq i64 %55, %0
  br i1 %56, label %41, label %42, !llvm.loop !12
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #1

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr nocapture) #1

; Function Attrs: nofree noinline norecurse nosync nounwind memory(argmem: readwrite) uwtable
define dso_local void @func_orig(i64 noundef %0, ptr noalias nocapture noundef %1, ptr noalias nocapture noundef readonly %2, ptr noalias nocapture noundef readonly %3) local_unnamed_addr #0 {
  %5 = icmp eq i64 %0, 0
  br i1 %5, label %8, label %6

6:                                                ; preds = %4
  %7 = load float, ptr %1, align 4, !tbaa !4
  br label %9

8:                                                ; preds = %9, %4
  ret void

9:                                                ; preds = %6, %9
  %10 = phi float [ %17, %9 ], [ %7, %6 ]
  %11 = phi i64 [ %18, %9 ], [ 0, %6 ]
  %12 = getelementptr inbounds float, ptr %3, i64 %11
  %13 = load float, ptr %12, align 4, !tbaa !4
  %14 = getelementptr inbounds float, ptr %2, i64 %11
  %15 = load float, ptr %14, align 4, !tbaa !4
  %16 = fadd float %13, %15
  %17 = fadd float %16, %10
  %18 = add nuw i64 %11, 1
  %19 = getelementptr inbounds float, ptr %1, i64 %18
  store float %17, ptr %19, align 4, !tbaa !4
  %20 = icmp eq i64 %18, %0
  br i1 %20, label %8, label %9, !llvm.loop !13
}

; Function Attrs: nofree noinline norecurse nosync nounwind memory(argmem: readwrite) uwtable
define dso_local void @func_pragma(i64 noundef %0, ptr noalias nocapture noundef %1, ptr noalias nocapture noundef readonly %2, ptr noalias nocapture noundef readonly %3) local_unnamed_addr #0 {
;size_tが<Nかどうか
  %5 = icmp eq i64 %0, 0
  br i1 %5, label %8, label %6 ;br condition THEN, ELSE

6:                                                ; preds = %4
;初期化?
  %7 = load float, ptr %1, align 4, !tbaa !4
  br label %9

8:                                                ; preds = %9, %4
  ret void

9:                                                ; preds = %6, %9
;phi命令とは、条件分岐時にresult = [result1、then], [result2, else]
  %10 = phi float [ %17, %9 ], [ %7, %6 ] ;A[i]の値
  %11 = phi i64 [ %18, %9 ], [ 0, %6 ] ;初めはi=0, 以降はi=i
  %12 = getelementptr inbounds float, ptr %3, i64 %11
  %13 = load float, ptr %12, align 4, !tbaa !4
  %14 = getelementptr inbounds float, ptr %2, i64 %11
  %15 = load float, ptr %14, align 4, !tbaa !4

  %17 = fadd float %16, %10 ;(C[i] + B[i]) + A[i]
  %16 = fadd float %13, %15 ;C[i] + B[i]

  %18 = add nuw i64 %11, 1 ;++i
  %19 = getelementptr inbounds float, ptr %1, i64 %18
  store float %17, ptr %19, align 4, !tbaa !4 ;A[i+1] = (C[i] + B[i]) + A[i]

 ;条件比較
  %20 = icmp eq i64 %18, %0
  br i1 %20, label %8, label %9, !llvm.loop !14
}

; Function Attrs: nofree noinline norecurse nosync nounwind memory(argmem: readwrite) uwtable
define dso_local void @func_pipe_by_hand(i64 noundef %0, ptr noalias nocapture noundef %1, ptr noalias nocapture noundef readonly %2, ptr noalias nocapture noundef readonly %3) local_unnamed_addr #0 {
  %5 = load float, ptr %3, align 4, !tbaa !4
  %6 = load float, ptr %2, align 4, !tbaa !4
  %7 = fadd float %5, %6
  %8 = icmp ugt i64 %0, 1
  br i1 %8, label %9, label %11

9:                                                ; preds = %4
  %10 = load float, ptr %1, align 4, !tbaa !4
  br label %18

11:                                               ; preds = %18, %4
  %12 = phi float [ %7, %4 ], [ %28, %18 ]
  %13 = add i64 %0, -1
  %14 = getelementptr inbounds float, ptr %1, i64 %13
  %15 = load float, ptr %14, align 4, !tbaa !4
  %16 = fadd float %12, %15
  %17 = getelementptr inbounds float, ptr %1, i64 %0
  store float %16, ptr %17, align 4, !tbaa !4
  ret void

18:                                               ; preds = %9, %18
  %19 = phi float [ %22, %18 ], [ %10, %9 ]
  %20 = phi i64 [ %29, %18 ], [ 1, %9 ]
  %21 = phi float [ %28, %18 ], [ %7, %9 ]
  %22 = fadd float %21, %19
  %23 = getelementptr inbounds float, ptr %1, i64 %20
  store float %22, ptr %23, align 4, !tbaa !4
  %24 = getelementptr inbounds float, ptr %3, i64 %20
  %25 = load float, ptr %24, align 4, !tbaa !4
  %26 = getelementptr inbounds float, ptr %2, i64 %20
  %27 = load float, ptr %26, align 4, !tbaa !4
  %28 = fadd float %25, %27
  %29 = add nuw i64 %20, 1
  %30 = icmp eq i64 %29, %0
  br i1 %30, label %11, label %18, !llvm.loop !16
}

; Function Attrs: nounwind uwtable
define dso_local i32 @main(i32 noundef %0, ptr nocapture noundef readonly %1) local_unnamed_addr #2 {
  %3 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  %4 = icmp slt i32 %0, 3
  br i1 %4, label %95, label %5

5:                                                ; preds = %2
  %6 = getelementptr inbounds ptr, ptr %1, i64 1
  %7 = load ptr, ptr %6, align 8, !tbaa !17
  %8 = tail call i32 @atoi(ptr nocapture noundef %7)
  %9 = sext i32 %8 to i64
  %10 = getelementptr inbounds ptr, ptr %1, i64 2
  %11 = load ptr, ptr %10, align 8, !tbaa !17
  %12 = tail call i32 @atoi(ptr nocapture noundef %11)
  %13 = sext i32 %12 to i64
  %14 = sub nsw i64 %13, %9
  %15 = tail call noalias ptr @calloc(i64 noundef 4, i64 noundef %14) #11
  %16 = tail call noalias ptr @calloc(i64 noundef 4, i64 noundef %14) #11
  %17 = tail call noalias ptr @calloc(i64 noundef 4, i64 noundef %14) #11
  %18 = tail call noalias ptr @calloc(i64 noundef 4, i64 noundef %14) #11
  %19 = tail call noalias ptr @calloc(i64 noundef 4, i64 noundef %14) #11
  %20 = icmp eq i32 %12, %8
  br i1 %20, label %60, label %21

21:                                               ; preds = %5
  %22 = uitofp i64 %9 to float
  %23 = tail call i64 @llvm.umax.i64(i64 %14, i64 1)
  %24 = icmp ult i64 %23, 4
  br i1 %24, label %41, label %25

25:                                               ; preds = %21
  %26 = and i64 %23, -4
  %27 = insertelement <4 x float> poison, float %22, i64 0
  %28 = shufflevector <4 x float> %27, <4 x float> poison, <4 x i32> zeroinitializer
  br label %29

29:                                               ; preds = %29, %25
  %30 = phi i64 [ 0, %25 ], [ %36, %29 ]
  %31 = phi <4 x i64> [ <i64 0, i64 1, i64 2, i64 3>, %25 ], [ %37, %29 ]
  %32 = uitofp <4 x i64> %31 to <4 x float>
  %33 = fadd <4 x float> %28, %32
  %34 = getelementptr inbounds float, ptr %18, i64 %30
  store <4 x float> %33, ptr %34, align 4, !tbaa !4
  %35 = getelementptr inbounds float, ptr %19, i64 %30
  store <4 x float> %33, ptr %35, align 4, !tbaa !4
  %36 = add nuw i64 %30, 4
  %37 = add <4 x i64> %31, <i64 4, i64 4, i64 4, i64 4>
  %38 = icmp eq i64 %36, %26
  br i1 %38, label %39, label %29, !llvm.loop !19

39:                                               ; preds = %29
  %40 = icmp eq i64 %23, %26
  br i1 %40, label %43, label %41

41:                                               ; preds = %21, %39
  %42 = phi i64 [ 0, %21 ], [ %26, %39 ]
  br label %50

43:                                               ; preds = %50, %39
  %44 = add nsw i64 %14, -1
  tail call void @func_orig(i64 noundef %44, ptr noundef %15, ptr noundef %18, ptr noundef %19)
  %45 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.1)
  tail call void @func_pragma(i64 noundef %44, ptr noundef %16, ptr noundef %18, ptr noundef %19)
  %46 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.2)
  tail call void @func_pipe_by_hand(i64 noundef %44, ptr noundef %17, ptr noundef %18, ptr noundef %19)
  %47 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.3)
  br i1 %20, label %65, label %48

48:                                               ; preds = %43
  %49 = tail call i64 @llvm.umax.i64(i64 %14, i64 1)
  br label %69

50:                                               ; preds = %41, %50
  %51 = phi i64 [ %56, %50 ], [ %42, %41 ]
  %52 = uitofp i64 %51 to float
  %53 = fadd float %22, %52
  %54 = getelementptr inbounds float, ptr %18, i64 %51
  store float %53, ptr %54, align 4, !tbaa !4
  %55 = getelementptr inbounds float, ptr %19, i64 %51
  store float %53, ptr %55, align 4, !tbaa !4
  %56 = add nuw i64 %51, 1
  %57 = icmp eq i64 %56, %23
  br i1 %57, label %43, label %50, !llvm.loop !20

58:                                               ; preds = %91
  %59 = icmp eq i64 %92, 0
  br i1 %59, label %65, label %66

60:                                               ; preds = %5
  %61 = add nsw i64 %14, -1
  tail call void @func_orig(i64 noundef %61, ptr noundef %15, ptr noundef %18, ptr noundef %19)
  %62 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.1)
  tail call void @func_pragma(i64 noundef %61, ptr noundef %16, ptr noundef %18, ptr noundef %19)
  %63 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.2)
  tail call void @func_pipe_by_hand(i64 noundef %61, ptr noundef %17, ptr noundef %18, ptr noundef %19)
  %64 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.3)
  br label %65

65:                                               ; preds = %60, %43, %58
  br label %66

66:                                               ; preds = %58, %65
  %67 = phi ptr [ @"??_C@_0M@JFAEHDGC@?5?3result?5OK?$AA@", %65 ], [ @"??_C@_0O@ECOBCFJO@?5?3error?5found?$AA@", %58 ]
  %68 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BK@JEPGKCBD@loops?4c?5check?5complete?$CFs?6?$AA@", ptr noundef nonnull %67)
  br label %95

69:                                               ; preds = %48, %91
  %70 = phi i64 [ %93, %91 ], [ 0, %48 ]
  %71 = phi i64 [ %92, %91 ], [ 0, %48 ]
  %72 = getelementptr inbounds float, ptr %15, i64 %70
  %73 = load float, ptr %72, align 4, !tbaa !4
  %74 = getelementptr inbounds float, ptr %16, i64 %70
  %75 = load float, ptr %74, align 4, !tbaa !4
  %76 = fcmp une float %73, %75
  br i1 %76, label %77, label %79

77:                                               ; preds = %69
  %78 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BE@IBJOJFBB@A1?9A2?5mismatch?3?5?$CFd?6?$AA@", i64 noundef %70)
  br label %79

79:                                               ; preds = %77, %69
  %80 = phi i64 [ 1, %77 ], [ %71, %69 ]
  %81 = getelementptr inbounds float, ptr %17, i64 %70
  %82 = load float, ptr %81, align 4, !tbaa !4
  %83 = fcmp une float %75, %82
  br i1 %83, label %84, label %86

84:                                               ; preds = %79
  %85 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BE@IMKAICCJ@A2?9A3?5mismatch?3?5?$CFd?6?$AA@", i64 noundef %70)
  br label %86

86:                                               ; preds = %84, %79
  %87 = phi i64 [ 1, %84 ], [ %80, %79 ]
  %88 = fcmp une float %82, %73
  br i1 %88, label %89, label %91

89:                                               ; preds = %86
  %90 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @"??_C@_0BE@NMOCCHBC@A3?9A1?5mismatch?3?5?$CFd?6?$AA@", i64 noundef %70)
  br label %91

91:                                               ; preds = %86, %89
  %92 = phi i64 [ 1, %89 ], [ %87, %86 ]
  %93 = add nuw i64 %70, 1
  %94 = icmp eq i64 %93, %49
  br i1 %94, label %58, label %69, !llvm.loop !21

95:                                               ; preds = %2, %66
  %96 = phi i32 [ 0, %66 ], [ 1, %2 ]
  ret i32 %96
}

; Function Attrs: inlinehint nounwind uwtable
define linkonce_odr dso_local i32 @printf(ptr noundef %0, ...) local_unnamed_addr #3 comdat {
  %2 = alloca ptr, align 8
  call void @llvm.lifetime.start.p0(i64 8, ptr nonnull %2) #12
  call void @llvm.va_start(ptr nonnull %2)
  %3 = load ptr, ptr %2, align 8, !tbaa !17
  %4 = call ptr @__acrt_iob_func(i32 noundef 1) #12
  %5 = call ptr @__local_stdio_printf_options()
  %6 = load i64, ptr %5, align 8, !tbaa !22
  %7 = call i32 @__stdio_common_vfprintf(i64 noundef %6, ptr noundef %4, ptr noundef %0, ptr noundef null, ptr noundef %3) #12
  call void @llvm.va_end(ptr nonnull %2)
  call void @llvm.lifetime.end.p0(i64 8, ptr nonnull %2) #12
  ret i32 %7
}

; Function Attrs: mustprogress nofree nounwind willreturn memory(read)
declare dso_local i32 @atoi(ptr nocapture noundef) local_unnamed_addr #4

; Function Attrs: mustprogress nofree nounwind willreturn allockind("alloc,zeroed") allocsize(0,1) memory(inaccessiblemem: readwrite)
declare dso_local noalias noundef ptr @calloc(i64 noundef, i64 noundef) local_unnamed_addr #5

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn
declare void @llvm.va_start(ptr) #6

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn
declare void @llvm.va_end(ptr) #6

; Function Attrs: noinline nounwind uwtable
define linkonce_odr dso_local ptr @__local_stdio_printf_options() local_unnamed_addr #7 comdat {
  ret ptr @__local_stdio_printf_options._OptionsStorage
}

declare dso_local ptr @__acrt_iob_func(i32 noundef) local_unnamed_addr #8

declare dso_local i32 @__stdio_common_vfprintf(i64 noundef, ptr noundef, ptr noundef, ptr noundef, ptr noundef) local_unnamed_addr #8

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #9

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i64 @llvm.umax.i64(i64, i64) #10

attributes #0 = { nofree noinline norecurse nosync nounwind memory(argmem: readwrite) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { inlinehint nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { mustprogress nofree nounwind willreturn memory(read) "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { mustprogress nofree nounwind willreturn allockind("alloc,zeroed") allocsize(0,1) memory(inaccessiblemem: readwrite) "alloc-family"="malloc" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { mustprogress nocallback nofree nosync nounwind willreturn }
attributes #7 = { noinline nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #8 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #9 = { nofree nounwind }
attributes #10 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #11 = { allocsize(0,1) }
attributes #12 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 2}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{!"clang version 16.0.6"}
!4 = !{!5, !5, i64 0}
!5 = !{!"float", !6, i64 0}
!6 = !{!"omnipotent char", !7, i64 0}
!7 = !{!"Simple C/C++ TBAA"}
!8 = distinct !{!8, !9, !10, !11}
!9 = !{!"llvm.loop.mustprogress"}
!10 = !{!"llvm.loop.isvectorized", i32 1}
!11 = !{!"llvm.loop.unroll.runtime.disable"}
!12 = distinct !{!12, !9, !11, !10}
!13 = distinct !{!13, !9}
!14 = distinct !{!14, !9, !15}
!15 = !{!"llvm.loop.pipelining.enable", i1 true}
!16 = distinct !{!16, !9}
!17 = !{!18, !18, i64 0}
!18 = !{!"any pointer", !6, i64 0}
!19 = distinct !{!19, !9, !10, !11}
!20 = distinct !{!20, !9, !11, !10}
!21 = distinct !{!21, !9}
!22 = !{!23, !23, i64 0}
!23 = !{!"long long", !6, i64 0}
