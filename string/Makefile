CXX = g++

CXXFLAGS = -std=c++11 -Wall -Wextra
LIBS = -lssl -lcrypto

TARGET = hmac_sha256
SOURCE = hmac_sha256.cpp

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE) $(LIBS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
