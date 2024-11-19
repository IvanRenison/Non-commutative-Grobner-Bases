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
    for i in range(2, len(line)):
      x = int(line[i])
      monomial *= names[x]
    poly += monomial
  return poly

def print_poly(poly): # Print with the format that the C++ code reads
  s: str = poly.__repr__()
  s = s.replace('*', '')
  for c in "abcdefghij":
    for i in range(2, 100):
      s = s.replace(f'{c}^{i}', f'{c}{f"{c}"*(i-1)}')
  print(s)

def main():

  # Capture prints in string because the library prints to stdout
  #old_stdout = sys.stdout
  #sys.stdout = StringIO()

  n = int(input())
  gens = []
  for _ in range(n):
    gens.append(read_poly())

  I = gb.NCIdeal(gens)
  G = I.groebner_basis(maxiter=10**1000)

  # Restore stdout
  #sys.stdout = old_stdout

  print(len(G))
  for poly in G:
    print_poly(poly)
  #print(*G, sep='\n')


if __name__ == '__main__':
  main()

