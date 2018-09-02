import json, os, threading
import numpy as np
import matplotlib.pyplot as plt

results = [[0] * 8] * 2
def thread_work(rf):
    file = tmp[rf] + ".json"
    # os.system("./run.sh {0} {1} {2} {3}".format(origin, b, rf, file))
    with open(file) as f:
        data = np.array(json.load(f))
    # means = np.array(data).mean()
    # print(means)
    means = np.array([data[i].mean() for i in range(len(data))])
    k = 0
    for i in range(1, 9):
        results[rf][i - 1] = means[k]
        k += 9 - i

origin = 300000
b = 160000
tmp = ["writer-first", "reader-first"]
# os.system("./getdata {0} >> origin.txt".format(origin))
# os.system("./getdata {0} >> search.txt".format(b))
# os.system("./getdata {0} >> insert.txt".format(b))
# os.system("./getdata {0} >> delete.txt".format(b))

threads = []
for rf in range(2):
    threads.append(threading.Thread(target=thread_work, args=(rf, )))
for t in threads:
    t.start()
for t in threads:
    t.join()
x = np.arange(0.1, 0.9, 0.1)
f, ax = plt.subplots()
ax.set_xlabel("search percent")
ax.set_ylabel("time")
for rf in range(2):
    ax.plot(x, results[rf], label=tmp[rf])

plt.tight_layout()
plt.show()
