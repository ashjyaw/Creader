// Author: Ash Yaw
// Version 0.0.1
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 8192  // most computers store in blocks of 4096 or 8192

// outline of all variables used
int exit_status = 0;
int fd;
char buffer[BUFFER_SIZE];
ssize_t bytes_read;

void Creader(char *curFile, int *exit_status) {
  if (curFile[0] == '-') {  // if argument passed in is - use standard input
    fd = STDIN_FILENO;
  } else {  // use open to open any other file or directory to file
    fd = open(curFile, O_RDONLY);
  }
  if (fd < 0) {  // error handle with file descriptor
    fprintf(stderr, "bobcat: %s: %s\n", curFile, strerror(errno));
    *exit_status = 1;
    return;
  }

  while (1) {                                    // infinite loop
    bytes_read = read(fd, buffer, BUFFER_SIZE);  // read from file descriptor
    if (bytes_read == 0) {
      break;
    } else if (bytes_read < 0) {  // warn
      fprintf(stderr, "bobcat: %s: %s\n", curFile, strerror(errno));
      *exit_status = 1;
      break;
    }

    for (int i = 0; i < bytes_read;
         i++) {  // may not even need this function including for safe keeping
      if (buffer[i] == 4) {  // ^d exit key
        close(fd);
        return;
      }
    }

    write(STDOUT_FILENO, buffer, bytes_read);  // write to stdout
  }

  if (fd != STDIN_FILENO) {
    close(fd);  // close valgrind
  }
}

int main(int argc, char *argv[]) {
  if (argc == 1)  // base case for no args just copy from standard input
  {
    Creader("-", &exit_status);
  } else {
    for (int i = 1; i < argc; i++)  // for any args
    {
       Creader(argv[i], &exit_status);
    }
  }
  return exit_status;
}
