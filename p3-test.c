#ifdef CS333_P3
// A starting point for writing your own p3 test program(s).
// Notes
// 1. The parent never gets to the wait() call, so killing any child will cause that
//    child to move to zombie indefinitely.
// 2. When running as a background process, the parent of the main process
//    will be init. This is how background processes work. Read sh.c for details.
// 3. Useful test: run in background then kill a child. Child will be in zombie. Now
//    kill parent. Should get "zombie!" equal to the number of zombies.
//    ps or ^p should now show init as the parent of all child processes.
//    init will then reap the zombies.
// 4. Can also be used to show that the RUNNABLE list is still round robin.

#include "types.h"
#include "user.h"
#include "param.h"
#include "pdx.h"
#define SLEEP_TIME 7*TPS

#define FORK_TEST  // For UNUSED test
//#define ROUND_ROBIN_TEST
//#define SLEEP_TEST
//#define KILL_TEST
//#define EXIT_TEST
//#define WAIT_TEST

#ifdef ROUND_ROBIN_TEST
int
main(int argc, char *argv[])
{
  int pid = 1;
  int j = 0;
  
  printf(1, "\n-------Round Robin Test-------\n");
  for(int i = 0; i < 6; i++){
    if(pid > 0){
      printf(1, "Looping to create processes\n");
      pid = fork();
    }else{	// to make the time slice on each process almost equivalent
      break;
    }
  }
  if(pid > 0){
    printf(1, "Press and hold CTRL+R to display RUNNABLE list and then CTRL+P\n\n");
    sleep(SLEEP_TIME);
    while(wait() != -1);  // if the process has no kids then it will be -1
  }

  // timer interrupt will invoke yield() to let
  // other process run when the time is up (time slice)
  while(1) ++j;  // infinite

  exit();	//not reachable
}
#endif // round robin

#ifdef SLEEP_TEST
int
main(int argc, char *argv[])
{
  printf(1, "\n-------Sleep Test-------\n");
  printf(1, "Process with PID %d is being moved to sleep state list now.\n", getpid());
  printf(1, "Press CTRL+S to display SLEEPING list and then CTRL+P.\n\n");
  sleep(SLEEP_TIME);
  printf(1, "Process is awake and exiting now.\n");
  printf(1, "Press CTRL+S to display SLEEPING list and then CTRL+P.\n\n");
  exit();
}
#endif // sleep

#ifdef KILL_TEST
static int
loopforever(void)
{
  int i = 0;

  while(1) ++i;  // infinite
  exit();  // not reachable

  return i;
}

int
main(int argc, char *argv[])
{
  int pid;

  printf(1, "\n-------Kill Test-------\n");

  printf(1, "Forking process with PID %d.\n", getpid());
  pid = fork();
  if(pid == 0) {
    printf(1, "the child process is going to loop forever now.\n");
    loopforever();
  }
  else {
    sleep(100);
    kill(pid);
    printf(1, "The processes with the pid get killed. Press CTRL+Z to display zombie list and then CTRL+P.\n");
    sleep(SLEEP_TIME);
    printf(1, "It's time to reap the zombie, the zombie list will be empty now.\n\n");
    wait();
  }
  printf(1, "Press CTRL+Z to display zombie list and then CTRL+P again.\n");

  exit();
}
#endif // kill

#ifdef EXIT_TEST
int
main(int argc, char *argv[])
{
  int pid;

  printf(1, "\n-------Exit Test-------\n");

  printf(1, "Forking process with PID %d.\n", getpid());
  pid = fork();
  if(pid == 0) {
    printf(1, "Exiting the child process now.\n");
    exit();
  }
  else {
    sleep(100);
    printf(1, "Press CTRL+Z once to display zombie list.\n");
    sleep(SLEEP_TIME);
    printf(1, "It's time to reap the zombie, the zombie list will be empty now.\n\n");
    wait();
  }
  printf(1, "Press CTRL+Z to display zombie list and then CTRL+P again.\n");

  exit();
}
#endif // exit

#ifdef WAIT_TEST
int
main(int argc, char *argv[])
{
  int pid;

  printf(1, "\n-------Wait Test-------\n");

  printf(1, "Forking process with PID %d..\n", getpid());
  pid = fork();
  if(pid == 0) {
    printf(1, "Press CTRL+F to display UNUSED list and then CTRL+P.\n\n");
    sleep(SLEEP_TIME);

    printf(1, "Killing child process now.\n");
    kill(0);
  }
  else {
    wait();
    printf(1, "\nChild has finished now.\n");
    printf(1, "Press CTRL+F once to display UNUSED list and then CTRL+P.\n\n");
    sleep(SLEEP_TIME);
  }

  printf(1, "Test is done, press CTRL+F and CTRL+P again.\n");
  exit();
}
#endif // wait

#ifdef FORK_TEST  // also for p2 to test
int
main(int argc, char *argv[])
{
  int rc, i = 0;
  int childCount = 9;

  if (argc > 1) {
    childCount = atoi(argv[1]);
  }
  if (!childCount) {
    printf(1, "No children to create, so %s is exiting.\n", argv[0]);
    exit();
  }

  printf(1, "Starting %d child processes that will run forever\n", childCount);

  do {
    rc = fork();
    if (rc < 0) {
      printf(2, "Fork failed!\n");
      exit();
    }
    if (rc == 0) { // child process
      while(1) i++;  // infinite
      exit();  // not reachable.
    }
    childCount--;
  } while(childCount);

  printf(1, "All child processes created\n");
  while(1) i++;  // loop forever and don't call wait. Good for zombie check
  exit();  // not reachable
}
#endif // fork

#endif // CS333_P3
