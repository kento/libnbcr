#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ibtls.h"
#include "common.h"

static void nbcr_st_init(void);

bint main(int argc, char **argv) 
{
  struct RDMA_communicator comm;
  struct RDMA_request *req1, *req2;
  char *data[2];
  uint64_t size;
  int ctl_tag;
  double ss, ee;

  size = 1024 * 1024 * 1024;

  data[0] = (char*)valloc(size);
  data[1] = (char*)valloc(size);
  
  memset(data[0], 1, size);
  memset(data[1], 1, size);
  RDMA_Passive_Init(&comm);
  int req_num = 2;
  struct RDMA_request req[req_num];
  int req_id = 0;
  RDMA_Irecv(data[req_id], size, NULL, RDMA_ANY_SOURCE, RDMA_ANY_TAG, &comm, &req[req_id]);  
  req_id = (req_id + 1) % req_num;
  fprintf(stderr, "%p: size=%lu: \n", data, size);
  while (1) {
    //RDMA_Recv(data, size, NULL, RDMA_ANY_SOURCE, RDMA_ANY_TAG, &comm);  
    ss = get_dtime();
    //    RDMA_Recv(data, 1, NULL, RDMA_ANY_SOURCE, RDMA_ANY_TAG, &comm);  
    RDMA_Irecv(data[req_id], size, NULL, RDMA_ANY_SOURCE, RDMA_ANY_TAG, &comm, &req[req_id]);  
    req_id = (req_id + 1) % req_num;
    fprintf(stderr, "req_id:%d\n", req_id);
    RDMA_Wait(&req[req_id]);
    ee = get_dtime();
    fprintf(stderr, "Latency: %d\n", ee - ss);

    free(req1);
    
  }
  return 0;
}

static void nbcr_st_init()
{

}

int dump(char* addr, int size) {
  int fd;
  int n_write = 0;
  int n_write_sum = 0;

  printf("dumping...\n");
  fd = open("/home/usr2/11D37048/tmp/test", O_WRONLY | O_CREAT); 
  if (fd <= 0) {
    fprintf(stderr, "error\n");
    exit(1);
  }
  do {
    n_write = write(fd, addr + n_write_sum, size);
    n_write_sum = n_write_sum + n_write;
    if (n_write_sum >= size) break;
  }  while(n_write > 0);
  close(fd);
  printf("dumping...ends\n");
}
