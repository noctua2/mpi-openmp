#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <mutex>
#include <unistd.h>
#include <limits.h>
#include <fstream>



int main(int argc, char** argv) {

  std::ofstream myfile;
  myfile.open ("example.txt");
  myfile << "Writing this to a file.\n";
  myfile.close();

  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	if (world_rank == 0)
	{
		std::ofstream outfile;
		outfile.open("/data/jaeschke/rank0.txt");
		outfile.close();
		int nthreads, tid;
		int sum = 0;
		int other_sum = 0;
		std::mutex m1;

		char hostname[HOST_NAME_MAX];
                gethostname(hostname, HOST_NAME_MAX);

#pragma omp parallel private(tid) reduction(+:sum)
		{
			tid = omp_get_thread_num();
			m1.lock();
			std::cout << "hello from thread " << tid << " mpi rank 0 on " << hostname << std::endl;
			m1.unlock();
			
			sum += tid;
		}
		MPI_Recv(&other_sum, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		std::cout << "my node is " << hostname << " my sum " << sum << " other sum " << other_sum << std::endl;
		outfile.close();
	}
	else if (world_rank == 1)
	{
		std::ofstream outfile;
                outfile.open("/data/jaeschke/rank1.txt");
                outfile.close();
                int nthreads, tid;
                int sum = 0;
                std::mutex m1;
		int other_sum = 0;

		char hostname[HOST_NAME_MAX];
                gethostname(hostname, HOST_NAME_MAX);


#pragma	omp parallel private(tid) reduction(+:sum)
               	{
                        tid = omp_get_thread_num();
//			m1.lock();
			std::cout << "hello from thread " << tid << " mpi rank 1 on " << hostname << std::endl;
//			m1.lock();
                        sum += tid;
               	}
                MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		//sleep(1000);
		std::cout << hostname << std::endl; 
		outfile.close();
	}
	MPI_Finalize();
}
