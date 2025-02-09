.PRECIOUS: %.run

COMP=g++ -Wall -Wextra -std=c++20 -O2
LIBS=-lgivaro -lgmp -lgmpxx -lflint -fopenmp
INCLUDE=-I.

HEADER_FILES := $(shell find . -name "*.hpp")
MAIN_FILES := $(wildcard mains/*.cpp)
MAIN_RUN_FILES := $(patsubst %.cpp, %.run, $(MAIN_FILES))

COMMON_TEST_CASES_FILES := $(wildcard test/commonTestCases/*.cpp)
COMMON_TEST_CASES_FILES := $(patsubst %.cpp, %.run, $(COMMON_TEST_CASES_FILES))

INTERNAL_TEST_FILES := $(wildcard test/internal_tests/*.cpp)
INTERNAL_TEST_RUN_FILES := $(patsubst %.cpp, %.run, $(INTERNAL_TEST_FILES))
BASE_TESTS_FILES := $(wildcard test/base_tests/*.cpp)
BASE_TESTS_RUN_FILES := $(patsubst %.cpp, %.run, $(BASE_TESTS_FILES))
COFACTOR_TESTS_FILES := $(wildcard test/cofactor_tests/*.cpp)
COFACTOR_TESTS_RUN_FILES := $(patsubst %.cpp, %.run, $(COFACTOR_TESTS_FILES))
PARALLELISM_TESTS_FILES := $(wildcard test/parallelism_tests/*.cpp)
PARALLELISM_TESTS_RUN_FILES := $(patsubst %.cpp, %.run, $(PARALLELISM_TESTS_FILES))

%.run: %.cpp $(HEADER_FILES)
	$(COMP) $(INCLUDE) $< -o $@ $(LIBS)

build_mains: $(MAIN_RUN_FILES)

commonTestCases: $(COMMON_TEST_CASES_FILES)
	cd test/commonTestCases && python3 gen.py

%_test_run: %.run
	./$<

# The test target depends on all the .run targets
internal_tests: $(patsubst %.cpp, %_test_run, $(INTERNAL_TEST_FILES))

build_tests: $(TEST_RUN_FILES)

base_tests: $(BASE_TESTS_RUN_FILES) build_mains commonTestCases
	cd test/base_tests && python3 run.py

InIdeal_tests: $(TEST_INIDEAL_RUN_FILES) build_mains
	cd test/InIdeal_tests && python3 run.py

cofactor_tests: $(COFACTOR_TESTS_RUN_FILES) commonTestCases
	cd test/cofactor_tests && python3 run.py

parallelism_tests: $(PARALLELISM_TESTS_RUN_FILES) build_mains commonTestCases
	cd test/parallelism_tests && python3 run.py

test: internal_tests cofactor_tests base_tests InIdeal_tests parallelism_tests

clean:
	rm -f *.run
	rm -f mains/*.run
	rm -f test/*.run
	rm -f test/base_tests/*.run
	rm -rf test/base_tests/output
	rm -f test/commonTestCases/*.run
	rm -rf test/commonTestCases/testCases
	rm -f test/InIdeal_tests/*.run
	rm -rf test/InIdeal_tests/testCases
	rm -f test/internal_tests/*.run
	rm -f test/cofactor_tests/*.run
