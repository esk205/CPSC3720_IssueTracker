CXX_9= g++9.1
CXX=g++
CXX_7 = g++-7
CXXFLAGS= -g -fprofile-arcs -ftest-coverage
CXXVERSION= -std=c++11

LINKFLAGS = -lrestbed -lpthread
LINKFLAGS_TEST = -lgtest -lpthread -lgmock

SRC_DIR_SERVER = src/server
SRC_DIR_CLIENT = src/client
SRC_DIR_SERVICE = src/service

TEST_DIR = test

SERVICE_INCLUDE = -I include/service

GCOV_9 = gcov9.1
GCOV = gcov
LCOV = lcov
COVERAGE_RESULTS = results.coverage
COVERAGE_DIR = coverage

STATIC_ANALYSIS = cppcheck

STYLE_CHECK = cpplint

PROGRAM_SERVER = issueServer
PROGRAM_CLIENT = issueClient
PROGRAM_TEST = test_issue
# PROGRAM_LOCAL = test_issue #change this to test_issue for local testing of coverage

.PHONY: all
all: $(PROGRAM_SERVER) $(PROGRAM_CLIENT) $(PROGRAM_TEST) coverage docs static style

# default rule for compiling .cc to .o
%.o: %.cppm
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf *~ $(SRC)/*.o $(TEST_SRC)/*.o *.gcov *.gcda *.gcno \
	$(COVERAGE_RESULTS) \
	$(PROGRAM_SERVER) \
	$(PROGRAM_TEST) \
	$(PROGRAM_CLIENT) \
	$(COVERAGE_DIR) \
	doxygen/html \
	obj bin \
	users.txt \
	context.txt \
	comments.txt \

server: $(PROGRAM_SERVER)

client: $(PROGRAM_CLIENT)

runServer: server
	./${PROGRAM_SERVER} &

stopServer:
	kill -9 ${PROGRAM_SERVER}

$(PROGRAM_SERVER): $(SRC_DIR_SERVER) $(SRC_DIR_SERVICE)
	$(CXX_9) $(CXXFLAGS) -o $(PROGRAM_SERVER) $(SERVICE_INCLUDE) \
	$(SRC_DIR_SERVER)/*.cpp $(SRC_DIR_SERVICE)/*.cpp $(LINKFLAGS)

$(PROGRAM_CLIENT): $(SRC_DIR_CLIENT) $(SRC_DIR_SERVICE)
	$(CXX_9) $(CXXFLAGS) -o $(PROGRAM_CLIENT) $(SERVICE_INCLUDE) \
	$(SRC_DIR_CLIENT)/*.cpp $(SRC_DIR_SERVICE)/*.cpp $(LINKFLAGS)

$(PROGRAM_TEST): $(TEST_DIR) $(SRC_DIR_SERVICE)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM_TEST) $(SERVICE_INCLUDE) \
	$(TEST_DIR)/*.cpp $(SRC_DIR_SERVICE)/*.cpp $(LINKFLAGS_TEST)
	
# $(PROGRAM_LOCAL): $(TEST_DIR) $(SRC_DIR_SERVICE)
# 	$(CXX_7) $(CXXFLAGS) -o $(PROGRAM_LOCAL) $(SERVICE_INCLUDE) \
# 	$(TEST_DIR)/*.cpp $(SRC_DIR_SERVICE)/*.cpp $(LINKFLAGS_TEST)
	
testing: $(PROGRAM_TEST)
	./$(PROGRAM_TEST)

memcheck: $(PROGRAM_TEST)
	valgrind --tool=memcheck --leak-check=yes ./$(PROGRAM_TEST)
fullmemcheck: $(PROGRAM_TEST)
	valgrind --tool=memcheck --leak-check=full ./$(PROGRAM_TEST)

coverage: $(PROGRAM_TEST)
	./$(PROGRAM_TEST)	
	$(GCOV) -b $(SRC_DIR_SERVICE)/*.cpp -o .
	#Remove all of the generated files from gcov
	rm -f *.gc*

# localcoverage: $(PROGRAM_LOCAL)
# 	./$(PROGRAM_LOCAL)
# 	# Determine code coverage
# 	$(LCOV) --capture --gcov-tool $(GCOV) --directory . --output-file $(COVERAGE_RESULTS) --rc lcov_branch_coverage=1
# 	# Only show code coverage for the source code files (not library files)
# 	$(LCOV) --extract $(COVERAGE_RESULTS) */$(SRC_DIR_SERVICE)/* -o $(COVERAGE_RESULTS) --rc lcov_branch_coverage=1
# 	#Generate the HTML reports
# 	genhtml $(COVERAGE_RESULTS)  --output-directory $(COVERAGE_DIR) --rc lcov_branch_coverage=1
# 	#Remove all of the generated files from gcov
# 	rm -f *.gcda *.gcno

static: ${SRC_DIR_SERVER} ${SRC_DIR_CLIENT} ${SRC_DIR_SERVICE} ${TEST_DIR}
	${STATIC_ANALYSIS} --verbose --enable=all ${SRC_DIR_SERVER} ${SRC_DIR_CLIENT} ${SRC_DIR_SERVICE} ${TEST_DIR} ${SRC_INCLUDE} --suppress=missingInclude

style: ${SRC_DIR_SERVICE} ${SRC_INCLUDE}
	${STYLE_CHECK} $(SRC_INCLUDE)/*.h $(SRC_DIR_SERVICE)/*.cpp $(SRC_DIR_CLIENT)/*.cpp $(SRC_DIR_SERVER)/*.cpp

docs:
	doxygen doxygen/doxyfile
