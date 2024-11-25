import os
import subprocess
import sys

os_system = os.system

def my_os_system(cmd: str):
  print(f"\033[90m{cmd}\033[0m")
  try:
    process = subprocess.Popen(cmd, shell=True)
    process.wait()
    if process.returncode != 0:
      sys.exit(process.returncode)
  except KeyboardInterrupt:
    process.terminate()
    process.wait()
    sys.exit(1)

os.system = my_os_system

testCasesFolder = "../commonTestCases/testCases"
programExe = "./Buchberger.run"

# test files
inputs: list[str] = [f for f in os.listdir(testCasesFolder) if f.endswith('.in')]
inputs.sort()

for input in inputs:
  os.system(f"{programExe} < {testCasesFolder}/{input}")
