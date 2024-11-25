.PRECIOUS: %.run

COMP=g++ -Wall -Wextra -std=c++20 -O2
LIBS=-lgivaro -llinbox -lgmp -lgmpxx -lblas -llapack -lflint
INCLUDE=-I./src

HEADER_FILES := $(shell find . -name "*.h")
MAIN_FILES := $(wildcard mains/*.cpp)
MAIN_RUN_FILES := $(patsubst %.cpp, %.run, $(MAIN_FILES))

COMMON_TEST_CASES_FILES := $(wildcard test/commonTestCases/*.cpp)
COMMON_TEST_CASES_FILES := $(patsubst %.cpp, %.run, $(COMMON_TEST_CASES_FILES))

INTERNAL_TEST_FILES := $(wildcard test/internal_tests/*.cpp)
INTERNAL_TEST_RUN_FILES := $(patsubst %.cpp, %.run, $(INTERNAL_TEST_FILES))
BASE_TESTS_FILES := $(wildcard test/base_tests/*.cpp)
BASE_TESTS_RUN_FILES := $(patsubst %.cpp, %.run, $(BASE_TESTS_FILES))
RECONSTRUCTION_TESTS_FILES := $(wildcard test/reconstruction_tests/*.cpp)
RECONSTRUCTION_TESTS_RUN_FILES := $(patsubst %.cpp, %.run, $(RECONSTRUCTION_TESTS_FILES))


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

reconstruction_tests: $(RECONSTRUCTION_TESTS_RUN_FILES) commonTestCases
	cd test/reconstruction_tests && python3 run.py

test: internal_tests reconstruction_tests base_tests InIdeal_tests

clean:
	rm -f *.run
	rm -f mains/*.run
	rm -f test/*.run
	rm -f test/base_tests/*.run
	rm -f test/internal_tests/*.run
	rm -f test/InIdeal_tests/*.run
	rm -rf test/base_tests/testCases
	rm -rf test/InIdeal_tests/testCases
