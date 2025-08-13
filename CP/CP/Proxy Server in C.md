This code sets up a **multithreaded proxy server** that handles HTTP GET requests and includes a **caching mechanism** to improve performance. It's designed to accept multiple client connections concurrently using threads.


# Socket Programming in C:
![[Pasted image 20250718185527.png]]

- ### Client Socket Process:
	- Set up a Socket (Prepare to communicate)
        - Create a socket
	    - Determine server IP address and port number
	    - Initiate the connection to the server
	- Send and receive packets (Exchange data with the server)
	    -  Write data (i.e., request) to the socket
	    -  Read data (i.e., response) from the socket
	    -  Do stuff with the data (e.g., display a Web page)
	 -  Close the socket.

- ### Server Socket Process:
	-  Set up a Socket (Prepare to communicate)
		-  Create a socket s_listen (i.e., the listening socket)
		-  Associate server’s IP address and port no. with the socket
	-  Wait to hear from a client
	    - Indicate how many connections can be pending on the socket
	    - Accept an incoming connection from a client, create a new socket s_new for the client.
	-  Send and receive packets (Exchange data with the client over the new socket s_new)
	    - Read data (i.e., client request) from the socket
	    - Handle the request
	    - Write data (i.e., server response) to the socket
	    - Close the socket s_new
	- Repeat 2.2-3.4 with the next connection request

## I. Main Server Program - `proxy.c`

Let's start by walking through the `main` function and then follow the execution flow to understand the other functions involved.

### `main(int argc, char * argv[])`

```c
int main(int argc, char * argv[]) {

	int client_socketId, client_len; // client_socketId == to store the client socket id
	struct sockaddr_in server_addr, client_addr; // Address of client and server to be assigned

    sem_init(&seamaphore,0,MAX_CLIENTS); // Initializing seamaphore and lock
    pthread_mutex_init(&lock,NULL); // Initializing lock for cache
    

	if(argc == 2)        //checking whether two arguments are received or not
	{
		port_number = atoi(argv[1]);
	}
	else
	{
		printf("Too few arguments\n");
		exit(1);
	}

	printf("Setting Proxy Server Port : %d\n",port_number);

    //creating the proxy socket
	proxy_socketId = socket(AF_INET, SOCK_STREAM, 0);

	if( proxy_socketId < 0)
	{
		perror("Failed to create socket.\n");
		exit(1);
	}

	int reuse =1;
	if (setsockopt(proxy_socketId, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0) 
        perror("setsockopt(SO_REUSEADDR) failed\n");

	bzero((char*)&server_addr, sizeof(server_addr));  
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port_number); // Assigning port to the Proxy
	server_addr.sin_addr.s_addr = INADDR_ANY; // Any available adress assigned

    // Binding the socket
	if( bind(proxy_socketId, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0 )
	{
		perror("Port is not free\n");
		exit(1);
	}
	printf("Binding on port: %d\n",port_number);

    // Proxy socket listening to the requests
	int listen_status = listen(proxy_socketId, MAX_CLIENTS);

	if(listen_status < 0 )
	{
		perror("Error while Listening !\n");
		exit(1);
	}

	int i = 0; // Iterator for thread_id (tid) and Accepted Client_Socket for each thread
	int Connected_socketId[MAX_CLIENTS];   // This array stores socket descriptors of connected clients

    // Infinite Loop for accepting connections
	while(1)
	{
		
		bzero((char*)&client_addr, sizeof(client_addr));			// Clears struct client_addr
		client_len = sizeof(client_addr); 

        // Accepting the connections
		client_socketId = accept(proxy_socketId, (struct sockaddr*)&client_addr,(socklen_t*)&client_len);	// Accepts connection
		if(client_socketId < 0)
		{
			fprintf(stderr, "Error in Accepting connection !\n");
			exit(1);
		}
		else{
			Connected_socketId[i] = client_socketId; // Storing accepted client into array
		}

		// Getting IP address and port number of client
		struct sockaddr_in* client_pt = (struct sockaddr_in*)&client_addr;
		struct in_addr ip_addr = client_pt->sin_addr;
		char str[INET_ADDRSTRLEN];										// INET_ADDRSTRLEN: Default ip address size
		inet_ntop( AF_INET, &ip_addr, str, INET_ADDRSTRLEN );
		printf("Client is connected with port number: %d and ip address: %s \n",ntohs(client_addr.sin_port), str);
		//printf("Socket values of index %d in main function is %d\n",i, client_socketId);
		pthread_create(&tid[i],NULL,thread_fn, (void*)&Connected_socketId[i]); // Creating a thread for each client accepted
		i++; 
	}
	close(proxy_socketId);									// Close socket
 	return 0;
}
```

  * **Purpose**: This is the entry point of the proxy server application. It initializes the server, sets up the listening socket, and then enters an infinite loop to accept incoming client connections, creating a new thread for each.
  * **Parameters**:
      * `int argc`: The number of command-line arguments provided when the program is run.
      * `char * argv[]`: An array of **pointers** to strings, where each string is a command-line argument. `argv[0]` is typically the program's name, `argv[1]` the first argument, and so on.
  * **Pointers and Variables**:
      * `int client_socketId`: This integer variable will store the **socket descriptor** for each new client connection accepted by the proxy. A socket descriptor is a small integer that uniquely identifies a socket in the operating system.
      * `int client_len`: An integer to store the size of the client's address structure.
      * `struct sockaddr_in server_addr`, `struct sockaddr_in client_addr`: These are structures that hold internet socket addresses.
          * `server_addr`: Will store the proxy server's own IP address and port.
          * `client_addr`: Will store the connecting client's IP address and port.
          * **Pointers**: ==When using these structures with socket functions, you often need to cast their **addresses** to `(struct sockaddr*)`. For example, `(struct sockaddr*)&server_addr` means "take the address of `server_addr` and treat it as a pointer to a generic `sockaddr` structure."
      * `sem_init(&seamaphore, 0, MAX_CLIENTS)`: Initializes a **semaphore**.
          * `&seamaphore`: The **address** of the `sem_t` variable to initialize.
          * `0`: Indicates that this semaphore is shared between threads of the same process.
          * `MAX_CLIENTS`: The initial value (or maximum value) of the semaphore. This means up to `MAX_CLIENTS` threads can proceed without blocking on this semaphore. It limits concurrent client handling.
      * `pthread_mutex_init(&lock, NULL)`: Initializes a ==**mutex** (mutual exclusion lock).
          * `&lock`: *The* **address** of the `pthread_mutex_t` variable to initialize.
          * `NULL`: Uses default mutex attributes. This mutex will be used to protect the shared **cache** from simultaneous access by multiple threads, preventing data corruption.
      * `port_number = atoi(argv[1])`: If a command-line argument is provided (`argc == 2`), it converts the first argument string (pointed to by `argv[1]`) to an integer and assigns it to `port_number`.
      * `proxy_socketId = socket(AF_INET, SOCK_STREAM, 0)`: Creates the main **listening socket** for the proxy server.
          * `AF_INET`: Specifies the address family as IPv4.
          * `SOCK_STREAM`: Specifies a stream socket, which provides reliable, ordered, and error-checked data delivery (TCP).
          * `0`: Specifies the default protocol for the given family and type.
          * The return value is a **socket descriptor** (an `int`). If it's less than `0`, an error occurred.
      * `setsockopt(proxy_socketId, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse))`: This sets a socket option.
          * `proxy_socketId`: The socket to apply the option to.
          * `SOL_SOCKET`: Specifies the option applies at the socket level.
          * `SO_REUSEADDR`: Allows the port to be reused immediately after the server closes, even if previous connections are in `TIME_WAIT` state. This is crucial for rapid development and testing.
          * `(const char*)&reuse`: A **pointer** to an integer (`reuse`) containing the option value (`1` for true).
          * `sizeof(reuse)`: The size of the option value.
      * `bzero((char*)&server_addr, sizeof(server_addr))`: Fills the `server_addr` structure with zeros. `(char*)&server_addr` casts the **address** of `server_addr` to a `char*` so `bzero` can operate on its raw bytes.
      * `server_addr.sin_family = AF_INET; server_addr.sin_port = htons(port_number); server_addr.sin_addr.s_addr = INADDR_ANY;`: Configures the `server_addr` structure:
          * `sin_family`: Address family (IPv4).
          * `sin_port`: The port number, converted to network byte order using `htons()`.
          * `sin_addr.s_addr`: The IP address. `INADDR_ANY` means the server will listen on all available network interfaces.
      * `bind(proxy_socketId, (struct sockaddr*)&server_addr, sizeof(server_addr))`: Associates the `proxy_socketId` with the `server_addr` (IP address and port). `(struct sockaddr*)&server_addr` is a **pointer** cast.
      * `listen(proxy_socketId, MAX_CLIENTS)`: Puts the `proxy_socketId` into a listening state, ready to accept incoming connections. `MAX_CLIENTS` specifies the maximum length of the queue of pending connections.
      * `int Connected_socketId[MAX_CLIENTS]`: An array to store the socket descriptors of currently connected clients. Each element is an `int`.
      * `while(1)`: An infinite loop, making the server continuously run and accept new connections.
      * `client_socketId = accept(proxy_socketId, (struct sockaddr*)&client_addr, (socklen_t*)&client_len)`: This is where the server waits for a client to connect.
          * `proxy_socketId`: The listening socket.
          * `(struct sockaddr*)&client_addr`: A **pointer** to a `sockaddr_in` structure where the client's address information will be stored.
          * `(socklen_t*)&client_len`: A **pointer** to an integer that, on input, specifies the size of `client_addr` and, on output, specifies the actual size of the stored client address.
          * It returns a **new socket descriptor** (`client_socketId`) dedicated to communication with this specific client. If it's less than `0`, an error occurred.
      * `struct sockaddr_in* client_pt = (struct sockaddr_in*)&client_addr;`: Creates a **pointer** `client_pt` that points to the `client_addr` structure. This makes it easier to access members like `sin_addr`.
      * `struct in_addr ip_addr = client_pt->sin_addr;`: Accesses the `sin_addr` member (which is of type `struct in_addr`) from the `client_addr` structure using the `client_pt` **pointer**. The `->` operator is used to access members of a structure through a pointer.
      * `inet_ntop(AF_INET, &ip_addr, str, INET_ADDRSTRLEN)`: Converts the numeric IP address (`ip_addr`, whose **address** is passed) into a human-readable string format, storing it in `str`.
      * `pthread_create(&tid[i], NULL, thread_fn, (void*)&Connected_socketId[i])`: This is the core of the multithreading. It creates a new thread for each accepted client.
          * `&tid[i]`: The **address** of the `pthread_t` variable that will store the ID of the new thread.
          * `NULL`: Uses default thread attributes.
          * `thread_fn`: The **function pointer** to the function that the new thread will execute. This is the client-handling logic.
          * `(void*)&Connected_socketId[i]`: The **argument** passed to `thread_fn`. It's a **pointer** to the `client_socketId` (which is stored in `Connected_socketId[i]`), cast to `void*` because `pthread_create` expects a `void*` argument. Inside `thread_fn`, this `void*` will be cast back to an `int*` to get the client's socket descriptor.
      * `i++`: Increments the thread counter to prepare for the next client.
      * `close(proxy_socketId)`: Closes the main listening socket when the server is shut down (though in this infinite loop, it's unreachable unless the loop breaks).

-----

### `thread_fn(void* socketNew)`

```c
void* thread_fn(void* socketNew)
{
	sem_wait(&seamaphore);
	int p;
	sem_getvalue(&seamaphore,&p);
	printf("Semaphore value:%d\n",p);
	int socket = *(int *)socketNew; // Dereferencing the void pointer to get the actual socket ID
	int bytes_send_client;
	char *buffer = (char*)calloc(MAX_BYTES, sizeof(char));
	bzero(buffer, MAX_BYTES);
	
	bytes_send_client = recv(socket, buffer, MAX_BYTES, 0);

	char *temp_buffer_ptr = NULL; 
	int len = 0;

	// This loop will execute as long as there is data to be received
	// from the client and the end of the HTTP header ("\r\n\r\n") has
	// not been found yet.
	while(bytes_send_client > 0)
	{
		len = strlen(buffer);
        //loop until u find "\r\n\r\n" in the buffer
		if(strstr(buffer, "\r\n\r\n") == NULL)
		{	
			bytes_send_client = recv(socket, buffer + len, MAX_BYTES - len, 0);
		}
		else{
			break;
		}
	}
	
	char *tempReq = (char*)malloc(strlen(buffer)*sizeof(char)+1);
    //tempReq, buffer both store the http request sent by client
	for (int i = 0; i < strlen(buffer); i++)
	{
		tempReq[i] = buffer[i];
	}
	
	//checking for the request in cache 
	struct cache_element* temp = find(tempReq);

	if( temp != NULL){
        //request found in cache, so sending the response to client from proxy's cache
		int size=temp->len/sizeof(char);
		int pos=0;
		char response[MAX_BYTES];
		while(posdata[pos];
				pos++;
			}
			send(socket,response,MAX_BYTES,0);
		}
		printf("Data retrived from the Cache\n\n");
		printf("%s\n\n",response);
		// close(socketNew);
		// sem_post(&seamaphore);
		// return NULL;
	}
	
	
	else if(bytes_send_client > 0)
	{
		len = strlen(buffer); 
		//Parsing the request
		ParsedRequest* request = ParsedRequest_create();
		
        //ParsedRequest_parse returns 0 on success and -1 on failure.On success it stores parsed request in
        // the request
		if (ParsedRequest_parse(request, buffer, len) < 0) 
		{
		   	printf("Parsing failed\n");
		}
		else
		{	
			bzero(buffer, MAX_BYTES);
			if(!strcmp(request->method,"GET"))							
			{
                
				if( request->host && request->path && (checkHTTPversion(request->version) == 1) )
				{
					bytes_send_client = handle_request(socket, request, tempReq);		// Handle GET request
					if(bytes_send_client == -1)
					{	
						sendErrorMessage(socket, 500);
					}

				}
				else
					sendErrorMessage(socket, 500);			// 500 Internal Error

			}
            else
            {
                printf("This code doesn't support any method other than GET\n");
            }
    
		}
        //freeing up the request pointer
		ParsedRequest_destroy(request);

	}

	else if( bytes_send_client < 0)
	{
		perror("Error in receiving from client.\n");
	}
	else if(bytes_send_client == 0)
	{
		printf("Client disconnected!\n");
	}

	shutdown(socket, SHUT_RDWR);
	close(socket);
	free(buffer);
	sem_post(&seamaphore);	
	
	sem_getvalue(&seamaphore,&p);
	printf("Semaphore post value:%d\n",p);
	free(tempReq);
	return NULL;
}
```

  * **Purpose**: This function is the main logic executed by each new thread. It receives an HTTP request from a client, checks the cache for it, serves from cache if found, otherwise forwards the request to the origin server, receives the response, and then caches it before sending it back to the client.
  * **Parameters**:
      * `void* socketNew`: A **generic pointer** (`void*`) to the `client_socketId` for this specific client connection. It needs to be cast back to an `int*` to be used.
  * **Pointers and Variables**:
      * `sem_wait(&seamaphore)`: Decrements the **semaphore** count. If the count is zero (meaning `MAX_CLIENTS` threads are already active), the calling thread will block until another thread releases the semaphore. This ensures we don't exceed `MAX_CLIENTS` concurrent connections.
      * `int socket = *(int *)socketNew;`: This is crucial for **pointer dereferencing**.
          * `(int *)socketNew`: Casts the `void*` `socketNew` back to an `int*` (a pointer to an integer).
          * `*(int *)socketNew`: **Dereferences** the `int*` to get the actual integer value that `socketNew` points to, which is the `client_socketId`. This `socket` variable is then used for all communication with this client within the thread.
      * `char *buffer = (char*)calloc(MAX_BYTES, sizeof(char))`: Dynamically allocates memory for `MAX_BYTES` characters and initializes them all to zero. This `buffer` will be used to receive the HTTP request from the client.
      * `bytes_send_client = recv(socket, buffer, MAX_BYTES, 0)`: Receives data from the client's `socket` into the `buffer`. `bytes_send_client` stores the number of bytes actually received.
      * `strstr(buffer, "\r\n\r\n")`: This string function searches for the first occurrence of the HTTP header termination sequence (`\r\n\r\n`) within the `buffer`. It returns a **pointer** to the first character of the found substring, or `NULL` if not found.
      * `char *tempReq = (char*)malloc(strlen(buffer)*sizeof(char)+1)`: Dynamically allocates memory for `tempReq`, which will store a copy of the raw HTTP request received from the client. This copy is used as the **key for the cache**.
      * `struct cache_element* temp = find(tempReq)`: Calls the `find` function (explained later) to check if the `tempReq` (the URL) exists in the cache. `temp` will be a **pointer** to the `cache_element` if found, or `NULL`.
      * **Cache Hit (`if(temp != NULL)`)**:
          * If the request is found in the cache, the stored response (`temp->data`) is sent back to the client in chunks of `MAX_BYTES`.
          * `temp->len`: The length of the cached data.
          * `temp->data[pos]`: Accessing individual characters of the cached data using array-like syntax with the **pointer** `temp->data`.
          * `send(socket, response, MAX_BYTES, 0)`: Sends the cached data to the client.
      * **Cache Miss (`else if(bytes_send_client > 0)`)**:
          * `ParsedRequest* request = ParsedRequest_create()`: Creates a new `ParsedRequest` object. This function returns a **pointer** to a dynamically allocated `ParsedRequest` structure.
          * `ParsedRequest_parse(request, buffer, len)`: Parses the raw HTTP request (`buffer`) into the `request` structure. This function populates the fields (method, host, path, headers, etc.) within the `ParsedRequest` structure pointed to by `request`.
          * `strcmp(request->method, "GET")`: Compares the HTTP method (pointed to by `request->method`) to "GET".
          * `request->host`, `request->path`, `request->version`: These are **pointers** to strings within the `ParsedRequest` structure, holding the parsed components of the URL and HTTP version.
          * `handle_request(socket, request, tempReq)`: If it's a valid GET request, calls `handle_request` to fetch the content from the remote server, send it to the client, and add it to the cache.
          * `sendErrorMessage(socket, 500)`: Sends an appropriate HTTP error if something goes wrong.
          * `ParsedRequest_destroy(request)`: Frees the memory allocated for the `ParsedRequest` structure and its internal components.
      * `shutdown(socket, SHUT_RDWR)`: Disables further sends and receives on the client socket.
      * `close(socket)`: Closes the client socket.
      * `free(buffer); free(tempReq);`: Frees the dynamically allocated memory for `buffer` and `tempReq`.
      * `sem_post(&seamaphore)`: Increments the **semaphore** count, signaling that this thread has finished handling a client and another waiting thread can now proceed.
      * `sem_getvalue(&seamaphore, &p)`: Retrieves the current value of the semaphore and stores it in `p`. `&p` is the **address** of the integer where the value will be stored.
      * `return NULL`: Threads typically return `NULL` from their `void*` function.

-----

### `sendErrorMessage(int socket, int status_code)`

```c
int sendErrorMessage(int socket, int status_code)
{
	char str[1024];
	char currentTime[50];
	time_t now = time(0);

	struct tm data = *gmtime(&now);
	strftime(currentTime,sizeof(currentTime),"%a, %d %b %Y %H:%M:%S %Z", &data);

	switch(status_code)
	{
		// ... (cases for 400, 403, 404, 500, 501, 505) ...
	}
	return 1;
}
```

  * **Purpose**: This function constructs and sends an HTTP error response back to the client based on a given HTTP status code.
  * **Parameters**:
      * `int socket`: The **socket descriptor** of the client connection to which the error message should be sent.
      * `int status_code`: The HTTP status code (e.g., 400 for Bad Request).
  * **Pointers and Variables**:
      * `char str[1024]`: A local character array (buffer) to build the HTTP error response string.
      * `char currentTime[50]`: A local character array to store the formatted current timestamp.
      * `time_t now = time(0)`: Gets the current time in seconds since the Epoch.
      * `struct tm data = *gmtime(&now)`: Converts the `time_t` value (`now`) into a broken-down time structure (`struct tm`) in Coordinated Universal Time (UTC). `gmtime()` returns a **pointer** to a `struct tm`, and `*` **dereferences** it to get the structure itself.
      * ` strftime(currentTime, sizeof(currentTime), ...)  `: Formats the `struct tm` data into a human-readable date and time string, storing it in `currentTime`.
      * `snprintf(str, sizeof(str), ...)`: Formats the full HTTP error response string (including headers and a simple HTML body) into the `str` buffer. It uses `currentTime` (a string **pointer**) to embed the current date.
      * `send(socket, str, strlen(str), 0)`: Sends the constructed HTTP error response string over the provided `socket`. `strlen(str)` provides the exact length of the string to send.

-----

### `checkHTTPversion(char* msg)`

```c
int checkHTTPversion(char* msg)
{
	// This Function checks for the HTTP version
	if( (strncmp(msg, "HTTP/1.1", 8) == 0) || (strncmp(msg, "HTTP/1.0", 8) == 0) )
		return 1;
	else
		return 0;
}
```

  * **Purpose**: This simple function validates if the provided string represents a supported HTTP version (either "HTTP/1.1" or "HTTP/1.0").
  * **Parameters**:
      * `char* msg`: A **pointer** to the string containing the HTTP version extracted from the client's request.
  * **Pointers and Variables**:
      * `strncmp(msg, "HTTP/1.1", 8)`: Compares the first 8 characters of the string pointed to by `msg` with "HTTP/1.1". `strncmp` is used for safe comparison of fixed lengths, preventing buffer overflows if `msg` is not null-terminated or shorter than expected.
      * `strncmp(msg, "HTTP/1.0", 8)`: Similar comparison for "HTTP/1.0".

-----

### `connectRemoteServer(char* host_addr, int port_num)`

```c
int connectRemoteServer(char* host_addr, int port_num)
{
	// Creating Socket for remote server ---------------------------
	int remoteSocket = socket(AF_INET, SOCK_STREAM, 0);

	if( remoteSocket < 0)
	{
		printf("Error in Creating Socket.\n");
		return -1;
	}
	
	// Get host by the name or ip address provided
	struct hostent *host = gethostbyname(host_addr);	
	if(host == NULL)
	{
		fprintf(stderr, "No such host exists.\n");	
		return -1;
	}

	// inserts ip address and port number of host in struct `server_addr`
	struct sockaddr_in server_addr;

	bzero((char*)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port_num);

	bcopy((char *)host->h_addr,(char *)&server_addr.sin_addr.s_addr,host->h_length);

	// Connect to Remote server ----------------------------------------------------
	if( connect(remoteSocket, (struct sockaddr*)&server_addr, (socklen_t)sizeof(server_addr)) < 0 )
	{
		fprintf(stderr, "Error in connecting !\n"); 
		return -1;
	}
	// free(host_addr); // This line is problematic if host_addr is not dynamically allocated for this specific function call.
	return remoteSocket;
}
```

  * **Purpose**: This function is responsible for creating a new socket and establishing a TCP connection to the actual web server (origin server) specified by the client's request.
  * **Parameters**:
      * `char* host_addr`: A **pointer** to a string containing the hostname (e.g., "[www.google.com](https://www.google.com)") or IP address of the remote server.
      * `int port_num`: The port number of the remote server (e.g., 80 for HTTP).
  * **Pointers and Variables**:
      * `int remoteSocket`: This integer will hold the **socket descriptor** for the connection to the remote server.
      * `struct hostent *host`: This is a **pointer** to a `struct hostent`. The `gethostbyname()` function takes a hostname (a `char*`) and returns a pointer to this structure, which contains information like the official name of the host, its aliases, and a list of IP addresses.
      * `struct sockaddr_in server_addr`: This structure holds the remote server's IP address and port number in a format suitable for socket functions.
      * `bzero((char*)&server_addr, sizeof(server_addr))`: Initializes the `server_addr` structure to all zeros. `(char*)&server_addr` casts the **address** of `server_addr` to a `char*` so `bzero` can operate on its raw bytes.
      * `server_addr.sin_family = AF_INET; server_addr.sin_port = htons(port_num);`: Sets the address family to IPv4 and converts the `port_num` to network byte order.
      * `bcopy((char *)host->h_addr,(char *)&server_addr.sin_addr.s_addr,host->h_length)`: Copies the IP address from the `host` structure (obtained via `gethostbyname()`) into `server_addr.sin_addr.s_addr`.
          * `(char *)host->h_addr`: This is a **pointer** to the first IP address in the list provided by `gethostbyname()`.
          * `(char *)&server_addr.sin_addr.s_addr`: This is the **address** of the `s_addr` field within `server_addr.sin_addr`, where the IP address will be stored.
          * `host->h_length`: The length of the address (e.g., 4 bytes for IPv4).
      * `connect(remoteSocket, (struct sockaddr*)&server_addr, (socklen_t)sizeof(server_addr))`: Attempts to establish a connection using the `remoteSocket` to the `server_addr`. `(struct sockaddr*)&server_addr` is a **pointer** cast to the generic `sockaddr` type.

-----

### `handle_request(int clientSocket, ParsedRequest *request, char *tempReq)`

```c
int handle_request(int clientSocket, ParsedRequest *request, char *tempReq)
{
	char *buf = (char*)malloc(sizeof(char)*MAX_BYTES);
	strcpy(buf, "GET ");
	strcat(buf, request->path);
	strcat(buf, " ");
	strcat(buf, request->version);
	strcat(buf, "\r\n");

	size_t len = strlen(buf);

	if (ParsedHeader_set(request, "Connection", "close") < 0){
		printf("set header key not work\n");
	}

	if(ParsedHeader_get(request, "Host") == NULL)
	{
		if(ParsedHeader_set(request, "Host", request->host) < 0){
			printf("Set \"Host\" header key not working\n");
		}
	}

	if (ParsedRequest_unparse_headers(request, buf + len, (size_t)MAX_BYTES - len) < 0) {
		printf("unparse failed\n");
		//return -1;				// If this happens Still try to send request without header
	}

	int server_port = 80;				// Default Remote Server Port
	if(request->port != NULL)
		server_port = atoi(request->port);

	int remoteSocketID = connectRemoteServer(request->host, server_port);

	if(remoteSocketID < 0)
		return -1;

	int bytes_send = send(remoteSocketID, buf, strlen(buf), 0);

	bzero(buf, MAX_BYTES);

	bytes_send = recv(remoteSocketID, buf, MAX_BYTES-1, 0);
	char *temp_buffer = (char*)malloc(sizeof(char)*MAX_BYTES); //temp buffer
	int temp_buffer_size = MAX_BYTES;
	int temp_buffer_index = 0;

	while(bytes_send > 0)
	{
		bytes_send = send(clientSocket, buf, bytes_send, 0);
		
		for(int i=0;i<bytes_send;i++){
			if(temp_buffer_index < temp_buffer_size){
				temp_buffer[temp_buffer_index] = buf[i];
				temp_buffer_index++;
			}
			else{
				temp_buffer_size += MAX_BYTES;
				temp_buffer = (char*)realloc(temp_buffer, temp_buffer_size*sizeof(char)); // Reallocating Memory
				temp_buffer[temp_buffer_index] = buf[i];
				temp_buffer_index++;
			}
		}
		bzero(buf, MAX_BYTES);
		bytes_send = recv(remoteSocketID, buf, MAX_BYTES-1, 0);
	}

	temp_buffer[temp_buffer_index] = '\0';
	add_cache_element(temp_buffer,temp_buffer_index,tempReq);

	free(temp_buffer);
	free(buf);
	close(remoteSocketID);
	return 1;
}
```

  * **Purpose**: This function takes a parsed client request, modifies it for the origin server, sends it, receives the response, and then forwards the response to the client while also adding it to the cache.
  * **Parameters**:
      * `int clientSocket`: The **socket descriptor** for the client connection.
      * `ParsedRequest *request`: A **pointer** to the `ParsedRequest` structure, which holds the parsed components of the client's HTTP request.
      * `char *tempReq`: A **pointer** to the original, raw HTTP request string from the client. This will be used as the key for caching the response.
  * **Pointers and Variables**:
      * `char *buf = (char*)malloc(sizeof(char)*MAX_BYTES)`: Dynamically allocates a buffer to construct the request to the remote server and later to receive the response from it. `buf` is a **pointer** to this allocated memory.
      * `strcpy(buf, "GET "); strcat(buf, request->path); ...`: This sequence constructs the HTTP request line (e.g., "GET /some/path HTTP/1.1") using strings pointed to by members of the `request` structure (`request->path`, `request->version`).
      * `ParsedHeader_set(request, "Connection", "close")`: This function from `proxy_parse.h` sets or adds an HTTP header. It takes the `ParsedRequest` **pointer**, and **pointers** to the key ("Connection") and value ("close") strings. This is often done to ensure the connection with the origin server closes after the response, simplifying proxy logic.
      * `ParsedHeader_get(request, "Host")`: This function from `proxy_parse.h` retrieves a header. It returns a **pointer** to a `ParsedHeader` structure if the "Host" header is found, otherwise `NULL`. This check ensures that the "Host" header is present, which is mandatory for HTTP/1.1 requests.
      * `ParsedRequest_unparse_headers(request, buf + len, (size_t)MAX_BYTES - len)`: This function from `proxy_parse.h` takes the parsed headers from the `request` structure and converts them back into a raw string format, appending them to `buf` after the request line. `buf + len` is a **pointer arithmetic** expression; it moves the **pointer** `buf` forward by `len` bytes, indicating where the headers should start.
      * `int remoteSocketID = connectRemoteServer(request->host, server_port)`: Calls the `connectRemoteServer` function to establish a connection to the origin web server. `request->host` is a **pointer** to the hostname string.
      * `send(remoteSocketID, buf, strlen(buf), 0)`: Sends the complete HTTP request (request line + headers) to the `remoteSocketID`.
      * `char *temp_buffer = (char*)malloc(sizeof(char)*MAX_BYTES)`: Dynamically allocates a temporary buffer to accumulate the full response received from the remote server.
      * `temp_buffer = (char*)realloc(temp_buffer, temp_buffer_size*sizeof(char))`: If the incoming response is larger than the current `temp_buffer_size`, `realloc` is used to expand the allocated memory for `temp_buffer`. If `realloc` is successful, `temp_buffer` might point to a new memory location.
      * `add_cache_element(temp_buffer, temp_buffer_index, tempReq)`: Calls the cache function to add the received response (`temp_buffer` and its length `temp_buffer_index`) with its corresponding request URL (`tempReq`) to the cache.
      * `free(temp_buffer); free(buf);`: Frees the dynamically allocated memory pointed to by `temp_buffer` and `buf` to prevent memory leaks.
      * `close(remoteSocketID)`: Closes the socket connection to the remote server.

-----

### Cache Management Functions

#### `find(char* url)`

```c
cache_element* find(char* url){

// Checks for url in the cache if found returns pointer to the respective cache element or else returns NULL
    cache_element* site=NULL;
	//sem_wait(&cache_lock);
    int temp_lock_val = pthread_mutex_lock(&lock);
	printf("Remove Cache Lock Acquired %d\n",temp_lock_val); 
    if(head!=NULL){
        site = head;
        while (site!=NULL)
        {
            if(!strcmp(site->url,url)){
				printf("LRU Time Track Before : %ld", site->lru_time_track);
                printf("\nurl found\n");
				// Updating the time_track
				site->lru_time_track = time(NULL);
				printf("LRU Time Track After : %ld", site->lru_time_track);
				break;
            }
            site=site->next;
        }       
    }
	else {
    printf("\nurl not found\n");
	}
	//sem_post(&cache_lock);
    temp_lock_val = pthread_mutex_unlock(&lock);
	printf("Remove Cache Lock Unlocked %d\n",temp_lock_val); 
    return site;
}
```

  * **Purpose**: Searches for a `cache_element` in the cache (a linked list) using the provided `url` as the key. If found, it updates the `lru_time_track` for that element to reflect its recent use, and returns a pointer to it.
  * **Parameters**:
      * `char* url`: A **pointer** to the URL string to search for in the cache.
  * **Pointers and Variables**:
      * `cache_element* site=NULL`: A **pointer** of type `cache_element*` used to traverse the cache linked list. It's initialized to `NULL`.
      * `pthread_mutex_lock(&lock)`: Acquires the **mutex lock**. This is critical because `head` and the cache linked list (`site->next`, `site->lru_time_track`) are shared resources accessed by multiple threads. The lock ensures that only one thread can access or modify the cache at a time, preventing race conditions and data corruption.
      * `if(head!=NULL)`: Checks if the cache is empty. `head` is a **global pointer** to the first element of the cache linked list.
      * `site = head`: Starts `site` at the beginning of the linked list.
      * `while (site!=NULL)`: Loops through each element in the linked list.
      * `if(!strcmp(site->url,url))`: Compares the `url` stored in the current `cache_element` (`site->url`, a **pointer** to a string) with the input `url` (**pointer** to a string). If they are identical (meaning the URL is found), `strcmp` returns `0`.
      * `site->lru_time_track = time(NULL)`: If the element is found, its `lru_time_track` is updated to the current time using `time(NULL)`. This marks it as recently used, which is important for the LRU eviction policy. `site->lru_time_track` accesses the `lru_time_track` member of the `cache_element` pointed to by `site`.
      * `site=site->next`: Moves the `site` **pointer** to the next `cache_element` in the linked list. `site->next` accesses the `next` member (a **pointer** to the next element) of the `cache_element` pointed to by `site`.
      * `pthread_mutex_unlock(&lock)`: Releases the **mutex lock**, allowing other threads to access the cache.

#### `remove_cache_element()`

```c
void remove_cache_element(){
    // If cache is not empty searches for the node which has the least lru_time_track and deletes it
    cache_element * p ;  	// Cache_element Pointer (Prev. Pointer)
	cache_element * q ;		// Cache_element Pointer (Next Pointer)
	cache_element * temp;	// Cache element to remove
    //sem_wait(&cache_lock);
    int temp_lock_val = pthread_mutex_lock(&lock);
	printf("Remove Cache Lock Acquired %d\n",temp_lock_val); 
	if( head != NULL) { // Cache != empty
		for (q = head, p = head, temp =head ; q -> next != NULL; 
			q = q -> next) { // Iterate through entire cache and search for oldest time track
			if(( (q -> next) -> lru_time_track) < (temp -> lru_time_track)) {
				temp = q -> next;
				p = q;
			}
		}
		if(temp == head) { 
			head = head -> next; /*Handle the base case*/
		} else {
			p->next = temp->next;	
		}
		cache_size = cache_size - (temp -> len) - sizeof(cache_element) - 
		strlen(temp -> url) - 1;     //updating the cache size
		free(temp->data);     		
		free(temp->url); // Free the removed element 
		free(temp);
	} 
	//sem_post(&cache_lock);
    temp_lock_val = pthread_mutex_unlock(&lock);
	printf("Remove Cache Lock Unlocked %d\n",temp_lock_val); 
}
```

  * **Purpose**: Implements the LRU (Least Recently Used) cache eviction policy. It finds the `cache_element` that was accessed the longest time ago and removes it from the cache to free up space.
  * **Parameters**: None (operates on the global `head` and `cache_size`).
  * **Pointers and Variables**:
      * `pthread_mutex_lock(&lock)`: Acquires the mutex lock before modifying the cache.
      * `cache_element * p`, `cache_element * q`, `cache_element * temp`: These are **pointers** used to traverse the linked list and keep track of elements.
          * `p`: A "previous" pointer, always pointing to the element *before* `temp`.
          * `q`: A "current" pointer, iterating through the list to find the element to remove.
          * `temp`: A "temporary" pointer, always pointing to the element currently identified as the least recently used.
      * `if(head != NULL)`: Checks if the cache is not empty.
      * `for (q = head, p = head, temp =head ; q -> next != NULL; q = q -> next)`: This loop iterates through the linked list to find the element with the smallest `lru_time_track`.
          * `q -> next`: Accesses the `next` pointer of the `cache_element` pointed to by `q`.
          * `q -> next -> lru_time_track`: Accesses the `lru_time_track` of the element pointed to by `q->next`.
          * `temp -> lru_time_track`: Accesses the `lru_time_track` of the element pointed to by `temp`.
      * `if(temp == head)`: If the element to be removed (`temp`) is the `head` of the list, then `head` is simply updated to point to the next element.
      * `else { p->next = temp->next; }`: If `temp` is not the head, `p->next` (the `next` pointer of the element *before* `temp`) is made to point to `temp->next` (the element *after* `temp`), effectively unlinking `temp` from the list.
      * `cache_size = cache_size - ...`: Decrements the global `cache_size` by the size of the removed element (its `data` length, the size of the `cache_element` structure itself, and the length of its `url`).
      * `free(temp->data); free(temp->url); free(temp);`: Frees the dynamically allocated memory for the actual data (`temp->data`), the URL string (`temp->url`), and the `cache_element` structure itself (`temp`). This is crucial for preventing memory leaks.
      * `pthread_mutex_unlock(&lock)`: Releases the mutex lock.

#### `add_cache_element(char* data,int size,char* url)`

```c
int add_cache_element(char* data,int size,char* url){
    // Adds element to the cache
	// sem_wait(&cache_lock);
    int temp_lock_val = pthread_mutex_lock(&lock);
	printf("Add Cache Lock Acquired %d\n", temp_lock_val);
    int element_size=size+1+strlen(url)+sizeof(cache_element); // Size of the new element which will be added to the cache
    if(element_size>MAX_ELEMENT_SIZE){
		//sem_post(&cache_lock);
        // If element size is greater than MAX_ELEMENT_SIZE we don't add the element to the cache
        temp_lock_val = pthread_mutex_unlock(&lock);
		printf("Add Cache Lock Unlocked %d\n", temp_lock_val);
		// free(data); // These free() calls are potentially problematic depending on how 'data' and 'url' were allocated prior to this function call.
		// printf("--\n");
		// free(url);
        return 0;
    }
    else
    {   while(cache_size+element_size>MAX_SIZE){
            // We keep removing elements from cache until we get enough space to add the element
            remove_cache_element();
        }
        cache_element* element = (cache_element*) malloc(sizeof(cache_element)); // Allocating memory for the new cache element
        element->data= (char*)malloc(size+1); // Allocating memory for the response to be stored in the cache element
		strcpy(element->data,data); 
        element -> url = (char*)malloc(1+( strlen( url )*sizeof(char)  )); // Allocating memory for the request to be stored in the cache element (as a key)
		strcpy( element -> url, url );
		element->lru_time_track=time(NULL);    // Updating the time_track
        element->next=head; 
        element->len=size;
        head=element;
        cache_size+=element_size;
        temp_lock_val = pthread_mutex_unlock(&lock);
		printf("Add Cache Lock Unlocked %d\n", temp_lock_val);
		//sem_post(&cache_lock);
		// free(data); // Again, these free() calls might be incorrect.
		// printf("--\n");
		// free(url);
        return 1;
    }
    return 0;
}
```

  * **Purpose**: Adds a new `cache_element` (HTTP response data and its corresponding URL) to the cache. It ensures that the cache doesn't exceed its `MAX_SIZE` by evicting older elements if necessary, and it also checks if the new element itself is too large to be cached.
  * **Parameters**:
      * `char* data`: A **pointer** to the HTTP response data (the content to be cached).
      * `int size`: The length of the `data` in bytes.
      * `char* url`: A **pointer** to the URL string (the key for the cache element).
  * **Pointers and Variables**:
      * `pthread_mutex_lock(&lock)`: Acquires the mutex lock before modifying the cache.
      * `int element_size = size + 1 + strlen(url) + sizeof(cache_element)`: Calculates the total size that the new `cache_element` will occupy in memory, including the data, URL string, and the `cache_element` structure itself. `size + 1` accounts for the null terminator for `data`. `strlen(url) + 1` accounts for the null terminator for `url`.
      * `if(element_size > MAX_ELEMENT_SIZE)`: Checks if the new element is individually too large to be cached. If so, it returns `0` (failure) without adding it.
      * `while(cache_size + element_size > MAX_SIZE)`: This loop implements cache eviction. If adding the new element would make the cache exceed `MAX_SIZE`, it repeatedly calls `remove_cache_element()` until enough space is freed.
      * `cache_element* element = (cache_element*) malloc(sizeof(cache_element))`: Dynamically allocates memory for the new `cache_element` structure itself. `element` is a **pointer** to this new structure.
      * `element->data = (char*)malloc(size+1)`: Dynamically allocates memory for the actual response data (`data`) within the new `cache_element`. `element->data` is a **pointer** to this memory. `strcpy(element->data, data)` copies the content.
      * `element->url = (char*)malloc(1 + (strlen(url) * sizeof(char)))`: Dynamically allocates memory for the URL string within the new `cache_element`. `element->url` is a **pointer** to this memory. `strcpy(element->url, url)` copies the URL.
      * `element->lru_time_track = time(NULL)`: Sets the `lru_time_track` of the new element to the current time, marking it as recently added/used.
      * `element->next = head`: This adds the new element to the *front* of the linked list (a common way to implement a simple LRU where the head is the most recently added). `element->next` points to what was previously the `head`.
      * `head = element`: Updates the global `head` **pointer** to point to the newly added element, making it the new beginning of the list.
      * `cache_size += element_size`: Updates the global `cache_size`.
      * `pthread_mutex_unlock(&lock)`: Releases the mutex lock.

-----

## II. Proxy Parse Library - `proxy_parse.c`

This library provides functions to parse HTTP requests into a structured format and unparse them back into raw strings.

### Structures

```c
// (Implicitly defined in proxy_parse.h, but important to understand)
struct ParsedHeader {
    char *key;      // Pointer to the header key (e.g., "Host")
    char *value;    // Pointer to the header value (e.g., "www.example.com")
    size_t keylen;  // Length of the key string
    size_t valuelen; // Length of the value string
};

struct ParsedRequest {
    char *buf;      // Pointer to the original raw request line (e.g., "GET http://www.example.com/ HTTP/1.1")
    size_t buflen;  // Length of the raw request line
    char *method;   // Pointer to the HTTP method (e.g., "GET")
    char *protocol; // Pointer to the protocol (e.g., "http")
    char *host;     // Pointer to the host (e.g., "www.example.com")
    char *port;     // Pointer to the port (e.g., "80")
    char *path;     // Pointer to the path (e.g., "/index.html")
    char *version;  // Pointer to the HTTP version (e.g., "HTTP/1.1")
    struct ParsedHeader *headers; // Pointer to an array of ParsedHeader structures
    size_t headerslen;   // Allocated capacity for headers array
    size_t headersused;  // Number of headers actually used
};
```

  * **`ParsedHeader`**: Represents a single HTTP header, storing its key and value as dynamically allocated strings.
      * `char *key`, `char *value`: **Pointers** to dynamically allocated character arrays that store the header's key and value strings.
  * **`ParsedRequest`**: Represents the entire parsed HTTP request. It holds pointers to various components of the request line and an array of `ParsedHeader` structures for the request headers.
      * `char *buf`: A **pointer** to a dynamically allocated string holding the *request line* from the original HTTP request (e.g., "GET [http://www.example.com/](http://www.example.com/) HTTP/1.1").
      * `char *method`, `char *protocol`, `char *host`, `char *port`, `char *path`, `char *version`: These are all **pointers** to character arrays (strings). During parsing, these pointers will be made to point to specific parts within the `buf` (or newly allocated memory for `path`), essentially "slicing" the request line into its components.
      * `struct ParsedHeader *headers`: A **pointer** to an array of `ParsedHeader` structures. This array is dynamically resized as needed.

-----

### `debug(const char * format, ...)`

```c
void debug(const char * format, ...) {
     va_list args;
     if (DEBUG) {
      va_start(args, format);
      vfprintf(stderr, format, args);
      va_end(args);
     }
}
```

  * **Purpose**: A debugging utility function that prints formatted output to `stderr` if the `DEBUG` macro is defined (usually `DEBUG` is set to `1`).
  * **Parameters**:
      * `const char * format`: A **pointer** to a format string, similar to `printf`.
      * `...`: Ellipsis, indicating a variable number of arguments (like `printf`).
  * **Pointers and Variables**:
      * `va_list args`: A special type from `<stdarg.h>` used to handle variable argument lists.
      * `va_start(args, format)`: Initializes `args` to point to the first optional argument.
      * `vfprintf(stderr, format, args)`: Prints the formatted output to `stderr` using the variable argument list.
      * `va_end(args)`: Cleans up the `va_list`.

-----

### `ParsedHeader_set(struct ParsedRequest *pr, const char * key, const char * value)`

```c
int ParsedHeader_set(struct ParsedRequest *pr, 
             const char * key, const char * value)
{
     struct ParsedHeader *ph;
     ParsedHeader_remove (pr, key); // Remove existing header with same key

     if (pr->headerslen <= pr->headersused+1) {
      pr->headerslen = pr->headerslen * 2;
      pr->headers = 
           (struct ParsedHeader *)realloc(pr->headers, 
        pr->headerslen * sizeof(struct ParsedHeader));
      if (!pr->headers)
           return -1;
     }

     ph = pr->headers + pr->headersused;
     pr->headersused += 1;
     
     ph->key = (char *)malloc(strlen(key)+1);
     memcpy(ph->key, key, strlen(key));
     ph->key[strlen(key)] = '\0';

     ph->value = (char *)malloc(strlen(value)+1);
     memcpy(ph->value, value, strlen(value));
     ph->value[strlen(value)] = '\0';

     ph->keylen = strlen(key)+1;
     ph->valuelen = strlen(value)+1;
     return 0;
}
```

  * **Purpose**: Adds or updates an HTTP header (key-value pair) within a `ParsedRequest` structure. If a header with the same key already exists, it's removed first.
  * **Parameters**:
      * `struct ParsedRequest *pr`: A **pointer** to the `ParsedRequest` structure where the header will be set.
      * `const char * key`: A **pointer** to the string representing the header key (e.g., "Host").
      * `const char * value`: A **pointer** to the string representing the header value (e.g., "[www.example.com](https://www.example.com)").
  * **Pointers and Variables**:
      * `struct ParsedHeader *ph`: A **pointer** used to refer to the new `ParsedHeader` element being added to the array.
      * `ParsedHeader_remove(pr, key)`: Calls another function (explained below) to remove any existing header with the same `key`.
      * `if (pr->headerslen <= pr->headersused+1)`: Checks if the allocated array for headers (`pr->headers`) needs to be resized.
          * `pr->headerslen`: The currently allocated capacity of the `headers` array.
          * `pr->headersused`: The number of header slots currently occupied.
      * `pr->headers = (struct ParsedHeader *)realloc(pr->headers, pr->headerslen * sizeof(struct ParsedHeader))`: If resizing is needed, `realloc` attempts to resize the `headers` array (pointed to by `pr->headers`). If successful, `pr->headers` might point to a new memory location.
      * `ph = pr->headers + pr->headersused`: Uses **pointer arithmetic** to get a **pointer** to the next available slot in the `headers` array where the new header will be stored.
      * `pr->headersused += 1`: Increments the count of used header slots.
      * `ph->key = (char *)malloc(strlen(key)+1); memcpy(ph->key, key, strlen(key)); ph->key[strlen(key)] = '\0';`: Dynamically allocates memory for the header `key` string, copies the content from the input `key` (pointed to by `key`), and null-terminates it. `ph->key` becomes a **pointer** to this new memory.
      * `ph->value = (char *)malloc(strlen(value)+1); memcpy(ph->value, value, strlen(value)); ph->value[strlen(value)] = '\0';`: Similar dynamic allocation and copying for the header `value`. `ph->value` becomes a **pointer** to this new memory.
      * `ph->keylen`, `ph->valuelen`: Store the lengths of the `key` and `value` strings.

-----

### `ParsedHeader_get(struct ParsedRequest *pr, const char * key)`

```c
struct ParsedHeader* ParsedHeader_get(struct ParsedRequest *pr, 
                      const char * key)
{
     size_t i = 0;
     struct ParsedHeader * tmp;
     while(pr->headersused > i)
     {
      tmp = pr->headers + i;
      if(tmp->key && key && strcmp(tmp->key, key) == 0)
      {
           return tmp;
      }
      i++;
     }
     return NULL;
}
```

  * **Purpose**: Searches for a specific header by its `key` within a `ParsedRequest` structure's headers array.
  * **Parameters**:
      * `struct ParsedRequest *pr`: A **pointer** to the `ParsedRequest` structure to search within.
      * `const char * key`: A **pointer** to the header key string to look for.
  * **Pointers and Variables**:
      * `struct ParsedHeader * tmp`: A **pointer** to a `ParsedHeader` structure, used to iterate through the `headers` array.
      * `while(pr->headersused > i)`: Loops through each header currently stored in the `pr->headers` array.
      * `tmp = pr->headers + i`: Uses **pointer arithmetic** to get a **pointer** to the `i`-th `ParsedHeader` element in the `headers` array.
      * `strcmp(tmp->key, key) == 0`: Compares the `key` string stored in the current header (`tmp->key`, a **pointer**) with the input `key` (a **pointer**).

-----

### `ParsedHeader_remove(struct ParsedRequest *pr, const char *key)`

```c
int ParsedHeader_remove(struct ParsedRequest *pr, const char *key)
{
     struct ParsedHeader *tmp;
     tmp = ParsedHeader_get(pr, key);
     if(tmp == NULL)
      return -1;

     free(tmp->key);
     free(tmp->value);
     tmp->key = NULL;
     return 0;
}
```

  * **Purpose**: Removes a specific header by its `key` from a `ParsedRequest` structure. It frees the memory associated with that header's key and value strings.
  * **Parameters**:
      * `struct ParsedRequest *pr`: A **pointer** to the `ParsedRequest` structure from which to remove the header.
      * `const char *key`: A **pointer** to the key string of the header to remove.
  * **Pointers and Variables**:
      * `struct ParsedHeader *tmp`: A **pointer** to the `ParsedHeader` structure that `ParsedHeader_get` finds.
      * `free(tmp->key); free(tmp->value);`: Frees the dynamically allocated memory for the header's key and value strings, which `tmp->key` and `tmp->value` point to.
      * `tmp->key = NULL;`: Sets the `key` pointer to `NULL` to indicate that this header slot is now "empty" or invalid, though it doesn't shift the array elements.

-----

### `ParsedHeader_create(struct ParsedRequest *pr)`

```c
void ParsedHeader_create(struct ParsedRequest *pr)
{
     pr->headers = 
     (struct ParsedHeader *)malloc(sizeof(struct ParsedHeader)*DEFAULT_NHDRS);
     pr->headerslen = DEFAULT_NHDRS;
     pr->headersused = 0;
} 
```

  * **Purpose**: Initializes the `headers` array within a `ParsedRequest` structure.
  * **Parameters**:
      * `struct ParsedRequest *pr`: A **pointer** to the `ParsedRequest` structure to initialize.
  * **Pointers and Variables**:
      * `pr->headers = (struct ParsedHeader *)malloc(...)`: Dynamically allocates an initial array of `DEFAULT_NHDRS` `ParsedHeader` structures. `pr->headers` is then made to **point** to the beginning of this allocated block.

-----

### `ParsedHeader_lineLen(struct ParsedHeader * ph)`

```c
size_t ParsedHeader_lineLen(struct ParsedHeader * ph)
{
     if(ph->key != NULL)
     {
      return strlen(ph->key)+strlen(ph->value)+4;
     }
     return 0; 
}
```

  * **Purpose**: Calculates the length of a single header line when unparsed into a string (e.g., "Key: Value\\r\\n").
  * **Parameters**:
      * `struct ParsedHeader * ph`: A **pointer** to the `ParsedHeader` structure.
  * **Pointers and Variables**:
      * `strlen(ph->key)`: Gets the length of the string pointed to by `ph->key`.
      * `strlen(ph->value)`: Gets the length of the string pointed to by `ph->value`.
      * `+4`: Accounts for the ": " (2 bytes) and "\\r\\n" (2 bytes).

-----

### `ParsedHeader_headersLen(struct ParsedRequest *pr)`

```c
size_t ParsedHeader_headersLen(struct ParsedRequest *pr) 
{
     if (!pr || !pr->buf)
      return 0;

     size_t i = 0;
     int len = 0;
     while(pr->headersused > i)
     {
      len += ParsedHeader_lineLen(pr->headers + i);
      i++;
     }
     len += 2; // For the final "\r\n" after all headers
     return len;
}
```

  * **Purpose**: Calculates the total length of all headers in a `ParsedRequest` when they are unparsed into a single string, including the final blank line (`\r\n`).
  * **Parameters**:
      * `struct ParsedRequest *pr`: A **pointer** to the `ParsedRequest` structure.
  * **Pointers and Variables**:
      * `pr->headers + i`: Uses **pointer arithmetic** to get a **pointer** to the `i`-th `ParsedHeader` in the array.
      * `ParsedHeader_lineLen(...)`: Calls the previously explained function to get the length of each individual header line.

-----

### `ParsedHeader_printHeaders(struct ParsedRequest * pr, char * buf, size_t len)`

```c
int ParsedHeader_printHeaders(struct ParsedRequest * pr, char * buf, 
                  size_t len)
{
     char * current = buf;
     struct ParsedHeader * ph;
     size_t i = 0;

     if(len < ParsedHeader_headersLen(pr))
     {
      debug("buffer for printing headers too small\n");
      return -1;
     }
  
     while(pr->headersused > i)
     {
      ph = pr->headers+i;
      if (ph->key) {
           memcpy(current, ph->key, strlen(ph->key));
           memcpy(current+strlen(ph->key), ": ", 2);
           memcpy(current+strlen(ph->key) +2 , ph->value, 
              strlen(ph->value));
           memcpy(current+strlen(ph->key) +2+strlen(ph->value) , 
              "\r\n", 2);
           current += strlen(ph->key)+strlen(ph->value)+4;
      }
      i++;
     }
     memcpy(current, "\r\n",2);
     return 0;
}
```

  * **Purpose**: Writes the parsed HTTP headers from a `ParsedRequest` structure into a provided character buffer, formatting them as `Key: Value\r\n`.
  * **Parameters**:
      * `struct ParsedRequest * pr`: A **pointer** to the `ParsedRequest` structure containing the headers.
      * `char * buf`: A **pointer** to the buffer where the headers will be written.
      * `size_t len`: The available length of the `buf`.
  * **Pointers and Variables**:
      * `char * current = buf`: A **pointer** that tracks the current position in the `buf` where data should be written. It starts at the beginning of `buf` and is incremented after each part of the header is copied.
      * `struct ParsedHeader * ph`: A **pointer** to the current `ParsedHeader` being processed.
      * `ph = pr->headers + i`: Uses **pointer arithmetic** to get the address of the `i`-th header in the `headers` array.
      * `memcpy(current, ph->key, strlen(ph->key))`: Copies the header key (pointed to by `ph->key`) into the `current` position of the `buf`.
      * `current += strlen(ph->key)`: Moves `current` **pointer** forward by the length of the copied key. Similar pointer arithmetic is used for copying the ": ", value, and "\\r\\n".

-----

### `ParsedHeader_destroyOne(struct ParsedHeader * ph)`

```c
void ParsedHeader_destroyOne(struct ParsedHeader * ph)
{
     if(ph->key != NULL)
     {
      free(ph->key);
      ph->key = NULL;
      free(ph->value);
      ph->value = NULL;
      ph->keylen = 0;
      ph->valuelen = 0;
     }
}
```

  * **Purpose**: Frees the dynamically allocated memory for the `key` and `value` strings within a single `ParsedHeader` structure.
  * **Parameters**:
      * `struct ParsedHeader * ph`: A **pointer** to the `ParsedHeader` structure to destroy.
  * **Pointers and Variables**:
      * `free(ph->key); free(ph->value);`: Frees the memory blocks that `ph->key` and `ph->value` point to.
      * `ph->key = NULL; ph->value = NULL;`: Sets the pointers to `NULL` after freeing, which is good practice to prevent dangling pointers.

-----

### `ParsedHeader_destroy(struct ParsedRequest * pr)`

```c
void ParsedHeader_destroy(struct ParsedRequest * pr)
{
     size_t i = 0;
     while(pr->headersused > i)
     {
      ParsedHeader_destroyOne(pr->headers + i);
      i++;
     }
     pr->headersused = 0;

     free(pr->headers);
     pr->headerslen = 0;
}
```

  * **Purpose**: Frees all dynamically allocated memory associated with the `headers` array within a `ParsedRequest` structure.
  * **Parameters**:
      * `struct ParsedRequest * pr`: A **pointer** to the `ParsedRequest` structure whose headers are to be destroyed.
  * **Pointers and Variables**:
      * `ParsedHeader_destroyOne(pr->headers + i)`: Calls `ParsedHeader_destroyOne` for each individual header in the `headers` array (accessed using **pointer arithmetic** `pr->headers + i`).
      * `free(pr->headers)`: Frees the main memory block that holds the array of `ParsedHeader` structures, which `pr->headers` points to.

-----

### `ParsedHeader_parse(struct ParsedRequest * pr, char * line)`

```c
int ParsedHeader_parse(struct ParsedRequest * pr, char * line)
{
     char * key;
     char * value;
     char * index1;
     char * index2;

     index1 = index(line, ':'); // Find the colon separating key and value
     if(index1 == NULL) { /* ... error ... */ }
     key = (char *)malloc((index1-line+1)*sizeof(char));
     memcpy(key, line, index1-line);
     key[index1-line]='\0';

     index1 += 2; // Move past ": "
     index2 = strstr(index1, "\r\n"); // Find the end of the header line
     value = (char *) malloc((index2-index1+1)*sizeof(char));
     memcpy(value, index1, (index2-index1));
     value[index2-index1] = '\0';

     ParsedHeader_set(pr, key, value);
     free(key);
     free(value);
     return 0;
}
```

  * **Purpose**: Parses a single raw HTTP header line (e.g., "Host: [www.example.com](https://www.example.com)\\r\\n") into its key and value components, and then adds them to the `ParsedRequest` structure.
  * **Parameters**:
      * `struct ParsedRequest * pr`: A **pointer** to the `ParsedRequest` structure to add the parsed header to.
      * `char * line`: A **pointer** to the raw header line string.
  * **Pointers and Variables**:
      * `char * key`, `char * value`: **Pointers** that will temporarily hold the extracted key and value strings.
      * `char * index1`, `char * index2`: **Pointers** used to mark positions within the `line` string.
      * `index1 = index(line, ':')`: Finds the first occurrence of `':'` in the `line` string. `index` returns a **pointer** to that character.
      * `key = (char *)malloc((index1-line+1)*sizeof(char))`: Dynamically allocates memory for the `key` string. `index1-line` calculates the length of the key by finding the difference in memory addresses between `index1` and `line`.
      * `memcpy(key, line, index1-line); key[index1-line]='\0';`: Copies the key characters from `line` into the `key` buffer and null-terminates it.
      * `index1 += 2`: Moves `index1` **pointer** past the `": "` characters to the start of the value.
      * `index2 = strstr(index1, "\r\n")`: Finds the end of the header line (`\r\n`) starting from `index1`. `strstr` returns a **pointer** to the `\r\n`.
      * `value = (char *) malloc((index2-index1+1)*sizeof(char))`: Dynamically allocates memory for the `value` string based on the length between `index1` and `index2`.
      * `memcpy(value, index1, (index2-index1)); value[index2-index1] = '\0';`: Copies the value characters and null-terminates them.
      * `ParsedHeader_set(pr, key, value)`: Calls the function to add this parsed key-value pair to the `ParsedRequest` structure.
      * `free(key); free(value);`: Frees the temporary memory allocated for `key` and `value` strings.

-----

### `ParsedRequest_destroy(struct ParsedRequest *pr)`

```c
void ParsedRequest_destroy(struct ParsedRequest *pr)
{
     if(pr->buf != NULL)
     {
      free(pr->buf);
     }
     if (pr->path != NULL) {
      free(pr->path);
     }
     if(pr->headerslen > 0)
     {
      ParsedHeader_destroy(pr);
     }
     free(pr);
}
```

  * **Purpose**: Frees all dynamically allocated memory associated with a `ParsedRequest` structure and its internal components.
  * **Parameters**:
      * `struct ParsedRequest *pr`: A **pointer** to the `ParsedRequest` structure to destroy.
  * **Pointers and Variables**:
      * `free(pr->buf); free(pr->path);`: Frees the memory blocks pointed to by `pr->buf` (the request line) and `pr->path` (if it was separately allocated).
      * `ParsedHeader_destroy(pr)`: Calls the function to free all headers within the `ParsedRequest`.
      * `free(pr)`: Frees the `ParsedRequest` structure itself, which `pr` points to.

-----

### `ParsedRequest_create()`

```c
struct ParsedRequest* ParsedRequest_create()
{
     struct ParsedRequest *pr;
     pr = (struct ParsedRequest *)malloc(sizeof(struct ParsedRequest));
     if (pr != NULL)
     {
      ParsedHeader_create(pr);
      pr->buf = NULL;
      pr->method = NULL;
      pr->protocol = NULL;
      pr->host = NULL;
      pr->path = NULL;
      pr->version = NULL;
      pr->buf = NULL;
      pr->buflen = 0;
     }
     return pr;
}
```

  * **Purpose**: Allocates memory for and initializes a new `ParsedRequest` structure.
  * **Parameters**: None.
  * **Pointers and Variables**:
      * `struct ParsedRequest *pr`: A **pointer** that will point to the newly allocated `ParsedRequest` structure.
      * `pr = (struct ParsedRequest *)malloc(sizeof(struct ParsedRequest))`: Dynamically allocates memory for the `ParsedRequest` structure.
      * `ParsedHeader_create(pr)`: Initializes the `headers` array within the new `ParsedRequest` structure.
      * `pr->buf = NULL; pr->method = NULL; ...`: Initializes all other **pointers** within the `ParsedRequest` structure to `NULL` to ensure they start in a defined state.

-----

### `ParsedRequest_unparse(struct ParsedRequest *pr, char *buf, size_t buflen)`

```c
int ParsedRequest_unparse(struct ParsedRequest *pr, char *buf, 
              size_t buflen)
{
     if (!pr || !pr->buf)
      return -1;

     size_t tmp;
     if (ParsedRequest_printRequestLine(pr, buf, buflen, &tmp) < 0)
      return -1;
     if (ParsedHeader_printHeaders(pr, buf+tmp, buflen-tmp) < 0)
      return -1;
     return 0;
}
```

  * **Purpose**: Reconstructs the entire raw HTTP request string (request line + headers) from a `ParsedRequest` structure into a provided buffer.
  * **Parameters**:
      * `struct ParsedRequest *pr`: A **pointer** to the `ParsedRequest` structure.
      * `char *buf`: A **pointer** to the destination buffer.
      * `size_t buflen`: The maximum size of the destination buffer.
  * **Pointers and Variables**:
      * `size_t tmp`: A variable to store the length of the request line printed. `&tmp` passes the **address** of `tmp` so `ParsedRequest_printRequestLine` can modify its value.
      * `ParsedRequest_printRequestLine(pr, buf, buflen, &tmp)`: Prints the request line into `buf` and updates `tmp` with its length.
      * `ParsedHeader_printHeaders(pr, buf+tmp, buflen-tmp)`: Prints the headers into `buf` starting *after* the request line. `buf+tmp` is **pointer arithmetic** that moves the pointer to the correct starting position.

-----

### `ParsedRequest_unparse_headers(struct ParsedRequest *pr, char *buf, size_t buflen)`

```c
int ParsedRequest_unparse_headers(struct ParsedRequest *pr, char *buf, 
                  size_t buflen)
{
     if (!pr || !pr->buf)
      return -1;

     if (ParsedHeader_printHeaders(pr, buf, buflen) < 0)
      return -1;
     return 0;
}
```

  * **Purpose**: Reconstructs only the HTTP headers from a `ParsedRequest` structure into a provided buffer.
  * **Parameters**: Same as `ParsedRequest_unparse`, but it only deals with headers.
  * **Pointers and Variables**:
      * `ParsedHeader_printHeaders(pr, buf, buflen)`: Directly calls `ParsedHeader_printHeaders` to write the headers into `buf`.

-----

### `ParsedRequest_totalLen(struct ParsedRequest *pr)`

```c
size_t ParsedRequest_totalLen(struct ParsedRequest *pr)
{
     if (!pr || !pr->buf)
      return 0;
     return ParsedRequest_requestLineLen(pr)+ParsedHeader_headersLen(pr);
}
```

  * **Purpose**: Calculates the total length of the entire HTTP request (request line + headers) if it were unparsed into a single string.
  * **Parameters**:
      * `struct ParsedRequest *pr`: A **pointer** to the `ParsedRequest` structure.
  * **Pointers and Variables**:
      * Calls `ParsedRequest_requestLineLen(pr)` and `ParsedHeader_headersLen(pr)` to get the lengths of the respective parts and sums them.

-----

### `ParsedRequest_parse(struct ParsedRequest * parse, const char *buf, int buflen)`

```c
int 
ParsedRequest_parse(struct ParsedRequest * parse, const char *buf, 
            int buflen)
{
     char *full_addr;
     char *saveptr; // For strtok_r
     char *index;
     char *currentHeader;

     // ... checks for valid buffer, copies to tmp_buf ...

     index = strstr(tmp_buf, "\r\n"); // Find end of request line
     // ... copy request line to parse->buf ...

     /* Parse request line */
     parse->method = strtok_r(parse->buf, " ", &saveptr);
     // ... error checks for method ...

     full_addr = strtok_r(NULL, " ", &saveptr);
     // ... error checks for full_addr ...

     parse->version = full_addr + strlen(full_addr) + 1; // Pointer arithmetic to get version
     // ... error checks for version ...

     parse->protocol = strtok_r(full_addr, "://", &saveptr);
     // ... error checks for protocol ...

     parse->host = strtok_r(NULL, "/", &saveptr);
     // ... error checks for host ...

     parse->path = strtok_r(NULL, " ", &saveptr);
     // ... handling for path ...

     parse->host = strtok_r(parse->host, ":", &saveptr); // Separate host and port
     parse->port = strtok_r(NULL, "/", &saveptr);

     // ... error checks for host and port ...
   
     /* Parse headers */
     currentHeader = strstr(tmp_buf, "\r\n")+2; // Start after first \r\n
     while (currentHeader[0] != '\0' && 
        !(currentHeader[0] == '\r' && currentHeader[1] == '\n')) {
      
      if (ParsedHeader_parse(parse, currentHeader)) { /* ... handle error ... */ }

      currentHeader = strstr(currentHeader, "\r\n"); // Find next header line
      // ... error checks ...
      currentHeader += 2;
     }
     free(tmp_buf);
     return ret;
}
```

  * **Purpose**: This is the core parsing function. It takes a raw HTTP request buffer and populates a `ParsedRequest` structure with its components (method, URL parts, version, and headers).
  * **Parameters**:
      * `struct ParsedRequest * parse`: A **pointer** to the `ParsedRequest` structure to populate.
      * `const char *buf`: A **pointer** to the raw HTTP request string.
      * `int buflen`: The length of the raw request buffer.
  * **Pointers and Variables**:
      * `char *tmp_buf = (char *)malloc(buflen + 1);`: Creates a mutable, null-terminated copy of the input `buf`. This is important because `strtok_r` modifies the string it operates on. `tmp_buf` is a **pointer** to this copy.
      * `char *saveptr`: A **pointer** used by `strtok_r` to maintain its internal state, allowing it to be re-entrant (safe for use in multithreaded environments).
      * `char *index`: A **pointer** used to find substrings like `\r\n` within `tmp_buf`.
      * `char *currentHeader`: A **pointer** that iterates through the header lines in `tmp_buf`.
      * `parse->method = strtok_r(parse->buf, " ", &saveptr)`: `strtok_r` is used repeatedly to break down the request line and URL into their individual components. It modifies `parse->buf` in place and returns **pointers** to the start of each token (e.g., "GET", "http", "[www.example.com](https://www.example.com)"). These **pointers** are then assigned to `parse->method`, `parse->protocol`, `parse->host`, `parse->path`, `parse->port`, and `parse->version`.
          * **Important**: Many of these `parse->` members directly **point** into the `parse->buf` (or `tmp_buf` initially). This means they are *not* separately allocated strings but rather "views" into a larger string. `parse->path` is an exception as it may be allocated separately if it's the root path.
      * `parse->version = full_addr + strlen(full_addr) + 1;`: This is an example of **pointer arithmetic**. `full_addr` points to "[http://www.example.com/some/path](https://www.google.com/search?q=http://www.example.com/some/path)". `strlen(full_addr)` gets its length. Adding `strlen(full_addr) + 1` moves the pointer past this part and the space, directly to the "HTTP/1.1" string which becomes `parse->version`.
      * `ParsedHeader_parse(parse, currentHeader)`: This function is called in a loop for each header line found. It parses individual header lines and adds them to the `ParsedRequest` structure.
      * `free(tmp_buf)`: Frees the temporary copy of the request buffer.

-----

### `ParsedRequest_requestLineLen(struct ParsedRequest *pr)`

```c
size_t ParsedRequest_requestLineLen(struct ParsedRequest *pr)
{
     if (!pr || !pr->buf)
      return 0;

     size_t len =  
      strlen(pr->method) + 1 + strlen(pr->protocol) + 3 + 
      strlen(pr->host) + 1 + strlen(pr->version) + 2;
     if(pr->port != NULL)
     {
      len += strlen(pr->port)+1;
     }
     /* path is at least a slash */
     len += strlen(pr->path);
     return len;
}
```

  * **Purpose**: Calculates the length of the HTTP request line if it were unparsed into a string.
  * **Parameters**:
      * `struct ParsedRequest *pr`: A **pointer** to the `ParsedRequest` structure.
  * **Pointers and Variables**:
      * It uses `strlen()` on the various **pointers** within the `ParsedRequest` structure (`pr->method`, `pr->protocol`, `pr->host`, `pr->version`, `pr->port`, `pr->path`) to sum up their lengths and add fixed lengths for spaces, "://", and "\\r\\n".

-----

### `ParsedRequest_printRequestLine(struct ParsedRequest *pr, char * buf, size_t buflen, size_t *tmp)`

```c
int ParsedRequest_printRequestLine(struct ParsedRequest *pr, 
                   char * buf, size_t buflen,
                   size_t *tmp)
{
     char * current = buf;

     if(buflen <  ParsedRequest_requestLineLen(pr)) { /* ... error ... */ }
     memcpy(current, pr->method, strlen(pr->method));
     current += strlen(pr->method);
     current[0]  = ' ';
     current += 1;

     // ... similar memcpy and pointer arithmetic for protocol, host, port, path, version ...

     *tmp = current-buf; // Calculate total length written
     return 0;
}
```

  * **Purpose**: Writes the components of the HTTP request line (method, URL parts, version) from a `ParsedRequest` structure into a provided character buffer, formatted as a standard HTTP request line.
  * **Parameters**:
      * `struct ParsedRequest *pr`: A **pointer** to the `ParsedRequest` structure.
      * `char * buf`: A **pointer** to the destination buffer.
      * `size_t buflen`: The maximum size of the destination buffer.
      * `size_t *tmp`: A **pointer** to a `size_t` variable where the total length of the written request line will be stored.
  * **Pointers and Variables**:
      * `char * current = buf`: A **pointer** that keeps track of the current writing position within the `buf`.
      * `memcpy(current, pr->method, strlen(pr->method))`: Copies the string pointed to by `pr->method` into the `current` position of `buf`.
      * `current += strlen(pr->method)`: Advances the `current` **pointer** by the length of the copied string. This pattern is repeated for all parts of the request line (`pr->protocol`, `pr->host`, `pr->port`, `pr->path`, `pr->version`), along with adding spaces, "://", and "\\r\\n".
      * `*tmp = current-buf;`: After writing all parts of the request line, this line calculates the total number of bytes written. It subtracts the starting **address** of `buf` from the final **address** of `current`. The result is then stored in the `size_t` variable pointed to by `tmp` (using **dereferencing**).

-----

This detailed explanation covers the core functions, global variables, and the extensive use of pointers in both the main proxy server logic and the `proxy_parse` library. Understanding how pointers are used for memory management (malloc, realloc, free), accessing structure members (`->`), string manipulation (`strcpy`, `strcat`, `strstr`, `strtok_r`), and passing data to threads is fundamental to grasping this C code.