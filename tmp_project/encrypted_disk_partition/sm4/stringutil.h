#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <string>
// #include <QTextCodec>
// #include <QByteArray>

typedef unsigned char BYTE;
typedef unsigned int Word;

const static BYTE lett16[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                              '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

std::string byte2_hex_str(const std::string &src);

std::string hex_str2_byte(const std::string &src);

std::string word2_string(const Word *src);

void pk_cs7(const unsigned char *input, const unsigned int len, const unsigned int n, unsigned char *output);
void pk_cs7_2(const char *input, const unsigned int len, const unsigned int n, unsigned char *output);

std::string s_blank(const unsigned int &n);

// std::string utf8Togb18030(const QString& qstr);

#endif // STRINGUTIL_H
