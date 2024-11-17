import os
import subprocess
import time
import sys

os_system = os.system

def my_os_system(cmd: str):
  print(f"\033[90m{cmd}\033[0m")
  try:
    process = subprocess.Popen(cmd, shell=True)
    process.wait()
  except KeyboardInterrupt:
    process.terminate()
    process.wait()
    sys.exit(1)

os.system = my_os_system

def make_tests():
  os.system(f"rm -rf ./testCases")
  os.system(f"mkdir -p ./testCases")

  for i in range(1, 14): # tri
    os.system(f"echo {i} | ./trisGen.run > ./testCases/tri{i}.in")

  for i in range(2, 4): # Nichols
    os.system(f"echo {i} | ./NicholsGen.run > ./testCases/Nichols{i}.in")

make_tests()

runners: list[tuple[str, str]] = [
  ("Buch", "./base_Buchberger.run"),
  #("F4", "./base_F4.run"),
  ("GB", "python ./base_operator_gb.py")
]

# test files
inputs: list[str] = [f for f in os.listdir("./testCases") if f.endswith('.in')]

# Run the program and return the time it took
def run_program(index: int, test: int) -> float:
  name, runner = runners[index]
  in_file = inputs[test]
  out_file = in_file.replace('.in', '_') + name + ".out"
  start_time = time.time()
  os.system(f"{runner} < ./testCases/{inputs[test]} > ./testCases/{out_file}")
  end_time = time.time()
  return end_time - start_time


times: list[list[float]] = [[0.0 for _ in range(len(runners))] for _ in range(len(inputs))]

for i in range(len(inputs)):
  # Run programs
  for j in range(len(runners)):
    times[i][j] = run_program(j, i)

  name0, runner0 = runners[0]
  in_file = inputs[i]
  out_file0 = in_file.replace('.in', '_') + name0 + ".out"

  # Check they gave equivalent results
  for j in range(1, len(runners)):
    name, runner = runners[j]
    out_file = in_file.replace('.in', '_') + name + ".out"

    os.system(f"./compare_bases.run ./testCases/{out_file0} ./testCases/{out_file} > ./testCases/compare_bases.out")

    if open("./testCases/compare_bases.out", "r").read().strip() != "Equivalent":
      print(f"Test {in_file} failed, {name0} and {name} gave different results")
      sys.exit(1)

# Print the results in a nicely formatted table
header = ["Test Case"] + [runner[0] for runner in runners]
print(f"{header[0]:<30} {' | '.join(f'{h:<10}' for h in header[1:])}")
print("-" * (31 + 13 * len(runners)))

for i in range(len(inputs)):
  test_name = os.path.basename(inputs[i])
  row = [test_name] + [f"{times[i][j]:.2f}s" for j in range(len(runners))]
  print(f"{row[0]:<30} {' | '.join(f'{r:<10}' for r in row[1:])}")
