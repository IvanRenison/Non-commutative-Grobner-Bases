import os
import random
import sys
import time

os_system = os.system

def my_os_system(cmd: str):
  print(f"\033[90m{cmd}\033[0m")
  os_system(cmd)

os.system = my_os_system


PolyRep = list[tuple[int, list[int]]]

# Set seed for reproducibility
random.seed(0)

def random_poly(x: int) -> PolyRep:
  poly: PolyRep = []
  m: int = random.randint(1, 5)
  for _ in range(m):
    c: int = random.randint(1, 5)
    k: int = random.randint(1, 5)
    vars: list[int] = [random.randint(0, x) for _ in range(k)]
    poly.append((c, vars))
  return poly


def gen_random_input() -> tuple[list[PolyRep], PolyRep, int]:
  n: int = random.randint(1, 5)
  x: int = random.randint(1, 5)
  assumptions: list[PolyRep] = []
  for _ in range(n):
    assumptions.append(random_poly(x))
  claim: PolyRep = random_poly(x)
  k: int = random.randint(1, 10)
  return (assumptions, claim, k)

def print_poly(poly: PolyRep):
  print(len(poly))
  for c, vars in poly:
    print(c, len(vars), *vars)

def print_input(assumptions: list[PolyRep], claim: PolyRep, k : int):
  print(len(assumptions))
  for poly in assumptions:
    print_poly(poly)
  print_poly(claim)
  print(k)


in_file: str = "inIdeal.in"

runners: list[tuple[str, str, str]] = [
  ("Buch", "./Buchberger_main.run", "buch_outIdeal.out"),
  ("F4", "./F4_main.run", "F4_outIdeal.out"),
  ("GB", "python ./operator_gb_main.py", "gb_outIdeal.out")
]


for test in range(100):
  assumptions, claim, k = gen_random_input()
  with open(in_file, "w") as f:
    # Set f as the standard output
    old_output = sys.stdout
    sys.stdout = f
    print_input(assumptions, claim, k)
    # Reset the standard output
    sys.stdout = old_output

  # Run programs measuring time
  times: list[float] = []
  for name, runner, out_file in runners:
    start_time = time.time()
    os.system(f"{runner} < {in_file} > {out_file}")
    end_time = time.time()
    times.append(end_time - start_time)

  # Compare
  outs: list[str] = []
  for name, runner, out_file in runners:
    outs.append(open(out_file, "r").read().strip())

  if "InIdeal" in outs and "NotInIdeal or Unknown" in outs:
    k *= 100
    with open(in_file, "w") as f:
      # Set f as the standard output
      old_output = sys.stdout
      sys.stdout = f
      print_input(assumptions, claim, k)
      # Reset the standard output
      sys.stdout = old_output
    for i, out in enumerate(outs):
      if out != "InIdeal":
        name, runner, out_file = runners[i]
        start_time = time.time()
        os.system(f"{runner} < {in_file} > {out_file}")
        end_time = time.time()
        times[i] = end_time - start_time
        outs[i] = open(out_file, "r").read().strip()


  print(f"Test {test}: ", end = "")
  for i in range(len(runners)):
    color_start, color_end = ("", "") if times[i] < 2 else ("\033[1m", "\033[0m")
    print(f"{runners[i][0]}: {color_start}{times[i]:.10f}s{color_end}", end=(" " if i + 1 == len(runners) else ", "))
  print()

  if len(set(outs)) != 1:
    print("ERROR", end = "")
    for name_run_outFile, out in zip(runners, outs):
      print(f" {name_run_outFile[0]} = {out}", end = "")
    print()
    exit(1)







