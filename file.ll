; ModuleID = 'advcalc2ir'
 declare i32 @printf(i8*, ...)
 @print.str = constant [4 x i8] c"%d\0A\00"

 define i32 @main() {
%x = alloca i32 
store i32 1, i32* %x 
%y = alloca i32 
%reg_0 = load i32, i32* %x 
%reg_1 = add i32 %reg_0, 3 
store i32 %reg_1, i32* %y 
%z = alloca i32 
%reg_2 = load i32, i32* %x 
%reg_3 = load i32, i32* %y 
%reg_4 = mul i32 %reg_2, %reg_3
%reg_5 = load i32, i32* %y 
%reg_6 = mul i32 %reg_4, %reg_5
%reg_7 = load i32, i32* %y 
%reg_8 = mul i32 %reg_6, %reg_7
store i32 %reg_8, i32* %z 
%reg_9 = load i32, i32* %z 
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_9 ) 
%reg_10 = load i32, i32* %x 
%reg_11 = load i32, i32* %x 
%reg_12 = xor i32 %reg_10, %reg_11 
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_12 ) 
%reg_13 = load i32, i32* %x 
%reg_14 = load i32, i32* %x 
%reg_15 = xor i32 %reg_13, %reg_14 
%reg_16 = load i32, i32* %z 
%reg_17 = load i32, i32* %y 
%reg_18 = add i32 %reg_16, %reg_17 
%reg_19 = or i32 %reg_15, %reg_18 
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_19 ) 
%reg_20 = load i32, i32* %x 
%reg_21 = load i32, i32* %x 
%reg_22 = xor i32 %reg_20, %reg_21 
%reg_23 = load i32, i32* %z 
%reg_24 = load i32, i32* %y 
%reg_25 = add i32 %reg_23, %reg_24 
%reg_26 = or i32 %reg_22, %reg_25 
%reg_27 = ashr i32 %reg_26, 1 
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_27 ) 
%reg_28 = load i32, i32* %x 
%reg_29 = load i32, i32* %x 
%reg_30 = xor i32 %reg_28, %reg_29 
%reg_31 = load i32, i32* %z 
%reg_32 = load i32, i32* %y 
%reg_33 = add i32 %reg_31, %reg_32 
%reg_34 = or i32 %reg_30, %reg_33 
%reg_35 = ashr i32 %reg_34, 1 
%reg_36 = shl i32 %reg_35, 1 
call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %reg_36 ) 
ret i32 0
}