import os

os.system(f"rm -rf ./testCases")
os.system(f"mkdir -p ./testCases")

for i in range(1, 14): # tri
  os.system(f"echo {i} | ./trisGen.run > ./testCases/tri{i}.in")

for i in range(3, 6): # trit
  os.system(f"echo {i} | ./tritsGen.run > ./testCases/trit{i}.in")

for i in range(2, 4): # Nichols
  os.system(f"echo {i} | ./NicholsGen.run > ./testCases/Nichols{i}.in")
