//  example.c
//  Created by Mary Nwosu and Melanie White on 12/7/22.

#include <semaphore.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char **argv)
{
  int fd, i,nloop=10,zero=0,*counter_ptr;
  sem_t *mutex;

  //open a file and map it into memory this is to hold the shared counter

  fd = open("log.txt",O_RDWR|O_CREAT,S_IRWXU);
  write(fd,&zero,sizeof(int));
  counter_ptr = mmap(NULL,sizeof(int),PROT_READ |PROT_WRITE,MAP_SHARED,fd,0);
  close(fd);

  /* create, initialize semaphore */
 if ((mutex = sem_open("examplesemaphore", O_CREAT, 0644, 1)) == SEM_FAILED) {
    perror("semaphore initilization");
    exit(1);
  }
 
  if (fork() == 0) { /* child process*/
    for (i = 0; i < nloop; i++) {
      printf("child wanting to enter crititical section\n");
      sem_wait(mutex);
      printf("child entered crititical section: %d\n", (*counter_ptr)++);
      sleep(2);
      printf("child leaving critical section\n");
      sem_post(mutex);
      sleep(1);
    }
    exit(0);
  }
  /* back to parent process */
  for (i = 0; i < nloop; i++) {
    printf("parent wanting to enter crititical section\n");
    sem_wait(mutex);
    printf("parent entered critical section: %d\n", (*counter_ptr)++);
    sleep(2);
    printf("parent leaving critical section\n");
    sem_post(mutex);
    sleep(1);
  }
  exit(0);
}

