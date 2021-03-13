# Image Feature Comparison
A tool for comparing opencv keypoint and descriptor extractor algorithms and their ability to track keypoints using brute force matching.

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
1. Opencv3 and Opencv_contrib, compiled from source. See [this archive](https://github.com/TheCynosure/opencv_3.2.0_archive) for detailed instructions.

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

Now this is useful for watching each algorithm, but the entire point of this repository is for a benchmark of these algorithms on the same dataset. To do that we will use the ```histogram.py``` script in the base directory of the project. There are various arguments which are listed below, but can also be obtained by running ```./histogram.py --help```.

```
--input <String>              The path to the directory containing test images (defaults to data/test_images).
--bayered <Boolean>           Are the image bayered?
--best-percent / --bp <Float> The best percentage of matches to consider for matching.
--exec_path <String>          The path to the executable (defaults to ./bin/feature_tracker).
--start_length <Int>          The starting track length that should appear on the graph.
--output <String>             The path to the directory to output the graph and test result data (also the title of the graph).
--regen <Boolean>             Is this the first time running the program / should we regenerate test result data?
--max_length <Int>            Maximum track length to consider.
--trim_less_than <Int>        Trim all tracks with less than this number of tracks.
```

To generate the graph (assumes the test data is in data/test_images):

```
./histogram.py --regen True
```

The 'regen' argument tells the script to generate all the data for the graph, if left out the program will attempt to just make the graph from the provided data and will fail. Afterwards you can just run:

```
./histogram.py
```

For an additional example please read [examples/README.md](https://github.com/umass-amrl/image-feature-comparison/tree/master/examples).
