-- ============================================================================
-- MySQL InnoDB 自增主键达到最大值测试
-- 用于验证两种不同的错误行为：
-- 1. ERROR 1062: Duplicate entry (主键冲突)
-- 2. ERROR 1467: Failed to read auto-increment value from storage engine
-- ============================================================================

-- ============================================================================
-- 测试1：INT UNSIGNED 达到最大值 (预期: ERROR 1062 主键冲突)
-- ============================================================================

DROP TABLE IF EXISTS test_int_unsigned_overflow;

CREATE TABLE test_int_unsigned_overflow (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    data VARCHAR(100)
) ENGINE=InnoDB;

-- INT UNSIGNED 最大值为 4,294,967,295
-- 我们将 AUTO_INCREMENT 设置为接近最大值
ALTER TABLE test_int_unsigned_overflow AUTO_INCREMENT = 4294967295;

-- 插入一条记录，会使用 id = 4294967295
INSERT INTO test_int_unsigned_overflow (data) VALUES ('第一条记录 - 使用最大值');

-- 查看当前状态
SELECT * FROM test_int_unsigned_overflow;
SHOW TABLE STATUS LIKE 'test_int_unsigned_overflow'\G

-- 尝试再次插入 - 这将触发错误
-- 预期错误: ERROR 1062 (23000): Duplicate entry '4294967295' for key 'PRIMARY'
-- 因为 AUTO_INCREMENT 已经是最大值，无法继续增长，尝试再次使用 4294967295 会冲突
INSERT INTO test_int_unsigned_overflow (data) VALUES ('第二条记录 - 将失败');

-- 测试结果：
--ERROR 1062 (23000): Duplicate entry '4294967295' for key 'test_int_unsigned_overflow.PRIMARY'[1062] [d87be68a-d539-3c09-0645-19bcc46d24be]



-- ============================================================================
-- 测试2：BIGINT UNSIGNED 达到最大值 (预期: ERROR 1467)
-- ============================================================================

DROP TABLE IF EXISTS test_bigint_unsigned_overflow;

CREATE TABLE test_bigint_unsigned_overflow (
    id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    data VARCHAR(100)
) ENGINE=InnoDB;

-- BIGINT UNSIGNED 最大值为 18,446,744,073,709,551,615
-- 我们将 AUTO_INCREMENT 设置为最大值
-- 注意：这个操作在某些 MySQL 版本中可能有限制
ALTER TABLE test_bigint_unsigned_overflow AUTO_INCREMENT = 18446744073709551615;

-- 查看当前状态
SHOW TABLE STATUS LIKE 'test_bigint_unsigned_overflow'\G

-- 尝试插入一条记录
-- 预期错误: ERROR 1467 (HY000): Failed to read auto-increment value from storage engine
-- 因为已经达到 BIGINT UNSIGNED 的绝对上限，存储引擎无法生成下一个有效值
INSERT INTO test_bigint_unsigned_overflow (data) VALUES ('尝试插入 - 将失败');

-- 测试结果
-- ERROR 1467 (HY000): Failed to read auto-increment value from storage engine[1467] [8b3c84ea-92ad-ca86-d28e-19bcc48cd3be]


-- ============================================================================
-- 补充测试：演示正常情况下自增的行为
-- ============================================================================

DROP TABLE IF EXISTS test_normal_auto_increment;

CREATE TABLE test_normal_auto_increment (
    id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    data VARCHAR(100)
) ENGINE=InnoDB;

-- 正常插入几条记录
INSERT INTO test_normal_auto_increment (data) VALUES ('记录1');
INSERT INTO test_normal_auto_increment (data) VALUES ('记录2');
INSERT INTO test_normal_auto_increment (data) VALUES ('记录3');

-- 查看正常的自增行为
SELECT * FROM test_normal_auto_increment;
SHOW TABLE STATUS LIKE 'test_normal_auto_increment'\G


-- ============================================================================
-- 清理测试表（可选）
-- ============================================================================
-- DROP TABLE IF EXISTS test_int_unsigned_overflow;
-- DROP TABLE IF EXISTS test_bigint_unsigned_overflow;
-- DROP TABLE IF EXISTS test_normal_auto_increment;


-- ============================================================================
-- 使用说明
-- ============================================================================
-- 1. 在 MySQL 命令行中执行：
--    mysql -u root -p < auto_increment_max_value_test.sql
--
-- 2. 或者在 MySQL 客户端中：
--    source auto_increment_max_value_test.sql;
--
-- 3. 观察输出的错误信息：
--    - 测试1 应该看到 ERROR 1062 (主键冲突)
--    - 测试2 应该看到 ERROR 1467 (无法读取自增值)
--
-- 注意事项：
-- - 测试2 在某些旧版本 MySQL 中可能表现不同
-- - 建议在测试环境中运行，不要在生产环境
-- - 不同的 MySQL 版本（5.x, 8.x）和 MariaDB 可能有细微差异
-- ============================================================================
