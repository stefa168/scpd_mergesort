#include "pingpong.h"
#include "mychrono.cpp"

using std::cout;
using std::endl;


class MyData{
public:
  Mychrono ch;
  int mitt;
  int dest;
};


void my_communication(char *text, int myid, int *x, int len, MyData *array, int num_procs){
  double elapsed_time;

  for(int i = 0; i < num_procs; i++){
    array[i].ch.start_chrono();
    if(myid == array[i].mitt) {
      MPI_Send(x, len, MPI_INT, array[i].dest, 0, MPI_COMM_WORLD);
      MPI_Recv(x, len, MPI_INT, array[i].dest, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else if(myid == array[i].dest){
      MPI_Recv(x, len, MPI_INT, array[i].mitt, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Send(x, len, MPI_INT, array[i].mitt, 0, MPI_COMM_WORLD);
    }
    array[i].ch.end_chrono();
    elapsed_time = 0.5 * array[i].ch.get_diff();
    if(myid == array[i].mitt) {
      printf("%s between %d and %d is %f seconds\n", text, array[i].mitt, array[i].dest, elapsed_time);
    }

    MPI_Barrier( MPI_COMM_WORLD );
  }
  if(myid == 0) printf("\n");
}

void init_array(int myid, MyData *array, int num_procs){
  int k = 0;
  for(int i = 0; k < num_procs; i++){
    for(int j = i + 1; j < num_procs; j++){
      array[k].mitt = i;
      array[k].dest = j;
      k++;
    }
  }

}

int main(int argc, char **argv){
  int myid;
  int num_procs, namelen;

  char processor_name[MPI_MAX_PROCESSOR_NAME];

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  MPI_Get_processor_name(processor_name, &namelen);

  MyData latency[num_procs];
  init_array(myid, latency, num_procs);

  MyData bandwidth[num_procs];
  init_array(myid, bandwidth, num_procs);

  printf("I am process %d on processor %s\n", myid, processor_name);

  MPI_Barrier( MPI_COMM_WORLD );
  if(myid == 0) printf("\n");

  // latency
  my_communication("Time of one-way communication", myid, NULL, 0, latency, num_procs);

  int mb_in_byte = 1048576;
  int len = mb_in_byte / sizeof(int);
  int x[len];
  printf("On %s sizeof(int) is: %d, so array of 1 MB has %d elements\n", processor_name, sizeof(int), len);

  MPI_Barrier( MPI_COMM_WORLD );
  if(myid == 0) printf("\n");

  // bandwidth
  my_communication("Time to send 1 MB", myid, x, len, bandwidth, num_procs);


/*
  ch.start_chrono();
  if(myid == 0) {
    MPI_Send(x, len, MPI_INT, 1, 0, MPI_COMM_WORLD);
    MPI_Recv(x, len, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  } else {
    MPI_Recv(x, len, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(x, len, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  ch.end_chrono();
*/

  // This time is divided by two to obtain an estimate of the time of one-way communication

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}
