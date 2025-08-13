- Declaration and Derefrence have the same syntax but different meanings.
- **void** type cast -> Generic pointer(Means the type is not known yet)
```C
void print(void *data, char datatype){
	if datatype == '--':
		(int*) data //Type casting it to any datatype you want
}
```

- In C array always decays into a pointer it means decau is the process in which an array gets coverted to a pointer.

-  `arr[5]==5[arr]` ??
*arr[n] means (arr+n) therefore 5[arr] eq *(5+arr)*

