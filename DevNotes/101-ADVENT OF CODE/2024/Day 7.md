```python
total  = 0

def concat(a,b):
    return int(str(a)+str(b))


def canReach(currentTotal,remaining,target):
    if len(remaining)==0:
        return currentTotal==target
    
    next = remaining[0]
    rest = remaining[1:]
    
    withAdd = canReach(currentTotal+next,rest,target)
    withMul = canReach(currentTotal*next,rest,target)

    concatTotal = concat(currentTotal,next)
    withConcat = canReach(concatTotal,rest,target)

    return (withAdd or withMul or withConcat)


with open("input.txt") as f:
    for line in f:
        target,rest = line.split(':')
        target = int(target)
        numbers = [int(x) for x in rest.split()]

        firstNum = numbers[0]
        remaining = numbers[1:]
        
        if canReach(firstNum,remaining,target):
            total = total + target

print("total:",total)

```
