.PRECIOUS: %.run

COMP=g++ -Wall -Wextra -std=c++20 -O2

HEADER_FILES := $(shell find . -name "*.h")

%.run: %.cpp $(HEADER_FILES)
	$(COMP) $< -o $@ -lgivaro -lgmpxx -lgmp



# Get a list of all .cpp files in the test folder
TEST_FILES := $(wildcard test/*.cpp)
TEST_RUN_FILES := $(patsubst %.cpp, %.run, $(TEST_FILES))
BASE_FILES := $(wildcard test_base/*.cpp)
BASE_RUN_FILES := $(patsubst %.cpp, %.run, $(BASE_FILES))

%_test_run: %.run
	./$<

# The test target depends on all the .run targets
test: $(patsubst %.cpp, %_test_run, $(TEST_FILES))

build_test: $(TEST_RUN_FILES)



TEST_INIDEAL_FILES := $(wildcard test_InIdeal/*.cpp)
TEST_INIDEAL_RUN_FILES := $(patsubst %.cpp, %.run, $(TEST_INIDEAL_FILES))

build_test_inideal: $(TEST_INIDEAL_RUN_FILES)

test_InIdeal: build_test_inideal
	cd test_InIdeal && python3 run.py

base_mains: $(BASE_RUN_FILES)


clean:
	rm -f *.run
	rm -f test/*.run
	rm -f test_InIdeal/*.run
	rm -rf test_InIdeal/testCases
