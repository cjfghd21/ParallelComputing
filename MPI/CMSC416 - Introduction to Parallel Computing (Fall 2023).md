<!DOCTYPE html>
<!-- saved from url=(0063)https://www.cs.umd.edu/class/fall2023/cmsc416/assignment1.shtml -->
<html lang="en"><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    <!-- Required meta tags -->
    
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

    <!-- Bootstrap CSS -->
    <link rel="stylesheet" href="./CMSC416 - Introduction to Parallel Computing (Fall 2023)_files/bootstrap.min.css" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
    <link rel="stylesheet" href="./CMSC416 - Introduction to Parallel Computing (Fall 2023)_files/all.css">

    <!-- Custom style -->
    <link rel="stylesheet" href="./CMSC416 - Introduction to Parallel Computing (Fall 2023)_files/cmsc.css">

    <title>CMSC416 - Introduction to Parallel Computing (Fall 2023)</title>
  </head>
  <body data-new-gr-c-s-check-loaded="14.1092.0" data-gr-ext-installed="">
    <!-- Navigation bar -->
    <nav class="navbar navbar-expand-lg navbar-light fixed-top" style="border-radius: 0; background-color: #e3f2fd;">
      <span class="navbar-brand mb-0 h1"><a href="https://www.cs.umd.edu/"><img src="./CMSC416 - Introduction to Parallel Computing (Fall 2023)_files/UMD_CMNS_CompSci_S1_Color.png" height="30px" alt="UMD CMNS logo"></a></span>
      <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarNav" aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation">
      <span class="navbar-toggler-icon"></span>
      </button>

      <div class="collapse navbar-collapse" id="navbarNav">
      <ul class="navbar-nav mr-auto">
        <li class="nav-item active">
          <a class="nav-link" href="https://www.cs.umd.edu/class/fall2023/cmsc416/index.shtml"><i class="fas fa-home"></i> CMSC416</a>
        </li>
        <li class="nav-item">
          <a class="nav-link" href="https://www.cs.umd.edu/class/fall2023/cmsc416/lectures.shtml">Lecture slides</a>
        </li>
        <li class="nav-item">
          <a class="nav-link" href="https://www.cs.umd.edu/class/fall2023/cmsc416/assignments.shtml">Assignments</a>
        </li>
        <li class="nav-item">
          <a class="nav-link" href="https://www.cs.umd.edu/class/fall2023/cmsc416/resources.shtml">Resources</a>
        </li>
      </ul>
      <form class="form-inline">
        <input class="form-control mr-sm-2" type="search" placeholder="Search" aria-label="Search">
        <button class="btn btn-outline-primary my-2 my-sm-0" type="submit">Search</button>
      </form>
      </div>
    </nav>
        

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

      <h5>Serial Algorithm</h5>
      <p>
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

      <p>
You can use the provided <a href="https://github.com/CMSC416/CMSC416/blob/04ebbad5e409310b1a5f09f5201b1c8f1418cd72/assignment-1/serial.C">serial
code</a> as a baseline to develop your parallel implementation.  It provides
some basic functionality such as parsing the input file and exporting the final
board state to a CSV file. Your task is to implement the parallel version using
C, C++, or Fortran, and MPI.
      </p>

      <h5>Input/Initialization</h5>

      <p>
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

      <p>
Your program should take four command line arguments: the name of the data
file, the number of generations to iterate, X_limit, and Y_limit.  To be more
specific, the command line of your program should be:
          </p><pre><code>
          ./life &lt;input file name&gt; &lt;# of generations&gt; &lt;X_limit&gt; &lt;Y_limit&gt;
          </code></pre>
      <p></p>

      <p>
The number of processes the program will run on is specified as part of the
mpirun command with the -np argument.
          </p><pre><code>
          mpirun -np &lt;number of processes&gt; ./life &lt;input file name&gt; &lt;# of generations&gt; &lt;X_limit&gt; &lt;Y_limit&gt;
          </code></pre>
      <p></p>

      <h5>Output</h5>

      <p>
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

      <p>
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

      <h5>Parallel Version</h5>
      <p>
Figure out how you will decompose the problem for parallel execution.  Remember
that MPI (at least the OpenMPI implementation) does not always have great
communication performance and so you will want to make message passing
infrequent. Also, you will need to be concerned about static load balancing
during data distribution/domain decomposition.
You should use the 1D decomposition (over rows) and implement two different versions: one using blocking Send/Recv and other using non-blocking Isend/Irecv.
      </p>

      <p>
You can assume that X_limit and Y_limit will be powers of 2 as will be the number of processes you will be running on. You can also assume that you will be running the program on a minimum of 4 processes and X_limit and Y_limit are much larger than the number of processes.
      </p>

      <p>When you need to distribute the initial state of the board to different processes, you can read the entire file on one process (say rank 0) and then send messages from rank 0 to everyone else.
      </p>

      <h5>What to Submit</h5>
      <p>
You must submit the following files and no other files:
        </p><ul>
          <li><code>life-blocking.[c,C,f77,f90]</code>: parallel version using blocking Send/Recv routines, where the file extension depends on the language used for the
implementation
          </li><li><code>life-nonblocking.[c,C,f77,f90]</code>: parallel version using non-blocking Isend/Irecv routines
          </li><li><code>Makefile</code> that will compile your code successfully on
zaratan when using mpicc or mpicxx.  You can see a sample Makefile <a href="https://github.com/CMSC416/CMSC416/blob/04ebbad5e409310b1a5f09f5201b1c8f1418cd72/assignment-1/Makefile">here</a>.  Make sure that the executable
names are life-blocking and life-nonblocking, and do not include the executables in the tarball.
          </li><li>You must also submit a short report (pdf) with performance
results (two line plots).  The line plots should present the execution times to run
the two parallel versions respectively on the input file <a href="https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign1/data/life.512x512.data">life.512x512.data</a> (for
4, 8, 16, 32, 64, and 128 processes), running on a 512x512 board for 500 iterations.
In total, you will be running each program 6 times.
In the report, you should mention:
            <ul>
              <li>how was the initial data distribution done</li>
              <li>how did you ensure that the blocking version does not deadlock
              </li><li>what are the performance results, and are they what you expected</li>
            </ul>
          </li>
        </ul>
You should put the code, Makefile and report in a single directory (named
<code>LastName-FirstName-assign1</code>), compress it to .tar.gz
(<code>LastName-FirstName-assign1.tar.gz</code>) and upload that to <a href="https://www.gradescope.com/courses/448329">gradescope</a>.
      <p></p>
        
      <p>
If you want to try a bigger board, to see if you can get better speedups with
more processes, try running on the input file <a href="https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign1/data/life.1024x1024.data">life.1024x1024.data</a>.
      </p>

      <h5>Tips</h5>
      <p>
      </p><ul>
        <li><a href="https://www.cs.umd.edu/class/fall2023/cmsc416/zaratan.shtml">Zaratan primer</a>
        </li><li>Use the compiler flag -g while debugging but -O2 when collecting performance numbers.
        </li><li>MPI_Wtime <a href="https://www.open-mpi.org/doc/v3.0/man3/MPI_Wtime.3.php">example</a>
        </li><li>Use the --exclusive flag with sbatch when collecting execution times.
      </li></ul>
      <p></p>

      <h5>Grading</h5>
      <p>
        The project will be graded as follows:
        </p><table class="table table-sm" style="width: auto;">
          <thead class="thead-light">
            <tr>
              <th scope="col">Component</th>
              <th scope="col">Percentage</th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <th scope="row">Runs correctly with 4 processes</th> 
              <td>30 (15% each version)</td>
            </tr>
            <tr>
              <th scope="row">Runs correctly with 16 processes</th> 
              <td>40 (20% each version)</td>
            </tr>
            <tr>
              <th scope="row">Speedup with 4 processes</th> 
              <td>10 (non-blocking version)</td>
            </tr>
            <tr>
              <th scope="row">Speedup with 16 processes</th> 
              <td>10 (non-blocking version)</td>
            </tr>
            <tr>
              <th scope="row">Writeup</th> 
              <td>10</td>
            </tr>
          </tbody>
        </table>                 

        NOTE: If your program does not compile when submitted on gradescope, you get 0 points. If your program does not run correctly, you do NOT get any points for performance/speedup.
      <p></p>
      
      <p>
The speedup will be computed using the provided serial code as the baseline.
You will get full points as long as your speedup is within 2 times (2x) of the
expected speedup. For performance worse than 2x of the expected speedup, you
will receive credit proportional to your speedup.
      </p>
      </div>
    </div>
  </div>
</div> <!-- end of document -->

    <div class="container">
      <div class="row">
        <div class="col-md-12" style="padding-top: 24px; text-align: center;">
          <a href="https://www.umd.edu/web-accessibility">Web Accessibility</a>
        </div>
      </div>
    </div>

    <!-- Optional JavaScript -->
    <!-- jQuery first, then Popper.js, then Bootstrap JS -->
    <script src="./CMSC416 - Introduction to Parallel Computing (Fall 2023)_files/jquery-3.3.1.slim.min.js.download" integrity="sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo" crossorigin="anonymous"></script>
    <script src="./CMSC416 - Introduction to Parallel Computing (Fall 2023)_files/popper.min.js.download" integrity="sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1" crossorigin="anonymous"></script>
    <script src="./CMSC416 - Introduction to Parallel Computing (Fall 2023)_files/bootstrap.min.js.download" integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM" crossorigin="anonymous"></script>
  


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

      div.grammarly-desktop-integration:before {
        content: attr(data-content);
      }
    </style><div aria-label="grammarly-integration" role="group" tabindex="-1" class="grammarly-desktop-integration" data-content="{&quot;mode&quot;:&quot;full&quot;,&quot;isActive&quot;:true,&quot;isUserDisabled&quot;:false}"></div></template></grammarly-desktop-integration></html>