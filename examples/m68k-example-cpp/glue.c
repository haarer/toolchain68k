// see porting.info of newlib, mostly derived from the sample

#include <uart.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>

#undef errno
extern int errno;

char *__env[1] = {0};
char **environ = __env;

// tbd : technical debt: understand exactly
//  https://github.com/raspberrypi/pico-sdk/issues/1368
//  https://stackoverflow.com/questions/34308720/where-is-dso-handle-defined
void *__dso_handle = 0;
// not solved:  -fno-use-cxa-atexit

/*
 * sbrk -- changes heap size size. Get nbytes more
 *         RAM. We just increment a pointer in what's
 *         left of memory on the board.
 */

extern char __end[] __attribute__((aligned(4)));

/* End of heap, if non NULL.  */
extern void *__heap_limit;

void *_sbrk_r(struct _reent *r, ptrdiff_t n)
{
  static char *heap = __end;
  char *end = __heap_limit;
  char *base = heap;
  char *new_heap = heap + n;

  if (!end)
  {
    /* Use sp - 256 as the heap limit.  */
    __asm__ __volatile__("move.l %/sp,%0" : "=r"(end));
    end -= 256;
  }
  if (n < 0 || (long)(end - new_heap) < 0)
  {
    r->_errno = ENOMEM;
    return (void *)-1;
  }
  heap = new_heap;
  return base;
}

/*
 * getpid -- only one process, so just return 1.
 */
#define __MYPID 1
int getpid()
{
  return __MYPID;
}

void _exit(int)
{
  for (;;)
    ;
}

/*
 * kill -- go out via exit...
 */
int kill(pid, sig)
int pid;
int sig;
{
  if (pid == __MYPID)
    _exit(sig);
  return 0;
}
/*
 * isatty -- returns 1 if connected to a terminal device,
 *           returns 0 if not. Since we're hooked up to a
 *           serial port, we'll say yes and return a 1.
 */
int isatty(fd)
int fd;
{
  return (1);
}

int __ssputws_r(char)
{
  return 0;
}

int getentropy(void *)
{
  return 0;
}

/*
 * open -- open a file descriptor. We don't have a filesystem, so
 *         we return an error.
 */
int open(buf, flags, mode)
char *buf;
int flags;
int mode;
{
  errno = EIO;
  return (-1);
}

/*
 * close -- close a file descriptor. We don't need
 *          to do anything, but pretend we did.
 */
int close(fd)
int fd;
{
  return (0);
}

/*
 * lseek -- move read/write pointer. Since a serial port
 *          is non-seekable, we return an error.
 */
off_t lseek(fd, offset, whence)
int fd;
off_t offset;
int whence;
{
  errno = ESPIPE;
  return ((off_t)-1);
}

int fstat(int __fd, struct stat *__sbuf)
{
  return 0;
}

/*
 * read  -- read bytes from the serial port. Ignore fd, since
 *          we only have stdin.
 */
int read(fd, buf, nbytes)
int fd;
char *buf;
int nbytes;
{
  int i = 0;

  for (i = 0; i < nbytes; i++)
  {
    *(buf + i) = i_uartGetch();
    if ((*(buf + i) == '\n') || (*(buf + i) == '\r'))
    {
      (*(buf + i)) = 0;
      break;
    }
  }
  return (i);
}
/*
 * write -- write bytes to the serial port. Ignore fd, since
 *          stdout and stderr are the same. Since we have no filesystem,
 *          open will only return an error.
 */

int write(fd, buf, nbytes)
int fd;
char *buf;
int nbytes;
{
  int i;

  for (i = 0; i < nbytes; i++)
  {
    if (*(buf + i) == '\n')
    {
      v_uartPutch('\r');
    }
    v_uartPutch(*(buf + i));
  }
  return (nbytes);
}