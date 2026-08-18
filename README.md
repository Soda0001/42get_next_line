*This project has been created as part of the 42 curriculum by <sedeniz>.*

# get_next_line

## Description

`get_next_line` is a project from the 42 curriculum focused on file descriptors, the `read()` system call, dynamic memory allocation, string manipulation, and static variables in C.

The goal of this project is to implement the following function:

```c
char	*get_next_line(int fd);
```

The function reads from a file descriptor and returns one line at a time. Each call to `get_next_line()` returns the next line available from the file descriptor, including the terminating `\n` character when one is present.

The implementation does not read the entire file at once. Instead, it reads the file in chunks whose size is determined by `BUFFER_SIZE`. Since a single `read()` operation can contain only part of a line, or several lines at once, the implementation needs persistent storage to preserve unread data between calls.

The mandatory implementation uses persistent storage to keep track of unread data between calls.

The bonus implementation extends this concept to support **multiple file descriptors simultaneously**. A static array of storage pointers is used so that each file descriptor maintains its own independent reading state.

```c
static char	*storage[MAX_FD];
```

This allows calls to `get_next_line()` to alternate between different file descriptors without mixing their stored data.

This repository contains both the **mandatory and bonus implementations** of the project.

---

## Instructions

### Compilation

`BUFFER_SIZE` can be provided to the compiler using the `-D` option:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=5 *.c
```

For example, different buffer sizes can be tested with:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1 *.c
```

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=100 *.c
```

The header file also provides a default value for `BUFFER_SIZE` when it has not already been defined during compilation.

Therefore, the project can also be compiled without explicitly specifying `BUFFER_SIZE`:

```bash
cc -Wall -Wextra -Werror *.c
```

The value of `BUFFER_SIZE` determines the maximum number of bytes requested from the file descriptor during each call to `read()`.

### Mandatory Compilation

The mandatory implementation can be compiled with:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=5 \
get_next_line.c get_next_line_utils.c main.c
```

### Bonus Compilation

The bonus implementation can be compiled with:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=5 \
get_next_line_bonus.c get_next_line_utils_bonus.c main.c
```

### Execution

`get_next_line()` expects an already-open file descriptor.

For testing, a file descriptor can be created using `open()` in a separate `main()` function:

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int	main(void)
{
	int	fd;

	fd = open("example.txt", O_RDONLY);
	if (fd < 0)
		return (1);

	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));

	close(fd);
	return (0);
}
```

`open()`, `printf()`, and `close()` are used here only by the test program. The `get_next_line()` implementation itself uses only the external functions permitted by the project:

* `read()`
* `malloc()`
* `free()`

---

## Algorithm

The implementation uses **persistent static storage combined with incremental reading**.

### 1. Initial validation and allocation

When `get_next_line()` is called, the file descriptor is first validated.

For the bonus implementation, the descriptor must also be within the supported range:

```c
if (fd < 0 || fd >= MAX_FD)
	return (NULL);
```

A temporary buffer of `BUFFER_SIZE + 1` bytes is then allocated.

The extra byte is used to add the terminating `\0` after each successful `read()`.

### 2. Check existing storage

Before performing another `read()`, the implementation checks whether the persistent storage already contains a newline.

If a newline is already present, the line can be extracted immediately without reading more data.

This is important because a previous `read()` may have retrieved more data than was needed for the previous line.

### 3. Incremental reading

If the storage does not contain a complete line, the implementation calls:

```c
read(fd, buff, BUFFER_SIZE);
```

The number of bytes returned by `read()` is then used to terminate the buffer:

```c
buff[bytes_read] = '\0';
```

The newly read data is appended to the existing storage.

The process continues until either:

* a newline is found,
* EOF is reached, or
* an error occurs.

### 4. Extracting a line

When a newline is found, its position is identified.

The line is then extracted from the beginning of the storage up to and including the newline.

For example:

```text
storage:
Hello\nWorld\n
```

The first call returns:

```text
Hello\n
```

and the remaining storage becomes:

```text
World\n
```

The remaining data is preserved for the next call.

### 5. EOF handling

When `read()` returns `0`, the end of the file has been reached.

If storage still contains data, that data is returned as the final line even if it does not end with `\n`.

For example:

```text
First line\n
Last line
```

The final call returns:

```text
Last line
```

After the remaining data has been returned, the storage is cleared.

A subsequent call returns:

```c
NULL
```

because there is nothing left to read.

---

## Why This Algorithm?

A single `read()` call does not necessarily correspond to a complete line.

For example, with:

```text
BUFFER_SIZE = 5
```

the line:

```text
Hello world\n
```

could be read as:

```text
Hello
 worl
d\n
```

The implementation therefore cannot simply return the result of one `read()` call.

Persistent storage allows the pieces to be accumulated until a complete line is available.

The opposite situation can also occur. A single `read()` can contain several lines:

```text
Hello\nWorld\n42\n
```

If the first call returns:

```text
Hello\n
```

the remaining:

```text
World\n42\n
```

must not be lost.

The storage preserves this unread data so that the next call can use it.

This approach allows the function to return exactly one line per call while reading the file incrementally instead of loading the entire file into memory.

---

## Bonus: Multiple File Descriptors

The bonus implementation extends the mandatory algorithm to support multiple file descriptors simultaneously.

Instead of using one storage pointer, the bonus implementation declares:

```c
static char	*storage[MAX_FD];
```

with:

```c
#define MAX_FD 1024
```

Each index in the array corresponds to a file descriptor.

For example:

```text
storage[3] → unread data belonging to fd 3
storage[4] → unread data belonging to fd 4
storage[7] → unread data belonging to fd 7
```

This means that every file descriptor maintains its own independent state.

### Why separate storage is necessary

Consider two files:

```text
File 1:
AAA
BBB
CCC
```

and:

```text
File 2:
111
222
333
```

Suppose both files are opened:

```text
fd1 = 3
fd2 = 4
```

Calls can then be made in an alternating order:

```c
get_next_line(fd1);
get_next_line(fd2);
get_next_line(fd1);
get_next_line(fd2);
```

The expected results are:

```text
AAA
111
BBB
222
```

The unread data belonging to `fd1` must remain separate from the unread data belonging to `fd2`.

The bonus implementation achieves this with:

```c
storage[fd]
```

When `get_next_line(fd)` is called, only the storage associated with that descriptor is accessed and modified.

Therefore:

```text
storage[3]
```

contains only data belonging to file descriptor `3`, while:

```text
storage[4]
```

contains only data belonging to file descriptor `4`.

This allows multiple files to be read independently and alternately.

---

## Bonus Algorithm

The bonus implementation follows these steps:

1. Validate the file descriptor.
2. Verify that the descriptor is within the `MAX_FD` range.
3. Allocate the temporary read buffer.
4. Access the storage associated with the current file descriptor.
5. Check whether that storage already contains a newline.
6. If a newline exists, extract and return the line immediately.
7. If no newline exists, read up to `BUFFER_SIZE` bytes.
8. Append the newly read data to `storage[fd]`.
9. Search the updated storage for a newline.
10. If a newline is found, extract the line and preserve the remaining data in `storage[fd]`.
11. If EOF is reached, return the remaining storage for that descriptor.
12. Clear `storage[fd]` after the final data has been returned.
13. Leave the storage belonging to all other file descriptors untouched.

The important difference from the mandatory implementation is that the state is indexed by file descriptor.

---

## Project Structure

The repository contains both mandatory and bonus implementations.

### Mandatory

* `get_next_line.c` — main mandatory `get_next_line()` logic and reading process.
* `get_next_line.h` — mandatory function prototypes, required headers, and `BUFFER_SIZE` definition.
* `get_next_line_utils.c` — utility functions used by the mandatory implementation.

### Bonus

* `get_next_line_bonus.c` — bonus `get_next_line()` implementation and per-file-descriptor storage management.
* `get_next_line_bonus.h` — bonus function prototypes, required headers, `BUFFER_SIZE`, and `MAX_FD`.
* `get_next_line_utils_bonus.c` — utility functions used by the bonus implementation.

The bonus utility file contains functions responsible for:

* Finding newline characters.
* Checking for newline characters.
* Extracting lines.
* Joining strings.
* Duplicating strings.
* Extracting substrings.
* Calculating string lengths.

---

## External Functions

The `get_next_line` implementation is limited to the external functions specified by the project:

| Function   | Purpose                               |
| ---------- | ------------------------------------- |
| `read()`   | Reads data from the file descriptor   |
| `malloc()` | Dynamically allocates memory          |
| `free()`   | Releases dynamically allocated memory |

Other functionality required by the implementation is provided by functions written as part of the project.

The test programs may use additional standard functions such as `open()`, `close()`, and `printf()`. These are not part of the `get_next_line()` implementation itself.

---

## Edge Cases

The implementation is designed to handle a variety of situations, including:

* Empty files.
* Files containing a single line.
* Files containing multiple lines.
* Empty lines.
* Lines shorter than `BUFFER_SIZE`.
* Lines longer than `BUFFER_SIZE`.
* Lines split across multiple `read()` calls.
* Multiple lines contained in a single `read()`.
* Files whose final line does not end with `\n`.
* Repeated calls to `get_next_line()`.
* EOF handling.
* Invalid file descriptors.
* Read errors.
* Very small `BUFFER_SIZE` values.
* Multiple file descriptors in the bonus implementation.
* Alternating calls between multiple file descriptors.

---

## Testing

The implementation was tested with different `BUFFER_SIZE` values, including very small values such as `1` and `5`, to verify that lines split across multiple `read()` calls are handled correctly.

### Mandatory Tests

Testing included:

* Multiple-line files.
* Empty files.
* Empty lines.
* Long lines.
* Lines shorter than `BUFFER_SIZE`.
* Lines longer than `BUFFER_SIZE`.
* Multiple lines contained in a single `read()`.
* Lines split across several `read()` calls.
* Files whose final line does not end with `\n`.
* Different `BUFFER_SIZE` values.
* Repeated calls to `get_next_line()`.
* EOF handling.
* Invalid file descriptors.
* Compilation with `-Wall -Wextra -Werror`.
* Norminette verification.
* Memory testing with Valgrind.

### Bonus Tests

The bonus implementation was additionally tested with multiple file descriptors to verify that each descriptor maintains its own independent storage.

For example:

```c
printf("%s", get_next_line(fd1));
printf("%s", get_next_line(fd2));
printf("%s", get_next_line(fd1));
printf("%s", get_next_line(fd2));
```

For two files containing:

```text
File 1:
AAA
BBB
CCC
```

and:

```text
File 2:
111
222
333
```

the expected output is:

```text
AAA
111
BBB
222
```

This verifies that the unread contents of the two files are not mixed together.

### Example Compilation

Mandatory:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=5 \
get_next_line.c get_next_line_utils.c main.c
```

Bonus:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=5 \
get_next_line_bonus.c get_next_line_utils_bonus.c main.c
```

### Valgrind

Memory usage can be checked with:

```bash
valgrind --leak-check=full ./a.out
```

The implementation was also checked using different buffer sizes and repeated calls to verify that allocated memory is correctly managed.

---

## Resources

### Official Documentation

* `read(2)` — Linux manual page for the `read()` system call.
* `malloc(3)` — Linux manual page for dynamic memory allocation.
* `free(3)` — Linux manual page for releasing dynamically allocated memory.
* 42 `get_next_line` project subject — official project requirements and constraints.
* 42 Norminette — used to verify compliance with the 42 coding standard.

### References and Tutorials

* Stack Overflow — [File descriptor flags and functions](https://stackoverflow.com/questions/50475058/file-descriptor-flags-and-functions)
* Stack Overflow — [Static and extern pointers in C](https://stackoverflow.com/questions/1286515/extern-and-static-pointers-in-c)
* You Suck at Programming — [Video](https://www.youtube.com/watch?v=o9iZucYSkQA&t=2892s)
* Portfolio Courses — [Video](https://www.youtube.com/watch?v=Hm5Jc7ajZsk)
* Unix & Linux Stack Exchange — [Limits on the number of file descriptors](https://unix.stackexchange.com/questions/84227/limits-on-the-number-of-file-descriptors)
* The Linux Kernel — [File descriptor limits](https://www.kernel.org/doc/html/latest/admin-guide/sysctl/fs.html#file-max-file-nr)
* Baeldung — [Limit file descriptors on Linux](https://www.baeldung.com/linux/limit-file-descriptors)

---

## AI Usage

AI was used as a learning and debugging aid during this project.

AI was specifically used for:

* Explaining file descriptors and how `read()` works.
* Understanding how `BUFFER_SIZE` is defined and passed to the compiler.
* Understanding static variables and persistent storage.
* Understanding how storage can be maintained separately for multiple file descriptors.
* Explaining dynamic memory allocation and memory ownership.
* Discussing edge cases and possible testing strategies.
* Assisting with writing and improving this README file.

AI was primarily used for **conceptual explanations, debugging, code review, and learning**. The implementation was written, tested, modified, and integrated by me, <sedeniz>.

---

## Conclusion

`get_next_line` provides practical experience with several fundamental concepts in C programming, including:

* File descriptors.
* System calls.
* Dynamic memory allocation.
* String manipulation.
* Static variables.
* Persistent program state.
* Memory management.
* Handling multiple file descriptors.

The main challenge of the project is maintaining the correct state between function calls while ensuring that no data is lost when lines are split across multiple reads or when a single read contains more data than is needed for the current line.

The mandatory implementation solves this problem using persistent storage.

The bonus implementation extends the same approach by maintaining **independent persistent storage for each file descriptor**, allowing multiple files to be read simultaneously and alternately without mixing their data.

The project therefore demonstrates both incremental file reading and the management of independent persistent states in C.
