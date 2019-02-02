# image-feature-comparison
A tool for comparing image feature tracking algorithms performance.

# Usage

Compile the program using cmake and make and then run using the following commands:

```./histogram.py <output_directory> regen```

Regen tells the script to generate all the data for the graph, if left out the program will attempt to just make the graph from the provided data. Afterwards you can just run:

```./histogram.py <output_directory>```

# Requirements
- Python3
- Matplotlib package
- Opencv3
   - Opencv_contrib xfeatures2d module

# Installing Requirements (Linux)
## Python3:

Use your package manager, for ubuntu use:

```apt-get install python3```

## Matplotlib:

```python3 -mpip install matplotlib```

## Opencv3:

Clone opencv3 and opencv_contrib, and run cmake for opencv3 using the following command in the build directory.

```cmake -DOPENCV_EXTRA_MODULES_PATH=<opencv_contrib>/modules -DOPENCV_ENABLE_NONFREE=ON ..```
