# image-feature-comparison
A tool for comparing opencv keypoint and descriptor extractor algorithms ability to track features over a large number similar scene images.

# Requirements
- Python3
- Matplotlib package
- Python-tk package
- Opencv3
   - Opencv_contrib xfeatures2d module
- CMake
- Opencv3 dependencies

# Installing Requirements (Linux)
## Python3:

Use your package manager, for ubuntu use:

```bash
apt-get install python3
```

## Matplotlib:

```bash
python3 -mpip install matplotlib
```

## Python-tk:

```bash
apt-get install python-tk
```

## Opencv3 and Opencv_contrib:

Clone opencv and opencv_contrib:

```git clone https://github.com/opencv/opencv.git```
```git clone https://github.com/opencv/opencv_contrib.git```

Grab the opencv dependencies, listed here:

https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html

Create a build directory:

```bash
cd <path_to_opencv>
mkdir build
cd build
```

Run cmake for opencv3 using the following command in the build directory.

```bash
cmake -DOPENCV_EXTRA_MODULES_PATH=<path_to_opencv_contrib>/modules -DOPENCV_ENABLE_NONFREE=ON ..
```

Run make and make install

```bash
make -j4 && sudo make install
```

# Usage

Compile the program using cmake and make and edit the parameters at the top of histogram.py to fit your needs. Most importantly change the ```input_path``` variable to your folder of images to test on, then run using the following commands:

```./histogram.py <output_directory> regen```

Regen tells the script to generate all the data for the graph, if left out the program will attempt to just make the graph from the provided data. Afterwards you can just run:

```./histogram.py <output_directory>```
