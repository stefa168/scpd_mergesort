
CPP = g++
CFLAGS = -O3 -std=c++11
MPICPP = mpic++
MPIRUN = mpirun

# DEFMACRO can be used to insert anything, not just the macro definition

################################################
build/mychrono.o:
	@echo "Building mychrono.o ..."
	@$(CPP) $(CFLAGS) $(DEFMACRO) -c common/mychrono.cpp -o build/mychrono.o

################################################
build/data_generation.o:
	@echo "Building data_generation.o ..."
	@$(CPP) $(CFLAGS) $(DEFMACRO) -c common/data_generation.cpp -o build/data_generation.o

build/merge_implementations.o:
	@echo "Building merge_implementations.o ..."
	@$(CPP) $(CFLAGS) $(DEFMACRO) -c common/merge_implementations.cpp -o build/merge_implementations.o

################################################
build/serial.o:
	@echo "Building serial.o ..."
	@$(CPP) $(CFLAGS) $(DEFMACRO) -c serial_impl/serial.cpp -o build/serial.o

bin/serial: build/serial.o build/data_generation.o build/merge_implementations.o build/mychrono.o
	@echo "Creating of the serial executable..."
	@$(CPP) -o $@ $^


################################################
build/pthread_divetimpera.o:
	@echo "Building pthread_divetimpera.o ..."
	@$(CPP) $(CFLAGS) $(DEFMACRO) -pthread -c pthread_impl/pthread_divetimpera.cpp -o build/pthread_divetimpera.o

bin/pthread_divetimpera: build/pthread_divetimpera.o build/data_generation.o build/merge_implementations.o build/mychrono.o
	@echo "Creating of the pthread_divetimpera executable..."
	@$(CPP) -pthread -o $@ $^

################################################
build/pthread_divetimpera_uncontrolled.o:
	@echo "Building pthread_divetimpera_uncontrolled.o ..."
	@$(CPP) $(CFLAGS) $(DEFMACRO) -pthread -c pthread_impl/pthread_divetimpera_uncontrolled.cpp -o build/pthread_divetimpera_uncontrolled.o

bin/pthread_divetimpera_uncontrolled: build/pthread_divetimpera_uncontrolled.o build/data_generation.o build/merge_implementations.o build/mychrono.o
	@echo "Creating of the pthread_divetimpera_uncontrolled executable..."
	@$(CPP) -pthread -o $@ $^

################################################
build/pthread_partitioning.o:
	@echo "Building pthread_partitioning.o ..."
	@$(CPP) $(CFLAGS) $(DEFMACRO) -pthread -c pthread_impl/pthread_partitioning.cpp -o build/pthread_partitioning.o

bin/pthread_partitioning: build/pthread_partitioning.o build/data_generation.o build/merge_implementations.o build/mychrono.o
	@echo "Creating of the pthread_partitioning executable..."
	@$(CPP) -pthread -o $@ $^


################################################
build/mpi_divetimpera.o:
	@echo "Building mpi_divetimpera.o ..."
	@$(MPICPP) $(CFLAGS) $(DEFMACRO) -c mpi_impl/mpi_divetimpera.cpp -o build/mpi_divetimpera.o

bin/mpi_divetimpera: build/mpi_divetimpera.o build/data_generation.o build/merge_implementations.o build/mychrono.o
	@echo "Creating of the mpi_divetimpera executable..."
	@$(MPICPP) -o $@ $^

################################################
build/mpi_partitioning.o:
	@echo "Building mpi_partitioning.o ..."
	@$(MPICPP) $(CFLAGS) $(DEFMACRO) -c mpi_impl/mpi_partitioning.cpp -o build/mpi_partitioning.o

bin/mpi_partitioning: build/mpi_partitioning.o build/data_generation.o build/merge_implementations.o build/mychrono.o
	@echo "Creating of the mpi_partitioning executable..."
	@$(MPICPP) -o $@ $^

################################################
clean:
	@echo "Cleaning..."
	@rm -f build/* bin/*

env:
	@echo "Creating environment..."
	@mkdir -p bin build

serial: bin/serial
	@echo "Run serial $(LEN) $(SEED)"
	@bin/serial $(LEN) $(SEED)

pthread_divetimpera: bin/pthread_divetimpera
	@echo "Run pthread_divetimpera $(LEN) $(GRAIN) $(NCO) $(SEED)"
	@bin/pthread_divetimpera $(LEN) $(GRAIN) $(NCO) $(SEED)

pthread_divetimpera_uncontrolled: bin/pthread_divetimpera_uncontrolled
	@echo "Run pthread_divetimpera_uncontrolled $(LEN) $(GRAIN) $(SEED)"
	@bin/pthread_divetimpera_uncontrolled $(LEN) $(GRAIN) $(SEED)

pthread_partitioning: bin/pthread_partitioning
	@echo "Run pthread_partitioning $(LEN) $(GRAIN) $(NCO) $(SEED)"
	@bin/pthread_partitioning $(LEN) $(GRAIN) $(NCO) $(SEED)

mpi_divetimpera: bin/mpi_divetimpera
	@echo "Run mpi -np $(NCO) mpi_divetimpera $(LEN) $(GRAIN) $(SEED)"
	@$(MPIRUN) -np $(NCO) ./bin/mpi_divetimpera $(LEN) $(GRAIN) $(SEED)

mpi_partitioning: bin/mpi_partitioning
	@echo "Run mpi -np $(NCO) mpi_partitioning $(LEN) $(GRAIN) $(SEED)"
	@$(MPIRUN) -np $(NCO) ./bin/mpi_partitioning $(LEN) $(GRAIN) $(SEED)

help:
	@echo "Usage: make [options] [args] [other flags]"
	@echo -e "Options: "
	@echo -e "  env\t\t\tcreate the necessary environment."
	@echo -e "  clean\t\t\tclean directory build and bin."
	@echo -e "  serial\t\t\texecute sequential mergesort."
	@echo -e "  pthread_divetimpera\t\texecute pthread mergesort with paradigm divide et impera."
	@echo -e "  pthread_divetimpera_uncontrolled\texecute pthread mergesort with paradigm divide et impera without a limit numbers of threads."
	@echo -e "  pthread_partitioning\t\texecute pthread mergesort with paradigm partitioning. "
	@echo -e "  mpi_divetimpera\t\texecute mpi mergesort with paradigm divide et impera. "
	@echo -e "  mpi_partitioning\t\texecute mpi mergesort with paradigm partitioning. "
	@echo ""
	@echo -e "Args can be"
	@echo -e "  LEN: length of the array"
	@echo -e "  GRAIN: grain of the problem -> (to use with mpi and pthread implementation)"
	@echo -e "  NCO: Number of CONcorrents executions -> number of processes/threads (to use with mpi and pthread implementation)"
	@echo -e "  SEED: it is an optional argument and sets the seed of the random generator"
	@echo ""
	@echo -e "Other flags can be"
	@echo -e "  DEFMACRO: at compile time to define a macro. Command \"make allmacro\" show all MACROs"
	@echo ""
	@echo -e "### WARNINGS ###"
	@echo -e "  - arguments in bin/pthread_partitioning and in bin/pthread_divetimpera are positional, so if it was defined SEED but not NCO the code will use value of SEED like NCO."
	@echo -e "    e.g. make pthread_partitioning LEN=1024 SEED=88 (we will have 88 threads and a random seed)."
	@echo -e "  - use with caution DEFMACRO because macros are applied to all compilations of files on which the executable is dependent. Make sure they are not named on different files to avoid unwanted outputs."


allmacro:
	@echo "All available MACROs: (e.g. make serial LEN=1024 DEFMACRO=\"-D THEWALL\")"
	@echo -e "  PRINT_ARRAY\t\t\t[common] print array before and after the sorting."
	@echo -e "  TWO_PTHREAD\t\t\t[pthread_divetimpera_uncontrolled] create two threads for every execution branch if is defined, one thread otherwise,"
	@echo -e "  CLASSIC_MERGE_PARTITION\t[implementations with partitioning paradigm] use classic merge function if is defined, merge_size function otherwise."
	@echo -e "  FILE\t\t\t\t[common] makes available two methods for reading / writing to files [OBSOLETE]."
	@echo -e "  MERGE_LOCAL_MALLOC_IMPL\t[common] use a merge version that creates two supporting local arrays if is defined, use a second array in the input parameters otherwise [OBSOLTE]."
