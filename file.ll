; ModuleID = 'advcalc2ir'
 declare i32 @printf(i8*, ...)
 @print.str = constant [4 x i8] c"%d\0A\00"

 define i32 @main() {
%axor = alloca i32 
store i32 3, i32* %axor 
%reg_0 = load i32, i32* %axor 
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_0 ) 
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 33 ) 
ret i32 0
}