#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/if_alg.h>
#include <errno.h>

/**
 * AF_ALG 使用流程:
 * 1. 创建 AF_ALG 类型的 socket。
 * 2. bind 到具体的算法（如 skcipher, cbc(aes)）。
 * 3. setsockopt 设置密钥。
 * 4. accept 获取操作句柄（会话）。
 * 5. sendmsg 发送控制信息（加密/解密、IV）和数据。
 * 6. read 读取处理后的结果。
 */

int main() {
    int tfmfd, opfd;
    struct sockaddr_alg sa = {
        .salg_family = AF_ALG,
        .salg_type = "skcipher",
        .salg_name = "cbc(aes)"
    };

    // 16 字节密钥 (AES-128) 和 16 字节初始化向量 (IV)
    unsigned char key[16] = "0123456789abcde";
    unsigned char iv[16]  = "123456789012345";
    unsigned char plaintext[16] = "qpzqpz"; // 必须是块大小的倍数（16字节）
    unsigned char ciphertext[16];
    unsigned char decrypted[16];

    // 确保 plaintext 长度填充到 16 字节
    unsigned char padded_plaintext[16] = {0};
    memcpy(padded_plaintext, plaintext, strlen((char *)plaintext));

    // 1. 创建 socket
    tfmfd = socket(AF_ALG, SOCK_SEQPACKET, 0);
    if (tfmfd < 0) {
        perror("socket AF_ALG");
        return 1;
    }

    // 2. 绑定算法
    if (bind(tfmfd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
        perror("bind algorithm");
        close(tfmfd);
        return 1;
    }

    // 3. 设置密钥
    if (setsockopt(tfmfd, SOL_ALG, ALG_SET_KEY, key, 16) < 0) {
        perror("setsockopt ALG_SET_KEY");
        close(tfmfd);
        return 1;
    }

    // 4. 获取操作句柄
    opfd = accept(tfmfd, NULL, 0);
    if (opfd < 0) {
        perror("accept");
        close(tfmfd);
        return 1;
    }

    // --- 加密过程 ---
    {
        struct msghdr msg = {0};
        struct cmsghdr *cmsg;
        // 注意：IV 需要 af_alg_iv 结构体包装
        char cbuf[CMSG_SPACE(sizeof(int)) + CMSG_SPACE(sizeof(struct af_alg_iv) + 16)] = {0};
        struct iovec iov;

        msg.msg_control = cbuf;
        msg.msg_controllen = sizeof(cbuf);

        // 设置操作：加密
        cmsg = CMSG_FIRSTHDR(&msg);
        cmsg->cmsg_level = SOL_ALG;
        cmsg->cmsg_type = ALG_SET_OP;
        cmsg->cmsg_len = CMSG_LEN(sizeof(int));
        *(int *)CMSG_DATA(cmsg) = ALG_OP_ENCRYPT;

        // 设置 IV
        cmsg = CMSG_NXTHDR(&msg, cmsg);
        cmsg->cmsg_level = SOL_ALG;
        cmsg->cmsg_type = ALG_SET_IV;
        cmsg->cmsg_len = CMSG_LEN(sizeof(struct af_alg_iv) + 16);
        struct af_alg_iv *alg_iv = (struct af_alg_iv *)CMSG_DATA(cmsg);
        alg_iv->ivlen = 16;
        memcpy(alg_iv->iv, iv, 16);

        // 设置待加密数据
        iov.iov_base = padded_plaintext;
        iov.iov_len = 16;
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;

        if (sendmsg(opfd, &msg, 0) < 0) {
            perror("sendmsg encrypt");
            return 1;
        }

        if (read(opfd, ciphertext, 16) < 0) {
            perror("read ciphertext");
            return 1;
        }
        printf("加密结果: ");
        for(int i=0; i<16; i++) printf("%02x", ciphertext[i]);
        printf("\n");
    }

    // 关闭上一个会话，开启新会话进行解密
    close(opfd);
    opfd = accept(tfmfd, NULL, 0);

    // --- 解密过程 ---
    {
        struct msghdr msg = {0};
        struct cmsghdr *cmsg;
        char cbuf[CMSG_SPACE(sizeof(int)) + CMSG_SPACE(sizeof(struct af_alg_iv) + 16)] = {0};
        struct iovec iov;

        msg.msg_control = cbuf;
        msg.msg_controllen = sizeof(cbuf);

        // 设置操作：解密
        cmsg = CMSG_FIRSTHDR(&msg);
        cmsg->cmsg_level = SOL_ALG;
        cmsg->cmsg_type = ALG_SET_OP;
        cmsg->cmsg_len = CMSG_LEN(sizeof(int));
        *(int *)CMSG_DATA(cmsg) = ALG_OP_DECRYPT;

        // 设置相同的 IV
        cmsg = CMSG_NXTHDR(&msg, cmsg);
        cmsg->cmsg_level = SOL_ALG;
        cmsg->cmsg_type = ALG_SET_IV;
        cmsg->cmsg_len = CMSG_LEN(sizeof(struct af_alg_iv) + 16);
        struct af_alg_iv *alg_iv = (struct af_alg_iv *)CMSG_DATA(cmsg);
        alg_iv->ivlen = 16;
        memcpy(alg_iv->iv, iv, 16);

        // 设置密文
        iov.iov_base = ciphertext;
        iov.iov_len = 16;
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;

        if (sendmsg(opfd, &msg, 0) < 0) {
            perror("sendmsg decrypt");
            return 1;
        }

        memset(decrypted, 0, sizeof(decrypted));
        if (read(opfd, decrypted, 16) < 0) {
            perror("read decrypted");
            return 1;
        }
        printf("解密结果: %s\n", decrypted);
    }

    close(opfd);
    close(tfmfd);
    return 0;
}
