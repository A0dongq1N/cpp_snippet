# MySQL++ 搜索示例

这个示例展示了如何使用MySQL++库来执行数据库查询，特别是使用LIKE操作符进行模糊搜索。

## 文件说明

- `mysql_search_example.cpp` - 主程序文件，包含数据库连接和查询逻辑
- `Makefile` - 编译配置文件
- `README.md` - 本说明文件

## 前置要求

### 1. 安装MySQL++开发库

**Ubuntu/Debian系统:**
```bash
sudo apt-get update
sudo apt-get install libmysql++-dev libmysqlclient-dev
```

**CentOS/RHEL系统:**
```bash
sudo yum install mysql++-devel mysql-devel
# 或者在较新版本上:
sudo dnf install mysql++-devel mysql-devel
```

### 2. 安装和配置MySQL服务器

确保MySQL服务器已安装并运行。

### 3. 创建测试数据库和表

连接到MySQL并执行以下SQL语句：

```sql
-- 创建测试数据库
CREATE DATABASE IF NOT EXISTS test_db;
USE test_db;

-- 创建用户表
CREATE TABLE IF NOT EXISTS t_user (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- 插入一些测试数据
INSERT INTO t_user (name, email) VALUES 
('john_doe', 'john@example.com'),
('jane_smith', 'jane@example.com'),
('g_anderson', 'g.anderson@example.com'),
('g____san', 'g____san@example.com'),    -- 精确匹配的目标记录
('gadbcsan', 'gabc@example.com'),        -- 不同的记录
('g1234san', 'g1234@example.com'),       -- 不同的记录
('gwilssan', 'gwils@example.com'),       -- 不同的记录
('mike_johnson', 'mike@example.com');
```

## 配置数据库连接

在运行程序之前，请修改 `mysql_search_example.cpp` 中的数据库连接参数：

```cpp
// 修改这些参数以匹配你的数据库配置
if (conn.connect("test_db",     // 数据库名
                "localhost",    // 主机名
                "root",         // 用户名
                "password")) {  // 密码
```

## 编译和运行

### 1. 编译程序

```bash
make
```

或者手动编译：
```bash
g++ -std=c++11 -Wall -Wextra -o mysql_search_example mysql_search_example.cpp -lmysqlpp -lmysqlclient
```

### 2. 运行程序

```bash
make run
```

或者直接运行：
```bash
./mysql_search_example
```

## 程序功能

程序会：

1. 连接到指定的MySQL数据库
2. 执行查询来搜索名称精确等于 "g____san" 的用户
3. 显示查询结果，只会找到完全匹配的记录

## 精确搜索解决方案

要只搜索出一条记录（精确匹配 "g____san"），有以下几种方法：

### 方法1: 使用LIKE with转义（推荐用于LIKE场景）

```cpp
// 转义下划线，使其成为字面字符而不是通配符
query << "SELECT * FROM t_user WHERE name LIKE '" 
      << mysqlpp::escape << "g\\_\\_\\_\\_san" << "'";
```

**说明：**
- 在SQL的LIKE语句中，下划线 `_` 是通配符，匹配任意单个字符
- 使用反斜杠转义：`\_` 表示字面的下划线字符
- `g\\_\\_\\_\\_san` 只匹配字面字符串 "g____san"

### 方法2: 使用REGEXP（推荐）

```cpp
// 在正则表达式中，下划线本身就是字面字符
query << "SELECT * FROM t_user WHERE name REGEXP '^g____san$'";
```

**说明：**
- 在REGEXP中，下划线 `_` 是字面字符，不是通配符
- `^` 表示字符串开始，`$` 表示字符串结束
- 确保完全精确匹配，不匹配包含该模式的更长字符串

### 方法3: 使用等号（最简单）

```cpp
// 直接使用等号进行精确匹配
query << "SELECT * FROM t_user WHERE name = '" 
      << mysqlpp::escape << search_key << "'";
```

**说明：**
- 最简单直接的方法
- 精确匹配，性能最好
- 如果业务允许使用等号，这是首选方案

### 对比说明

| 方法 | 查询语句 | 优点 | 缺点 |
|------|----------|------|------|
| LIKE转义 | `name LIKE 'g\\_\\_\\_\\_san'` | 符合LIKE语法要求 | 语法复杂，需要转义 |
| REGEXP | `name REGEXP '^g____san$'` | 语法简洁，功能强大 | 性能略低于等号 |
| 等号 | `name = 'g____san'` | 最简单，性能最好 | 不是LIKE语法 |

**匹配结果（所有方法）：**
- ✅ 匹配：`g____san`（精确匹配字面下划线）
- ❌ 不匹配：`gadbcsan`, `g1234san`, `gwilssan` 等

**错误示例（不转义的LIKE）：**
```cpp
// 错误：这会匹配多条记录
query << "SELECT * FROM t_user WHERE name LIKE 'g____san'";
// g____san 被解释为：g + 任意4个字符 + san
// 会匹配：gadbcsan, g1234san, gwilssan 等多条记录
```

## 编译问题解决方案

### 问题1: 找不到头文件 mysql++/mysql++.h

**错误信息：**
```
mysql_search_example.cpp:1:29: fatal error: mysql++/mysql++.h: No such file or directory
 #include <mysql++/mysql++.h>
                             ^
compilation terminated.
```

**解决方案：**
1. 安装MySQL++开发包：
   ```bash
   # CentOS/RHEL系统
   yum install mysql++-devel mysql-devel
   
   # Ubuntu/Debian系统
   apt-get install libmysql++-dev libmysqlclient-dev
   ```

### 问题2: 找不到头文件 mysql_version.h

**错误信息：**
```
/usr/include/mysql++/common.h:131:28: fatal error: mysql_version.h: No such file or directory
 # include <mysql_version.h>
                            ^
compilation terminated.
```

**解决方案：**
1. 获取正确的编译参数：
   ```bash
   mysql_config --cflags --libs
   ```
   
2. 更新Makefile，添加正确的头文件路径和库链接：
   ```makefile
   CXXFLAGS = -std=c++11 -Wall -Wextra -I/usr/include/mysql
   LIBS = -lmysqlpp -L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -ldl -lssl -lcrypto
   ```

3. 重新编译：
   ```bash
   make clean && make
   ```

## 故障排除

### 编译错误
- 确保已安装MySQL++开发包
- 检查库文件路径是否正确
- 使用 `mysql_config --cflags --libs` 获取正确的编译参数

### 运行时错误
- 确保MySQL服务器正在运行
- 检查数据库连接参数是否正确
- 确保数据库和表已创建
- 检查用户权限

### 常见错误信息
- "Can't connect to MySQL server" - MySQL服务器未运行或连接参数错误
- "Access denied" - 用户名或密码错误
- "Unknown database" - 数据库不存在

## 清理

删除编译生成的文件：
```bash
make clean
```
