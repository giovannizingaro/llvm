; ModuleID = 'cifrario.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@main.key = private unnamed_addr constant [2 x i8] c"x\F0", align 1
@.str = private unnamed_addr constant [4 x i8] c"OK\0A\00", align 1
@.str1 = private unnamed_addr constant [7 x i8] c"ERROR\0A\00", align 1
@.str2 = private unnamed_addr constant [3 x i8] c"%x\00", align 1

; Function Attrs: nounwind uwtable
define void @crypt(i8* %_key, i8* %_input, i8* %output) #0 {
entry:
  %_key.addr = alloca i8*, align 8
  %_input.addr = alloca i8*, align 8
  %output.addr = alloca i8*, align 8
  %key = alloca i8*, align 8
  %input = alloca i8*, align 8
  %subkeys = alloca [4 x i8], align 1
  %tmp = alloca i8, align 1
  store i8* %_key, i8** %_key.addr, align 8
  call void @llvm.dbg.declare(metadata !{i8** %_key.addr}, metadata !19), !dbg !20
  store i8* %_input, i8** %_input.addr, align 8
  call void @llvm.dbg.declare(metadata !{i8** %_input.addr}, metadata !21), !dbg !22
  store i8* %output, i8** %output.addr, align 8
  call void @llvm.dbg.declare(metadata !{i8** %output.addr}, metadata !23), !dbg !24
  call void @llvm.crypto.key(metadata !{i8* %_key}), !dbg !25
  call void @llvm.crypto.plain(metadata !{i8* %_input}), !dbg !25
  call void @llvm.dbg.declare(metadata !{i8** %key}, metadata !26), !dbg !27
  %0 = load i8** %_key.addr, align 8, !dbg !27
  store i8* %0, i8** %key, align 8, !dbg !27
  call void @llvm.dbg.declare(metadata !{i8** %input}, metadata !28), !dbg !29
  %1 = load i8** %_input.addr, align 8, !dbg !29
  store i8* %1, i8** %input, align 8, !dbg !29
  call void @llvm.dbg.declare(metadata !{[4 x i8]* %subkeys}, metadata !30), !dbg !34
  call void @llvm.dbg.declare(metadata !{i8* %tmp}, metadata !35), !dbg !36
  %2 = load i8** %key, align 8, !dbg !37
  %arrayidx = getelementptr inbounds i8* %2, i64 1, !dbg !37
  %3 = load i8* %arrayidx, align 1, !dbg !37
  %arrayidx1 = getelementptr inbounds [4 x i8]* %subkeys, i32 0, i64 0, !dbg !37
  store i8 %3, i8* %arrayidx1, align 1, !dbg !37
  %4 = load i8** %key, align 8, !dbg !38
  %arrayidx2 = getelementptr inbounds i8* %4, i64 0, !dbg !38
  %5 = load i8* %arrayidx2, align 1, !dbg !38
  %arrayidx3 = getelementptr inbounds [4 x i8]* %subkeys, i32 0, i64 1, !dbg !38
  store i8 %5, i8* %arrayidx3, align 1, !dbg !38
  %6 = load i8** %key, align 8, !dbg !39
  %arrayidx4 = getelementptr inbounds i8* %6, i64 0, !dbg !39
  %7 = load i8* %arrayidx4, align 1, !dbg !39
  %conv = zext i8 %7 to i32, !dbg !39
  %8 = load i8** %key, align 8, !dbg !39
  %arrayidx5 = getelementptr inbounds i8* %8, i64 1, !dbg !39
  %9 = load i8* %arrayidx5, align 1, !dbg !39
  %conv6 = zext i8 %9 to i32, !dbg !39
  %xor = xor i32 %conv, %conv6, !dbg !39
  %conv7 = trunc i32 %xor to i8, !dbg !39
  %arrayidx8 = getelementptr inbounds [4 x i8]* %subkeys, i32 0, i64 2, !dbg !39
  store i8 %conv7, i8* %arrayidx8, align 1, !dbg !39
  %10 = load i8** %key, align 8, !dbg !40
  %arrayidx9 = getelementptr inbounds i8* %10, i64 1, !dbg !40
  %11 = load i8* %arrayidx9, align 1, !dbg !40
  %conv10 = zext i8 %11 to i32, !dbg !40
  %shl = shl i32 %conv10, 10, !dbg !40
  %xor11 = xor i32 %shl, 64751, !dbg !40
  %conv12 = trunc i32 %xor11 to i8, !dbg !40
  %arrayidx13 = getelementptr inbounds [4 x i8]* %subkeys, i32 0, i64 3, !dbg !40
  store i8 %conv12, i8* %arrayidx13, align 1, !dbg !40
  %12 = load i8** %input, align 8, !dbg !41
  %13 = load i8* %12, align 1, !dbg !41
  store i8 %13, i8* %tmp, align 1, !dbg !41
  %14 = load i8* %tmp, align 1, !dbg !42
  %conv14 = zext i8 %14 to i32, !dbg !42
  %arrayidx15 = getelementptr inbounds [4 x i8]* %subkeys, i32 0, i64 0, !dbg !42
  %15 = load i8* %arrayidx15, align 1, !dbg !42
  %conv16 = zext i8 %15 to i32, !dbg !42
  %xor17 = xor i32 %conv14, %conv16, !dbg !42
  %conv18 = trunc i32 %xor17 to i8, !dbg !42
  store i8 %conv18, i8* %tmp, align 1, !dbg !42
  %16 = load i8* %tmp, align 1, !dbg !42
  %conv19 = zext i8 %16 to i32, !dbg !42
  %shl20 = shl i32 %conv19, 2, !dbg !42
  %17 = load i8* %tmp, align 1, !dbg !42
  %conv21 = zext i8 %17 to i32, !dbg !42
  %shr = ashr i32 %conv21, 30, !dbg !42
  %or = or i32 %shl20, %shr, !dbg !42
  %conv22 = trunc i32 %or to i8, !dbg !42
  store i8 %conv22, i8* %tmp, align 1, !dbg !42
  %18 = load i8* %tmp, align 1, !dbg !43
  %conv23 = zext i8 %18 to i32, !dbg !43
  %arrayidx24 = getelementptr inbounds [4 x i8]* %subkeys, i32 0, i64 1, !dbg !43
  %19 = load i8* %arrayidx24, align 1, !dbg !43
  %conv25 = zext i8 %19 to i32, !dbg !43
  %xor26 = xor i32 %conv23, %conv25, !dbg !43
  %conv27 = trunc i32 %xor26 to i8, !dbg !43
  store i8 %conv27, i8* %tmp, align 1, !dbg !43
  %20 = load i8* %tmp, align 1, !dbg !43
  %conv28 = zext i8 %20 to i32, !dbg !43
  %shl29 = shl i32 %conv28, 3, !dbg !43
  %21 = load i8* %tmp, align 1, !dbg !43
  %conv30 = zext i8 %21 to i32, !dbg !43
  %shr31 = ashr i32 %conv30, 29, !dbg !43
  %or32 = or i32 %shl29, %shr31, !dbg !43
  %conv33 = trunc i32 %or32 to i8, !dbg !43
  store i8 %conv33, i8* %tmp, align 1, !dbg !43
  %22 = load i8* %tmp, align 1, !dbg !44
  %conv34 = zext i8 %22 to i32, !dbg !44
  %arrayidx35 = getelementptr inbounds [4 x i8]* %subkeys, i32 0, i64 2, !dbg !44
  %23 = load i8* %arrayidx35, align 1, !dbg !44
  %conv36 = zext i8 %23 to i32, !dbg !44
  %xor37 = xor i32 %conv34, %conv36, !dbg !44
  %conv38 = trunc i32 %xor37 to i8, !dbg !44
  store i8 %conv38, i8* %tmp, align 1, !dbg !44
  %24 = load i8* %tmp, align 1, !dbg !44
  %conv39 = zext i8 %24 to i32, !dbg !44
  %shl40 = shl i32 %conv39, 4, !dbg !44
  %25 = load i8* %tmp, align 1, !dbg !44
  %conv41 = zext i8 %25 to i32, !dbg !44
  %shr42 = ashr i32 %conv41, 28, !dbg !44
  %or43 = or i32 %shl40, %shr42, !dbg !44
  %conv44 = trunc i32 %or43 to i8, !dbg !44
  store i8 %conv44, i8* %tmp, align 1, !dbg !44
  %26 = load i8* %tmp, align 1, !dbg !45
  %conv45 = zext i8 %26 to i32, !dbg !45
  %arrayidx46 = getelementptr inbounds [4 x i8]* %subkeys, i32 0, i64 3, !dbg !45
  %27 = load i8* %arrayidx46, align 1, !dbg !45
  %conv47 = zext i8 %27 to i32, !dbg !45
  %xor48 = xor i32 %conv45, %conv47, !dbg !45
  %conv49 = trunc i32 %xor48 to i8, !dbg !45
  store i8 %conv49, i8* %tmp, align 1, !dbg !45
  %28 = load i8* %tmp, align 1, !dbg !45
  %conv50 = zext i8 %28 to i32, !dbg !45
  %shl51 = shl i32 %conv50, 5, !dbg !45
  %29 = load i8* %tmp, align 1, !dbg !45
  %conv52 = zext i8 %29 to i32, !dbg !45
  %shr53 = ashr i32 %conv52, 27, !dbg !45
  %or54 = or i32 %shl51, %shr53, !dbg !45
  %conv55 = trunc i32 %or54 to i8, !dbg !45
  store i8 %conv55, i8* %tmp, align 1, !dbg !45
  %30 = load i8* %tmp, align 1, !dbg !46
  %31 = load i8** %output.addr, align 8, !dbg !46
  store i8 %30, i8* %31, align 1, !dbg !46
  ret void, !dbg !47
}

; Function Attrs: nounwind readnone
declare void @llvm.dbg.declare(metadata, metadata) #1

; Function Attrs: nounwind
declare void @llvm.crypto.key(metadata) #2

; Function Attrs: nounwind
declare void @llvm.crypto.plain(metadata) #2

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %out = alloca i8, align 1
  %correct = alloca i8, align 1
  %plain = alloca i8, align 1
  %key = alloca [2 x i8], align 1
  store i32 0, i32* %retval
  call void @llvm.dbg.declare(metadata !{i8* %out}, metadata !48), !dbg !49
  call void @llvm.dbg.declare(metadata !{i8* %correct}, metadata !50), !dbg !51
  store i8 0, i8* %correct, align 1, !dbg !51
  call void @llvm.dbg.declare(metadata !{i8* %plain}, metadata !52), !dbg !53
  store i8 -51, i8* %plain, align 1, !dbg !53
  call void @llvm.dbg.declare(metadata !{[2 x i8]* %key}, metadata !54), !dbg !58
  %0 = bitcast [2 x i8]* %key to i8*, !dbg !58
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* getelementptr inbounds ([2 x i8]* @main.key, i32 0, i32 0), i64 2, i32 1, i1 false), !dbg !58
  %arraydecay = getelementptr inbounds [2 x i8]* %key, i32 0, i32 0, !dbg !59
  call void @crypt(i8* %arraydecay, i8* %plain, i8* %out), !dbg !59
  %call = call i32 @memcmp(i8* %out, i8* %correct, i64 1) #5, !dbg !60
  %cmp = icmp eq i32 %call, 0, !dbg !60
  br i1 %cmp, label %if.then, label %if.else, !dbg !60

if.then:                                          ; preds = %entry
  %call1 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([4 x i8]* @.str, i32 0, i32 0)), !dbg !60
  br label %if.end, !dbg !60

if.else:                                          ; preds = %entry
  %call2 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([7 x i8]* @.str1, i32 0, i32 0)), !dbg !60
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %1 = load i8* %out, align 1, !dbg !62
  %conv = zext i8 %1 to i32, !dbg !62
  %call3 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.str2, i32 0, i32 0), i32 %conv), !dbg !62
  %2 = load i32* %retval, !dbg !63
  ret i32 %2, !dbg !63
}

; Function Attrs: nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #2

; Function Attrs: nounwind readonly
declare i32 @memcmp(i8*, i8*, i64) #3

declare i32 @printf(i8*, ...) #4

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone }
attributes #2 = { nounwind }
attributes #3 = { nounwind readonly "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { nounwind readonly }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!16, !17}
!llvm.ident = !{!18}

!0 = metadata !{i32 786449, metadata !1, i32 12, metadata !"clang version 3.4.1 (tags/RELEASE_34/dot1-final 212513)", i1 false, metadata !"", i32 0, metadata !2, metadata !2, metadata !3, metadata !2, metadata !2, metadata !""} ; [ DW_TAG_compile_unit ] [/home/llvm_dfa/llvm/print/toy2/cifrario.c] [DW_LANG_C99]
!1 = metadata !{metadata !"cifrario.c", metadata !"/home/llvm_dfa/llvm/print/toy2"}
!2 = metadata !{i32 0}
!3 = metadata !{metadata !4, metadata !12}
!4 = metadata !{i32 786478, metadata !1, metadata !5, metadata !"crypt", metadata !"crypt", metadata !"", i32 5, metadata !6, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, void (i8*, i8*, i8*)* @crypt, null, null, metadata !2, i32 8} ; [ DW_TAG_subprogram ] [line 5] [def] [scope 8] [crypt]
!5 = metadata !{i32 786473, metadata !1}          ; [ DW_TAG_file_type ] [/home/llvm_dfa/llvm/print/toy2/cifrario.c]
!6 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !7, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!7 = metadata !{null, metadata !8, metadata !10, metadata !8}
!8 = metadata !{i32 786447, null, null, metadata !"", i32 0, i64 64, i64 64, i64 0, i32 0, metadata !9} ; [ DW_TAG_pointer_type ] [line 0, size 64, align 64, offset 0] [from unsigned char]
!9 = metadata !{i32 786468, null, null, metadata !"unsigned char", i32 0, i64 8, i64 8, i64 0, i32 0, i32 8} ; [ DW_TAG_base_type ] [unsigned char] [line 0, size 8, align 8, offset 0, enc DW_ATE_unsigned_char]
!10 = metadata !{i32 786447, null, null, metadata !"", i32 0, i64 64, i64 64, i64 0, i32 0, metadata !11} ; [ DW_TAG_pointer_type ] [line 0, size 64, align 64, offset 0] [from ]
!11 = metadata !{i32 786470, null, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, metadata !9} ; [ DW_TAG_const_type ] [line 0, size 0, align 0, offset 0] [from unsigned char]
!12 = metadata !{i32 786478, metadata !1, metadata !5, metadata !"main", metadata !"main", metadata !"", i32 29, metadata !13, i1 false, i1 true, i32 0, i32 0, null, i32 0, i1 false, i32 ()* @main, null, null, metadata !2, i32 29} ; [ DW_TAG_subprogram ] [line 29] [def] [main]
!13 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !14, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!14 = metadata !{metadata !15}
!15 = metadata !{i32 786468, null, null, metadata !"int", i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ] [int] [line 0, size 32, align 32, offset 0, enc DW_ATE_signed]
!16 = metadata !{i32 2, metadata !"Dwarf Version", i32 4}
!17 = metadata !{i32 1, metadata !"Debug Info Version", i32 1}
!18 = metadata !{metadata !"clang version 3.4.1 (tags/RELEASE_34/dot1-final 212513)"}
!19 = metadata !{i32 786689, metadata !4, metadata !"_key", metadata !5, i32 16777221, metadata !8, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [_key] [line 5]
!20 = metadata !{i32 5, i32 0, metadata !4, null}
!21 = metadata !{i32 786689, metadata !4, metadata !"_input", metadata !5, i32 33554438, metadata !10, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [_input] [line 6]
!22 = metadata !{i32 6, i32 0, metadata !4, null}
!23 = metadata !{i32 786689, metadata !4, metadata !"output", metadata !5, i32 50331655, metadata !8, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [output] [line 7]
!24 = metadata !{i32 7, i32 0, metadata !4, null}
!25 = metadata !{i32 8, i32 0, metadata !4, null} ; [ DW_TAG_imported_declaration ]
!26 = metadata !{i32 786688, metadata !4, metadata !"key", metadata !5, i32 9, metadata !10, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [key] [line 9]
!27 = metadata !{i32 9, i32 0, metadata !4, null}
!28 = metadata !{i32 786688, metadata !4, metadata !"input", metadata !5, i32 10, metadata !10, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [input] [line 10]
!29 = metadata !{i32 10, i32 0, metadata !4, null}
!30 = metadata !{i32 786688, metadata !4, metadata !"subkeys", metadata !5, i32 11, metadata !31, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [subkeys] [line 11]
!31 = metadata !{i32 786433, null, null, metadata !"", i32 0, i64 32, i64 8, i32 0, i32 0, metadata !9, metadata !32, i32 0, null, null, null} ; [ DW_TAG_array_type ] [line 0, size 32, align 8, offset 0] [from unsigned char]
!32 = metadata !{metadata !33}
!33 = metadata !{i32 786465, i64 0, i64 4}        ; [ DW_TAG_subrange_type ] [0, 3]
!34 = metadata !{i32 11, i32 0, metadata !4, null}
!35 = metadata !{i32 786688, metadata !4, metadata !"tmp", metadata !5, i32 12, metadata !9, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [tmp] [line 12]
!36 = metadata !{i32 12, i32 0, metadata !4, null}
!37 = metadata !{i32 13, i32 0, metadata !4, null}
!38 = metadata !{i32 14, i32 0, metadata !4, null}
!39 = metadata !{i32 15, i32 0, metadata !4, null}
!40 = metadata !{i32 16, i32 0, metadata !4, null}
!41 = metadata !{i32 17, i32 0, metadata !4, null}
!42 = metadata !{i32 19, i32 0, metadata !4, null}
!43 = metadata !{i32 20, i32 0, metadata !4, null}
!44 = metadata !{i32 21, i32 0, metadata !4, null}
!45 = metadata !{i32 22, i32 0, metadata !4, null}
!46 = metadata !{i32 23, i32 0, metadata !4, null}
!47 = metadata !{i32 24, i32 0, metadata !4, null}
!48 = metadata !{i32 786688, metadata !12, metadata !"out", metadata !5, i32 30, metadata !9, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [out] [line 30]
!49 = metadata !{i32 30, i32 0, metadata !12, null}
!50 = metadata !{i32 786688, metadata !12, metadata !"correct", metadata !5, i32 31, metadata !9, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [correct] [line 31]
!51 = metadata !{i32 31, i32 0, metadata !12, null}
!52 = metadata !{i32 786688, metadata !12, metadata !"plain", metadata !5, i32 32, metadata !9, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [plain] [line 32]
!53 = metadata !{i32 32, i32 0, metadata !12, null}
!54 = metadata !{i32 786688, metadata !12, metadata !"key", metadata !5, i32 33, metadata !55, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [key] [line 33]
!55 = metadata !{i32 786433, null, null, metadata !"", i32 0, i64 16, i64 8, i32 0, i32 0, metadata !9, metadata !56, i32 0, null, null, null} ; [ DW_TAG_array_type ] [line 0, size 16, align 8, offset 0] [from unsigned char]
!56 = metadata !{metadata !57}
!57 = metadata !{i32 786465, i64 0, i64 2}        ; [ DW_TAG_subrange_type ] [0, 1]
!58 = metadata !{i32 33, i32 0, metadata !12, null}
!59 = metadata !{i32 35, i32 0, metadata !12, null}
!60 = metadata !{i32 36, i32 0, metadata !61, null}
!61 = metadata !{i32 786443, metadata !1, metadata !12, i32 36, i32 0, i32 0} ; [ DW_TAG_lexical_block ] [/home/llvm_dfa/llvm/print/toy2/cifrario.c]
!62 = metadata !{i32 37, i32 0, metadata !12, null}
!63 = metadata !{i32 38, i32 0, metadata !12, null}
