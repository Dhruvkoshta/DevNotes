l1 = []
l2 = []
total = 0
with open("input.txt","r") as file:
    for line in file.readlines():
        a = line.strip().split("  ")
        l1.append(int(a[0].strip()))
        l2.append(int(a[1].strip()))
        l1.sort()
        l2.sort()
for i in l1:
    count = l2.count(i)
    val = count * i
    total += val
print(total)
