/*
 * CJsonScanner.cpp
 *
 *  Created on: 10.12.2015
 *      Author: mnl
 */

#include <nsp/CJsonScanner.h>
#include <string>

using namespace std;

namespace APT {

CJsonScanner::CJsonScanner(std::istream &input) : jsonFlexLexer(&input) {
  token = 0;
}

CJsonScanner::~CJsonScanner() {
  if (token != 0) {
    delete token;
  }
}

CJsonToken *CJsonScanner::nextToken() {
  if (token != 0) {
    delete token;
    token = 0;
  }
  int scanResult = yylex();
  if (scanResult == -1) {
    string illegalChar(YYText());
    throw illegalChar;
  }
  return token;
}

int CJsonScanner::scannedLine() { return yylineno; }

} /* namespace APT */

int yyFlexLexer::yywrap() { return 1; }
