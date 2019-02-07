# Running an example:

First use this link to get the compressed example dataset:

```
https://drive.google.com/file/d/1EXNRWHJtd5h8mbiexAPw2dt1yLEDcOcJ/view?usp=sharing
```

Uncompress the test dataset using the following command:

```bash
tar -xvf outdoor_image_comparison_example.tar.gz
```

Change the following line in histogram.py:

```python
input_path = "<path_to_the_example_dataset>"
```

Finally run the python script:

```bash
mkdir <path_to_output_folder>
./histogram.py <path_to_output_folder> regen
```

You should end up with a graph like this:
![example graph](example_graph.png)
