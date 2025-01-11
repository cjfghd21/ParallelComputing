<!DOCTYPE html>
<!-- saved from url=(0063)https://www.cs.umd.edu/class/fall2023/cmsc416/assignment1.shtml -->
<html lang="en"><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    <!-- Required meta tags -->
    

<div class="container-fluid">
  <div class="add-padding">
    <div class="row" style="padding-top: 70px;">
      <div class="col-md-12 pt-md-3">
      <h2 class="text-center">Introduction to Parallel Computing (CMSC416)</h2>
      <h4>Assignment 1: MPI</h4>
      <p>
The purpose of this programming assignment is to gain experience in parallel
programming on a cluster and MPI. For this assignment, you have to write a
parallel implementation of a program to simulate the <a href="https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life">Game of Life</a>.
      </p>

       <h4> Serial Algorithm </h4>
The game of life simulates simple cellular automata. The game is played on a
rectangular board containing cells. At the start, some of the cells are
occupied, the rest are empty. The game consists of constructing successive
generations of the board. The rules for constructing the next generation from
the previous one are:
      </p><ol>
        <li>death: cells with 0,1,4,5,6,7, or 8 neighbors die (0,1 of loneliness and 4-8 of over population)</li>
        <li>survival: cells with 2 or 3 neighbors survive to the next generation.</li>
        <li>birth: an unoccupied cell with 3 neighbors becomes occupied in the next generation.</li>
      </ol>
For this assignment, the game board has finite size. The x-axis starts at 0 and
ends at X_limit-1 (supplied on the command line). Likewise, the y-axis start at
0 and ends at Y_limit-1 (supplied on the command line).
      <p></p>

 
You can use the provided <a href="https://github.com/CMSC416/CMSC416/blob/04ebbad5e409310b1a5f09f5201b1c8f1418cd72/assignment-1/serial.C">serial
code</a> as a baseline to develop your parallel implementation.  It provides
some basic functionality such as parsing the input file and exporting the final
board state to a CSV file. Your task is to implement the parallel version using
C, C++, or Fortran, and MPI.
      </p>

      ## Input/Initialization 

Your program should read in a file containing the coordinates of the initial
cells. Sample files are located here: <a href="https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign1/data/life.1.256x256.data">life.1.256x256.data</a> and
<a href="https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign1/data/life.2.256x256.data">life.2.256x256.data</a>
(256x256 board). Each line in this file represents the coordinates of a cell on
the board that is live.  For instance, the following entry:
          </p><pre><code> 
          1,3
          </code></pre>
means that the cell at position [1, 3] is live in the initial state. You can
also find many other sample patterns on the web (use your favorite search
engine on "game of life" and/or "Conway").
      <p></p>

Your program should take four command line arguments: the name of the data
file, the number of generations to iterate, X_limit, and Y_limit.  To be more
specific, the command line of your program should be:
          </p><pre><code>
          ./life &lt;input file name&gt; &lt;# of generations&gt; &lt;X_limit&gt; &lt;Y_limit&gt;
          </code></pre>
      <p></p>

The number of processes the program will run on is specified as part of the
mpirun command with the -np argument.
          </p><pre><code>
          mpirun -np &lt;number of processes&gt; ./life &lt;input file name&gt; &lt;# of generations&gt; &lt;X_limit&gt; &lt;Y_limit&gt;
          </code></pre>
      <p></p>

      ## Output


Your program should write a single file called
<code>&lt;input-file-name&gt;.&lt;no-of-generations&gt;.csv</code> (from one
designated rank) that contains comma separated values representing the board.
There should be one line (containing the x coordinate, a comma, and then the y
coordinate) for each occupied cell at the end of the last iteration.

Sample output files are available:
        </p><ul>
          <li><a href="https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign1/data/life.1.256x256.100.csv">life.1.256x256.100.csv</a> is the output of the file <a href="https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign1/data/life.1.256x256.data">life.1.256x256.data</a> run for 100 generations on a 256x256 board</li>
          <li><a href="https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign1/data/life.2.256x256.100.csv">life.2.256x256.100.csv</a> is the output of the file <a href="https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign1/data/life.2.256x256.data">life.2.256x256.data</a> run for 100 generations on a 256x256 board</li>
        </ul>
Similar to the output files above, your output in the file should be sorted by
the X and Y coordinates.
      <p></p>


The only print from your program to standard output should be from process 0
that looks like this:
        </p><pre><code>
        TIME: Min: 25.389 s Avg: 27.452 s Max: 41.672 s
        </code></pre>
where Min, Avg and Max time (in seconds) are calculated using MPI reduction
operations over the individual time measurements of the "main" loop (over
generations) on different processes for the sample <a href="https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign1/data/life.512x512.data">life.512x512.data</a> input
file.
      <p></p>

      ## Parallel Version 

Figure out how you will decompose the problem for parallel execution.  Remember
that MPI (at least the OpenMPI implementation) does not always have great
communication performance and so you will want to make message passing
infrequent. Also, you will need to be concerned about static load balancing
during data distribution/domain decomposition.
You should use the 1D decomposition (over rows) and implement two different versions: one using blocking Send/Recv and other using non-blocking Isend/Irecv.
      </p>


You can assume that X_limit and Y_limit will be powers of 2 as will be the number of processes you will be running on. You can also assume that you will be running the program on a minimum of 4 processes and X_limit and Y_limit are much larger than the number of processes.
      </p>
  </div>
</div> <!-- end of document -->


</body><grammarly-desktop-integration data-grammarly-shadow-root="true"><template shadowrootmode="open"><style>
      div.grammarly-desktop-integration {
        position: absolute;
        width: 1px;
        height: 1px;
        padding: 0;
        margin: -1px;
        overflow: hidden;
        clip: rect(0, 0, 0, 0);
        white-space: nowrap;
        border: 0;
        -moz-user-select: none;
        -webkit-user-select: none;
        -ms-user-select:none;
        user-select:none;
      }
