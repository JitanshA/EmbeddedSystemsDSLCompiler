# Context-Free Grammar for Embedded Systems DSL

## Grammar Rules

### Start Rule
```
S -> function_list
```

### Function List
```
function_list -> function function_list | ε
```

### Function Definition
```
function -> type identifier ( function_parameters ) { statementList }
```

### Function Parameters
```
function_parameters -> type identifier , function_parameters | type identifier | ε
```

### Statement
```
statement -> identifier declaration
           | identifier definition
           | conditional
           | GPIO_operation
           | while_loop
           | return expression ;
```

### Variable Declarations and Definitions
```
identifier declaration -> type identifier ;
type -> int | bool
identifier definition -> int identifier = expression ; | bool identifier = bool_value ;
bool_value -> true | false
```

### Conditional Statements
```
conditional -> if ( expression ) { statementList } optional_else
optional_else -> else { statementList } | ε
```

### Expressions
```
expression -> logical_expression
logical_expression -> logical_expression && relational_expression
                   | logical_expression || relational_expression
                   | relational_expression
relational_expression -> relational_expression < additive_expression
                       | relational_expression > additive_expression
                       | relational_expression <= additive_expression
                       | relational_expression >= additive_expression
                       | relational_expression == additive_expression
                       | relational_expression != additive_expression
                       | additive_expression
additive_expression -> additive_expression + multiplicative_expression
                     | additive_expression - multiplicative_expression
                     | multiplicative_expression
multiplicative_expression -> multiplicative_expression * term
                           | multiplicative_expression / term
                           | term
term -> identifier | NUMBER | ( expression )
```

### Statement List
```
statementList -> statement statementList | ε
```

### GPIO Operations
```
GPIO_operation -> READ_PIN ( GPIO_PIN ) ; | SET_PIN ( GPIO_PIN, GPIO_values ) ;
GPIO_values -> HIGH | LOW
GPIO_PIN -> POSITIVE_NUMBER
```

### While Loop
```
while_loop -> while ( expression ) { statementList }
```
