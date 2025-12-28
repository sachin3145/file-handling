# Simple File-Backed Entry Manager (C)

## Overview

This is a small C application that manages a list of entries stored in a file.

- Data is loaded into memory at startup
- The user can print, add, and delete entries
- Data is saved back to the file on exit
- File I/O can be done using **stdio** or **POSIX** APIs
- The project is intentionally simple and single-threaded

## Project Structure

```bash
/project
│
├── data/
│   └── appdata          # persistent storage file
│
├── include/
│   ├── dbiface.h        # dispatches file operations
│   ├── dbposix.h        # POSIX file handling
│   ├── dbstdio.h        # stdio file handling
│   ├── entry.h          # entry operations (print/add/delete)
│   └── logger.h         # logging and error handling
│
├── src/
│   ├── dbiface.c
│   ├── dbposix.c
│   ├── dbstdio.c
│   ├── entry.c
│   └── logger.c
│
└── main.c               # event loop and program entry point
```

## Compilation

Use `gcc` to compile the project:

```bash
gcc -I include src/*.c main.c -o app
```

## Usage

```bash
./app 
```

## License

This project is licensed under the MIT License. See the LICENSE file for details.

