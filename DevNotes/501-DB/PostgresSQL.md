# Basic Commands:

### Create Table
```sql
CREATE TABLE cars (  
  brand VARCHAR(255),  
  model VARCHAR(255),  
  year INT  
);
```

### Insert into 
```sql
INSERT INTO cars (brand, model, year)  
VALUES ('Ford', 'Mustang', 1964);
```

### Alter table
```sql
ALTER TABLE cars  
ADD color VARCHAR(255);
```

```sql
ALTER TABLE cars  
ALTER COLUMN year TYPE VARCHAR(4);
```
### Update Table
```sql
UPDATE cars  
SET color = 'red'  
WHERE brand = 'Volvo';
```

### Where Clause (All operators):
We can operate with different operators in the `WHERE` clause:

|           |                                                                  |
| --------- | ---------------------------------------------------------------- |
| `=`       | Equal to                                                         |
| `<`       | Less than                                                        |
| `>`       | Greater than                                                     |
| `<=`      | Less than or equal to                                            |
| `>=`      | Greater than or equal to                                         |
| `<>`      | Not equal to                                                     |
| `!=`      | Not equal to                                                     |
| `LIKE`    | Check if a value matches a pattern (case sensitive)              |
| `ILIKE`   | Check if a value matches a pattern (case insensitive)            |
| `AND`     | Logical AND                                                      |
| `OR`      | Logical OR                                                       |
| `IN`      | Check if a value is between a range of values                    |
| `BETWEEN` | Check if a value is between a range of values                    |
| `IS NULL` | Check if a value is NULL                                         |
| `NOT`     | Makes a negative result e.g. `NOT LIKE`, `NOT IN`, `NOT BETWEEN` |

### JOINS:
![[Pasted image 20250726191832.png]]

### UNION:

The `UNION` operator is used to combine the result-set of two or more queries.

The queries in the union must follow these rules:

- They must have the same number of columns
- The columns must have the same data types
- The columns must be in the same order

Combine `products` and `testproducts` using the `UNION` operator:
```sql
SELECT product_id, product_name  
FROM products  
UNION  
SELECT testproduct_id, product_name  
FROM testproducts  
ORDER BY product_id;
```
