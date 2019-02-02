#!/usr/bin/python3
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np
import subprocess
import threading
import sys

##### User set variables #####

input_path = "../feature_tracker/test/bag_images_raw/outdoor/"
bayered = "true"; 
best_percent = "0.30"
output_base = "graphing_data"
ft_exec_path = "./../feature_tracker/build/feature_tracker.out"

##### End user variables #####

detectors = ["AKAZE", "BRISK", "ORB", "SIFT", "SURF"]
colors = ['b', 'r', 'g', 'm', 'k']
#Get all the histogram data

def ft_thread(detector, output_path):
    print("Running %s" % (detector))
    subprocess.run([ft_exec_path, "--input=%s" % (input_path), "--draw=false","--bayered=%s" % (bayered), "--output=%s" % (output_path), "--best_percent=%s" % (best_percent), "--detector=%s" % (detector)])
    print("Finished %s" % (detector))

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

output_base = sys.argv[1]
if len(sys.argv) > 2 and sys.argv[2] == "regen":
    print("Generating data!")
    regen()
else:
    print("Not regenerating data!")
   
   
plt.figure(figsize=(8,4))
legend_handles = []
for detector, color in zip(detectors, colors):
    output_path = "%s/output.%s" % (output_base, detector)
    raw_data = np.loadtxt(output_path, int)
    vals, counts = np.unique(raw_data, return_counts=True)
    data = np.zeros(len(vals))
    data_sum_total = vals.sum()
    data_sum_curr = 0;
    data_sum_curr = data_sum_total
    for i in range(0, len(data)):
      data[i] = data_sum_curr / data_sum_total
      data_sum_curr -= vals[i]
    line2 = plt.plot(vals, data, label=detector)
    plt.setp(line2, color=color, linestyle='--')
    legend_handles.append(mpatches.Patch(color=color, label=detector))

output_base = output_base.split('/')
while '' in output_base:
    output_base.remove('')
output_base = output_base[-1].replace("_", " ")

plt.suptitle(output_base)
plt.xlabel('Feature Track Length')
plt.ylabel('% At-Least')
plt.legend(handles=legend_handles)
plt.figure(1).savefig("%s/hist.png" % (output_base))
plt.show()


