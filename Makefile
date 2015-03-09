
EXECUTABLES = ringrun jacorun
COMPILER = mpicc
FLAGS = -O3 -Wall

all: $(EXECUTABLES)

int_ring: int_ring.c
	$(COMPILER) $(FLAGS) int_ring.c -o int_ring

ringrun: int_ring
	mpirun -np 4 ./int_ring 4

jacobi-mpi: jacobi-mpi.c
	$(COMPILER) $(FLAGS) jacobi-mpi.c -o jacobi-mpi

jacorun: jacobi-mpi
	mpirun -np 10 ./jacobi-mpi 100 10

clean:
	rm -rf $(EXECUTABLES)
