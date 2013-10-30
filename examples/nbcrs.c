#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "ibtls.h"
#include "common.h"

#include "nbcr.h"

int main(int argc, char **argv) {
  struct RDMA_communicator comm;
  struct RDMA_request *req1, *req2;
  nbcr_perfc *perfc;
  nbcr_ckpt_info *ckpt_info;

  void *data;
  uint64_t chunk, offset = 0;
  int ctl_tag;
  double ss, ee;
  int max;

  chunk = 1024 * 1024 * 1;
  max   = 1024 * 1024 * 1024;

  RDMA_Passive_Init(&comm);

  //  data = (char*)valloc(max);
  //  memset(data, 1, max);
  struct RDMA_request perfc_req, ckpt_info_req, ckpt_req;

  perfc = (nbcr_perfc*)RDMA_Alloc(sizeof(nbcr_perfc));
  ckpt_info = (nbcr_ckpt_info*)RDMA_Alloc(sizeof(nbcr_ckpt_info));
  memset(perfc, 1, sizeof(perfc));

  // for (s = max; s >= min; s = s / 2) {
  sleep(1);
  ss = get_dtime();
  while (1) {
    unsigned int total_ckpt_size;
    unsigned int ckpt_size;
    uint64_t chunk;
    uint64_t sum_chunk = 0;
    double tp;
    int a = 0;
    int read_times = 0;

    RDMA_Irecv_silent_offset(ckpt_info, 0, sizeof(nbcr_ckpt_info), NULL, RDMA_ANY_SOURCE, NBCR_CKPT_TAG, &comm, &ckpt_info_req);  
    RDMA_Wait(&ckpt_info_req);
    fprintf(stderr, "id:%d gen:%lu size:%lu\n", ckpt_info->id, ckpt_info->gen, ckpt_info->size);
    total_ckpt_size = ckpt_info->size;
    data = RDMA_Alloc(total_ckpt_size);  
    for (ckpt_size = 0; ckpt_size <= total_ckpt_size;) {
      RDMA_Irecv_silent_offset(perfc, sizeof(nbcr_ckpt_info), sizeof(nbcr_perfc), NULL, RDMA_ANY_SOURCE, NBCR_CKPT_TAG, &comm, &perfc_req);  
      RDMA_Wait(&perfc_req);
      fprintf(stderr, "%f: gen:%lu ts:%f(%f) ts_nn:%f(%f)\n", get_dtime(), perfc->gen, perfc->ts, perfc->tp, perfc->ts_nn, perfc->tp_nn);
      if (perfc->gen == 99) {
	exit(1);
      }
      tp = perfc->tp_nn - (get_dtime() - perfc->ts); //0.01 is for safty
      //      fprintf(stderr, "Time period: %f\n", tp);
      if (tp < 0.0001) {
	double slp =  (perfc->ts + perfc->tp - get_dtime());

	if (slp > 0 && slp < 10) {
	  //	  fprintf(stderr, "Sleep: %f (a:%d)\n", slp, ++a);
	  usleep(slp * 1000000);
	} else {
	  //  fprintf(stderr, "Try:\n",  ++a);
	  usleep(30000);
	}
	continue;
      }
      chunk = (tp * 3125038407.48457) * 0.6;
      if (chunk < 0) {
	chunk = 1;
      }
      //      fprintf(stderr, "chunk: %d\n", chunk);
      if (ckpt_size + chunk > total_ckpt_size) {
	chunk = total_ckpt_size - ckpt_size;
      }
      //      usleep(tp * 1000000);
      //      continue;
      RDMA_Irecv_silent_offset(data, ckpt_size, chunk, NULL, RDMA_ANY_SOURCE, NBCR_CKPT_TAG, &comm, &ckpt_req);  
      //      RDMA_Irecv_silent(data, chunk, NULL, RDMA_ANY_SOURCE, NBCR_CKPT_TAG, &comm, &ckpt_req);  
      RDMA_Wait(&ckpt_req);
      read_times++;
      sum_chunk += chunk;
      fprintf(stderr, "sum_chunk(times:%d): %lu(chunk:%d)\n", read_times, sum_chunk, chunk);
      //      ckpt_size += chunk;
      double b = (tp - ((double)chunk/3125038407.48457)) * 1000000;
      if (b > 0) {
	usleep(b);
      }
      fprintf(stderr, "Sleep b: %f\n", b);
      b = (perfc->ts + perfc->tp - get_dtime()) * 1000000;// + 30000;
      if (b > 0 && b < 10000000) {
	fprintf(stderr, "Sleep a:%f\n", b);
	usleep(b);
      }

    }
    //    RDMA_Free(data);
  }
  ee = get_dtime();
  sleep(1);
  //  fprintf(stderr, "[%c] Size: %d [bytes], Latency: %f, Throughput: %f [Gbytes/sec]\n", data[max-chunk], max, ee- ss, (max/1000000000.0)/(ee - ss));

    //  dump(data, max);
  return 0;
}

int dump(char* addr, int size) {
  int fd;
  int n_write = 0;
  int n_write_sum = 0;

  printf("dumping...\n");
  fd = open("/home/usr2/11D37048/tmp/test", O_WRONLY | O_CREAT, 0777); 
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
  return 0;
}
