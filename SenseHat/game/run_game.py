import os
import subprocess

for filename in os.listdir(os.getcwd()):   
    print (filename)
    proc = subprocess.Popen(["./game", filename])
    proc.wait()