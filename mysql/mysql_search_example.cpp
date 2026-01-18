#include <iostream>
#include <string>
#include <cstdlib>
#include <mysql++/mysql++.h>

/*
有两个用户名：
"g____san"（四个下划线）
"gadbcsan"
用户名称带下划线，比如
在使用mysql++库，执行模糊搜索时：
#include <mysql++/mysql++.h>
...
std::string search_key = "g____san";
query << "select * from t_user where" 
           <<" name like '%" << mysqlpp::escape << search_key <<  "%' "
会把这两个名字都搜出来，实际上只想搜 "g____san"

*/

// 数据库配置结构体
struct DBConfig {
    std::string database;
    std::string host;
    std::string user;
    std::string password;
    unsigned int port;
    
    // 默认值
    DBConfig() : database("test_db"), host("localhost"), 
                 user("root"), password(""), port(3306) {}
};

// 从环境变量加载配置
// 支持的环境变量：
//   MYSQL_HOST     - 数据库主机地址
//   MYSQL_DATABASE - 数据库名称
//   MYSQL_USER     - 数据库用户名
//   MYSQL_PASSWORD - 数据库密码
//   MYSQL_PORT     - 数据库端口
DBConfig load_config_from_env() {
    DBConfig config;  // 使用默认值初始化
    
    // 从环境变量读取配置，如果存在则覆盖默认值
    const char* db_host = std::getenv("MYSQL_HOST");
    const char* db_name = std::getenv("MYSQL_DATABASE");
    const char* db_user = std::getenv("MYSQL_USER");
    const char* db_pass = std::getenv("MYSQL_PASSWORD");
    const char* db_port = std::getenv("MYSQL_PORT");
    
    if (db_host) config.host = db_host;
    if (db_name) config.database = db_name;
    if (db_user) config.user = db_user;
    if (db_pass) config.password = db_pass;
    if (db_port) config.port = std::atoi(db_port);
    
    return config;
}

int main() {
    try {
        // 从环境变量加载数据库配置
        DBConfig config = load_config_from_env();
        
        std::cout << "数据库配置信息:" << std::endl;
        std::cout << "  Host: " << config.host << std::endl;
        std::cout << "  Database: " << config.database << std::endl;
        std::cout << "  User: " << config.user << std::endl;
        std::cout << "  Port: " << config.port << std::endl;
        std::cout << std::endl;
        
        // 连接到MySQL数据库
        mysqlpp::Connection conn(false);
        if (conn.connect(config.database.c_str(), 
                        config.host.c_str(), 
                        config.user.c_str(), 
                        config.password.c_str(),
                        config.port)) {
            std::cout << "成功连接到数据库!" << std::endl;
        } else {
            std::cerr << "无法连接到数据库!" << std::endl;
            return 1;
        }

        // 创建查询对象
        mysqlpp::Query query = conn.query();
        
        // 你的搜索关键字 - 精确匹配字面字符串 "g____san"  
        // 在LIKE中，下划线(_)是通配符，需要转义为字面字符
        std::string search_key = "g____san";
        
        //方法1
        // 使用LIKE，转义下划线使其成为字面字符而不是通配符
        // query << "SELECT * FROM t_user WHERE name LIKE '" 
        //       << mysqlpp::escape << "g\\_\\_\\_\\_san" << "'";

        //方法2
        query << "SELECT * FROM t_user WHERE name REGEXP '" 
              << mysqlpp::escape << "g____san" << "'";
        
        std::cout << "执行的SQL查询: " << query.str() << std::endl;
        
        // 执行查询
        mysqlpp::StoreQueryResult result = query.store();
        
        if (result) {
            std::cout << "查询成功! 找到 " << result.num_rows() << " 条记录:" << std::endl;
            std::cout << "----------------------------------------" << std::endl;
            
            // 遍历查询结果
            for (size_t i = 0; i < result.num_rows(); ++i) {
                mysqlpp::Row row = result[i];
                
                // 假设t_user表有id, name, email等字段
                // 请根据你的实际表结构修改这部分
                std::cout << "记录 " << (i + 1) << ":" << std::endl;
                
                // 显示所有字段
                for (size_t j = 0; j < row.size(); ++j) {
                    std::cout << "  " << result.field_name(j) << ": " 
                              << row[j] << std::endl;
                }
                std::cout << "----------------------------------------" << std::endl;
            }
        } else {
            std::cout << "查询执行成功，但没有找到匹配的记录。" << std::endl;
        }
        
    } catch (const mysqlpp::Exception& er) {
        // 处理MySQL++异常
        std::cerr << "MySQL++错误: " << er.what() << std::endl;
        return 1;
    } catch (const std::exception& er) {
        // 处理标准异常
        std::cerr << "标准异常: " << er.what() << std::endl;
        return 1;
    }
    
    return 0;
}
