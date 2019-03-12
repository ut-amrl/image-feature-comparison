# Image Feature Comparison
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
1. Python3:
   ```bash
   apt-get install python3
   ```
1. Matplotlib:
   ```bash
   python3 -mpip install matplotlib
   ```
1. Python-tk:
   ```bash
   apt-get install python-tk
   ```
1. Opencv3 and Opencv_contrib, compiled from source. See [compiling Opencv](opencv.md) for detailed instructions.

# Building the image-feature-comparison tool:

1. Clone this repository:
   ```bash
   git clone https://github.com/umass-amrl/image-feature-comparison.git
   ```
1. Run make from the project dir:
   ```bash
   cd image-feature-comparison
   make -j
   ```

# Usage

The program that does all the heavy lifting is feature_tracker.out, it is located in the build directory if you followed the above instructions. You can run this program directly from the build directory, for usage fo that program please run the following:

```bash
./bin/feature_tracker --help
```

The flags in "main.cc" are the important ones that control how feature_tracker runs.

Now this is useful for watching each algorithm, but the entire point of this repository is for a benchmark of these algorithms on the same dataset. To do that we will use the ```histogram.py``` script in the base directory of the project. First open the script using your favorite text editor to find the configuration variables at the top, labeled "User set variables". Here is an explanation of each one:

```python
input_path   # Path to directory containing image data.
bayered      # Is data bayered? If unsure just set to the string false.
best_percent # The feature tracking only considers the best X percent of matches. 0.3 (30%) or lower works well.
output_base  # Default path to output the graph and test results, this is a folder and also the name of your graph.
```

To generate the graph:

```
./histogram.py <output_directory> regen
```

The 'regen' argument tells the script to generate all the data for the graph, if left out the program will attempt to just make the graph from the provided data and will fail. Afterwards you can just run:

```
./histogram.py <output_directory>
```

For an additional example please read [examples/README.md](https://github.com/umass-amrl/image-feature-comparison/tree/master/examples).
