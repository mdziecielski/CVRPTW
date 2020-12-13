import os
import sys
import subprocess

for filename in os.listdir('Input'):
    print(filename)
    for x in range(50,301,50):    
        for y in range(50,301,50):
            print('halo')
            # a = subprocess.run(["../CVRPTW2", filename, "Output/{}-out.txt".format(filename), str(x), str(y)], capture_output = True)
            os.system('../CVRPTW2 {} {} {} {}'.format(filename, "Output/{}-out.txt".format(filename), str(x), str(y)))
            # print('co')
            # print(str(a.stdout).replace("\\r\\n","")[2:-1])
            # print('jest')

# os.system("../test")