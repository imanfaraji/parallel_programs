The Copyright of all of the following work goes to Iman Faraji. 

# MPI ACCELERATOR BENCHMARK

A benchmark suite for MPI for clusters with GPU accelerators.

MPI-ACC is a benchmark suite that I have developed for evaluating communication performance of the clusters with GPU accelerators. This benchmark currently includes 3 communication patterns: 1) 2d-5point Stencil; 2) 3D-Torus; and 3) Collective on a sub communicator and allows multiple of them to run concurrently on the CPU and/or on the GPU. 

# Installing:
Change the Makefile and set your own MPI and CUDA library paths.
Run make to install
Run make clean to delete the current installation


# Running

benchmark_name: <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;+2DSTENCIL: <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;input: <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-type: "cpu" or "gpu" <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-size <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-dim_wght <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-dim_x <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-dim_y <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;+3DTORUS: <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;input: <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-type: "cpu" or "gpu" <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-size <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-dim_wght <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-dim_x <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-dim_y <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-dim_z <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;+COLLSUBCOMM: <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;input: <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-type: "cpu" or "gpu" <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-size <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-dim_x_keep <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-dim_y_keep <br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-dim_z_keep <br />


# -------------

# Sample Parallel Programs

Program1: 

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;MPI -- Measuring value of PI using 
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;OpenMP -- Measuring value of PI using 
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Posix Thread -- Measuring value of PI using 
        
Program2:

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;MPI -- Building a 3D mesh using cartesian communicator and using its sub-communicator in a broadast call

Program3: 

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;MPI -- Finding the minimum value in a list of numbers

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;OpenMP -- Finding the minimum value in a list of numbers
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Posix Thread -- Finding the minimum value in a list of numbers  
    
Program4:     

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;MPI -- Parallelelizing Mandelbrot program using Dynamic Assignment
      
Program5: 

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;MPI -- Using Divide and Conquer Strategy to add a list of numbers
