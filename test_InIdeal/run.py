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


in_out_folder: str = "./testCases"
in_file: str = "inIdeal.in"

runners: list[tuple[str, str, str]] = [
  ("Buch", "./Buchberger_main.run", "buch_outIdeal.out"),
  ("F4", "./F4_main.run", "F4_outIdeal.out"),
  ("GB", "python ./operator_gb_main.py", "gb_outIdeal.out")
]

# Create the folder
os.system(f"mkdir -p {in_out_folder}")

def write_input(assumptions: list[PolyRep], claim: PolyRep, k : int, test: int):
  with open(f"{in_out_folder}/{in_file}{test}", "w") as f:
    # Set f as the standard output
    old_output = sys.stdout
    sys.stdout = f
    print_input(assumptions, claim, k)
    # Reset the standard output
    sys.stdout = old_output

def run_program(index: int, test: int) -> tuple[float, str]: # Returns time and output
  _, runner, out_file = runners[index]
  start_time: float = time.time()
  os.system(f"{runner} < {in_out_folder}/{in_file}{test} > {in_out_folder}/{out_file}{test}")
  end_time: float = time.time()
  out: str = open(f"{in_out_folder}/{out_file}{test}", "r").read().strip()
  return end_time - start_time, out

tests: list[tuple[list[PolyRep], PolyRep, int]] = []

def create_tests():
  for test in range(100):
    assumptions, claim, k = gen_random_input()
    write_input(assumptions, claim, k, test)

    tests.append((assumptions, claim, k))


def run_test():
  for test in range(100):
    assumptions, claim, k = tests[test]

    # Run programs measuring time
    times: list[float] = [0] * len(runners)
    outs: list[str] = [""] * len(runners)
    for i in range(len(runners)):
      times[i], outs[i] = run_program(i, test)

    # Compare
    if "InIdeal" in outs and "NotInIdeal or Unknown" in outs:
      k *= 100
      write_input(assumptions, claim, k, test)
      for i, out in enumerate(outs):
        if out != "InIdeal":
          times[i], outs[i] = run_program(i, test)


    print(f"Test {test}: ", end = "")
    max_time_i: int = max(range(len(runners)), key=lambda i: times[i])
    for i in range(len(runners)):
      color_start, color_end = ("", "") if outs[1] == "NotInIdeal or Unknown" or i != max_time_i else ("\033[1m", "\033[0m")
      print(f"{runners[i][0]}: {color_start}{times[i]:.10f}s{color_end}", end=(" " if i + 1 == len(runners) else ", "))
    print()

    if len(set(outs)) != 1:
      print("ERROR", end = "")
      for name_run_outFile, out in zip(runners, outs):
        print(f" {name_run_outFile[0]} = {out}", end = "")
      print()
      exit(1)


create_tests()
run_test()

runners = [
  ("Buch_Zp", "./Buchberger_Zp_main.run", "buch_Zp_outIdeal.out"),
  ("F4_Zp", "./F4_Zp_main.run", "F4_Zp_outIdeal.out"),
]

run_test()
