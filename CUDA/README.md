
## Introduction to Parallel Computing (CMSC416)

#### Assignment 4/5: CUDA

The purpose of this programming assignment is to gain experience in
writing GPU kernels. This assignment is split into two different
problems. The [Game of
Life](https://www.cs.umd.edu/class/fall2023/cmsc416/assignment4.shtml#416)
implementation is the mandatory Assignment 4. Applying [image effects to
videos](https://www.cs.umd.edu/class/fall2023/cmsc416/assignment4.shtml#818x)
is the optional extra-credit Assignment 5.



#### Game of Life

##### Serial Algorithm

The [game of
life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) simulates
simple cellular automata. The game is played on a rectangular board
containing cells. At the start, some of the cells are occupied, the rest
are empty. The game consists of constructing successive generations of
the board. The rules for constructing the next generation from the
previous one are:

1.  death: cells with 0,1,4,5,6,7, or 8 neighbors die (0,1 of loneliness
    and 4-8 of over population)
2.  survival: cells with 2 or 3 neighbors survive to the next
    generation.
3.  birth: an unoccupied cell with 3 neighbors becomes occupied in the
    next generation.

For this assignment, the game board has finite size. The x-axis starts
at 0 and ends at X_limit-1 (supplied on the command line). Likewise, the
y-axis starts at 0 and ends at Y_limit-1 (supplied on the command line).

##### GPU Algorithm

You are provided with [starter
code](https://github.com/CMSC416/CMSC416/blob/13109134f9dc6293519b290a47e9775000b78715/assignment-4/game-of-life/game-of-life.cu)
to handle reading/writing data and copying it between the CPU and GPU.
Your job is to write two GPU kernels: *padded_matrix_copy*, which copies
the current life array into a previous_life array, and *compute_on_gpu*,
which computes a single iteration of Game of Life. The starter code
contains the function *compute*, which is a serial CPU implementation of
what you are going to implement.

The GPU kernels are structured as follows:

              __global__ void padded_matrix_copy(int *dst, int *src, int width, int height, int padding)

              __global__ void compute_on_gpu(int *life, int *previous_life, int X_limit, int Y_limit)
            

*life* is the board you are writing into, *previous_life* are the values
of the board at the previous iteration, and *X_limit* & *Y_limit* are
the board size. *padded_matrix_copy* and *compute_on_gpu* will be called
with block size 8x8 and grid size provided on the command line. You can
change the block size with the *blockDimSize* variable. Your kernel
should be able to handle when there are more cells than threads
available on the GPU (i.e. you need to implement striding). To check
this, we will test your code with different values of grid size.

##### Running

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
favorite search engine on \"game of life\" and/or \"Conway\").

Your program should take five command line arguments: the name of the
data and output file, the number of generations to iterate, X_limit, and
Y_limit. To be more specific, the command line of your program should
be:

              ./life <input file name> <# of generations> <X_limit> <Y_limit> <gridSizeX> <gridSizeY> <output file name>
              

To get a GPU in your sbatch job add the following setting to the top

                #SBATCH --gres=gpu:a100_1g.5gb
              

A sample sbatch script can be found
[here](https://github.com/CMSC416/CMSC416/blob/13109134f9dc6293519b290a47e9775000b78715/assignment-4/game-of-life/submit.sh).
If you use interactive jobs, then add the `--gres=gpu:a100_1g.5gb`
argument when you run salloc.

##### Output

Your program should write \<output file name\> that contains comma
separated values representing the board. There should be one line
(containing the x coordinate, a comma, and then the y coordinate) for
each occupied cell at the end of the last iteration.

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

The only print from your program to standard output should be the GPU
timing (up to three decimal places) that looks like this:

              TIME: 4.529 s
            

Three input files that will be used for testing correctness are
available via these links:
[life.22x22.data](https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign4/life.22x22.data)
[life.32x32.data](https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign4/life.32x32.data)
[life.512x512.data](https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign1/data/life.512x512.data).


If you want to try a bigger board try running on the input file
[life.1024x1024.data](https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign1/data/life.1024x1024.data).




