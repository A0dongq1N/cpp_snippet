#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

using namespace std;

/**
 * HMAC-SHA256散列,无十六进制编码
 */
string HMAC256EncodeNoHex(string &src, const string &key)
{
    const EVP_MD *engine = EVP_sha256();
    unsigned int len = SHA256_DIGEST_LENGTH;

    unsigned char md[EVP_MAX_MD_SIZE] = {0};
    HMAC(engine, key.c_str(), key.length(), (const unsigned char *)src.c_str(), src.length(), md, &len);

    return string((char*)md, len);
}

/**
 * HMAC-SHA256散列,十六进制编码输出
 */
string HMAC256EncodeHex(string &src, const string &key)
{
    string raw = HMAC256EncodeNoHex(src, key);
    ostringstream oss;
    for (unsigned char c : raw) {
        oss << hex << setw(2) << setfill('0') << (int)c;
    }
    return oss.str();
}

int main()
{
    string data = "Hello, HMAC-SHA256!";
    string key  = "my_secret_key";

    cout << "=== HMAC-SHA256 示例 ===" << endl;
    cout << "数据: " << data << endl;
    cout << "密钥: " << key << endl;
    cout << endl;

    string raw = HMAC256EncodeNoHex(data, key);
    cout << "原始摘要长度: " << raw.size() << " 字节" << endl;

    cout << "原始摘要(hex): ";
    for (unsigned char c : raw) {
        cout << hex << setw(2) << setfill('0') << (int)c;
    }
    cout << endl;

    string hexResult = HMAC256EncodeHex(data, key);
    cout << "Hex编码结果:   " << hexResult << endl;

    return 0;
}
