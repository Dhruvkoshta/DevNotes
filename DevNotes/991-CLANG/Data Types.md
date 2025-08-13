| Data Type | Size         | Description                                                                                           | Example |
| --------- | ------------ | ----------------------------------------------------------------------------------------------------- | ------- |
| `int`     | 2 or 4 bytes | Stores whole numbers, without decimals                                                                | `1`     |
| `float`   | 4 bytes      | Stores fractional numbers, containing one or more decimals. Sufficient for storing 6-7 decimal digits | `1.99`  |
| `double`  | 8 bytes      | Stores fractional numbers, containing one or more decimals. Sufficient for storing 15 decimal digits  | `1.99`  |
| `char`    | 1 byte       | Stores a single character/letter/number, or ASCII values                                              | `'A'`   |



| `%d` or `%i` | `int`                                                                                                                        |     |
| ------------ | ---------------------------------------------------------------------------------------------------------------------------- | --- |
| `%f` or `%F` | `float`                                                                                                                      |     |
| `%lf`        | `double`                                                                                                                     |     |
| `%c`         | `char`                                                                                                                       |     |
| `%s`         | Used for **[strings](https://www.w3schools.com/c/c_strings.php) (text)**, which you will learn more about in a later chapter |     |

## Boolean Expression:
```c
#include <stdbool.h>

bool isProgrammingFun = true;  
bool isFishTasty = false;
// returned as 0,1

```


## Short Hand if else:
```c
_variable_ = (_condition_) ? _expressionTrue_ : _expressionFalse_;
```


## Switch statements:
```c
switch (_expression_) {  
  case x:  
    _// code block_  
    break;  
  case y:  
    _// code block_  
    break;  
  default:  
    _// code block_  
}
```

### *sizeof() includes the \0 wehere as strlen does not!!!*



