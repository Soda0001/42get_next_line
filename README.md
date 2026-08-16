*This project has been created as part of the 42 curriculum by <sedeniz>.*

# get_next_line

## Description

`get_next_line` is a project from the 42 curriculum focused on file descriptors, the `read()` system call, dynamic memory allocation, and static variables in C.

The goal of this project is to implement a function with the following prototype:

```c
char	*get_next_line(int fd);
```

The function reads from a file descriptor and returns one line at a time. Each call to `get_next_line()` returns the next line of the file, including the terminating newline character when one is present.

The implementation reads the file in chunks determined by `BUFFER_SIZE` rather than reading the entire file at once. Since a single `read()` operation can contain parts of multiple lines, or only part of a line, the program maintains previously read data between function calls using a static storage variable.

This repository currently contains the **mandatory part of the project only**. The bonus implementation has not been completed yet.

## Instructions

### Compilation

The project can be compiled by providing `BUFFER_SIZE` during compilation:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=5 *.c
```

`BUFFER_SIZE` determines how many bytes are requested from the file descriptor during each call to `read()`.

Different buffer sizes can be tested, for example:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1 *.c
```

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=100 *.c
```

A default value is also provided in the header file, so the project can be compiled without explicitly defining `BUFFER_SIZE`:

```bash
cc -Wall -Wextra -Werror *.c
```

The default value is used only when `BUFFER_SIZE` has not already been defined by the compiler.

### Execution

`get_next_line()` expects an already-open file descriptor.

For testing purposes, a file can be opened with `open()` in a separate test `main()`:

```c
int	fd;

fd = open("example.txt", O_RDONLY);
printf("%s", get_next_line(fd));
printf("%s", get_next_line(fd));
close(fd);
```

`open()` is used only by the test program. The `get_next_line` implementation itself uses only the external functions permitted by the project: `read()`, `malloc()`, and `free()`.

## Algorithm

The implementation uses a **static storage variable combined with incremental reading**.

The main steps are:

1. `get_next_line()` validates the file descriptor and allocates a temporary buffer of `BUFFER_SIZE + 1` bytes.
2. A static `storage` variable preserves unread data between calls to `get_next_line()`.
3. Before performing another `read()`, the existing `storage` is checked for a newline.
4. If a newline already exists, the line is extracted immediately.
5. If no complete line is available, `read()` obtains up to `BUFFER_SIZE` bytes.
6. The newly read data is appended to `storage`.
7. The storage is searched again for a newline.
8. When a newline is found, the line is returned and any remaining data is kept in `storage` for the next call.
9. When `read()` returns `0`, the end of the file has been reached. Any remaining data without a terminating newline is returned as the final line.
10. After the final data has been returned, the static storage is cleared so subsequent calls return `NULL`.

### Why this algorithm?

A single `read()` call does not necessarily correspond to a complete line. Depending on `BUFFER_SIZE`, a line may be split across several reads, or one read may contain multiple lines.

For example, with:

```text
BUFFER_SIZE = 5
```

the line:

```text
Hello world\n
```

could be received as:

```text
Hello
 worl
d\n
```

Persistent storage is therefore necessary to combine the pieces until a complete line is available.

The static `storage` variable allows data to survive between calls. This is particularly important when a `read()` obtains more data than is needed for the current line. The unused data remains in `storage` and is used by the next call.

The algorithm also handles the case where the final line of a file does not contain `\n`. When `read()` reaches EOF, the remaining contents of `storage` are returned as the final line.

## Project Structure

* `get_next_line.c` — main `get_next_line()` logic and reading process.
* `get_next_line.h` — function prototypes, required headers, and `BUFFER_SIZE` definition.
* `get_next_line_utils.c` — helper functions used by the project.
* `ft_strjoin.c` — joins the current storage with newly read data.
* `ft_substr.c` — extracts a portion of a string.
* `ft_strdup.c` — duplicates strings.
* `ft_strlen.c` — calculates string lengths.
* `append_to_storage.c` — appends newly read data to persistent storage.
* `extract_line.c` — extracts a complete line and updates the remaining storage.
* `handle_eof.c` — handles remaining storage when EOF is reached.
* Additional helper source files contain functions used for newline searching and file descriptor validation.

## External Functions

The mandatory implementation is limited to the external functions specified by the project:

* `read()`
* `malloc()`
* `free()`

Other functionality required by the implementation is provided through functions written as part of this project.

## Bonus

The bonus part has **not been implemented yet**.

The current implementation is the mandatory version. The bonus version, which requires handling multiple file descriptors simultaneously, has not been completed yet.

## Testing

The implementation was tested with different `BUFFER_SIZE` values, including small values such as `1` and `5`, to ensure that lines split across multiple `read()` calls are handled correctly.

Tests included:

* Files containing multiple lines.
* Empty files.
* Files containing an empty line.
* Files whose final line does not end with `\n`.
* Different `BUFFER_SIZE` values.
* Repeated calls to `get_next_line()` on the same file descriptor.
* Compilation with and without an explicitly supplied `BUFFER_SIZE`.
* Compilation with `-Wall -Wextra -Werror`.
* Norminette verification.

## Resources

### References

* Stack Overflow — [File descriptor flags and functions](https://stackoverflow.com/questions/50475058/file-descriptor-flags-and-functions)
* Stack Overflow — [Static and extern pointers in C](https://stackoverflow.com/questions/1286515/extern-and-static-pointers-in-c)
* You Suck at Programming — [Video](https://www.youtube.com/watch?v=o9iZucYSkQA&t=2892s)
* Portfolio Courses — [Video](https://www.youtube.com/watch?v=Hm5Jc7ajZsk)
* Unix & Linux Stack Exchange — [Limits on the number of file descriptors](https://unix.stackexchange.com/questions/84227/limits-on-the-number-of-file-descriptors)
* The Linux Kernel — [File descriptor limits](https://www.kernel.org/doc/html/latest/admin-guide/sysctl/fs.html#file-max-file-nr)
* Baeldung — [Limit file descriptors on Linux](https://www.baeldung.com/linux/limit-file-descriptors)

### Official Documentation

* `read(2)` — Linux manual page for the `read()` system call.
* `malloc(3)` — Linux manual page for dynamic memory allocation.
* `free(3)` — Linux manual page for releasing dynamically allocated memory.
* 42 `get_next_line` project subject — official project requirements and constraints.
* 42 Norminette — used to verify compliance with the 42 coding standard.

### AI Usage

AI was used as a learning and debugging aid during this project.

Specifically, AI was used to:

* Explain file descriptors and how `read()` operates.
* Explain `BUFFER_SIZE` and compiler definitions.
* Help identify and understand compiler and Norminette errors.
* Discuss edge cases and test scenarios.
* Review the overall algorithm and assist with debugging.
* Asissting with writing this Readme file.

AI was used primarily for **conceptual explanations, debugging, and code review**. The implementation was developed, tested, and integrated by me, <sedeniz>.
