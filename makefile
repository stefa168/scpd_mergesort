
CPP=g++
CFLAGS = -O3 -std=c++11
MPICPP = mpic++
MPIRUN = mpirun

################################################
build/data_generation.o:
	$(CPP) $(CFLAGS) $(DEFMACRO) -c common/data_generation.cpp -o build/data_generation.o

build/merge_implementations.o:
	$(CPP) $(CFLAGS) $(DEFMACRO) -c common/merge_implementations.cpp -o build/merge_implementations.o

################################################
build/serial.o:
	$(CPP) $(CFLAGS) $(DEFMACRO) -c serial_impl/serial.cpp -o build/serial.o

bin/serial: build/serial.o build/data_generation.o build/merge_implementations.o
	$(CPP) -o $@ $^

################################################
build/pthread_divetimpera.o:
	$(CPP) $(CFLAGS) $(DEFMACRO) -pthread -c pthread_impl/pthread_divetimpera.cpp -o build/pthread_divetimpera.o

bin/pthread_divetimpera: build/pthread_divetimpera.o build/data_generation.o build/merge_implementations.o
	$(CPP) -pthread -o $@ $^

################################################
build/pthread_partitioning.o:
	$(CPP) $(CFLAGS) $(DEFMACRO) -pthread -c pthread_impl/pthread_partitioning.cpp -o build/pthread_partitioning.o

bin/pthread_partitioning: build/pthread_partitioning.o build/data_generation.o build/merge_implementations.o
	$(CPP) -pthread -o $@ $^


################################################
build/mpi_divetimpera.o:
	$(MPICPP) $(CFLAGS) $(DEFMACRO) -c mpi_impl/mpi_divetimpera.cpp -o build/mpi_divetimpera.o

bin/mpi_divetimpera: build/mpi_divetimpera.o build/data_generation.o build/merge_implementations.o
	$(MPICPP) -o $@ $^

################################################
build/mpi_partitioning.o:
	$(MPICPP) $(CFLAGS) $(DEFMACRO) -c mpi_impl/mpi_partitioning.cpp -o build/mpi_partitioning.o

bin/mpi_partitioning: build/mpi_partitioning.o build/data_generation.o build/merge_implementations.o
	$(MPICPP) -o $@ $^

################################################
clean:
	rm -f build/* bin/*

serial: bin/serial
	bin/serial $(LEN) $(SEED)

pthread_divetimpera: bin/pthread_divetimpera
	bin/pthread_divetimpera $(LEN) $(SEED)

# WARN: arguments in bin/pthread_partitioning are positional, so
# if it defined SEED but not NCO, the code use value of SEED like NCO
# e.g. make pthread_partitioning LEN=1024 SEED=88
# we will have 88 threads and a random seed
pthread_partitioning: bin/pthread_partitioning
	bin/pthread_partitioning $(LEN) $(NCO) $(SEED)

mpi_divetimpera: bin/mpi_divetimpera
	$(MPIRUN) -np $(NCO) ./bin/mpi_divetimpera $(LEN) $(SEED)

mpi_partitioning: bin/mpi_partitioning
	$(MPIRUN) -np $(NCO) ./bin/mpi_partitioning $(LEN) $(SEED)

help:
	@echo "Usage: make [options] [args] [other flags]"
	@echo -e "Options: "
	@echo -e "  serial\t\texecute sequential mergesort."
	@echo -e "  pthread_divetimpera\texecute pthread mergesort with paradigm divide et impera."
	@echo -e "  pthread_partitioning\texecute pthread mergesort with paradigm partitioning. "
	@echo -e "  mpi_divetimpera\t\texecute mpi mergesort with paradigm divide et impera. "
	@echo -e "  mpi_partitioning\t\texecute mpi mergesort with paradigm partitioning. "
	@echo ""
	@echo -e "Args can be"
	@echo -e "  LEN: length of the array"
	@echo -e "  NCO: Number of COncorrent -> number of processes/threads (use with mpi and pthread implementions)"
	@echo -e "  SEED: it is an optional argument and sets the seed of the random generator"
	@echo ""
	@echo -e "Other flags can be"
	@echo -e "  DEFMACRO: to define a macro (e.g. make serial LEN=1024 DEFMACRO=\"-D THEWALL\")"
	@echo -e "    Warnings:"
	@echo -e "      - use with caution because macros are applied to all builds of the dependency. Make sure they are not named on different files to avoid unwanted output "
	@echo -e "      - remember to do \"make clean\" first"
	@echo ""
