import sage.all as sage
import operator_gb as gb
import sys
from io import StringIO


F = gb.FreeAlgebra(gb.QQ, names = ('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j')) # Assuming up to 10 variables
names = F._first_ngens(10)


def read_poly():
  n = int(input())
  poly = names[0] - names[0] # Zero polynomial, I don't now a better way to do this
  for _ in range(n):
    line = input().split()
    monomial = sage.Integer(int(line[0]))
    if (len(line) > 2):
      for i in range(2, len(line)):
        x = int(line[i])
        monomial *= names[x]
    poly += monomial
  return poly


def main():

  # Capture prints in string because the library prints to stdout
  old_stdout = sys.stdout
  sys.stdout = StringIO()

  n = int(input())
  assumptions = []
  for _ in range(n):
    assumptions.append(read_poly())
  claim = read_poly()

  k = int(input())

  proof = gb.certify(assumptions, claim, maxiter = k, verbose = 0)

  # Restore stdout
  sys.stdout = old_stdout

  if proof == False:
    print("NotInIdeal or Unknown")
  else:
    print("InIdeal")


if __name__ == '__main__':
  main()
