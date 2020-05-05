#include <string>
#include "stringUtil.h"

std::string byte2_hex_str(const std::string &src)
{
	const unsigned int l = src.size();
    std::string ret(l * 2, '0');
    for (unsigned int i = 0; i < l; i++) {
        ret[i * 2] = lett16[static_cast<unsigned char>(src[i]) / 16];
        ret[i * 2 + 1] = lett16[static_cast<unsigned char>(src[i]) % 16];
    }
    return ret;
}

std::string hex_str2_byte(const std::string &src)
{
    std::string ret(src.size() / 2, '0');
    unsigned int le16[70 + 1] = { 0 };
    for (auto i = 48; i < 58; i++)
        le16[i] = i - 48;
    le16['A'] = 10; le16['B'] = 11;
    le16['C'] = 12; le16['D'] = 13;
    le16['E'] = 14; le16['F'] = 15;

    for (unsigned int i = 0; i * 2 < src.size(); i++) {
        ret[i] = static_cast<BYTE>(le16[static_cast<unsigned int>(src[i * 2])] * 16 + le16[static_cast<unsigned int>(src[i * 2 + 1])]
        );
    }
    return ret;
}

std::string int2_hex_str(const int &a) {
    std::string ret(2, '0');
    ret[0] = lett16[a / 16];
    ret[1] = lett16[a % 16];

    return ret;
}

std::string word2_string(const Word *src)
{
    std::string ret;
    BYTE a = '0', b = '0', c = '0', d = '0';

    for (auto i = 0; i < 8; i++) {

        a = static_cast<BYTE>(src[i] & 0xFF);
        auto s_a = int2_hex_str(static_cast<int>(a));

        b = static_cast<BYTE>((src[i] >> 8) & 0xFF);
        auto s_b = int2_hex_str(static_cast<int>(b));

        c = static_cast<BYTE>((src[i] >> 16) & 0xFF);
        auto s_c = int2_hex_str(static_cast<int>(c));

        d = static_cast<BYTE>((src[i] >> 24) & 0xFF);
        auto s_d = int2_hex_str(static_cast<int>(d));

        ret += s_d + s_c + s_b + s_a;
    }

    return ret;
}

void pk_cs7(const unsigned char *input, const unsigned int len, const unsigned int n, unsigned char *output)
{
	const auto l = n - len;
    for (unsigned int i = 0; i < len; i++)
        output[i] = input[i];
    for (unsigned int i = 0; i < l; i++) {
        output[i + len] = l;
    }
}

void pk_cs7_2(const char *input, const unsigned int len, const unsigned int n, unsigned char *output)
{
	const auto l = n - len;
    for (unsigned int i = 0; i < len; i++)
        output[i] = static_cast<unsigned char>(input[i]);
    for (unsigned int i = 0; i < l; i++) {
        output[i + len] = l;
    }
}

std::string s_blank(const unsigned int &n)
{
    std::string res;
    for (unsigned int i = 0; i < n; ++i)
        res += " ";
    return res;
}

// std::string utf8Togb18030(const QString& qstr)
// {
//     QTextCodec* pCodec = QTextCodec::codecForName("gb18030");
//     if(!pCodec) return "";
//     QByteArray ar = pCodec->fromUnicode(qstr);
//     std::string cstr = ar.data();
//     return cstr;
// }
//

