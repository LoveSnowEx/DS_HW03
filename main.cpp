#include <bits/stdc++.h>
#include "poly.h"
using namespace std;

#define BUFSIZE 4096
#define CLS() { printf("\033[2J\033[1;1H"); }
#define PAUSE() { printf("按下Enter鍵以繼續...");\
				fgetc(stdin); }

char buf[BUFSIZE];

int readline() {
	if(fgets(buf, BUFSIZE, stdin) == NULL) return 0;
	if(strchr(buf, '\n')) *strchr(buf, '\n') = '\0';
	return strlen(buf);
}

char main_hint[] = "1.  新增多項式\n"
					"2.  顯示多項式\n"
					"3.  顯示多項式指定次方之係數\n"
					"4.  新增多項式之非零項\n"
					"5.  移除多項式之非零項\n"
					"6.  相加多項式\n"
					"7.  相減多項式\n"
					"8.  相乘多項式\n"
					"9.  相除多項式\n"
					"10. 結束\n"
					"請輸入您要執行之動作(1˜10)：";


list<pair<Poly, string>> polys;

Poly &get_poly(int idx) {
	return next(polys.begin(), idx)->first;
}

string get_polyname(int idx) {
	return next(polys.begin(), idx)->second;
}

void save_poly(Poly poly, string name) {
	polys.push_back(make_pair(poly, name));
}

void enter_poly() {
	while(1) {
		printf("請輸入一個多項式(ex: 3xˆ2+1)：");
		while(readline() == 0);
		try {
			Poly poly = Poly(buf);
			save_poly(poly, string());
			break;
		}
		catch(const int fail) {
			printf("輸入格式錯誤\n"
				"是否重新輸入? (Y/n): ");
			readline();
			if(*buf == 'n') break;
			else continue;
		}
	}
}

void show_polys() {
	printf("顯示多項式列表: \n");
	for(int i = 0; i < polys.size(); i++) {
		printf("(%d) \"%s\" : %s\n", i+1, get_polyname(i).c_str(), get_poly(i).str().c_str());
	}
	printf("\n");
}

void show_coef() {
	if(polys.size() == 0) {
		printf("目前沒有任何多項式, 請先新增多項式\n");
		return;
	}
	int n;
	do {
		show_polys();
		printf("請選擇目標多項式(1~%ld):", polys.size());
		while(readline() == 0);
		n = strtol(buf, NULL, 10);
		if(n > 0 && n <= polys.size()) break;

		printf("超出輸入範圍,請重新輸入\n");
	} while(1);

	printf("請輸入欲顯示係數之項的次方\n");
	while(readline() == 0);
	int expo = strtol(buf, NULL, 10);
	printf("%d 次項的係數為: %d\n", expo, get_poly(n-1)[expo]);
}


void add_term() {
	if(polys.size() == 0) {
		printf("目前沒有任何多項式, 請先新增多項式\n");
		return;
	}
	int n;
	do {
		show_polys();
		printf("請選擇目標多項式(1~%ld):", polys.size());
		while(readline() == 0);
		n = strtol(buf, NULL, 10);
		if(n > 0 && n <= polys.size()) break;

		printf("超出輸入範圍,請重新輸入\n");
	} while(1);

	printf("請輸入欲新增之非零項的係數及次方(如3x^2需輸入 \"3 2\"):");
	while(readline() == 0);
	char *cur = buf;
	int coef = strtol(cur, &cur, 10), expo = strtol(cur, &cur, 10);
	Poly &target = get_poly(n-1);
	target.add_term(coef, expo);
	printf("多項式已修改成: %s\n", target.str().c_str());
}

void remove_term() {
	if(polys.size() == 0) {
		printf("目前沒有任何多項式, 請先新增多項式\n");
		return;
	}
	int n;
	do {
		show_polys();
		printf("請選擇目標多項式(1~%ld):", polys.size());
		while(readline() == 0);
		n = strtol(buf, NULL, 10);
		if(n > 0 && n <= polys.size()) break;

		printf("超出輸入範圍,請重新輸入\n");
	} while(1);

	printf("請輸入欲新增之非零項的次方(如x^2需輸入 \"2\"):");
	while(readline() == 0);
	char *cur = buf;
	int expo = strtol(cur, &cur, 10);
	Poly &target = get_poly(n-1);
	target.remove_term(expo);
	printf("多項式已修改成: %s\n", target.str().c_str());
}

void poly_add() {
	if(polys.size() == 0) {
		printf("目前沒有任何多項式, 請先新增多項式\n");
		return;
	}
	int n, m;
	show_polys();
	do {
		printf("請選擇第一個多項式(1~%ld):", polys.size());
		while(readline() == 0);
		n = strtol(buf, NULL, 10);
		if(n > 0 && n <= polys.size()) break;

		printf("超出輸入範圍,請重新輸入\n");
	} while(1);
	do {
		printf("請選擇第二個多項式(1~%ld):", polys.size());
		while(readline() == 0);
		m = strtol(buf, NULL, 10);
		if(m > 0 && m <= polys.size()) break;

		printf("超出輸入範圍,請重新輸入\n");
	} while(1);
	Poly result = get_poly(n-1) + get_poly(m-1);
	printf("相加後結果為: %s\n", result.str().c_str());
	printf("是否儲存結果? (Y/n): ");
	readline();

	if(*buf == 'n') return;
	save_poly(result, string());
	printf("儲存成功\n");
}

void poly_sub() {
	if(polys.size() == 0) {
		printf("目前沒有任何多項式, 請先新增多項式\n");
		return;
	}
	int n, m;
	show_polys();
	do {
		printf("請選擇第一個多項式(1~%ld):", polys.size());
		while(readline() == 0);
		n = strtol(buf, NULL, 10);
		if(n > 0 && n <= polys.size()) break;

		printf("超出輸入範圍,請重新輸入\n");
	} while(1);
	do {
		printf("請選擇第二個多項式(1~%ld):", polys.size());
		while(readline() == 0);
		m = strtol(buf, NULL, 10);
		if(m > 0 && m <= polys.size()) break;

		printf("超出輸入範圍,請重新輸入\n");
	} while(1);
	Poly result = get_poly(n-1) - get_poly(m-1);
	printf("相減後結果為: %s\n", result.str().c_str());
	printf("是否儲存結果? (Y/n): ");
	readline();

	if(*buf == 'n') return;
	save_poly(result, string());
	printf("儲存成功\n");
}

void poly_mul() {
	if(polys.size() == 0) {
		printf("目前沒有任何多項式, 請先新增多項式\n");
		return;
	}
	int n, m;
	show_polys();
	do {
		printf("請選擇第一個多項式(1~%ld):", polys.size());
		while(readline() == 0);
		n = strtol(buf, NULL, 10);
		if(n > 0 && n <= polys.size()) break;

		printf("超出輸入範圍,請重新輸入\n");
	} while(1);
	do {
		printf("請選擇第二個多項式(1~%ld):", polys.size());
		while(readline() == 0);
		m = strtol(buf, NULL, 10);
		if(m > 0 && m <= polys.size()) break;

		printf("超出輸入範圍,請重新輸入\n");
	} while(1);
	Poly result = get_poly(n-1) * get_poly(m-1);
	printf("相乘後結果為: %s\n", result.str().c_str());
	printf("是否儲存結果? (Y/n): ");
	readline();

	if(*buf == 'n') return;
	save_poly(result, string());
	printf("儲存成功\n");
}

void poly_div() {
	if(polys.size() == 0) {
		printf("目前沒有任何多項式, 請先新增多項式\n");
		return;
	}
	int n, m;
	show_polys();
	do {
		printf("請選擇第一個多項式(1~%ld):", polys.size());
		while(readline() == 0);
		n = strtol(buf, NULL, 10);
		if(n > 0 && n <= polys.size()) break;

		printf("超出輸入範圍,請重新輸入\n");
	} while(1);
	do {
		printf("請選擇第二個多項式(1~%ld):", polys.size());
		while(readline() == 0);
		m = strtol(buf, NULL, 10);
		if(m > 0 && m <= polys.size()) break;

		printf("超出輸入範圍,請重新輸入\n");
	} while(1);
	auto [qut, rmn] = get_poly(n-1) / get_poly(m-1);
	printf("相除後商為: %s\n", qut.str().c_str());
	printf("相除後餘為: %s\n", rmn.str().c_str());
	printf("是否儲存結果? (Y/n): ");
	readline();

	if(*buf == 'n') return;
	save_poly(qut, string());
	save_poly(rmn, string());
	printf("儲存成功\n");
}

void (*functions[9])() = {
	enter_poly,
	show_polys,
	show_coef,
	add_term,
	remove_term,
	poly_add,
	poly_sub,
	poly_mul,
	poly_div
};

int main_menu() {
	CLS();
	printf("%s", main_hint);
	while(readline() == 0);
	int opt = strtol(buf, NULL, 10);
	if(opt > 0 && opt < 10) {
		(*functions[opt - 1])();
		PAUSE();
		return 1;
	} else {
		return 0;
	}
}

int main( ){
	while(main_menu());
//	readline();
//	Poly p = Poly(buf);
//	printf("%s\n", p.str().c_str());
//	auto pp = make_pair(p, "");
	return 0;
}
