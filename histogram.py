#!/usr/bin/python3
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np
import subprocess
import threading
import sys

##### User set variables #####

input_path = ""
bayered = "false"
best_percent = "0.3"
output_base = ""
ft_exec_path = "./build/feature_tracker.out"

##### End user variables #####

# Algorithms to test
detectors = ["AKAZE", "BRISK", "ORB", "SIFT", "SURF"]
# Colors for each algorithm
colors = ['b', 'r', 'g', 'm', 'k']

#Run each algorithm separately, in its own thread 
def ft_thread(detector, output_path):
    print("Running %s" % (detector))
    if input_path[-1] != '/':
        input_path += '/'
    subprocess.run([ft_exec_path, "--input=%s" % (input_path), "--draw=false","--bayered=%s" % (bayered), "--output=%s" % (output_path), "--best_percent=%s" % (best_percent), "--detector=%s" % (detector)])
    print("Finished %s" % (detector))

#Generate the data by spawning a bunch of threads,
#each pointing to its own output path inside the 
#specified folder.
def regen():
  subprocess.run(["mkdir", output_base])
  threads = []
  for detector in detectors:
      output_path = "%s/output.%s" % (output_base, detector)
      t = threading.Thread(target=ft_thread, args=(detector, output_path))
      t.start()
      threads.append(t)
      
  for t in threads:
      t.join()

#Simplistic arguement checking.
if len(sys.argv) >= 1:
    output_base = sys.argv[1]
else:
    print("Defaulting to output_path: %s" % output_base)

if len(sys.argv) > 2 and sys.argv[2] == "regen":
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
    data = np.zeros(len(vals))
    data_sum_total = vals.sum()
    data_sum_curr = 0;
    data_sum_curr = data_sum_total
    #Get the amount of each track length that are atleast some length.
    #Do this by subtracting those that are no longer that length from the total.
    for i in range(0, len(data)):
      data[i] = data_sum_curr / data_sum_total
      data_sum_curr -= vals[i]
    #Plot this line and make it colorful
    line2 = plt.plot(vals, data, label=detector)
    plt.setp(line2, color=color, linestyle='--')
    #Add a legend entry for this algorithm
    legend_handles.append(mpatches.Patch(color=color, label=detector))

#Set the title to the output_path, but beautified a bit
output_base = output_base.split('/')
while '' in output_base:
    output_base.remove('')
output_base = output_base[-1].replace("_", " ")
#Format the graph and then show/output it
plt.suptitle(output_base)
plt.xlabel('Feature Track Length')
plt.ylabel('% At-Least')
plt.legend(handles=legend_handles)
plt.figure(1).savefig("%s/hist.png" % (output_base))
plt.show()


