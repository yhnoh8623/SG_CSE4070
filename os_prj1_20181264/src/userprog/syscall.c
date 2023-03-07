#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"

static void syscall_handler (struct intr_frame *);

void halt();
void system_exit(int exitnum);
bool validcheck(const void* addr);
int read(int fd, void *buffer, unsigned size);
int write(int fd, void*buffer, unsigned size);
int fibonacci(int n);
int max_of_four_int(int a, int b, int c, int d);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  int check = *(uint32_t *)(f->esp);
  if(check == SYS_HALT)
  {
    halt();
  }
  if(check == SYS_EXIT)
  {
    if(validcheck(f->esp+4) == false)
      system_exit(-1);
    else
      system_exit(*(uint32_t*)(f->esp+4)); 
  }
  if(check == SYS_EXEC)
  {
    if(validcheck(f->esp+4) == false)
      system_exit(-1);
    else
      f->eax = process_execute((const char*)*(uint32_t*)(f->esp+4));
  }
  if(check == SYS_WAIT)
  {
    if(validcheck(f->esp+4) == false)
      system_exit(-1);
    else
      f->eax = process_wait((tid_t)*(uint32_t*)(f->esp+4));
  }
  if(check == SYS_READ)
  {
    if(validcheck(f->esp+4)==false || validcheck(f->esp+8)==false || validcheck(f->esp+12)==false)
      system_exit(-1);
    f->eax = read((int)*(uint32_t*)(f->esp+4), (void*)*(uint32_t*)(f->esp+8), (unsigned)*(uint32_t*)(f->esp+12));
  }
  if(check == SYS_WRITE)
  {
    if(validcheck(f->esp+4)==false || validcheck(f->esp+8)==false || validcheck(f->esp+12)==false)
      system_exit(-1);
    f->eax = write((int)*(uint32_t*)(f->esp+4), (void*)*(uint32_t*)(f->esp+8), (unsigned)*(uint32_t*)(f->esp+12));
  }
  if(check == SYS_FIBO)
  {
    if(validcheck(f->esp+4) == false)
      system_exit(-1);
    f->eax = fibonacci((int)*(uint32_t*)(f->esp+4));
  }
  if(check == SYS_MAX)
  {
    if(validcheck(f->esp+4)== false || validcheck(f->esp+8) == false || validcheck(f->esp+12) == false || validcheck(f->esp+16) == false)
      system_exit(-1);
    f->eax = max_of_four_int((int)*(uint32_t*)(f->esp+4), (int)*(uint32_t*)(f->esp+8), (int)*(uint32_t*)(f->esp+12), (int)*(uint32_t*)(f->esp+16));
  }
}
bool validcheck(const void* addr)
{
  if(is_user_vaddr(addr)==false || addr == NULL)
    return false;

  else
    return true;
}
void halt()
{
  shutdown_power_off();
}
void system_exit(int exitnum)
{
  printf("%s: exit(%d)\n", thread_name(), exitnum);
  thread_current()->exit_status = exitnum;
  thread_exit();
}
int read(int fd, void *buffer, unsigned size)
{
  int ret = 0;
  if(fd!=0)
    return -1;
  else{
    int len = (int)size;
    for(int i=0; i<len; i++)
    {
      uint8_t c = input_getc();

      ret++;
      *((char*) buffer+i) = c;
      if(c=='\0')
        break;
      
    }
  }
  return ret;
}
int write(int fd, void*buffer, unsigned size)
{
  if(fd!=1)
    return -1;
  else{
    int ret = 0;
    int len = (int)size;
    putbuf((const char*)buffer, (size_t)size);
    return len;
  }
}
int fibonacci(int n)
{
  if(n==1)
    return 1;
  if(n==2)
    return 1;
  int before = 1;
  int after = 1;
  int save;
  for(int i=0; i<n-2; i++)
  {
    save = after;
    after += before;
    before = save;
  }
  return after;
}

int max_of_four_int(int a, int b, int c, int d)
{
  int ret = a;
  if(ret<b) 
    ret = b;
  if(ret<c)
    ret = c;
  if(ret<d)
    ret = d;
  return ret;
}