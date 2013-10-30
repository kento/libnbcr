#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "nbcr_util.h"
#include "nbcr.h"


nbcr_comm* nbcr_init(void* data, size_t size, int id)
{
  nbcr_comm *comm;
  void* region;

  comm = (nbcr_comm*)malloc(sizeof(nbcr_comm));
  comm->on_ckpt = 0;
  comm->data = data;
  //  comm->size = size;
  RDMA_Active_Init(&(comm->comm), &(comm->param));
  region =  RDMA_Alloc(sizeof(nbcr_perfc) + sizeof(nbcr_ckpt_info) + size);
  comm->ckpt =  region + sizeof(nbcr_ckpt_info) + sizeof(nbcr_perfc);

  /* ===============================================
   * Checkpoint information initialization
   */
  comm->ckpt_info = region;
  comm->ckpt_info->size = size;
  comm->ckpt_info->id = id;
  comm->ckpt_info->gen = -1;
  //  comm->ckpt_info = (nbcr_ckpt_info*)RDMA_Alloc(sizeof(nbcr_ckpt_info));
  //  comm->ckpt_info->size = size;
  //  comm->ckpt_info->id = id;
  //  comm->ckpt_info->gen = -1;
  //  RDMA_Isend(comm->ckpt_info, sizeof(nbcr_ckpt_info), NULL, -1, NBCR_CKPT_INFO_TAG, &(comm->comm), &(comm->ckpt_info_req));
  /* ===============================================*/


  /* ===============================================
   * Performabce counter initialization
   */
  comm->perfc = region + sizeof(nbcr_ckpt_info);
  comm->perfc->gen   = 1;
  comm->perfc->ts   = 1;
  comm->perfc->ts_nn   = 0;
  comm->perfc->tp_nn   = 10000;
  //  comm->perfc = (nbcr_perfc*)RDMA_Alloc(sizeof(nbcr_perfc));
  //  comm->perfc->gen   = -1;
  //  comm->perfc->ts   = -1;
  //  RDMA_Isend(comm->perfc, sizeof(nbcr_perfc), NULL, -1, NBCR_PERFC_TAG, &(comm->comm), &(comm->perfc_req));
  /* ===============================================*/


  return comm;
}

int nbcr_nn_start(nbcr_comm *comm)
{
  comm->perfc->ts_nn = get_dtime();
  return 0;
}

int nbcr_nn_end(nbcr_comm *comm)
{
  double ts, tp;
  ts = get_dtime();
  tp = ts - comm->perfc->ts_nn;

  if (comm->perfc->tp_nn > tp) {
    comm->perfc->tp_nn = tp;
  }
  return 0;
}

int nbcr_checkpoint(nbcr_comm *comm, unsigned long gen)
{
  void* ckpt;
  int checkpoint = 0;
  double ts;

  comm->perfc->gen = gen;
  ts = get_dtime();
  comm->perfc->tp = ts - comm->perfc->ts;
  comm->perfc->ts  = ts;
  if (comm->on_ckpt) {
    int status;
    status = RDMA_Trywait(&(comm->req));
    if (status != 0) {
      checkpoint = 2;
      fprintf(stderr, "Checkpoint start\n");
    } 
    checkpoint = 0;
    //fprintf(stderr, "Checkpoint still is on going\n");
  } else {
    checkpoint = 1;
    fprintf(stderr, "First checkpoint start\n");
  }
  if (checkpoint > 0) {
    comm->ckpt_info->gen = gen;
    //memcpy(comm->ckpt, comm->data, comm->ckpt_info->size);
    RDMA_Isend(comm->ckpt_info, sizeof(nbcr_perfc) + sizeof(nbcr_ckpt_info) + comm->ckpt_info->size, NULL, -1, NBCR_CKPT_TAG, &(comm->comm), &(comm->req));
    comm->on_ckpt = 1;
  }
  return checkpoint;  
}

void nbcr_finalize(nbcr_comm *comm)
{
  RDMA_Wait(&(comm->req));
  /*TODO: Find out a way to complete two of below RDMA_Wait()s*/
  //  RDMA_Wait(&(comm->perfc_req));
  //  RDMA_Wait(&(comm->ckpt_info_req));
  /*===========================*/
  RDMA_Free(comm->ckpt);
  //  RDMA_Free(comm->perfc);
  //  RDMA_Free(comm->ckpt_info);
  free(comm);
  return;
}
