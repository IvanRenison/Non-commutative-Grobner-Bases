import os
import random
import sys
import time

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
our_out_file: str = "our_outIdeal.out"
gb_out_file: str = "gb_outIdeal.out"


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

  start_time = time.time()
  os.system(f"./Buchberger_main.run < {in_file} > {our_out_file}")
  end_time = time.time()
  our_time = end_time - start_time

  start_time = time.time()
  os.system(f"python ./operator_gb_main.py  < {in_file} > {gb_out_file}")
  end_time = time.time()
  gb_time = end_time - start_time

  # Compare
  our = open(our_out_file, "r").read().strip()
  gb = open(gb_out_file, "r").read().strip()

  retray: bool = False

  if our != gb:
    if our == "NotInIdeal or Unknown":
      k *= 100
      with open(in_file, "w") as f:
        # Set f as the standard output
        old_output = sys.stdout
        sys.stdout = f
        print_input(assumptions, claim, k)
        # Reset the standard output
        sys.stdout = old_output

      start_time = time.time()
      os.system(f"./Buchberger_main.run < {in_file} > {our_out_file}")
      end_time = time.time()
      our_time = end_time - start_time

      our = open(our_out_file, "r").read().strip()

      retray = True

  if our != gb:
    print(f"Error: {our = }, {gb = }")
    exit(1)

  print(f"Test {test} passed, with {gb} ({'with retry' if retray else 'direct'}), in {our_time = }, {gb_time = }")
  if our_time > gb_time:
    print("Our time is more than GB time 🤨")






