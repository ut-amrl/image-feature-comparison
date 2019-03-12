# Compiling Opencv3 and Opencv_contrib from source

1. Clone opencv and opencv_contrib:
    ```bash
    git clone https://github.com/opencv/opencv.git
    git clone https://github.com/opencv/opencv_contrib.git
    ```
1. Grab the opencv dependencies, listed here:
    https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html
1. Create a build directory under the opencv directory:
    ```bash
    cd <path_to_opencv>
    mkdir build
    cd build
    ```
1. Run cmake for opencv3 using the following command in the build directory.
    ```bash
    cmake -DOPENCV_EXTRA_MODULES_PATH=<path_to_opencv_contrib>/modules -DOPENCV_ENABLE_NONFREE=ON ..
    ```
1. Run make and make install
    ```bash
    make -j`nproc` && sudo make install
    ```
