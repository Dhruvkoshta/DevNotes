## Example: Read in File

```C
FILE *fp = fopen("input.txt", "r");
char line[1024];
char **lines = NULL;
size_t num_lines = 0;
if (!fp) {
	abort();
}  
// 1. Read in the file.
while (1) {    
	char *l = fgets(line, sizeof(line), fp);
	if (!l) {
	    break; // End of file (hopefully).    
	}    
	// Make room in the list of lines.
	if (!num_lines) {
		lines = malloc(sizeof(char *));
	} else {
	    lines = realloc(lines, (num_lines + 1) * sizeof(char *));
	}    
	if (!lines) {
	    abort(); // We are out of memory so panic.
	}    
	char *copy = strdup(l);
	if (!copy) {
	    abort(); // We are out of memory so panic.
	}    
	lines[num_lines] = copy;
    num_lines++;
}
```

we have the next line of the file in `line`, but we overwrite that buffer every time we read a new line from the file, so we need to store it somewhere. We use the `lines` variable for this, which is defined as:

```c
  char **lines = NULL;  size_t num_lines = 0;
```

Storing a copy of `line` is a two-part process:

1. Make a copy of the line itself.
2. Store the pointer to that line in `lines`.

However, in order to store that pointer, we first need to make room in `lines`, which means allocating some memory. There are actually two cases here:

1. Lines is empty (nothing is stored), which happens at the start.
2. Lines is non-empty but doesn't have enough room.

We distinguish these by looking at `num_lines` which starts at `0`. In the former case, we allocate enough memory for a single line, like so:

```c
      lines = malloc(sizeof(char *));
```

==The latter case is more complicated, however, because we already have something in `lines`, it's just that there's not (necessarily) enough room in memory to add another value. This means we (may) need to

1. Allocate enough memory to hold the new number of values.
2. Copy over the current contents of `lines` into the new memory region.
3. De-allocate the original memory.

### When you call `malloc(size)` the system guarantees that the returned pointer is _at least_ big enough to hold an object of size `size`—assuming that the allocation succeeds—but it's allowed to be larger

As we are doing here, then it's not efficient to have to copy the contents of the object over and over again. Instead, you can allocate more space than the programmer asked for and then when they ask for more, just say "ok" without taking any other action. All of this is handled automatically by the `realloc()` function call.


If this function were to return without cleaning up, it would create what's called a "memory leak". Remember that the only variable in our program that knows about any of this memory is `lines`, which points to the list of pointers for the individual stored lines. `lines` is on the stack and will be lost when the function returns, so if the function returns without cleaning up, then there is no program variable pointing to any of this memory and it's just lost.[[6]](https://educatedguesswork.org/posts/memory-management-1/#fn6) The result of a memory leak is that the leaked memory isn't available for new allocations but also can't be used because there's nothing pointing to it. If the program runs long enough and has a big enough leak, you can eventually accumulate enough leaked memory to affect the program function or even cause it to run out of memory, so you want to clean up. This is one reason why it often works to restart a program that seems stalled.

```c
  // Clean up.  
  fclose(fp);  
  for (size_t i=0; i<num_lines; i++) {
      free(lines[i]);  
	}  
  free(lines);
```

## How `malloc()` works [#](https://educatedguesswork.org/posts/memory-management-1/#how-malloc\(\)-works)

So far we've just been treating `malloc()` as a kind of black box, and that's generally fine for most programming tasks, but it's helpful to have some sense of what's going on internally. The first thing to realize is that `malloc()` isn't magic. In fact, you can write your own memory allocator in C (Firefox, for instance, uses a custom allocator).

At a very high level, you should think of `malloc()` as having access to one or more large contiguous blocks of memory, which it then dispenses on demand. On a very simple computer, `malloc()` would just have access to the entire memory of the machine, but on a modern multiprocess operating system, it gets chunks of memory from the operating system. For our purposes, let's easiest to think of it as having a big contiguous chunk of memory to work with. As I said, we usually wouldn't start at memory location 0, so we'll just assume the block starts at 1000.

The figure below shows the situation after a single allocation of size 200, with the allocation being red and the unallocated space being blue. What's happened here is just that `malloc(200)` just picked the first available memory region, which is at the start of the block because no memory has been allocated.

![[Pasted image 20250722230727.png]]

The allocation starts at address 1000 and goes to address 1199, so `malloc()` just returns the address `1000`, which points to the start of the allocated region.

The next figure shows the situation with two more allocations, one of size 400 and one of size 200. Again, this is what you'd expect: the allocator just picks the lowest available region. As noted above, a real allocator would probably leave some extra space to facilitate growing the allocation but we're trying to keep things simple for the purpose of examples. Designing fast memory allocators is a whole (complicated) topic all on its own.

![[Pasted image 20250722230831.png]]

As the program runs longer and memory is allocated and freed you tend get lots of small holes that can't be filled with big allocations, and so you have to allocate higher and higher memory regions. This is called _fragmentation_. In the extreme, you can get to the point where you can't allocate new memory even though there's actually plenty of free space; it's just not in a convenient form. There are techniques for avoiding this kind of fragmentation as well as for allocating memory more efficiently, but they're too advanced to cover here.

When you call `malloc()` the allocator finds a suitable region and allocates it. When you call `free()` it adds it to the list of holes (or adjusts the highest allocation value if it's the highest allocation).

Interestingly, it's not always necessary to store a list of every chunk of allocation memory. You can do this, but that means you need some data structure that lets you look up the allocations from their addresses. A common thing people do instead is to store the per-allocation metadata as a header right before the allocated region. The header contains the size of the allocation and maybe some other stuff. For instance, the first allocation above might look like this:
![[Pasted image 20250722230943.png]]

## Multiple References and UAF [#](https://educatedguesswork.org/posts/memory-management-1/#multiple-references-and-uaf)

Let's consider a slight modification of the function we've been looking at, in which along with printing out all the lines, we instead return the last line in sort order. With a lot of trimming, the function might look like this:

![[Pasted image 20250722231049.png]]

This function gets called like this:

```c
char *largest = find_largest("input.txt");printf("%s\n", largest);
```

The experienced C programmer will immediately note that this code has a serious bug, because we are trying to use the memory pointed to `largest` after we have `free()`d it. When the calling function tries to use `largest`, there are **no guarantees at all** about what will happen. This is called a _use after free (UAF)_ bug. 
For example, the allocator might have reallocated the memory in response to some other call to `malloc()`, in which case it is now full of some other data. Of course, it's also quite likely that the region is still unused and has the same contents as before; it's just that the allocator added it to the list of holes. In this case, the program may work fine under test but then fail unpredictably later when some change to your code causes allocations to happen differently and suddenly `largest` points to some memory reason being used for something else.

The reason this is all possible is that in C pointers are just values that hold the memory address; effectively they're just numbers and they behave like numbers. So if you assign a pointer value to another variable, now you have two variables that point to the same thing (i.e., they have the same value). When we call free on the first copy of the variable, that doesn't have any effect at all on the other copy (or on the first one, for that matter). It just changes the state of the memory region addressed by the variable. Once you've called `free(x)` you're still left with whatever is in `x`, and nothing in C stops you from using it; it's just illegal to do so, and it's your job not to, or else.


## Memory Leaks

A **memory leak** happens when dynamic memory is allocated but never freed.

If a memory leak happens in a loop or in a function that is called frequently it could take up too much memory and cause the computer to slow down.

There is a risk of a memory leak if a pointer to dynamic memory is lost before the memory can be freed. This can happen accidentally, so it is important to be careful and keep track of pointers to dynamic memory.

### Example 1

The pointer is overwritten:
```C
int x = 5;  
int *ptr;  
ptr = calloc(2, sizeof(*ptr));  
ptr = &x;
```
In this example, after the pointer is changed to point at `x`, the memory allocated by `calloc()` can no longer be accessed.

### Example 2

The pointer exists only inside a function:
```C
void myFunction() {  
  int *ptr;  
  ptr = malloc(sizeof(*ptr));  
}  
  
int main() {  
  myFunction();  
  printf("The function has ended");  
  return 0;  
}
```

In this example, the memory that was allocated inside of the function remains allocated after the function ends but it cannot be accessed anymore. One way to prevent this problem is to free the memory before the function ends.

###  Example 3

The pointer gets lost when reallocation fails:
```C
int* ptr;  
ptr = malloc(sizeof(*ptr));  
ptr = realloc(ptr, 2*sizeof(*ptr));
```

If `realloc()` is unable to reallocate memory it will return a pointer to `NULL` and the original memory will remain reserved.

In this example, if `realloc()` fails then the `NULL` pointer is assigned to the `ptr` variable, overwriting the original memory address so that it cannot be accessed anymore.
