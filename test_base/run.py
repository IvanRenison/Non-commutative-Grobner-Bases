import os
import time
from typing import Optional

os_system = os.system

def my_os_system(cmd: str):
  print(f"\033[90m{cmd}\033[0m")
  os_system(cmd)

os.system = my_os_system

runners: list[tuple[str, str, str]] = [
  ("Buch", "./base_Buchberger.run", "buch.out"),
  ("F4", "./base_F4.run", "F4.out"),
  ("GB", "python ./base_operator_gb.py", "gb.out")
]

# test files
inputs: list[str] = [f for f in os.listdir("./made-testCases") if f.endswith('.txt')]

# Run the program and return the time it took
def run_program(index: int, test: int) -> float:
  _, runner, out_file = runners[index]
  start_time = time.time()
  os.system(f"{runner} < ./made-testCases/{inputs[test]} > ./{out_file}")
  end_time = time.time()
  return end_time - start_time


times: list[list[float]] = [[0.0 for _ in range(len(runners))] for _ in range(len(inputs))]

for i in range(len(inputs)):
  for j in range(len(runners)):
    times[i][j] = run_program(j, i)

# Print the results in a nicely formatted table
header = ["Test Case"] + [runner[0] for runner in runners]
print(f"{header[0]:<30} {' | '.join(f'{h:<10}' for h in header[1:])}")
print("-" * (31 + 13 * len(runners)))

for i in range(len(inputs)):
  test_name = os.path.basename(inputs[i])
  row = [test_name] + [f"{times[i][j]:.2f}s" for j in range(len(runners))]
  print(f"{row[0]:<30} {' | '.join(f'{r:<10}' for r in row[1:])}")
