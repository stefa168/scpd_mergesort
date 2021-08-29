#include "pingpong.h"
#include "mychrono.cpp"

using std::cout;
using std::endl;

int main(int argc, char **argv){
  int myid;
  int num_procs;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  Mychrono ch;
  double elapsed_time;
  int len = 1;
  int x[len];

  ch.start_chrono();
  if(myid == 0) {
    MPI_Send(x, len, MPI_INT, 1, 0, MPI_COMM_WORLD);
    MPI_Recv(x, len, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  } else {
    MPI_Recv(x, len, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(x, len, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  ch.end_chrono();

  // This time is divided by two to obtain an estimate of the time of one-way communication
  elapsed_time = 0.5 * ch.get_diff();
  cout << myid << ": Elapsed time = " << elapsed_time << " seconds\n" << endl;

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}
