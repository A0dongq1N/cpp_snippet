#!/bin/bash
# MySQL++ 库安装脚本
# 适用于 CentOS/RHEL/TencentOS 系统

set -e

# 颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== MySQL++ 库安装脚本 ===${NC}"

# 检查是否为 root 用户
if [ "$EUID" -ne 0 ]; then 
    echo -e "${YELLOW}提示: 某些步骤可能需要 root 权限${NC}"
fi

# 1. 安装依赖
echo -e "\n${GREEN}步骤 1: 安装编译依赖...${NC}"
sudo yum install -y gcc gcc-c++ make mysql-devel wget

# 2. 下载 MySQL++
echo -e "\n${GREEN}步骤 2: 下载 MySQL++ 源码...${NC}"
MYSQL_PP_VERSION="3.3.0"
MYSQL_PP_TAR="mysql++-${MYSQL_PP_VERSION}.tar.gz"
MYSQL_PP_URL="https://tangentsoft.com/mysqlpp/releases/${MYSQL_PP_TAR}"

# 检查是否已下载
if [ ! -f "$MYSQL_PP_TAR" ]; then
    echo "正在下载 MySQL++ ${MYSQL_PP_VERSION}..."
    wget "$MYSQL_PP_URL" || {
        echo -e "${RED}下载失败！请手动下载：${NC}"
        echo "  wget $MYSQL_PP_URL"
        echo "或从 https://tangentsoft.com/mysqlpp/wiki?name=Releases 下载"
        exit 1
    }
else
    echo "MySQL++ 源码包已存在，跳过下载"
fi

# 3. 解压
echo -e "\n${GREEN}步骤 3: 解压源码...${NC}"
tar xzf "$MYSQL_PP_TAR"
cd "mysql++-${MYSQL_PP_VERSION}"

# 4. 配置
echo -e "\n${GREEN}步骤 4: 配置编译选项...${NC}"
./configure --prefix=/usr/local

# 5. 编译
echo -e "\n${GREEN}步骤 5: 编译 MySQL++...${NC}"
make -j$(nproc)

# 6. 安装
echo -e "\n${GREEN}步骤 6: 安装 MySQL++...${NC}"
sudo make install

# 7. 配置链接库
echo -e "\n${GREEN}步骤 7: 配置动态链接库...${NC}"
echo "/usr/local/lib" | sudo tee /etc/ld.so.conf.d/mysqlpp.conf
sudo ldconfig

# 8. 验证安装
echo -e "\n${GREEN}步骤 8: 验证安装...${NC}"
if [ -f "/usr/local/include/mysql++/mysql++.h" ]; then
    echo -e "${GREEN}✓ MySQL++ 头文件安装成功！${NC}"
    echo "  位置: /usr/local/include/mysql++/mysql++.h"
else
    echo -e "${RED}✗ 头文件未找到${NC}"
fi

if [ -f "/usr/local/lib/libmysqlpp.so" ]; then
    echo -e "${GREEN}✓ MySQL++ 库文件安装成功！${NC}"
    echo "  位置: /usr/local/lib/libmysqlpp.so"
else
    echo -e "${RED}✗ 库文件未找到${NC}"
fi

# 9. 清理
cd ..
echo -e "\n${YELLOW}是否删除临时文件？(y/n)${NC}"
read -r response
if [[ "$response" =~ ^([yY][eE][sS]|[yY])$ ]]; then
    rm -rf "mysql++-${MYSQL_PP_VERSION}"
    echo "临时文件已删除"
fi

echo -e "\n${GREEN}=== 安装完成！===${NC}"
echo -e "\n编译时需要添加以下选项："
echo -e "  ${YELLOW}-I/usr/local/include -L/usr/local/lib -lmysqlpp${NC}"
echo -e "\n或者修改 Makefile 中的路径"
