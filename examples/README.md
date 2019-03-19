# Running an example:

1. Create the data folder to put your images in:
    ``` bash
    mkdir data
    cd data
    ```
1. Download the example dataset from here: https://drive.google.com/file/d/1EXNRWHJtd5h8mbiexAPw2dt1yLEDcOcJ/view?usp=sharing
1. Uncompress the test dataset:
    ```bash
    tar -xvf outdoor_image_comparison_example.tar.gz
    ```
1. Run the python script from the directory of the project:
    ```bash
    ./histogram.py --regen True
    ```

You should end up with a graph like this:
![example graph](example_graph.png)
