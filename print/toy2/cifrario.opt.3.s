; ModuleID = 'cifrario.s'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str2 = private unnamed_addr constant [3 x i8] c"%x\00", align 1
@str = private unnamed_addr constant [6 x i8] c"ERROR\00"

; Function Attrs: nounwind uwtable
define void @crypt(i8* nocapture readnone %_key, i8* nocapture readnone %_input, i8* nocapture %output) #0 {
entry:
  tail call void @llvm.dbg.value(metadata !{i8* %_key}, i64 0, metadata !19), !dbg !20
  tail call void @llvm.dbg.value(metadata !{i8* %_input}, i64 0, metadata !21), !dbg !22
  tail call void @llvm.dbg.value(metadata !{i8* %output}, i64 0, metadata !23), !dbg !24
  tail call void @llvm.crypto.key(metadata !{i8* %_key}), !dbg !25
  tail call void @llvm.crypto.plain(metadata !{i8* %_input}), !dbg !25
  tail call void @llvm.dbg.value(metadata !{i8* %_key}, i64 0, metadata !26), !dbg !27
  tail call void @llvm.dbg.value(metadata !{i8* %_input}, i64 0, metadata !28), !dbg !29
  tail call void @llvm.dbg.declare(metadata !30, metadata !31), !dbg !35
  tail call void @llvm.dbg.value(metadata !36, i64 0, metadata !37), !dbg !38
  store i8 -32, i8* %output, align 1, !dbg !39
  ret void, !dbg !40
}

; Function Attrs: nounwind readnone
declare void @llvm.dbg.declare(metadata, metadata) #1

; Function Attrs: nounwind
declare void @llvm.crypto.key(metadata) #2

; Function Attrs: nounwind
declare void @llvm.crypto.plain(metadata) #2

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
if.end:
  tail call void @llvm.dbg.value(metadata !41, i64 0, metadata !42), !dbg !43
  tail call void @llvm.dbg.value(metadata !44, i64 0, metadata !45), !dbg !46
  tail call void @llvm.dbg.value(metadata !44, i64 0, metadata !47), !dbg !49
  tail call void @llvm.dbg.value(metadata !44, i64 0, metadata !50), !dbg !51
  tail call void @llvm.dbg.value(metadata !52, i64 0, metadata !53) #2, !dbg !54
  tail call void @llvm.crypto.key(metadata !52) #2, !dbg !55
  tail call void @llvm.crypto.plain(metadata !{null}) #2, !dbg !55
  tail call void @llvm.dbg.value(metadata !52, i64 0, metadata !56) #2, !dbg !57
  tail call void @llvm.dbg.declare(metadata !30, metadata !31) #2, !dbg !58
  tail call void @llvm.dbg.value(metadata !36, i64 0, metadata !59) #2, !dbg !60
  tail call void @llvm.dbg.value(metadata !36, i64 0, metadata !61), !dbg !62
  tail call void @llvm.dbg.value(metadata !36, i64 0, metadata !63), !dbg !64
  tail call void @llvm.dbg.value(metadata !36, i64 0, metadata !61), !dbg !65
  %puts = tail call i32 @puts(i8* getelementptr inbounds ([6 x i8]* @str, i64 0, i64 0)), !dbg !66
  %call3 = tail call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.str2, i64 0, i64 0), i32 224) #2, !dbg !65
  ret i32 0, !dbg !68
}

; Function Attrs: nounwind
declare i32 @printf(i8* nocapture readonly, ...) #3

; Function Attrs: nounwind readnone
declare void @llvm.dbg.value(metadata, i64, metadata) #1

; Function Attrs: nounwind
declare i32 @puts(i8* nocapture readonly) #2

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone }
attributes #2 = { nounwind }
attributes #3 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

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
!30 = metadata !{[4 x i8]* undef}
!31 = metadata !{i32 786688, metadata !4, metadata !"subkeys", metadata !5, i32 11, metadata !32, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [subkeys] [line 11]
!32 = metadata !{i32 786433, null, null, metadata !"", i32 0, i64 32, i64 8, i32 0, i32 0, metadata !9, metadata !33, i32 0, null, null, null} ; [ DW_TAG_array_type ] [line 0, size 32, align 8, offset 0] [from unsigned char]
!33 = metadata !{metadata !34}
!34 = metadata !{i32 786465, i64 0, i64 4}        ; [ DW_TAG_subrange_type ] [0, 3]
!35 = metadata !{i32 11, i32 0, metadata !4, null}
!36 = metadata !{i8 -32}
!37 = metadata !{i32 786688, metadata !4, metadata !"tmp", metadata !5, i32 12, metadata !9, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [tmp] [line 12]
!38 = metadata !{i32 22, i32 0, metadata !4, null}
!39 = metadata !{i32 23, i32 0, metadata !4, null}
!40 = metadata !{i32 24, i32 0, metadata !4, null}
!41 = metadata !{i8 0}
!42 = metadata !{i32 786688, metadata !12, metadata !"correct", metadata !5, i32 31, metadata !9, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [correct] [line 31]
!43 = metadata !{i32 31, i32 0, metadata !12, null}
!44 = metadata !{i8 -51}
!45 = metadata !{i32 786688, metadata !12, metadata !"plain", metadata !5, i32 32, metadata !9, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [plain] [line 32]
!46 = metadata !{i32 32, i32 0, metadata !12, null}
!47 = metadata !{i32 786688, metadata !4, metadata !"input", metadata !5, i32 10, metadata !10, i32 0, metadata !48} ; [ DW_TAG_auto_variable ] [input] [line 10]
!48 = metadata !{i32 35, i32 0, metadata !12, null}
!49 = metadata !{i32 10, i32 0, metadata !4, metadata !48}
!50 = metadata !{i32 786689, metadata !4, metadata !"_input", metadata !5, i32 33554438, metadata !10, i32 0, metadata !48} ; [ DW_TAG_arg_variable ] [_input] [line 6]
!51 = metadata !{i32 6, i32 0, metadata !4, metadata !48}
!52 = metadata !{i8* undef}
!53 = metadata !{i32 786689, metadata !4, metadata !"_key", metadata !5, i32 16777221, metadata !8, i32 0, metadata !48} ; [ DW_TAG_arg_variable ] [_key] [line 5]
!54 = metadata !{i32 5, i32 0, metadata !4, metadata !48}
!55 = metadata !{i32 8, i32 0, metadata !4, metadata !48} ; [ DW_TAG_imported_declaration ]
!56 = metadata !{i32 786688, metadata !4, metadata !"key", metadata !5, i32 9, metadata !10, i32 0, metadata !48} ; [ DW_TAG_auto_variable ] [key] [line 9]
!57 = metadata !{i32 9, i32 0, metadata !4, metadata !48}
!58 = metadata !{i32 11, i32 0, metadata !4, metadata !48}
!59 = metadata !{i32 786688, metadata !4, metadata !"tmp", metadata !5, i32 12, metadata !9, i32 0, metadata !48} ; [ DW_TAG_auto_variable ] [tmp] [line 12]
!60 = metadata !{i32 22, i32 0, metadata !4, metadata !48}
!61 = metadata !{i32 786688, metadata !12, metadata !"out", metadata !5, i32 30, metadata !9, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [out] [line 30]
!62 = metadata !{i32 23, i32 0, metadata !4, metadata !48}
!63 = metadata !{i32 786689, metadata !4, metadata !"output", metadata !5, i32 50331655, metadata !8, i32 0, metadata !48} ; [ DW_TAG_arg_variable ] [output] [line 7]
!64 = metadata !{i32 7, i32 0, metadata !4, metadata !48}
!65 = metadata !{i32 37, i32 0, metadata !12, null}
!66 = metadata !{i32 36, i32 0, metadata !67, null}
!67 = metadata !{i32 786443, metadata !1, metadata !12, i32 36, i32 0, i32 0} ; [ DW_TAG_lexical_block ] [/home/llvm_dfa/llvm/print/toy2/cifrario.c]
!68 = metadata !{i32 38, i32 0, metadata !12, null}
