# Makefile for MySQL++ example
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I/usr/include/mysql
LIBS = -lmysqlpp -L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -ldl -lssl -lcrypto
TARGET = mysql_search_example
SOURCE = mysql_search_example.cpp

# 默认目标
all: $(TARGET)

# 编译目标
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE) $(LIBS)

# 清理编译生成的文件
clean:
	rm -f $(TARGET)

# 运行程序
run: $(TARGET)
	./$(TARGET)

# 安装MySQL++开发包的提示
install-deps:
	@echo "在Ubuntu/Debian系统上安装MySQL++依赖:"
	@echo "sudo apt-get update"
	@echo "sudo apt-get install libmysql++-dev libmysqlclient-dev"
	@echo ""
	@echo "在CentOS/RHEL系统上安装MySQL++依赖:"
	@echo "sudo yum install mysql++-devel mysql-devel"
	@echo "或者 (在较新版本上):"
	@echo "sudo dnf install mysql++-devel mysql-devel"

.PHONY: all clean run install-deps
