

**1. Program to find the best of two test average marks out of three test’s marks accepted from the user.**

``` Python
m1 = int(input("Enter marks for test1 : "))
m2 = int(input("Enter marks for test2 : "))
m3 = int(input("Enter marks for test3 : "))

if m1 <= m2 and m1 <= m3:
    avgMarks = (m2+m3)/2
elif m2 <= m1 and m2 <= m3:
    avgMarks = (m1+m3)/2
elif m3 <= m1 and m3 <= m2:
    avgMarks = (m1+m2)/2

print("Average of best two test marks out of three test’s marks is", avgMarks);
```

**2. Program to generate a Fibonacci sequence up to a specified length.**

``` Python
def f(n):
    if n<=1:
        return n
    else:
        return f(n-1)+f(n-2)
num=int(input("Number of terms : "))
for i in range(num):
    print(f(i))
```

**3.a) Develop a program to check whether a given number is Palindrome or not.**

``` Python
val = int(input("Enter a value : "))
str_val = str(val)
if str_val == str_val[::-1]:
    print("Palindrome")
else:
    print("Not Palindrome")

for i in range(10):
    if str_val.count(str(i)) > 0:
        print(str(i),"appears", str_val.count(str(i)), "times");
```

**3.b) Develop a program to check whether a given character is Palindrome or not.**

``` Python
val = input("Enter a char: ")
char_val = str(val)

# Check if the string is a palindrome
if char_val == char_val[::-1]:
    print("Palindrome")
else:
    print("Not Palindrome")
```

``` Python
def is_palindrome_number(number):
    original_number = number
    reversed_number = 0
    while number > 0:
        digit = number % 10  #
        reversed_number = reversed_number * 10 + digit
        number = number // 10
    return original_number == reversed_number
```
**4. Develop a program to convert Decimal to binary, Octal and Hexa-decimal and vice-versa using functions and Loops.**

**a). Binary to Decimal**

``` Python
def bin2dec(n):
    if n<=1:
        return n
    else:
        return ((n%10)+2*bin2dec(n//10))
num=int(input("Enter binary number : "))
dec=bin2dec(num)
print("Decimal Equivalent Number : ",dec)
```

**b). Decimal to Binary**

``` Python
def dec2bin(n):
    if n<=1:
        return str(n)
    else:
        return dec2bin(n // 2) + str(n % 2)
num=int(input("Enter a deciaml number : "))
bin1=int(dec2bin(num)[::-1])
print("Decial Binary Equivalent Number : ",bin1)
```

**c). Decimal to Octal**

``` Python
def dec2oct(n):
    if n<8:
        return str(n)
    else:
        return (str(n%8)+str(dec2oct(n//8)))
num=int(input("Enter any Decimal Number : "))
oct1=int(dec2oct(num)[::-1])
print("Equivalent Octal Number : ",oct1)
```

**d). Octal to Decimal**

``` Python
def oct2dec(n):
    if n<8:
        return n
    else:
        return ((n%10)+8*oct2dec(n//10))
num=int(input("Enter any Octal Number : "))
dec=oct2dec(num)
print("Equivalent Decimal Number : ",dec)
```

**e). Hexadecimal to Decimal**

``` Python
def hex_to_dec_manual(hex_str):
    decimal_value = 0
    power = 0
    hex_map = {
        '0': 0, '1': 1, '2': 2, '3': 3, '4': 4,
        '5': 5, '6': 6, '7': 7, '8': 8, '9': 9,
        'A': 10, 'B': 11, 'C': 12, 'D': 13, 'E': 14, 'F': 15
    }
    hex_str = hex_str.upper()
    
    for digit_char in reversed(hex_str):
        digit_value = hex_map[digit_char]
        decimal_value += digit_value * (16 ** power)
        power += 1
    return decimal_value

```

**f). Decimal to Hexadecimal**

``` Python
def dec2hex(n):
    hex_chars = "0123456789ABCDEF" 
    hexadecimal_str = "" 
    while decimal_num > 0: 
	    remainder = decimal_num % 16 
	    hexadecimal_str = hex_chars[remainder] + hexadecimal_str 
	    decimal_num = decimal_num // 16
	return hexadecimal_str
num=int(input("Enter any Decimal Number : "))
print("Equivalent Hexadecimal Number : ",num)
```

**II. OOPS Concepts in Python Programming: Classes, Objects and Inheritance.**

**5. Declare a base class to calculate Resistance from voltage and current and extend the class to calculate inductance and capacitance with varying voltage and current values with respect to time.**

``` Python
class ResistanceCalculator:
    def __init__(self, voltage, current):
        self.voltage = voltage
        self.current = current

    def calculate_resistance(self):
        if self.current != 0:
            return self.voltage / self.current
        else:
            return float('inf')

class InductanceCalculator(ResistanceCalculator):
    def __init__(self, voltage, current):
        super().__init__(voltage, current)

    def calculate_inductance(self, frequency):
        if self.current != 0 and frequency != 0:
            return self.voltage / (2 * 3.14159 * frequency * self.current)
        else:
            return float('inf')

class CapacitanceCalculator(ResistanceCalculator):
    def __init__(self, voltage, current):
        super().__init__(voltage, current)

    def calculate_capacitance(self, frequency):
        if self.current != 0 and frequency != 0:
            return self.current / (2 * 3.14159 * frequency * self.voltage)
        else:
            return float('inf')

# Usage example:
voltage_value = 220 # Volts
current_value = 0.1 # Amperes
frequency_value = 50 # Hertz

# Calculate resistance
resistance_calculator = ResistanceCalculator(voltage_value, current_value)
resistance_result = resistance_calculator.calculate_resistance()
print("Resistance:", resistance_result, "ohms")

# Calculate inductance
inductance_calculator = InductanceCalculator(voltage_value, current_value)
inductance_result = inductance_calculator.calculate_inductance(frequency_value)
print("Inductance:", inductance_result, "Henries")

# Calculate capacitance
capacitance_calculator = CapacitanceCalculator(voltage_value, current_value)
capacitance_result = capacitance_calculator.calculate_capacitance(frequency_value)
print("Capacitance:", capacitance_result, "Farads")
```

**6. By using the concept of inheritance compose a program to find the area of triangle, circle and rectangle.**

``` Python
import math
class Shape:
    def __init__(self):
        self.area = 0
        self.name = ""

    def showArea(self):
        print("The area of the", self.name, "is", self.area, "units")

class Circle(Shape):
    def __init__(self, radius):
        super().__init__()
        self.name = "Circle"
        self.radius = radius

    def calcArea(self):
        self.area = math.pi * self.radius * self.radius

class Rectangle(Shape):
    def __init__(self, length, breadth):
        super().__init__()
        self.area = 0
        self.name = "Rectangle"
        self.length = length
        self.breadth = breadth

    def calcArea(self):
        self.area = self.length * self.breadth

class Triangle(Shape):
    def __init__(self,base,height):
        super().__init__()
        self.area = 0
        self.name = "Triangle"
        self.base = base
        self.height = height

    def calcArea(self):
        self.area = self.base * self.height / 2

c1 = Circle(5)
c1.calcArea()
c1.showArea()

r1 = Rectangle(5, 4)
r1.calcArea()
r1.showArea()

t1 = Triangle(3, 4)
t1.calcArea()
t1.showArea()
```

**III. Application to Field Theory.**

**7. Develop a program to demonstrate the electric field lines due to a point charge.**

``` Python
import numpy as np
import matplotlib.pyplot as plt

# Constants
k = 8.99e9 # Coulomb's constant, N m²/C²
q = 1e-9 # Charge, C

# Create a grid of points
x = np.linspace(-10, 10, 400)
y = np.linspace(-10, 10, 400)
X, Y = np.meshgrid(x, y)

# Calculate electric field components
def electric_field(q, r0, x, y):
    """Return the electric field vector E = (Ex, Ey) due to a point charge q at r0."""
    den = np.hypot(x - r0[0], y - r0[1])**3 # Calculate the denominator
    Ex = k * q * (x - r0[0]) / den
    Ey = k * q * (y - r0[1]) / den
    return Ex, Ey

# Position of the point charge
r0 = np.array([0.0, 0.0])

# Calculate the electric field components
Ex, Ey = electric_field(q, r0, X, Y)

# Plotting
fig, ax = plt.subplots(figsize=(8, 8))
color = np.log(np.hypot(Ex, Ey))

# Plot electric field lines
ax.streamplot(X, Y, Ex, Ey, color=color, linewidth=1, cmap='inferno', density=2)

# Add point charge to the plot
ax.plot(r0[0], r0[1], 'ro')

# Labels and title
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_title('Electric Field Lines due to a Point Charge')

# Set limits
ax.set_xlim(-10, 10)
ax.set_ylim(-10, 10)

plt.show()
```

**8. Develop a program on Standing waves animation.**

``` Python
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Parameters
L = 10 # Length of the string
k = 2 * np.pi / L # Wave number
omega = 2 * np.pi # Angular frequency
A = 1 # Amplitude
x = np.linspace(0, L, 1000) # Position along the string

# Create a figure and axis
fig, ax = plt.subplots()
line, = ax.plot(x, np.zeros_like(x), lw=2)

# Set up plot limits and labels
ax.set_xlim(0, L)
ax.set_ylim(-2 * A, 2 * A)
ax.set_xlabel('Position')
ax.set_ylabel('Displacement')
ax.set_title('Standing Waves')

# Function to update the frame
def update(frame):
    t = frame / 30 # Time variable
    y = 2 * A * np.sin(k * x) * np.cos(omega * t) # Standing wave equation
    line.set_ydata(y)
    return line,

# Create the animation
ani = animation.FuncAnimation(fig, update, frames=300, interval=20, blit=True)

# Display the animation
plt.show()
```

**IV. Application to signals and systems and controls systems.**

**9. Develop a Program for Sine Wave Generation.**

``` Python
import math
import numpy as np
import matplotlib.pyplot as plt

def generate_sine_wave(freq, amplitude, duration, sampling_rate):
    num_samples = int(duration * sampling_rate)
    time_period = 1.0 / sampling_rate
    time_values = np.arange(0, duration, time_period)
    sine_wave = amplitude * np.sin(2 * np.pi * freq * time_values)
    return time_values, sine_wave

# Example usage
frequency = 5 # Frequency of the sine wave in Hz
amplitude = 1.0 # Amplitude of the sine wave
duration = 2 # Duration of the sine wave in seconds
sampling_rate = 44100 # Sampling rate (number of samples per second)
time_values, sine_wave = generate_sine_wave(frequency, amplitude, duration, sampling_rate)

# Plot the sine wave
plt.figure(figsize=(8, 4))
plt.plot(time_values, sine_wave)
plt.xlabel('Time (seconds)')
plt.ylabel('Amplitude')
plt.title('Sine Wave')
plt.grid(True)
plt.show()
```

**10. Program to display pole – zero plot for a given transfer function.**

``` Python
import numpy as np
import matplotlib.pyplot as plt
from scipy import signal

					numerator_coeffs = [5, 2] # This represents: 5s + 2
denominator_coeffs = [3, 2, 5] # This represents: 3s^2 + 2s + 5

transfer_function = signal.TransferFunction(numerator_coeffs, denominator_coeffs)
poles = transfer_function.poles
zeros = transfer_function.zeros

plt.figure(figsize=(8, 6))
plt.scatter(np.real(poles), np.imag(poles), marker='x', color='red', label='Poles')
plt.scatter(np.real(zeros), np.imag(zeros), marker='o', color='blue', label='Zeros')
plt.axhline(0, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.xlabel('Real')
plt.ylabel('Imaginary')
plt.title('Pole-Zero Plot of Transfer Function')
plt.legend()
plt.grid()
plt.show()
```

**11. Program to solve a given 2nd order difference equation using Z transform.**

``` Python
import numpy as np
import matplotlib.pyplot as plt

def unilateral_z_transform_difference_equation(a, x):
    n = len(x)
    y = np.zeros(n)
    y[0] = x[0] # Initial condition
    for i in range(1, n):
        y[i] = a * y[i-1] + x[i]
    return y

# Define the difference equation parameters
a = 0.5
# Generate an input sequence
n_samples = 10
x = np.ones(n_samples) # Impulse sequence
# Solve the difference equation using the unilateral Z-transform
y = unilateral_z_transform_difference_equation(a, x)

# Print the output sequence
print("Output sequence (y):", y)
# Plot the input and output sequences
plt.stem(range(n_samples), x, basefmt="b-", linefmt="b-", markerfmt="bo", label="Input x[n]")
plt.stem(range(n_samples), y, basefmt="r-", linefmt="r-", markerfmt="ro", label="Output y[n]")
plt.xlabel("n")
plt.ylabel("Amplitude")
plt.title("Difference Equation Solution using Unilateral Z-Transform")
plt.legend()
plt.show()
```

**12. Program to solve a given 2nd order differential equation using Laplace transform.**

``` Python
import sympy as sp
from scipy import signal

t,s=sp.symbols('t,s')
#y"+5y’+6y=0
#y(0)=0,y'(0)=2
#s^2L[y(s)]-y'(0)-y(0)+5sL[y(t)]-y(0)+6L[y(t)]
#s^2L[y(s)]-5sL[y(t)]+6L[y(t)]=y(0)+y’(0)
coeff=[1,5,6]
initial_cond=[0,2] #y(0),y'(0)
l_eq_lhs=(coeff[0]*(s**2)+coeff[1]*(s)+coeff[2])
eq_rhs=initial_cond[0]+initial_cond[1]
l_eq_rhs=eq_rhs/s
tf=l_eq_rhs/l_eq_lhs
l_eq=sp.inverse_laplace_transform(tf,s,t)
print(l_eq)
```

**13. Program to display Bode plot for a given second order system.**

``` Python
import numpy as np
import matplotlib.pyplot as plt
from scipy import signal

# Define the transfer function
numerator = [1]
denominator = [1, 2, 1] # Example: Second-order system (s^2 + 2s + 1)
system = signal.TransferFunction(numerator, denominator)

# Frequency range for Bode plot
omega = np.logspace(-2, 2, 1000)

# Calculate frequency response
_, mag, phase = signal.bode(system, omega)

# Plot Bode magnitude plot
plt.figure(figsize=(10, 6))
plt.subplot(2, 1, 1)
plt.semilogx(omega, mag)
plt.xlabel('Frequency (rad/s)')
plt.ylabel('Magnitude (dB)')
plt.title('Bode Magnitude Plot')
plt.grid(True)

# Plot Bode phase plot
plt.subplot(2, 1, 2)
plt.semilogx(omega, phase)
plt.xlabel('Frequency (rad/s)')
plt.ylabel('Phase (degrees)')
plt.title('Bode Phase Plot')
plt.grid(True)
plt.tight_layout()
plt.show()
```

**14. Program to display Nyquist plot for a given second order system.**

``` Python
import numpy as np
import matplotlib.pyplot as plt
from scipy import signal

# Define the transfer function
numerator = [1]
denominator = [1, 2, 1] # Example: Second-order system (s^2 + 2s + 1)
system = signal.TransferFunction(numerator, denominator)

# Frequency range for Nyquist plot
omega = np.logspace(-2, 2, 1000)

# Calculate frequency response
_, h = signal.freqresp(system, omega)

# Extract real and imaginary parts
real_part = np.real(h)
imag_part = np.imag(h)

# Plot Nyquist plot
plt.figure(figsize=(8, 6))
plt.plot(real_part, imag_part)
plt.plot(real_part, -imag_part) # Plot the mirrored half

plt.xlabel('Real Part')
plt.ylabel('Imaginary Part')
plt.title('Nyquist Plot')
plt.grid(True)
plt.axis('equal') # Equal aspect ratio
plt.show()
```

### Activity Programs

**1. Develop a program to convert Octal to Hexadecimal and Hexadecimal to Octal.**

**Octal to Hexadecimal:**

``` Python
def dec2hex(n):
    if n<=1:
        return str(n)
    else:
        if(n%16>9):
            return ((chr(ord('A')+(n%16)-10))+str(dec2hex(n//16)))
        else:
            return (str(n%16)+str(dec2hex(n//16)))

def oct2dec(n):
    if n<=1:
        return n
    else:
        return ((n%10)+8*oct2dec(n//10))

def oct2hex(n):
    dec1=oct2dec(n)
    hex1=dec2hex(dec1)
    return hex1

num=int(input("Enter octal number : "))
print("Hexadecimal number = ",oct2hex(num)[::-1])
```

**2. Write a python program to implement insertion sort and merge sort using lists.**

**Insertion sort:**

``` Python
def insertion_sort(lst):
    for i in range(1, len(lst)):
        key = lst[i]
        j = i - 1
        while j >= 0 and key < lst[j]:
            lst[j + 1] = lst[j]
            j -= 1
        lst[j + 1] = key
lst=eval(input("Enter the list to be sorted : "))
insertion_sort(lst)
print("Sorted list :",lst)
```

**Merge sort:**

``` Python
def merge_sort(arr):
    if len(arr) <= 1:
        return arr

    mid = len(arr) // 2
    left_half = arr[:mid]
    right_half = arr[mid:]

    left_sorted = merge_sort(left_half)
    right_sorted = merge_sort(right_half)

    return merge(left_sorted, right_sorted)

def merge(left, right):
    merged_list = []
    i = 0
    j = 0

    while i < len(left) and j < len(right):
        if left[i] < right[j]:
            merged_list.append(left[i])
            i += 1
        else:
            merged_list.append(right[j])
            j += 1

    while i < len(left):
        merged_list.append(left[i])
        i += 1

    while j < len(right):
        merged_list.append(right[j])
        j += 1

    return merged_list

```

**3. Generating a Random Number using inbuilt functions randrange (), shuffle, uniform.**

**From a given list of specified Random Numbers:**

``` Python
import random
# prints a random value from the list
list1 = [1, 2, 3, 4, 5, 6]
print(random.choice(list1))

# prints a random item from the string
string = "striver"
print(random.choice(string))
```

**Random floating-point number between 0 and 1:**

``` Python
import random
random_float = random.random()
print("Random float:", random_float)
```

**Random integer between a given range (e.g., 1 and 100):**

``` Python
import random
random_int = random.randint(1, 100)
print("Random integer:", random_int)
```

**4. Program to find and print all permutations of a given sequence [Integers/String] using list and functions.**

``` Python
def toString(List):
    return ''.join(List)

def permute(a, l, r):
    if l == r:
        print (toString(a))
    else:
        for i in range(l, r + 1):
            a[l], a[i] = a[i], a[l]
            permute(a, l + 1, r)
            a[l], a[i] = a[i], a[l] # backtrack

string = '123'
n = len(string)
a = list(string)
permute(a, 0, n-1)
```

**5. Program to find and print all Combinations of a given sequence [ Integers/String] using list and functions.**

``` Python
l=eval(input("Enter a list of integer : "))
r=int(input("Enter the number of terms : "))
n=len(l)
if r==1:
    for i in range(n):
        print(l[i])
elif r==n:
    print(l)
else:
    for i in range(n-r):
        for j in range(n):
            if i!=j:
                print(l[i],l[j])
```

**6. Develop a Program for Cos Wave Generation.**

``` Python
import math
import numpy as np
import matplotlib.pyplot as plt

def generate_cos_wave(freq, amplitude, duration, sampling_rate):
    num_samples = int(duration * sampling_rate)
    time_period = 1.0 / sampling_rate
    time_values = np.arange(0, duration, time_period)
    cos_wave = amplitude * np.cos(2 * np.pi * freq * time_values)
    return time_values, cos_wave

frequency = 5
amplitude = 1.0
duration = 2
sampling_rate = 44100
time_values, cos_wave = generate_cos_wave(frequency, amplitude, duration, sampling_rate)

plt.figure(figsize=(8, 4))
plt.plot(time_values, cos_wave)
plt.xlabel('Time (seconds)')
plt.ylabel('Amplitude')
plt.title('Cosine Wave')
plt.grid(True)
plt.show()
```

**7. Program to solve a given 1st order difference equation using Z transform.**

``` Python
import numpy as np
import matplotlib.pyplot as plt

def difference_equation(a, x):
    n = len(x)
    y = np.zeros(n)
    y[0] = x[0]
    for i in range(1, n):
        y[i] = a * y[i-1] + x[i]
    return y

a = 0.5
n_samples = 10
x = np.ones(n_samples)
y = difference_equation(a, x)

print("Output sequence (y):", y)
plt.stem(range(n_samples), x, basefmt="b-", linefmt="b-", markerfmt="bo", label="Input x[n]")
plt.stem(range(n_samples), y, basefmt="r-", linefmt="r-", markerfmt="ro", label="Output y[n]")
plt.xlabel("n")
plt.ylabel("Amplitude")
plt.title("Difference Equation Solution using Unilateral Z-Transform")
plt.legend()
plt.show()
```

**8. Program to solve a given 1st order differential equation using Laplace transform.**

``` Python
import sympy as sp

t,s,Y=sp.symbols('t s Y')
ode=sp.Eq(sp.diff(Y,t)+3*Y,3*sp.exp(-4*t))
l_eq=sp.laplace_transform(ode.lhs-ode.rhs,t,s,noconds=True)
y_s=sp.solve(l_eq,Y)[0]
solution=sp.inverse_laplace_transform(y_s,s,t,noconds=True)
print(solution)
```



``` Python
import math
#marks
xi=[4,8,11,17,20,24,32]
#number of students
fi=[3,5,9,5,4,3,1]

#mean
n=len(xi)
sum_xifi=0
sum_fi=0
for i in range(n):
    sum_xifi+=(xi[i]*fi[i])
    sum_fi+=fi[i]
mean=(sum_xifi/sum_fi)
print("Mean = ",mean)

#median
xi1=xi
for i in range(n):
    for j in range(fi[i]-1):
        xi1.append(xi[i])
xi_asc=sorted(xi1)
n1=len(xi_asc)
if n1%2==0:
    median=(xi_asc[int(n1/2)]+xi_asc[int((n1/2)+1)])/2
else :
    median=xi_asc[(n1+1)/2]
print("Median = ",median)

#mode
max1=fi[1]
for i in range(1,n):
    if fi[i]>max1:
        mode=xi[i]
print("Mode = ",mode)

#varience
sum2=0
for i in range(n):
    sum2+=(fi[i]*(xi[i]-mean)**2)
var=sum2/n1
print("Varience = ",var)

#Standard Deviation
std=math.sqrt(var)
print("Standard Deviation = ",std)
```

**10. Program To Generate Random Numbers:**

**Random floating-point number between 0 and 1:**

``` Python
import random
random_float = random.random()
print("Random float:", random_float)
```

**Random integer between a given range (e.g., 1 and 100):**

``` Python
import random
random_int = random.randint(1, 100)
print("Random integer:", random_int)
```

**11. Program to print all permutations for a given length of sequence.**

**Using List & Using Library functions**

The document mentions this program but does not provide the code implementation for it.

**12. Program to print all permutations of coin tossing for a given number of flips.**

``` Python
import random

heads = 0
tails = 0

flips = 0 # Initialize flips to 0
while flips < 100:
    if random.randint(1,2) == 1:
        heads += 1
    else:
        tails += 1
    flips += 1
print("you got ", heads," heads, and ", tails," tails!")
input ("exit")
```

**13. Program to print all combinations of the dice using recursion and memorization.**

``` Python
def dice_combinations(k, memo={}):
    if k == 1:
        memo[1] = [(i,) for i in range(1, 7)]
        return memo[1]

    elif k in memo:
        return memo[k]

    else:
        prev_res = dice_combinations(k-1, memo)
        res = []

        for comb in prev_res:
            for j in range(1, 7):
                res.append(comb + (j,))
        memo[k] = res
        return res
```