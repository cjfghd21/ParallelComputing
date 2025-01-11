
## Introduction to Parallel Computing (CMSC416)

#### Video Effects

##### Serial Algorithm

For this assignment you will implement convolutions with image kernels
on a video file. Image kernels are used for blurring, edge detection,
and other effects. An image kernel is a square matrix which is applied
to each pixel in an image. It is applied by setting the pixel to the dot
product of its neighbors and the image kernel.
[Wikipedia](https://en.wikipedia.org/wiki/Kernel_(image_processing)) and
[this visualization](https://setosa.io/ev/image-kernels/) contain
further reading.

For example, if *k* is a 3x3 kernel, then pixel *i*, *j* of *img* would
be computed as

            new_img[i][j] = img[i-1][j-1]*k[0][0] + img[i-1][j]*k[0][1] + img[i-1][j+1]*k[0][2] +
                            img[i][j-1]*k[1][0] +   img[i][j]*k[1][1] +   img[i][j+1]*k[1][2] +
                            img[i+1][j-1]*k[2][0] + img[i+1][j]*k[2][1] + img[i+1][j+1]*k[2][2] 
            

The same pattern can be extended for kernels of size 5x5, 7x7, \... In
this assignment you will apply a kernel to each pixel of each frame in a
video. The pixels are in BGR format. You should convolve each color
channel independently. Additionally, border pixels will be ignored.

##### GPU Algorithm

Your task is to implement this procedure on the GPU. We have provided
[starter
code](https://github.com/CMSC416/CMSC416/tree/13109134f9dc6293519b290a47e9775000b78715/assignment-4/video-effect)
to handle reading/writing the video file (driver.cc) and moving data
on/off the GPU (video-effect.cu). You need to implement the
*convolveGPU* kernel in *video-effect.cu*. A serial implementation of
this algorithm is provided in the *convolveCPU* function. *driver.cc*
contains code to read/write the video and does not need to be edited.

The GPU kernel is structured as follows:

            __global__ void convolveGPU(float const* in, float *out, int width, int height, float const* kernel, int kernelWidth, int kernelHeight) {
                    /* your code here */
            }
            

*in* is the video frame you are convolving. *out* is the frame you are
storing results into. The frames are in packed BGR format (see the
*convolveCPU* example for how to read each color channel). *width* and
*height* give the dimensions of the video frame. *kernel*,
*kernelWidth*, and *kernelHeight* provide the image kernel. You can
assume the kernel is square with an odd dimension \>= 3.

*convolveGPU* will be called with block size 8x8 and grid size
⌈height/8⌉x⌈width/8⌉. You can change the block size with the
*blockDimSize* variable. Your kernel should be able to handle when there
are more pixels than threads available on the GPU (i.e. you need to
implement striding).

##### Running

The starter code is setup to take an input video file name, output file
name, and kernel name. The available kernel names are blur, edge,
sharpen, and identity. You can also provide an optional frame number
which will be dumped into a csv file. Dumped frames can be used to check
for correctness. The command line arguments are:

            ./video-effect <input_filename> <output_filename> <kernel_name> <gridSizeX> <gridSizeY> <optional:frame_number> <optional:frame_filename>
            

You can check your output for [this
video](https://pixabay.com/videos/forest-trees-woods-mountain-nature-89420/)
([download
link](https://pixabay.com/videos/download/video-89420_source.mp4)). We
provide the output for frame 100 with the edge kernel
[here](https://www.cs.umd.edu/class/fall2023/cmsc416/assignments/assign4/forest-edge-100.csv).

To get a GPU in your sbatch job add the following setting to the top

                #SBATCH --gres=gpu:a100_1g.5gb
              

A sample sbatch script can be found
[here](https://github.com/CMSC416/CMSC416/blob/13109134f9dc6293519b290a47e9775000b78715/assignment-4/video-effect/submit.sh).
If you use interactive jobs, then add the `--gres=gpu:a100_1g.5gb`
argument when you run salloc. For this code you need to load cuda and
opencv with `module load cuda opencv`.

##### Output

The program will write out a video file with *\_out* appended to the
base filename. For instance `./video-effect video.mp4 blur` will write
out *video_out.mp4* with the blur effect. You can copy videos off of
DeepThought2 to view them.

Running *./video-effect video.mp4 edge 100 frame.csv* will write the
100th frame into *frame.csv*. Your output should match
forest-edge-100.csv.

The program will output to stdout:

              Total time: 3.221 s
              Frames processed per second: 80.039 frames/s
            


