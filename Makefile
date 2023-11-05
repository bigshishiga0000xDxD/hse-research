CXX = clang++
CPPFLAGS = -std=c++17
SRC_DIR = src/cpp
BUILD_DIR = build

clean:
	rm build/*

main_332: main_332.o matrix.o
	$(CXX) $(CPPFLAGS) -o $(BUILD_DIR)/main_332 $(BUILD_DIR)/main_332.o $(BUILD_DIR)/matrix.o

main_333: main_333.o matrix.o
	$(CXX) $(CPPFLAGS) -o $(BUILD_DIR)/main_333 $(BUILD_DIR)/main_333.o $(BUILD_DIR)/matrix.o

main_233: main_233.o matrix.o
	$(CXX) $(CPPFLAGS) -o $(BUILD_DIR)/main_233 $(BUILD_DIR)/main_233.o $(BUILD_DIR)/matrix.o

main_243: main_243.o matrix.o
	$(CXX) $(CPPFLAGS) -o $(BUILD_DIR)/main_243 $(BUILD_DIR)/main_243.o $(BUILD_DIR)/matrix.o

main_253: main_253.o matrix.o
	$(CXX) $(CPPFLAGS) -o $(BUILD_DIR)/main_253 $(BUILD_DIR)/main_253.o $(BUILD_DIR)/matrix.o

main_332.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/matrix.hpp
	$(CXX) $(CPPFLAGS) -c $(SRC_DIR)/main.cpp -o $(BUILD_DIR)/main_332.o -D'SHAPE_N=3' -D'SHAPE_M=3' -D'FIELD_Q=2'

main_333.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/matrix.hpp
	$(CXX) $(CPPFLAGS) -c $(SRC_DIR)/main.cpp -o $(BUILD_DIR)/main_333.o -D'SHAPE_N=3' -D'SHAPE_M=3' -D'FIELD_Q=3'

main_233.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/matrix.hpp
	$(CXX) $(CPPFLAGS) -c $(SRC_DIR)/main.cpp -o $(BUILD_DIR)/main_233.o -D'SHAPE_N=2' -D'SHAPE_M=3' -D'FIELD_Q=3'

main_243.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/matrix.hpp
	$(CXX) $(CPPFLAGS) -c $(SRC_DIR)/main.cpp -o $(BUILD_DIR)/main_243.o -D'SHAPE_N=2' -D'SHAPE_M=4' -D'FIELD_Q=3'

main_253.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/matrix.hpp
	$(CXX) $(CPPFLAGS) -c $(SRC_DIR)/main.cpp -o $(BUILD_DIR)/main_253.o -D'SHAPE_N=2' -D'SHAPE_M=5' -D'FIELD_Q=3'

matrix.o: $(SRC_DIR)/matrix.hpp
	$(CXX) $(CPPFLAGS) -c $(SRC_DIR)/matrix.hpp -o $(BUILD_DIR)/matrix.o


test_clique_sizes: main_332 main_333 main_233 main_243 main_253
	echo "1000000\nn\ny\n" > input
	./$(BUILD_DIR)/main_332 < input > results/test_clique_sizes.txt

	echo "100000\nn\ny\n" > input
	./$(BUILD_DIR)/main_333 < input >> results/test_clique_sizes.txt

	echo "1000000\nn\ny\n" > input
	./$(BUILD_DIR)/main_233 < input >> results/test_clique_sizes.txt

	echo "500000\nn\ny\n" > input
	./$(BUILD_DIR)/main_243 < input >> results/test_clique_sizes.txt

	echo "100000\nn\ny\n" > input
	./$(BUILD_DIR)/main_253 < input >> results/test_clique_sizes.txt

	rm input

test_clique_format: main_233 main_243 main_253 src/py/test_isomorph.py
	echo "1000000\ny\nn\n" > input
	echo "2 3 3" > output
	./$(BUILD_DIR)/main_233 < input >> output
	python3 src/py/test_isomorph.py < output > results/test_clique_format.txt

	echo "500000\ny\nn\n" > input
	echo "2 4 3" > output
	./$(BUILD_DIR)/main_243 < input >> output
	python3 src/py/test_isomorph.py < output >> results/test_clique_format.txt

	echo "100000\ny\nn\n" > input
	echo "2 5 3" > output
	./$(BUILD_DIR)/main_253 < input >> output
	python3 src/py/test_isomorph.py < output >> results/test_clique_format.txt

	rm input output
