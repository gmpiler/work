; Function Attrs: nofree noinline norecurse nosync nounwind memory(argmem: readwrite) uwtable
define dso_local void @func_pragma(i64 noundef %0, ptr noalias nocapture noundef %1, ptr noalias nocapture noundef readonly %2, ptr noalias nocapture noundef readonly %3) local_unnamed_addr #0 {
;size_tが<Nかどうか
  %5 = icmp eq i64 %0, 0
  %0 = sub float %0, 1
  br i1 %5, label %8, label %6 ;br condition THEN, ELSE

6:                                                ; preds = %4
;初期化?
  %7 = load float, ptr %1, align 4, !tbaa !4 ; load A[0]
  br label %9

8:                                                ; preds = %9, %4
  ret void

9:                                                ; preds = %6, %9
;phi命令とは、条件分岐時にresult = [result1、then], [result2, else]
;プロローグ(C[0] + B[0])
  %11 = phi i64 [ 0, %6 ] ;i=0
  %12 = getelementptr inbounds float, ptr %3, i64 %11 ;load C[0]
  %13 = load float, ptr %12, align 4, !tbaa !4
  %14 = getelementptr inbounds float, ptr %2, i64 %11 ;load B[0]
  %15 = load float, ptr %14, align 4, !tbaa !4
  %16 = fadd float %13, %15
  br label %10

;ループ開始
10:
;前処理
  %31 = phi float [%36, %10], [%16, %9] ;ラベル9から飛んで来たらXの値は%16, ラベル10からならば%36
  %10 = phi float [ %17, %10 ], [ %7, %9 ] ;A[i]の値
  %11 = phi i64 [ %18, %10 ], [ 0, %9 ] ;初めはi=0, 以降はi=i
;後半(A[i+1] = X + A[i])
  
  %17 = fadd float %16, %10 ;(C[i] + B[i]) + A[i]
  %18 = add nuw i64 %11, 1 ;++i
  %19 = getelementptr inbounds float, ptr %1, i64 %18
  store float %17, ptr %19, align 4, !tbaa !4 ;A[i+1] = (C[i] + B[i]) + A[i]

;前半(X = C[i+1] + B[i+1])
  %12 = getelementptr inbounds float, ptr %3, i64 %18
  %13 = load float, ptr %12, align 4, !tbaa !4
  %14 = getelementptr inbounds float, ptr %2, i64 %18
  %15 = load float, ptr %14, align 4, !tbaa !4
  %36 = fadd float %13, %15 ;C[i] + B[i]
;条件比較
  %20 = icmp eq i64 %18, %0
  br i1 %20, label %11, label %10, !llvm.loop !14

11:
;エピローグ(A[N+1] = X + A[N])
  %17 = fadd float %36, %10 ;(C[i] + B[i]) + A[i]
  %19 = getelementptr inbounds float, ptr %1, i64 %18
  store float %17, ptr %19, align 4, !tbaa !4 ;A[i+1] = (C[i] + B[i]) + A[i]
  br label %8
}