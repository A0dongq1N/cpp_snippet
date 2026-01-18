# Makefile for MySQL++ example
CXX = g++

# 检测 MySQL++ 安装位置
# 优先级：/usr/local (源码安装) > /usr (包管理器安装)
MYSQLPP_PREFIX := $(shell if [ -d "/usr/local/include/mysql++" ]; then echo "/usr/local"; else echo "/usr"; fi)
MYSQL_PREFIX := $(shell if [ -d "/usr/include/mysql" ]; then echo "/usr"; else echo "/usr/local"; fi)

# 编译选项
CXXFLAGS = -std=c++11 -Wall -Wextra
CXXFLAGS += -I$(MYSQLPP_PREFIX)/include
CXXFLAGS += -I$(MYSQL_PREFIX)/include/mysql

# 链接选项
LIBS = -L$(MYSQLPP_PREFIX)/lib -lmysqlpp
LIBS += -L$(MYSQL_PREFIX)/lib64/mysql -L$(MYSQL_PREFIX)/lib/mysql
LIBS += -lmysqlclient -lpthread -lz -lm -ldl -lssl -lcrypto

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

# 运行程序（自动加载环境变量）
run: $(TARGET)
	@bash -c "source /etc/profile 2>/dev/null || true; ./$(TARGET)"

# 检查依赖
check-deps:
	@echo "检查 MySQL++ 安装状态..."
	@if [ -f "/usr/local/include/mysql++/mysql++.h" ] || [ -f "/usr/include/mysql++/mysql++.h" ]; then \
		echo "✓ MySQL++ 已安装"; \
	else \
		echo "✗ MySQL++ 未安装"; \
		echo ""; \
		echo "请运行以下命令安装:"; \
		echo "  make install-deps"; \
		echo "或"; \
		echo "  chmod +x install_mysqlpp.sh && ./install_mysqlpp.sh"; \
	fi

# 安装MySQL++开发包
install-deps:
	@echo "=== MySQL++ 安装指南 ==="
	@echo ""
	@echo "方法1: 使用安装脚本 (推荐)"
	@echo "  chmod +x install_mysqlpp.sh"
	@echo "  ./install_mysqlpp.sh"
	@echo ""
	@echo "方法2: 手动安装"
	@echo ""
	@echo "在 Ubuntu/Debian 系统上:"
	@echo "  sudo apt-get update"
	@echo "  sudo apt-get install libmysql++-dev libmysqlclient-dev"
	@echo ""
	@echo "在 CentOS/RHEL 系统上 (如果仓库中有):"
	@echo "  sudo yum install mysql++-devel mysql-devel"
	@echo ""
	@echo "在 CentOS/RHEL/TencentOS 上源码编译:"
	@echo "  1. 安装依赖: sudo yum install -y gcc gcc-c++ make mysql-devel wget"
	@echo "  2. 下载源码: wget https://tangentsoft.com/mysqlpp/releases/mysql++-3.3.0.tar.gz"
	@echo "  3. 解压: tar xzf mysql++-3.3.0.tar.gz"
	@echo "  4. 编译安装:"
	@echo "     cd mysql++-3.3.0"
	@echo "     ./configure --prefix=/usr/local"
	@echo "     make -j\$$(nproc)"
	@echo "     sudo make install"
	@echo "  5. 配置链接库:"
	@echo "     echo '/usr/local/lib' | sudo tee /etc/ld.so.conf.d/mysqlpp.conf"
	@echo "     sudo ldconfig"

.PHONY: all clean run install-deps check-deps
