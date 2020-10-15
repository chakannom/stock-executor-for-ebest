#pragma once
#pragma warning(disable: 4996)

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	구조체 필드값을 문자열 형태로 변환하는 유틸리티 함수입니다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CStringA _scopy_a(const char* szData, int nSize);

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	문자열을 구조체 필드로 적절히 채우는 유틸리티 함수입니다. (넘치면 잘림)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void _smove_a(char* szTarget, int nSize, CStringA strSource);

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	 숫자에 천 단위로 컴마 삽입하는 유틸리티 함수입니다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CStringA _comma_a(const char* szData, int nSize, int decimal = 0);

#define SCOPY_A(x) _scopy_a(x, sizeof x)
#define SMOVE_A(fTarget, strSource) _smove_a(fTarget, sizeof fTarget, strSource)
#define COMMA_A(x) _comma_a(x, sizeof x)
#define COMMAF_A(x, d) _comma_a(x, sizeof x, d)