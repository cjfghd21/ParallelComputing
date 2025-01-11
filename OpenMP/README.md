
## Introduction to Parallel Computing (CMSC416) {#introduction-to-parallel-computing-cmsc416 .text-center}

#### Assignment 3: OpenMP


The purpose of this programming assignment is to gain experience in
parallel programming on a cluster and OpenMP. You will start with a
working serial program
([quake.c](https://github.com/CMSC416/CMSC416/blob/886b7c3ed30da6853f7b2be87c76fb1b4f70bb4e/assignment-3/quake.c))
that models an earthquake, analyze its performance and then add OpenMP
directives to create a parallel program.

The goal is to be systematic in figuring out how to parallelize this
program. You can use one of three things: manually inserted timers,
gprof, or HPCToolkit and/or Hatchet (that you used in Assignment 2) to
figure out what parts of the program take the most time. From there you
should examine the loops in the most important subroutines and figure
out how to add OpenMP directives. The program will be run on a single
compute node of zaratan.

##### Using OpenMP

To compile OpenMP we will be using gcc version 9.4.0 (the default
version on zaratan, which you can get by doing `module load gcc` on the
zaratan login node), which nicely has OpenMP support built in. In
general, you can compile this assignment with:

            gcc -fopenmp -O2 -o quake quake.c -lm
            

The -fopenmp tells the compiler to, you guessed it, recognize OpenMP
directives. -lm is required because our program uses the math library.

The environment variable **OMP_NUM_THREADS** sets the number of threads
(and presumably cores) that will run the program. Set the value of this
environment variable in the script you submit the job from. It defaults
to using all available cores, and on a zaratan node that means 128 (and
you might not want to do that). Also, in order to minimize performance
variability, you want to add these to your job submission steps or
script:

            #SBATCH --exclusive

            #SBATCH --mem-bind=local

            export OMP_PROCESSOR_BIND=true
            

##### Running the program

Quake reads its input file from standard input, and produces its output
on standard output. Quake generates an output message periodically
(every 30 of its simulation time steps), so you should be able to tell
if it is making progress.

When the program runs correctly, all versions (serial or parallel)
running the quake.in input, irrespective of the number of threads used,
should output this at the 3840th timestep:\
        Time step 3840\
        5903: -3.98e+00 -4.62e+00 -6.76e+00\
        16745: 2.45e-03 2.66e-02 -1.01e-01\
        30169 nodes 151173 elems 3855 timesteps

This is the output for quake.in.short:\
        Time step 30\
        978: 8.01e-03 7.19e-03 8.41e-03\
        3394: -3.69e-21 1.57e-20 -5.20e-20\
        7294 nodes 35025 elems 34 timesteps

Since quake runs for a while on the
[quake.in](https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign3/quake.in)
input dataset for a small numbers of threads,
[quake.in.short](https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign3/quake.in.short)
is another input file that runs for much less time (you can use this for
testing).


::: {.alert .alert-danger role="alert"}
NOTE: If the output file or timing output is not as described above, you
will not get any points. To time your program, use omp_get_wtime() by
placing one call at the beginning of main (next to the
omp_get_max_threads call) and another one toward the end of main (before
the \"Done. Terminating the simulation\" print statement). Sample timing
code below:
:::

        double start, end; 



        start = omp_get_wtime(); 

        ... work to be timed ... 

        end = omp_get_wtime(); 



        printf("TIME %.5f s\n", end - start);

          

::: {.alert .alert-danger role="alert"}
Also, if you add any other print statements to your code either during
performance analysis or for debugging, make sure that you comment them
out before the final submission.
:::

