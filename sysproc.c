#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#ifdef PDX_XV6
#include "pdx-kernel.h"
#endif // PDX_XV6

//P2
#include "uproc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      return -1;
    }
    sleep(&ticks, (struct spinlock *)0);
  }
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  xticks = ticks;
  return xticks;
}

#ifdef PDX_XV6
// shutdown QEMU
int
sys_halt(void)
{
  do_shutdown();  // never returns
  return 0;
}
#endif // PDX_XV6

//P1 - date syscall
int
sys_date(void)
{
  struct rtcdate *d;

  if(argptr(0, (void*)&d, sizeof(struct rtcdate)) < 0)
    return -1;
  
  cmostime(d);
  return 0;
}

//P2 - UID,GID,PPID
uint
sys_getuid(void)
{
  return myproc()->uid;
}

uint
sys_getgid(void)
{
  return myproc()->gid;
}

uint
sys_getppid(void)
{
  if(!myproc()->parent)
    return myproc()->pid;
  return myproc()->parent->pid;
}

int
sys_setuid(void)
{
  int uid;
  if(argint(0,&uid) < 0)
    return -1;
  if(uid < 0 || uid > 32767)
    return -1;
  myproc()->uid = uid;
  return 0;
}

int
sys_setgid(void)
{
  int gid;
  if(argint(0,&gid) < 0)
    return -1;
  if(gid < 0 || gid > 32767)
    return -1;
  myproc()->gid = gid;
  return 0;
}

int
sys_getprocs()
{
  int max;
  struct uproc* table;
  if(argint(0,&max) < 0)
  {
    return -1;
  }
  if(argptr(1,(void*)&table, sizeof(struct uproc) * max) < 0)
  {
    return -1;
  }
  return getprocs(max,table);
}

//P4 -- MLFQ priority
int
sys_setpriority(void)
{
  int pid;
  int priority;
  if(argint(0,&pid)<0 || argint(1,&priority)<0)
    return -1;
  if(pid<0 || priority<0 || priority>MAXPRIO)
    return -1;
  return setpriority(pid,priority);
}

int
sys_getpriority(void)
{
  int pid;
  if(argint(0,&pid) < 0)
    return -1;
  return getpriority(pid);
}
