all: json-test

json-test: json-test.cpp ../json.h
	$(CXX) json-test.cpp -std=c++11 -o $@

clean:
	rm -f json-test