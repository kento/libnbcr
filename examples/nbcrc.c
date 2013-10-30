/*
#include <sys/time.h>
#include <stdio.h>

#include <unistd.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
*/

#include <stdio.h>
#include <stdlib.h>

#include "nbcr.h"

#ifndef ID
#define ID 1
#endif

int main(int argc, char **argv)
{
  int i = 0;
  int size;
  char* data;
  nbcr_comm *comm;
  int status;

  if (argc < 2) {
    printf("./nbcrc <send size(Bytes)>\n");
    exit(1);
  }
  
  size = atoi(argv[1]);
  data = (char*)valloc(size);

  comm = nbcr_init(data, size, &size);  
  //comm = nbcr_init(data, size);  
  for (i = 0; i < 1000000; i++) {
    usleep(1000000);
    status = nbcr_checkpoint(comm, i);
    //    fprintf(stderr, "%d\n", status);
  }
  nbcr_finalize(comm);
  return 0;
}
