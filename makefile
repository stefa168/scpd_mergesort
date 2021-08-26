
CPP = g++
CFLAGS = -O3 -std=c++11
MPICPP = mpic++
MPIRUN = mpirun

# DEFMACRO can be used to insert anything, not just the macro definition

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
build/pthread_divetimpera_uncontrolled.o:
	$(CPP) $(CFLAGS) $(DEFMACRO) -pthread -c pthread_impl/pthread_divetimpera_uncontrolled.cpp -o build/pthread_divetimpera_uncontrolled.o

bin/pthread_divetimpera_uncontrolled: build/pthread_divetimpera_uncontrolled.o build/data_generation.o build/merge_implementations.o
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
	bin/pthread_divetimpera $(LEN) $(NCO) $(SEED)

pthread_divetimpera_uncontrolled: bin/pthread_divetimpera_uncontrolled
	bin/pthread_divetimpera_uncontrolled $(LEN) $(SEED)

pthread_partitioning: bin/pthread_partitioning
	bin/pthread_partitioning $(LEN) $(NCO) $(SEED)

mpi_divetimpera: bin/mpi_divetimpera
	$(MPIRUN) -np $(NCO) ./bin/mpi_divetimpera $(LEN) $(SEED)

mpi_partitioning: bin/mpi_partitioning
	$(MPIRUN) -np $(NCO) ./bin/mpi_partitioning $(LEN) $(SEED)

help:
	@echo "Usage: make [options] [args] [other flags]"
	@echo -e "Options: "
	@echo -e "  serial\t\t\texecute sequential mergesort."
	@echo -e "  pthread_divetimpera\t\texecute pthread mergesort with paradigm divide et impera."
	@echo -e "  pthread_divetimpera_uncontrolled\texecute pthread mergesort with paradigm divide et impera without a limit numbers of threads."
	@echo -e "  pthread_partitioning\t\texecute pthread mergesort with paradigm partitioning. "
	@echo -e "  mpi_divetimpera\t\texecute mpi mergesort with paradigm divide et impera. "
	@echo -e "  mpi_partitioning\t\texecute mpi mergesort with paradigm partitioning. "
	@echo ""
	@echo -e "Args can be"
	@echo -e "  LEN: length of the array"
	@echo -e "  NCO: Number of CONcorrents executions -> number of processes/threads (to use with mpi and pthread implementation)"
	@echo -e "  SEED: it is an optional argument and sets the seed of the random generator"
	@echo ""
	@echo -e "Other flags can be"
	@echo -e "  DEFMACRO: at compile time to define a macro (e.g. make serial LEN=1024 DEFMACRO=\"-D THEWALL\")"
	@echo ""
	@echo -e "### WARNINGS ###"
	@echo -e "- arguments in bin/pthread_partitioning and in bin/pthread_divetimpera are positional, so if it was defined SEED but not NCO the code will use value of SEED like NCO."
	@echo -e "  e.g. make pthread_partitioning LEN=1024 SEED=88 (we will have 88 threads and a random seed)."
	@echo -e "- use with caution DEFMACRO because macros are applied to all compilations of files on which the executable is dependent. Make sure they are not named on different files to avoid unwanted outputs."
