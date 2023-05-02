# #DOCUMENTATION

## ##Introduction

The goal of this project is to create a transpiler that can convert AdvCalc++ assignment statements and expressions into LLVM IR code to calculate and output the given statements. The transpiler is written in C and is able to handle the following features:

-   Printing basic arithmetic operations to an llvm file (addition, subtraction, multiplication, division, modulus)
-   Printing basic assignment statements to an llvm file (integers)
-   Printing binary operations to an llvm file (xor, left shift, right shift, left rotate, right rotate)
-   Printing unary operations to an llvm file (not)

It might be beneficial for the reader to know the definitions of the following terms in order to make full use of the documentation:

-   Intermediate Representation: the data structure or code used internally by a compiler or virtual machine to represent source code. [1]
-   Static Single Assignment (SSA for short): a property of an intermediate representation that requires each variable to be assigned exactly once and defined before it is used. [2]
-   Register: the data structure that holds a value in intermediate representation languages.

# 2 Program Interface

The program is written in C and that requires the user who has the source code to first compile the program. For that there is a Makefile ready for use in the folder, so opening a new terminal in the root directory and running `make` will suffice. Compilation outputs the executable file `advcalc2ir` which takes a command line argument, that is the input file `{file_name}.adv`. So the executable can be run by the command `./advcalc2ir {file_name}.adv` and that, if there are no errors in the `.adv` file, outputs `{file_name}.ll` (Note that the file name has only its extension changed). The `.ll` file contains the LLVM code translated from AdvCalc++. If you have LLVM tools installed on your machine, you can run `lli {file_name}.ll` to run the file and see the outputs printed in standard output. You can also run the following commands to generate an executable from the `.ll` file:

```
$ llc file.ll -o file.s
$ clang file.s -o myexec
$ ./myexec
```

To clear the compiled program files and the executable, you can run `make clean`

### 3.1 Validity of statements

The assignment operation's right hand side can be any valid expression of the user's like while the LHS ought to be an identifier. If there is an error in this syntax, the statement is invalidated and `"Error in line ${line_number}!"` is printed to standard output and the AdvCalc++ program becomes invalid and the output ".ll" file is not generated(rather is first generated but then removed).
As to the validity of the expressions, the precedence of the operations determine the expression's next expected token. If the token is not of the expected type, the expression is deemed invalid. Parantheses must abide by the stack rule. For the precedence of the operations, you can take [C's operation precedence table](https://en.cppreference.com/w/c/language/operator_precedence) as reference.

## Implementation Details

###Tokenizing
Since we already implemented a working tokenizer in the previous project, the only additions to the tokenizer are the 'modulus' \verb|(%)| and the 'division'\verb|(/)|.

###Parsing
The same recursive parser and BNF that was used in the first project is used in this project with extra 'modulus' and 'division' rules. In the first project the expressions were parsed and evaluated. In this project, the program generates the LLVM IR code for the expressions and statements that are given.
Multiple helper functions are used to achieve this. One of them is makeOperation which mainly takes input as our 'result' which returns the output of the first called hierarchical recursive parser function. This may confuse the reader, so below some examples are provided to make it more clear.

This was the function hierarchy in the AdvCalc Interpreter project:

`parseStatement`: parses statements as an assignment or a `bitwise or expression`

`parseAssignment`: parses an assignment as an identifier and a `bitwise or expression`

`parseBitwiseOrExpression`: iteratively parses the current `bitwise or expression`, splitting it into parts of `bitwise and expression`s so long as there is a `bitwise or operator`.

`parseBitwiseAndExpression`: iteratively parses the current `bitwise and expression`, splitting it into parts of `expression`s so long as there is a `bitwise and operator`.

`parseExpression`: iteratively parses the current `expression`, splitting it into parts of `term`s so long as there is an `additive operator`.

`parseTerm`: iteratively parses the current `term`, splitting it into parts of `factor`s so long as there is a `multiplicative operator`.

`parseFactor`: parses a factor as an identifier, an integer, a unary function, a binary function or a `bitwise or expression` in parentheses

`parseVariable`: parses an identifier, checks if the variable is in the hashmap and inserts it returns 0 or returns the current value

`parseUnaryFunction`: parses a `bitwise or expression` in parentheses and takes its complement

`parseBinaryFunction`: parses two `bitwise or expression`s in parentheses with a separator and applies a binary function () to it

For example, the code below was the parseBitwiseOrExpression function in the first project:

'''

int_64 parseBitwiseOrExpression(){
int_64 result = parseBitwiseAndExpression(); // first (bitwise and expression) to be bitwise 'or'ed

    while (token_index < token_count && strcmp(current_token.symbol, "|")==0){    // bitwise 'or' and 'and' (second lowest precedency in the grammar)
        token t = current_token;
        matchToken(OPERATOR_BITWISE);
        result |= parseBitwiseAndExpression();  // get second (bitwise and expression) to be 'bitwise or'ed with the first (bitwise and expression)
    }
    return result;

};

'''

As you can see, the expressions were parsed and their results were given to the parent evaluation function.

In the AdvCalc++ transpiler, the return value from parse functions isn't an integer generated by evaluating the given expression but rather the name of the register that refers to the value of the expression. Below is an explanation for the parse function that parses bitwise or expression, namely `parseBitwiseOrExpression`:

The return value is char*, which is a string that contains the name of the register that holds the descendant expression's (namely a "bitwise and" expression) result. It is matched with the operator and the makeOperation function which takes the result of the first called function is called.
Then the result of the second called function and generates the LLVM IR code for the operation. In the makeOperation function, a new register is created and the LLVM code which instructs this new register to store the result of the operation in that register is printed in the ".ll" file. This new register is created to comply with the SSA rule of LLVM. The new register is named as reg_i where i is the `current_reg_id` which is incremented everytime a new register is created. This ensures that the names of registers are unique. Then the char* that makeOperation returns which is the variable storing the result of the operation is returned. Below is the code of the `parseBitwiseOrExpression` function:

```
// in parser.c

char* parseBitwiseOrExpression(){
char* result = parseBitwiseAndExpression(); // first (bitwise and expression) to be bitwise 'or'ed

    while (token_index < token_count && strcmp(current_token.symbol, "|")==0){    // bitwise 'or' and 'and' (second lowest precedency in the grammar)
        token t = current_token;
        matchToken(OPERATOR_BITWISE);
        result = makeOperation(result, parseBitwiseAndExpression(), "%s = or i32 %s, %s \n");
    }
    return result;

};

char* makeOperation(char* result, char* term, const char* format){
char\* temp_result = createRegDefault();
fprintf(output_file, format, temp_result, result, term);
free(term);
free(result);
return temp_result;
}
```

```
// in reg.c

char* createRegDefault(){
char* reg = malloc(128 \* sizeof(char));
sprintf(reg,"%%reg\_%d", current_reg_id);
current_reg_id++;
return reg;
}
```

The same process applies for other parse functions, one can look at the source code for further inspection.

###Hashing

In this project, hashing is not used for storing the values but instead only storing the variables's names to know if they are declared before or not. If they are not initialized and a find function is called to find them (this means the variable is involved in an expression, which is an error in this project) then we throw an error. The hashing algorithm is the same as the first project. The djb2 hash function is used and probing is done linearly. The hashmap implementation is also the same.

###LLVM IR Code Generation

Writing functions for some operations to generate the LLVM IR code became cumbersome. For example, bit rotating operations needed multiple lines and register creations. So two special functions for rotation operations are defined. LLVM functions were not preferred for this operation but rather boilerplate code was generated. This choice was made simply because the development team did not want to further complicate the code.

### 4.4 Error Handling

We defined the `matchToken` function to check if the current token is the expected token. If it is not, it returns an error.
This works because the expected token is the only possible token for the statement according to the BNF and the recursive hierarchy. In other words, the grammar of our BNF is unambiguous.
We also defined a `raiseTokenError` function to raise error if the token is not one of the expected types in `parseFactor`.
This way also ensured our hierarchy worked properly.
Source:

## Appendix

### BNF

Below is the Extended Backus-Naur form for AdvCalc++.

```
<Statement> ::= <Assignment> | <BitwiseOrExpression>
<Assignment> ::= <Variable> "=" <BitwiseOrExpression>
<BitwiseOrExpression> ::= <BitwiseAndExpression> ("|" <BitwiseAndExpression>)*
<BitwiseAndExpression> ::= <Expression> ("&" <Expression>)*
<Expression> ::= <Term> (("+"|"-") <Term>)*
<Term> ::= <Factor> (("*" | "/" | "%") <Factor>)
<Factor> ::= <Variable> | <integer> | <UnaryFunction> | <BinaryFunction> | "(" <BitwiseOrExpression> ")"
<Variable> ::= <identifier>
<UnaryFunction> ::= "not" "(" <BitwiseOrExpression> ")"
<BinaryFunction> ::= <BinaryFunctionOperator>  "(" <BitwiseOrExpression> "," <BitwiseOrExpression> ")"
<BinaryFunctionOperator> ::= "xor" | "lr" | "rr" | "ls" | "rs"
<integer> ::= <digit>+
<identifier> ::= <letter> (<letter>)*
```

[1]: Wikipedia - https://en.wikipedia.org/wiki/Intermediate_representation
[2]: Wikipedia - https://en.wikipedia.org/wiki/Static_single-assignment_form
