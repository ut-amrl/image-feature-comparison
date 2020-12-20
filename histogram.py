#!/usr/bin/python3
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np
import subprocess
import threading
import sys
import optparse

##### Arugment Parsing   #####

parser = optparse.OptionParser()
parser.add_option('--input', action="store", dest="input_path", help="The path to the directory containing test images.", default="data/test_images/")
parser.add_option('--bayered', action="store", dest="bayered", help="Are the image bayered?", default=False)
parser.add_option('--best-percent', '--bp', action="store", dest="best_percent", help="The best X percentage of matches to consider.", default=0.3)
parser.add_option('--exec_path', action="store", dest="exec_path", help="The path to the executable for feature tracking.", default="./bin/feature_tracker")
parser.add_option('--start_length', action="store", dest="start_length", help="The starting track length that should appear on the graph", default="2")
parser.add_option('--output', action="store", dest="output_path", help="The path to the directory to output the graph and test result data.", default="output")
parser.add_option('--regen', action="store", dest="regen", help="Is this the first time running the program / should we regenerate test result data?", default=False)
parser.add_option('--max_length', action="store", dest="max_length", help="Maximum track length to consider.", default="100000000")
parser.add_option('--trim_less_than', action="store", dest="trim_less_than", help="Trim all matches with less than this count.", default="0")
options, args = parser.parse_args()

##### User set variables #####

input_path = options.input_path
bayered = bool(options.bayered)
best_percent = float(options.best_percent)
output_base = options.output_path
ft_exec_path = options.exec_path
start_length = int(options.start_length)
max_length = int(options.max_length)
trim_length = int(options.trim_less_than)

##### End user variables #####

# Algorithms to test
detectors = ["AKAZE", "BRISK", "ORB", "SIFT", "SURF", "FREAK"]
# Colors for each algorithm
colors = ['b', 'r', 'g', 'm', 'k', 'c']

#Run each algorithm separately, in its own thread
def ft_thread(detector, output_path):
    global input_path
    print("Running %s" % (detector))
    if input_path[-1] != '/':
        input_path += '/'
    subprocess.run([ft_exec_path, "--input=%s" % (input_path), "--draw=false","--bayered=%s" % (bayered), "--output=%s" % (output_path), "--best_percent=%s" % (best_percent), "--detector=%s" % (detector.lower())])
    print("Finished %s" % (detector))

#Generate the data by spawning a bunch of threads,
#each pointing to its own output path inside the
#specified folder.
def regen():
  subprocess.run(["mkdir", "-p", output_base])
  threads = []
  for detector in detectors:
      output_path = "%s/output.%s" % (output_base, detector)
      t = threading.Thread(target=ft_thread, args=(detector, output_path))
      t.start()
      threads.append(t)

  for t in threads:
      t.join()

if options.regen:
    print("Generating data!")
    regen()
else:
    print("Not regenerating data!")

#Create a large plot
plt.figure(figsize=(8,4))
legend_handles = []
for detector, color in zip(detectors, colors):
    #Load data for this specific algorithm / detector.
    output_path = "%s/output.%s" % (output_base, detector)
    raw_data = np.loadtxt(output_path, int)
    #Get the different track lengths, and their counts
    vals, counts = np.unique(raw_data, return_counts=True)
    if trim_length != 0:
        i = 0
        while i < len(vals):
            if counts[i] < trim_length:
                counts = np.delete(counts, i)
                vals = np.delete(vals, i)
                i -= 1
            i += 1
    # Find where in the vals the max element is.
    # Default to the end of the array if the user didn't specify.
    index_of_max = len(vals)
    for i in range(0, len(vals)):
        if vals[i] == max_length:
            index_of_max = i
            break
    # Truncate the values to only the ones the user requested.
    truncated_vals = list(filter(lambda x: x >= start_length and x <= max_length, vals))
    # Also truncate the counts
    truncated_counts = counts[index_of_max - len(truncated_vals):index_of_max + 1]
    data = np.zeros(len(truncated_vals))
    data_sum_total = truncated_counts.sum()
    data_sum_curr = data_sum_total
    #Get the amount of each track length that are atleast some length.
    #Do this by subtracting those that are no longer that length from the total.
    for i in range(0, len(data)):
      data[i] = data_sum_curr / data_sum_total
      data_sum_curr -= truncated_counts[i]
    #Plot this line and make it colorful
    line2 = plt.plot(truncated_vals, data, label=detector)
    plt.setp(line2, color=color, linestyle='-')
    #Add a legend entry for this algorithm
    legend_handles.append(mpatches.Patch(color=color, label=detector))

#Set the title to the output_path, but beautified a bit
output_title = output_base.split('/')
while '' in output_title:
    output_title.remove('')
output_title = output_title[-1].replace("_", " ")
#Format the graph and then show/output it
plt.suptitle(output_title)
plt.xlabel('Feature Track Length')
plt.ylabel('% At-Least')
plt.legend(handles=legend_handles)
plt.figure(1).savefig("%s/hist.png" % (output_base))
plt.show()



