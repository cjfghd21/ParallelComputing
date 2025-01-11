#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include "mpi.h"


using namespace std;

//finding min,max,avg time then printing to stdout.
void getTimes(double *times, int numprocs){
    double min,max,avg=0;
    
    for (int i = 0; i < numprocs; i++){
        if(i == 0){
            min=times[i];
            max=times[i];
        }else{
            if(min > times[i])
                min = times[i];
            if(max < times[i])
                max = times[i];
        }
        avg += times[i];
    }

    avg = avg/numprocs;
    fflush(stdout);
    fprintf(stdout,"TIME: Min: %.3f s Avg: %.3f s Max: %.3f s", min,avg,max);
    fflush(stdout);
    
    
}

int** create2DArray(int Y_limit, int X_limit){
    int ** ptr = nullptr;
    int * pool = nullptr;
    try{
        ptr = new int*[Y_limit];
        pool = new int[Y_limit * X_limit];
        int* ret = pool;

        for (int i=0; i < Y_limit; i++, pool += X_limit)
            ptr[i] = pool;

        fill(ret, ret + Y_limit * X_limit, 0);

        return ptr;
    }
    catch(bad_alloc& ex)
    {
        delete [] ptr;
        throw ex;
    } 
    
}

int *convert_to_1d(int **life,int X_limit, int Y_limit,int myid){
    int * data = (int *) malloc(sizeof(int) * X_limit * Y_limit);
    int z =0;

    for(int i = 0; i <X_limit * Y_limit; i++){
        data[i] = 0;
    }

    for(int i=0; i < Y_limit; i++){
        for(int j=0; j < X_limit; j++){
            z = i * X_limit + j;
            data[z] = life[i][j];  
        }
    }

    return data;
}

/*
 * Reads the input file line by line and stores it in a 2D matrix.
 */
int *read_input_file(string const &input_file_name, int X_limit, int Y_limit) {
    
    //allocating contiguous memory for scatter.
    int * data = (int *) malloc(sizeof(int) * X_limit * Y_limit);
 

    //initialzing the 1-d arraay with 0
    for(int i = 0; i <X_limit * Y_limit; i++){
        data[i] = 0;
    }
    // Open the input file for reading.
    ifstream input_file;
    input_file.open(input_file_name);
    if (!input_file.is_open())
        perror("Input file cannot be opened");

    string line, val;
    int x, y, z;
    while (getline(input_file, line)) {
        stringstream ss(line);
        
        // Read x coordinate.
        getline(ss, val, ',');
        x = stoi(val);
        
        // Read y coordinate.
        getline(ss, val);
        y = stoi(val);

        // coneverting 2d-coordinate to 1d-array index
        z = y + Y_limit * x;
        // Populate the life matrix.
        data[z] = 1;
    }
    input_file.close();
    return data;
}

/* 
 * Writes out the final state of the 2D matrix to a csv file. 
 */
void write_output(int **result_matrix, int X_limit, int Y_limit,
                  string const &input_name, int num_of_generations) {
    
    // Open the output file for writing.
    ofstream output_file;
    string input_file_name = input_name.substr(0, input_name.length() - 5);
    output_file.open(input_file_name + "." + to_string(num_of_generations) +
                    ".csv");
    if (!output_file.is_open())
        perror("Output file cannot be opened");
    
    // Output each live cell on a new line. 
    for (int i = 0; i < X_limit; i++) {
        for (int j = 0; j < Y_limit; j++) {
            if (result_matrix[i][j] == 1) {
                output_file << i << "," << j << "\n";
            }
        }
    }
    output_file.close();
}

//depending on flag, True will caculate 2nd to last row,  false first and last row of life.
void computeHelper(int **life, int**previous_life, int X_limit, int nY_limit, bool flag){
    int neighbors = 0;
    if(flag){
    //calculate 2nd and 2nd to last row exclding padding. (3rd and 3rd to last including padding)
        for (int i = 2; i < nY_limit; i++) {
            for (int j = 1; j < X_limit+1; j++) {
                neighbors = previous_life[i - 1][j - 1] + previous_life[i - 1][j] +
                previous_life[i - 1][j + 1] + previous_life[i][j - 1] +
                previous_life[i][j + 1] + previous_life[i + 1][j - 1] +
                previous_life[i + 1][j] + previous_life[i + 1][j + 1];

                if (previous_life[i][j] == 0) {
                    // A cell is born only when an unoccupied cell has 3 neighbors.
                    if (neighbors == 3)
                        life[i - 1][j - 1] = 1;
                } else {
                    // An occupied cell survives only if it has either 2 or 3 neighbors.
                    // The cell dies out of loneliness if its neighbor count is 0 or 1.
                    // The cell also dies of overpopulation if its neighbor count is 4-8.
                    if (neighbors != 2 && neighbors != 3) {
                        life[i - 1][j - 1] = 0;
                    }
                }
            }
        }
    }
    else{
    //calculate first and last row as request recieved after wait
        for (int i = 1; i < nY_limit + 1; i += nY_limit-1){
            for (int j = 1; j < X_limit + 1; j++) {
                neighbors = previous_life[i - 1][j - 1] + previous_life[i - 1][j] +
                previous_life[i - 1][j + 1] + previous_life[i][j - 1] +
                previous_life[i][j + 1] + previous_life[i + 1][j - 1] +
                previous_life[i + 1][j] + previous_life[i + 1][j + 1];

                if (previous_life[i][j] == 0) {
                    // A cell is born only when an unoccupied cell has 3 neighbors.
                    if (neighbors == 3)
                        life[i - 1][j - 1] = 1;
                } else {
                    // An occupied cell survives only if it has either 2 or 3 neighbors.
                    // The cell dies out of loneliness if its neighbor count is 0 or 1.
                    // The cell also dies of overpopulation if its neighbor count is 4-8.
                    if (neighbors != 2 && neighbors != 3) {
                        life[i - 1][j - 1] = 0;
                    }
                }
            }
        }
    }
}

 
//Processes the life array for the specified number of iterations.
void compute(int **life, int **previous_life, int X_limit, int nY_limit, int myid, int numprocs) {
    
    MPI_Request request[4];

    // Update the previous_life matrix with the current life matrix state.
    for (int i = 0; i < nY_limit; i++) {
        for (int j = 0; j < X_limit; j++) {
            previous_life[i + 1][j + 1] = life[i][j];
        }
    }

    //non blocking send and recieve
    //don't wait for signal and first calcualte the rows that does not depend on rows of other processes
    //when done, wait for signal then do the rest of the calculation as needed.
    if(myid == 0){
        MPI_Isend(&(life[nY_limit-1][0]),X_limit, MPI_INT, myid+1,0, MPI_COMM_WORLD, &request[0]);
        computeHelper(life,previous_life,X_limit,nY_limit,true); 
        MPI_Irecv(&(previous_life[nY_limit+1][1]),X_limit,MPI_INT, myid+1, 0,MPI_COMM_WORLD, &request[1]);
        MPI_Wait(&request[1], MPI_STATUS_IGNORE); //wait for recieve
        computeHelper(life,previous_life,X_limit,nY_limit,false); //caculate the rest after recieving.
        MPI_Wait(&request[0], MPI_STATUS_IGNORE); //wait for send if not already done.
    }
    else if(myid == numprocs - 1){
        MPI_Isend(&(life[0][0]),X_limit, MPI_INT, myid-1,0, MPI_COMM_WORLD, &request[0]);
        computeHelper(life,previous_life,X_limit,nY_limit,true); 
        MPI_Irecv(&(previous_life[0][1]),X_limit,MPI_INT, myid-1, 0,MPI_COMM_WORLD, &request[1]);
        MPI_Wait(&request[1], MPI_STATUS_IGNORE); //wait for recieve
        computeHelper(life,previous_life,X_limit,nY_limit,false); //caculate the rest after recieving.
        MPI_Wait(&request[0], MPI_STATUS_IGNORE); //wait for send if not already done.
    }
    else{    
        MPI_Isend(&(life[0][0]),X_limit, MPI_INT, myid-1,0, MPI_COMM_WORLD, &request[0]);
        computeHelper(life,previous_life,X_limit,nY_limit,true);  //calculate independent rows first.
        MPI_Irecv(&(previous_life[0][1]),X_limit,MPI_INT, myid-1, 0,MPI_COMM_WORLD, &request[1]);

        MPI_Isend(&(life[nY_limit-1][0]),X_limit, MPI_INT, myid+1,0, MPI_COMM_WORLD,&request[2]);
        MPI_Irecv(&(previous_life[nY_limit+1][1]),X_limit,MPI_INT, myid+1, 0,MPI_COMM_WORLD,&request[3]);
        MPI_Wait(&request[1],MPI_STATUS_IGNORE);
        MPI_Wait(&request[3],MPI_STATUS_IGNORE); //wait for both recieve    
        computeHelper(life,previous_life,X_limit,nY_limit,false);
        MPI_Waitall(2,request,MPI_STATUS_IGNORE); // wait for rest of the signal.    
    }
 
    // For simulating each generation, calculate the number of live
    // neighbors for each cell and then determine the state of the cell in
    // the next iteration.
}

/**
  * The main function to execute "Game of Life" simulations on a 2D board.
  */
int main(int argc, char *argv[]) {
    int myid, numprocs, i, namelen;
    int num_of_generations = stoi(argv[2]);
    int X_limit = stoi(argv[3]);
    int Y_limit = stoi(argv[4]);
    char   processor_name[MPI_MAX_PROCESSOR_NAME];
    if (argc != 5){
        perror("Expected arguments: ./life <input_file> <num_of_generations> <X_limit> <Y_limit>");
    }

    string input_file_name = argv[1];


    //1d-array to hold converted 2-d array that contains data from files.
    int *init;    


    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Get_processor_name(processor_name,&namelen);
    
    double start,end;        //record start and end time for computation.
    double times[numprocs];  //hold time it took for each process. index = process#.   
    int nY_limit = Y_limit/numprocs;   //new row count for each process.
    
    
    
    //creating contiguous 2d array to hold data for each process.  See create2DArray function above.
    int **life = create2DArray(nY_limit,X_limit);
    

    /*reading in initial state of the board*/
    if(myid == 0){
        init = read_input_file(input_file_name, X_limit, Y_limit);
        MPI_Scatter(init, X_limit*nY_limit, MPI_INT, life[0], X_limit*nY_limit, MPI_INT, 0, MPI_COMM_WORLD);
    }
    else{
        MPI_Scatter(NULL, X_limit*nY_limit, MPI_INT, life[0], X_limit*nY_limit, MPI_INT, 0, MPI_COMM_WORLD);
    }

    
    // Use previous_life to track the pervious state of the board.
    // Pad the previous_life matrix with 0s on all four sides by setting all
    // cells in the following rows and columns to 0:
    //  1. Row 0
    //  2. Column 0
    //  3. Row X_limit+1
    //  4. Column Y_limit+1

    int **previous_life;
    previous_life = create2DArray(nY_limit+2,X_limit+2);
  

    ///////////computation////////////////////////////////
    start = MPI_Wtime();
    for (int numg = 0; numg < num_of_generations; numg++) {
        compute(life,previous_life,X_limit,nY_limit,myid,numprocs);
    }
           
    end = MPI_Wtime();
    ///////////computation end////////////////
    
    //gathering time for each process to find min/max/avg
    double time[1];
    time[0] = end - start;
    MPI_Gather(time,1, MPI_DOUBLE,times,1,MPI_DOUBLE,0,MPI_COMM_WORLD);

    //finding min,max,avg then printing to stdout
    if(myid ==0){
        getTimes(times,numprocs);
    }


    //converting to 1-d array for gather.
    int *tmp;   
    tmp = convert_to_1d(life,X_limit,nY_limit,myid);
    int **result = create2DArray(X_limit,Y_limit); 
    MPI_Gather(tmp,X_limit*nY_limit, MPI_INT,result[0],X_limit*nY_limit,MPI_INT,0,MPI_COMM_WORLD);


 
    // Write out the final state to the output file.
    write_output(result, X_limit, Y_limit, input_file_name, num_of_generations);    

    //freeing allocated memory
    delete[] tmp;
    delete[] life;
    delete[] previous_life;
    delete[] result;
    
    MPI_Finalize();
    return 0;
}