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

runner = "./base_F4_parallel.run"

bases_comparator: str = "./../../mains/compare_bases.run"
testCasesFolder = "../commonTestCases/testCases"
outputFolder = "./output"

threads = [1, 2, 4, 6, 8, 16]

os.system(f"rm -rf {outputFolder}")
os.system(f"mkdir -p {outputFolder}")

# test files
inputs: list[str] = [f for f in os.listdir(testCasesFolder) if f.endswith('.in')]
inputs.sort()

# Run the program and return the time it took
def run_program(test: int, thr: int) -> float:
  in_file = inputs[test]
  out_file = in_file.replace('.in', '_') + str(thr) + ".out"
  start_time = time.time()
  os.system(f"{runner} {thr} < {testCasesFolder}/{inputs[test]} > {outputFolder}/{out_file}")
  end_time = time.time()
  return end_time - start_time

times: list[list[float]] = [[0.0 for _ in range(len(threads))] for _ in range(len(inputs))]

for i in range(len(inputs)):
  # Run programs
  for j in range(len(threads)):
    times[i][j] = run_program(i, threads[j])

  in_file = inputs[i]
  out_file0 = in_file.replace('.in', '_') + str(threads[0]) + ".out"

  # Check they gave equivalent results
  for j in range(1, len(threads)):
    out_file =in_file.replace('.in', '_') + str(threads[j]) + ".out"

    os.system(f"{bases_comparator} {outputFolder}/{out_file0} {outputFolder}/{out_file} > {outputFolder}/compare_bases.out")

    if open(f"{outputFolder}/compare_bases.out", "r").read().strip() != "Equivalent":
      print(f"Test {in_file} failed, {threads[0]} and {threads[j]} gave different results")
      sys.exit(1)

# Print the results in a nicely formatted table
header = ["Test Case"] + [str(thread) + " threads" for thread in threads]
print(f"{header[0]:<30} {' | '.join(f'{h:<10}' for h in header[1:])}")
print("-" * (31 + 13 * len(threads)))

for i in range(len(inputs)):
  test_name = os.path.basename(inputs[i])
  row = [test_name] + [f"{times[i][j]:.2f}s" for j in range(len(threads))]
  print(f"{row[0]:<30} {' | '.join(f'{r:<10}' for r in row[1:])}")
