#include <uart.h>
#include <reent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#undef errno
extern int errno;

char *__env[1] = { 0 };
char **environ = __env;

/*
 * sbrk -- changes heap size size. Get nbytes more
 *         RAM. We just increment a pointer in what's
 *         left of memory on the board.
 */

extern char __end[] __attribute__ ((aligned (4)));

/* End of heap, if non NULL.  */
extern void *__heap_limit;

void *_sbrk_r (struct _reent *r, ptrdiff_t n)
{
  static char *heap = __end;
  char *end = __heap_limit;
  char *base = heap;
  char *new_heap = heap + n;

  if (!end)
  {
    /* Use sp - 256 as the heap limit.  */
    __asm__ __volatile__ ("move.l %/sp,%0" : "=r"(end));
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

int _close_r (struct _reent *r, int f)
{
  return -1;
}

int _execve_r (struct _reent *r, const char *n, char *const *a, char *const *e)
{
  r->_errno=ENOMEM;
  return -1;
}

int _fork_r (struct _reent *r)
{
  r->_errno=EAGAIN;
  return -1;
}

int _fstat_r (struct _reent *r, int f, struct stat *s)
{
  s->st_mode = S_IFCHR;
  return 0;
}

int _getpid_r (struct _reent *r)
{
  return 1;
}

int isatty(int file)
{
  return 1;
}

int _kill_r (struct _reent *r, int p, int s)
{
  r->_errno=EINVAL;
  return(-1);
}

int _link_r (struct _reent *r, const char *o, const char *n)
{
  r->_errno=EMLINK;
  return -1;
}

_off_t _lseek_r (struct _reent *r, int f, _off_t p, int d)
{
  return 0;
}

int _open_r (struct _reent *r, const char *n, int f, int m)
{
  return -1;
}

int _stat_r (struct _reent *r, const char *f, struct stat *s)
{
  s->st_mode = S_IFCHR;
  return 0;
}

_CLOCK_T_ _times_r (struct _reent *r, struct tms *t)
{
  return -1;
}

int _unlink_r (struct _reent *r, const char *n)
{
  r->_errno=ENOENT;
  return -1;
}

int _wait_r (struct _reent *r, int *s)
{
  r->_errno=ECHILD;
  return -1;
}

_ssize_t _read_r (struct _reent *r, int f, void *b, size_t s)
{
  size_t todo;
  char *ptr = b;
  char ch;
  if(r->_stdin && f == fileno(r->_stdin))
  {
    for (todo = 0; todo < s; todo++)
    {
      ch = (char)i_uartGetch();
      if(ch == -1)
      {
        if(todo == 0) return(-1);
          break;
      }
      *ptr++ = ch;
    }
    return todo;
  }
  else
  {
    r->_errno=EBADF;
    return -1;
  }
}

_ssize_t _write_r (struct _reent *r, int f, const void *b, size_t s)
{
  size_t todo;
  char *ptr = (char*)b;
  if((r->_stdout && f == fileno(r->_stdout)) || (r->_stderr && f == fileno(r->_stderr)))
  {
    for (todo = 0; todo < s; todo++)
      v_uartPutch(*ptr++);
    return s;
  }
  else
  {
    r->_errno=EBADF;
    return -1;
  }
}

void __malloc_lock (struct _reent *r)
{
}

void __malloc_unlock (struct _reent *r)
{
}

int _gettimeofday_r(struct _reent *r, struct timeval *p, void *tz)
{
  return 0;
}

int settimeofday(const struct timeval *t, const struct timezone *tz)
{
  return 0;
}
