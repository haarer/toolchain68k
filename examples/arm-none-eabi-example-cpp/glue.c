// see porting.info of newlib, mostly derived from the sample

#include "uart.h"
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>

#undef errno
extern int errno;


void _exit(int s)
{
  for (;;)
    ;
}


/*
char *__env[1] = {0};
char **environ = __env;
*/

extern char _end[] __attribute__((aligned(4)));

void *_sbrk(int incr)
{
  static unsigned char *heap = NULL;
  unsigned char *prev_heap;

  if (heap == NULL)
  {
    heap = (unsigned char *)&_end;
  }
  prev_heap = heap;

  heap += incr;

  return prev_heap;
}


#define __MYPID 1
int _getpid(void)
{
  return __MYPID;
}



int _kill(int pid, int sig)
{
  if (pid == __MYPID)
    _exit(sig);
  return 0;
}


int _isatty(fd)
int fd;
{
  return (1);
}

int _open(buf, flags, mode)
char *buf;
int flags;
int mode;
{
  errno = EIO;
  return (-1);
}

int _close(fd)
int fd;
{
  return (0);
}


off_t _lseek(fd, offset, whence)
int fd;
off_t offset;
int whence;
{
  errno = ESPIPE;
  return ((off_t)-1);
}

int _fstat(int __fd, struct stat *__sbuf)
{
  return 0;
}




int __ssputws_r(char c)
{
  uartPutch(c);
  return 1;
}



int _read(fd, buf, nbytes)
int fd;
char *buf;
int nbytes;
{
  int i = 0;

  for (i = 0; i < nbytes; i++)
  {
    *(buf + i) = uartGetch();
    if ((*(buf + i) == '\n') || (*(buf + i) == '\r'))
    {
      (*(buf + i)) = 0;
      break;
    }
  }
  return (i);
}




int _write(fd, buf, nbytes)
int fd;
char *buf;
int nbytes;
{
  int i;

  for (i = 0; i < nbytes; i++)
  {
    if (*(buf + i) == '\n')
    {
      uartPutch('\r');
    }
    uartPutch(*(buf + i));
  }
  return (nbytes);
}


