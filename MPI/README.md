
## Introduction to Parallel Computing (CMSC416)

#### Assignment 1: MPI


The purpose of this programming assignment is to gain experience in
parallel programming on a cluster and MPI. For this assignment, you have
to write a parallel implementation of a program to simulate the [Game of
Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life).

##### Serial Algorithm

The game of life simulates simple cellular automata. The game is played
on a rectangular board containing cells. At the start, some of the cells
are occupied, the rest are empty. The game consists of constructing
successive generations of the board. The rules for constructing the next
generation from the previous one are:

1.  death: cells with 0,1,4,5,6,7, or 8 neighbors die (0,1 of loneliness
    and 4-8 of over population)
2.  survival: cells with 2 or 3 neighbors survive to the next
    generation.
3.  birth: an unoccupied cell with 3 neighbors becomes occupied in the
    next generation.

For this assignment, the game board has finite size. The x-axis starts
at 0 and ends at X_limit-1 (supplied on the command line). Likewise, the
y-axis start at 0 and ends at Y_limit-1 (supplied on the command line).

You can use the provided [serial
code](https://github.com/CMSC416/CMSC416/blob/04ebbad5e409310b1a5f09f5201b1c8f1418cd72/assignment-1/serial.C)
as a baseline to develop your parallel implementation. It provides some
basic functionality such as parsing the input file and exporting the
final board state to a CSV file. Your task is to implement the parallel
version using C, C++, or Fortran, and MPI.

##### Input/Initialization

Your program should read in a file containing the coordinates of the
initial cells. Sample files are located here:
[life.1.256x256.data](https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign1/data/life.1.256x256.data)
and
[life.2.256x256.data](https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign1/data/life.2.256x256.data)
(256x256 board). Each line in this file represents the coordinates of a
cell on the board that is live. For instance, the following entry:

     
              1,3
              

means that the cell at position \[1, 3\] is live in the initial state.
You can also find many other sample patterns on the web (use your
favorite search engine on "game of life" and/or "Conway").

Your program should take four command line arguments: the name of the
data file, the number of generations to iterate, X_limit, and Y_limit.
To be more specific, the command line of your program should be:

              ./life <input file name> <# of generations> <X_limit> <Y_limit>
              

The number of processes the program will run on is specified as part of
the mpirun command with the -np argument.

              mpirun -np <number of processes> ./life <input file name> <# of generations> <X_limit> <Y_limit>
              

##### Output

Your program should write a single file called
`<input-file-name>.<no-of-generations>.csv` (from one designated rank)
that contains comma separated values representing the board. There
should be one line (containing the x coordinate, a comma, and then the y
coordinate) for each occupied cell at the end of the last iteration.
Sample output files are available:

-   [life.1.256x256.100.csv](https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign1/data/life.1.256x256.100.csv)
    is the output of the file
    [life.1.256x256.data](https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign1/data/life.1.256x256.data)
    run for 100 generations on a 256x256 board
-   [life.2.256x256.100.csv](https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign1/data/life.2.256x256.100.csv)
    is the output of the file
    [life.2.256x256.data](https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign1/data/life.2.256x256.data)
    run for 100 generations on a 256x256 board

Similar to the output files above, your output in the file should be
sorted by the X and Y coordinates.

The only print from your program to standard output should be from
process 0 that looks like this:

            TIME: Min: 25.389 s Avg: 27.452 s Max: 41.672 s
            

where Min, Avg and Max time (in seconds) are calculated using MPI
reduction operations over the individual time measurements of the "main"
loop (over generations) on different processes for the sample
[life.512x512.data](https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign1/data/life.512x512.data)
input file.

##### Parallel Version

Figure out how you will decompose the problem for parallel execution.
Remember that MPI (at least the OpenMPI implementation) does not always
have great communication performance and so you will want to make
message passing infrequent. Also, you will need to be concerned about
static load balancing during data distribution/domain decomposition. You
should use the 1D decomposition (over rows) and implement two different
versions: one using blocking Send/Recv and other using non-blocking
Isend/Irecv.

You can assume that X_limit and Y_limit will be powers of 2 as will be
the number of processes you will be running on. You can also assume that
you will be running the program on a minimum of 4 processes and X_limit
and Y_limit are much larger than the number of processes.

When you need to distribute the initial state of the board to different
processes, you can read the entire file on one process (say rank 0) and
then send messages from rank 0 to everyone else.

##### What to Submit

You must submit the following files and no other files:

-   `life-blocking.[c,C,f77,f90]`: parallel version using blocking
    Send/Recv routines, where the file extension depends on the language
    used for the implementation
-   `life-nonblocking.[c,C,f77,f90]`: parallel version using
    non-blocking Isend/Irecv routines
-   `Makefile` that will compile your code successfully on zaratan when
    using mpicc or mpicxx. You can see a sample Makefile
    [here](https://github.com/CMSC416/CMSC416/blob/04ebbad5e409310b1a5f09f5201b1c8f1418cd72/assignment-1/Makefile).
    Make sure that the executable names are life-blocking and
    life-nonblocking, and do not include the executables in the tarball.
-   You must also submit a short report (pdf) with performance results
    (two line plots). The line plots should present the execution times
    to run the two parallel versions respectively on the input file
    [life.512x512.data](https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign1/data/life.512x512.data)
    (for 4, 8, 16, 32, 64, and 128 processes), running on a 512x512
    board for 500 iterations. In total, you will be running each program
    6 times. In the report, you should mention:
    -   how was the initial data distribution done
    -   how did you ensure that the blocking version does not deadlock
    -   what are the performance results, and are they what you expected

You should put the code, Makefile and report in a single directory
(named `LastName-FirstName-assign1`), compress it to .tar.gz
(`LastName-FirstName-assign1.tar.gz`) and upload that to
[gradescope](https://www.gradescope.com/courses/448329).

If you want to try a bigger board, to see if you can get better speedups
with more processes, try running on the input file
[life.1024x1024.data](https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign1/data/life.1024x1024.data).

##### Tips

-   [Zaratan
    primer](https://www.cs.umd.edu/class/fall2023/cmsc416/zaratan.shtml)
-   Use the compiler flag -g while debugging but -O2 when collecting
    performance numbers.
-   MPI_Wtime
    [example](https://www.open-mpi.org/doc/v3.0/man3/MPI_Wtime.3.php)
-   Use the --exclusive flag with sbatch when collecting execution
    times.

##### Grading

The project will be graded as follows:

| Component                        | Percentage                |
|----------------------------------|---------------------------|
| Runs correctly with 4 processes  | 30 (15% each version)     |
| Runs correctly with 16 processes | 40 (20% each version)     |
| Speedup with 4 processes         | 10 (non-blocking version) |
| Speedup with 16 processes        | 10 (non-blocking version) |
| Writeup                          | 10                        |

NOTE: If your program does not compile when submitted on gradescope, you
get 0 points. If your program does not run correctly, you do NOT get any
points for performance/speedup.

The speedup will be computed using the provided serial code as the
baseline. You will get full points as long as your speedup is within 2
times (2x) of the expected speedup. For performance worse than 2x of the
expected speedup, you will receive credit proportional to your speedup.

</div>

</div>

</div>

</div>

<div class="container">

<div class="row">

<div class="col-md-12" style="padding-top: 24px; text-align: center;">

[Web Accessibility](https://www.umd.edu/web-accessibility)

</div>

</div>

</div>

<div class="grammarly-desktop-integration"
aria-label="grammarly-integration" role="group" tabindex="-1"
data-content="{&quot;mode&quot;:&quot;full&quot;,&quot;isActive&quot;:true,&quot;isUserDisabled&quot;:false}">

</div>
