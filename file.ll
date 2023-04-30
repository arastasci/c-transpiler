; ModuleID = 'advcalc2ir'
 declare i32 @printf(i8*, ...)
 @print.str = constant [4 x i8] c"%d\0A\00"

 define i32 @main() {
%xa = alloca i32 
store i32 3, i32* %xa 
%y = alloca i32 
store i32 5, i32* %y 
%zvalue = alloca i32 
%reg_1 = load i32, i32* %y 
%reg_2 = add i32 1, %reg_1 
%reg_3 = mul i32 %reg_0, %reg_2 
%reg_4 = add i32 23, %reg_3 
%reg_5 = load i32, i32*  
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_5 )%k = alloca i32 
%reg_7 = load i32, i32* %y 
%reg_8 = sub i32 %reg_6, %reg_7 
%reg_9 = load i32, i32*  
%reg_10 = sub i32 %reg_8, %reg_9 
%reg_12 = load i32, i32* %y 
%reg_13 = mul i32 3, %reg_12 
%reg_14 = add i32 2, 5 
%reg_15 = mul i32 1, %reg_14 
%reg_16 = mul i32 %reg_13, %reg_15 
%reg_17 = add i32 %reg_11, %reg_16 
%reg_18 = load i32, i32*  
%reg_19 = add i32 %reg_18, 1 
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_19 )ret i32 0
}