#include "nbcr_config.h"
#include "ibtls.h"


#define NBCR_CKPT_TAG 10
#define NBCR_PERFC_TAG 20
#define NBCR_CKPT_INFO_TAG 30

struct __nbcr_perfc {
  /*Generation*/
  unsigned long gen;
  /*Time stampe at the end of the main loop (by gettimeofday())*/
  double ts;
  /*Time period of the main loap (by gettimeofday())*/
  double tp;
  /*Time stampe to satrt a non-network phase*/
  double ts_nn;
  /*Minimal time period of a non-network phase*/
  double tp_nn;  

};
typedef struct __nbcr_perfc nbcr_perfc;


struct __nbcr_ckpt_info {
  /*
   * To distinguish which checkpoints data comes form which processes.
   * int id is used for a file of checkpoint. "nbcr.<generation>.<id>"
   * An ID number, like MPI_rank, is supposed to be set
   */
  int id; 
  /*
    Checkpoint generaation is being checkpointed
   */
  unsigned long gen;

  /*Checkpoint size*/
  size_t size;
};
typedef struct __nbcr_ckpt_info nbcr_ckpt_info;

struct __nbcr_comm {
  struct RDMA_communicator comm;
  struct RDMA_param param;
  struct RDMA_request req;

  /*Checkpoint region address*/
  void* ckpt;

  /*Oridinal data address to be checkpointed*/
  void* data;

  int on_ckpt;

  //struct RDMA_request perfc_req;
  nbcr_perfc *perfc;

  //  struct RDMA_request ckpt_info_req;
  nbcr_ckpt_info *ckpt_info;

};
typedef struct __nbcr_comm nbcr_comm;

nbcr_comm* nbcr_init(void* data, size_t size, int id);

int nbcr_checkpoint(nbcr_comm *comm, unsigned long gen);

void nbcr_finalize(nbcr_comm *comm);

int nbcr_nn_start(nbcr_comm *comm);

int nbcr_nn_end(nbcr_comm *comm);
