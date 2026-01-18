# MySQL++ 安装指南

## 问题描述

编译时遇到错误：
```
fatal error: mysql++/mysql++.h: No such file or directory
```

这是因为系统中没有安装 MySQL++ 开发库。

## 解决方案

### 方法 1：使用自动安装脚本 ⭐ 推荐

```bash
# 在 mysql 目录下执行
./install_mysqlpp.sh
```

脚本会自动完成以下步骤：
1. 安装编译依赖（gcc、make、mysql-devel 等）
2. 下载 MySQL++ 3.3.0 源码
3. 编译并安装到 `/usr/local`
4. 配置动态链接库
5. 验证安装

### 方法 2：使用 Makefile 提示

```bash
# 查看详细安装说明
make install-deps
```

### 方法 3：手动安装（分步骤）

#### 步骤 1：安装编译依赖

```bash
sudo yum install -y gcc gcc-c++ make mysql-devel wget
```

#### 步骤 2：下载 MySQL++ 源码

```bash
# 下载最新稳定版 3.3.0
wget https://tangentsoft.com/mysqlpp/releases/mysql++-3.3.0.tar.gz

# 或者如果下载失败，使用备用链接
# 访问 https://tangentsoft.com/mysqlpp/wiki?name=Releases 手动下载
```

#### 步骤 3：解压源码

```bash
tar xzf mysql++-3.3.0.tar.gz
cd mysql++-3.3.0
```

#### 步骤 4：配置编译选项

```bash
./configure --prefix=/usr/local
```

#### 步骤 5：编译

```bash
# 使用多核编译加速
make -j$(nproc)
```

#### 步骤 6：安装

```bash
sudo make install
```

#### 步骤 7：配置动态链接库

```bash
# 添加库路径
echo "/usr/local/lib" | sudo tee /etc/ld.so.conf.d/mysqlpp.conf

# 更新链接库缓存
sudo ldconfig
```

#### 步骤 8：验证安装

```bash
# 检查头文件
ls -la /usr/local/include/mysql++/mysql++.h

# 检查库文件
ls -la /usr/local/lib/libmysqlpp.so

# 检查库是否在系统路径中
ldconfig -p | grep mysqlpp
```

### 方法 4：使用包管理器（如果可用）

#### Ubuntu/Debian 系统

```bash
sudo apt-get update
sudo apt-get install libmysql++-dev libmysqlclient-dev
```

#### CentOS/RHEL/Fedora 系统（如果 EPEL 仓库有）

```bash
# 启用 EPEL 仓库
sudo yum install epel-release

# 安装 MySQL++
sudo yum install mysql++-devel mysql-devel
```

**注意**：TencentOS 3 的 EPEL 仓库中可能没有 MySQL++，需要使用源码编译方式。

## 验证安装

### 检查安装状态

```bash
make check-deps
```

### 测试编译

```bash
# 尝试编译示例程序
make clean
make
```

如果编译成功，说明 MySQL++ 安装正确！

## 编译后运行

```bash
# 配置数据库连接（使用环境变量）
export MYSQL_HOST=9.134.93.213
export MYSQL_DATABASE=test_db
export MYSQL_USER=root
export MYSQL_PASSWORD=root1234

# 运行程序
./mysql_search_example
```

或使用配置文件（已创建 `db_config.conf`）：

```bash
./mysql_search_example
```

## 常见问题

### Q1: configure 失败，提示找不到 MySQL

**解决**：
```bash
# 确保安装了 mysql-devel
sudo yum install mysql-devel

# 如果还是失败，指定 MySQL 路径
./configure --prefix=/usr/local --with-mysql=/usr
```

### Q2: 编译时提示 "cannot find -lmysqlclient"

**解决**：
```bash
# 安装 MySQL 客户端库
sudo yum install mysql mysql-devel

# 或创建软链接
sudo ln -s /usr/lib64/mysql/libmysqlclient.so /usr/lib64/libmysqlclient.so
```

### Q3: 运行时提示 "error while loading shared libraries: libmysqlpp.so.3"

**解决**：
```bash
# 确保配置了动态链接库路径
echo "/usr/local/lib" | sudo tee /etc/ld.so.conf.d/mysqlpp.conf
sudo ldconfig

# 验证
ldconfig -p | grep mysqlpp
```

### Q4: 下载速度太慢或无法下载

**解决**：
1. 使用代理或 VPN
2. 手动下载：访问 https://tangentsoft.com/mysqlpp/wiki?name=Releases
3. 使用国内镜像（如果有）

### Q5: 权限不足

**解决**：
```bash
# 在需要的命令前加 sudo
sudo make install
sudo ldconfig
```

## 卸载 MySQL++

如果需要卸载：

```bash
cd mysql++-3.3.0
sudo make uninstall

# 删除配置文件
sudo rm -f /etc/ld.so.conf.d/mysqlpp.conf
sudo ldconfig
```

## 其他 Linux 发行版

### Arch Linux

```bash
sudo pacman -S mysql++
```

### openSUSE

```bash
sudo zypper install libmysql++-devel
```

### Alpine Linux

```bash
apk add mysql++-dev
```

## 参考资源

- MySQL++ 官网：https://tangentsoft.com/mysqlpp/
- MySQL++ 文档：https://tangentsoft.com/mysqlpp/doc/
- GitHub 镜像：https://github.com/tangentsoft/mysqlpp

## 快速命令总结

```bash
# 一键安装（推荐）
./install_mysqlpp.sh

# 或者手动安装
sudo yum install -y gcc gcc-c++ make mysql-devel wget
wget https://tangentsoft.com/mysqlpp/releases/mysql++-3.3.0.tar.gz
tar xzf mysql++-3.3.0.tar.gz
cd mysql++-3.3.0
./configure --prefix=/usr/local && make -j$(nproc) && sudo make install
echo "/usr/local/lib" | sudo tee /etc/ld.so.conf.d/mysqlpp.conf
sudo ldconfig

# 验证并编译
cd /data/workspace/github/cpp_snippet/mysql
make check-deps
make
```
