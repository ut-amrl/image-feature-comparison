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
1. Make sure you are on the __3.2.0 branch__, then run cmake for opencv3 using the following command in the build directory.
   I found that the opencv_python3 failed a lot to build, you can turn it back on if you want.
    ```bash
    cmake -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules -DOPENCV_ENABLE_NONFREE=ON -DBUILD_opencv_xfeatures2d=ON -DBUILD_opencv_python3=OFF ..
    ```
1. Run make and make install
    ```bash
    make -j`nproc` && sudo make install
    ```
