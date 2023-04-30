; ModuleID = 'advcalc2ir'
 declare i32 @printf(i8*, ...)
 @print.str = constant [4 x i8] c"%d\0A\00"

 define i32 @main() {
%x = alloca i32 
store i32 3, i32* %x 
%y = alloca i32 
store i32 5, i32* %y 
%zvalue = alloca i32 
%reg_0 = load i32, i32* %x 
%reg_1 = load i32, i32* %y 
%reg_2 = add i32 1, %reg_1 
%reg_3 = mul i32 %reg_0, %reg_2 
%reg_4 = add i32 23, %reg_3 
store i32 %reg_4, i32* %zvalue 
%reg_5 = load i32, i32* %zvalue 
%k = alloca i32 
%reg_6 = load i32, i32* %x 
%reg_7 = load i32, i32* %y 
%reg_8 = sub i32 %reg_6, %reg_7 
%reg_9 = load i32, i32* %zvalue 
%reg_10 = sub i32 %reg_8, %reg_9 
store i32 %reg_10, i32* %k 
%reg_11 = load i32, i32* %x 
%reg_12 = load i32, i32* %y 
%reg_13 = mul i32 3, %reg_12 
%reg_14 = add i32 2, 5 
%reg_15 = mul i32 1, %reg_14 
%reg_16 = mul i32 %reg_13, %reg_15 
%reg_17 = add i32 %reg_11, %reg_16 
store i32 %reg_17, i32* %k 
%reg_18 = load i32, i32* %k 
%reg_19 = add i32 %reg_18, 1 
ret i32 0
}