#include <bits/stdc++.h>
using namespace std;

class Poly {
	class Term {
	public:
		int coef, expo;
		Term() : coef(0), expo(0) {}

		Term(int coef, int expo) : coef(coef), expo(expo) {}

		static bool cmp_expo(const Term &lhs, const Term &rhs) {
			return lhs.expo > rhs.expo;
		}
	};
public:
	Term terms[1024];
	int size;
	bool sorted;
	stringstream ss;

	Poly() : size(0), sorted(false) {}

	 Poly(const Poly &other) {
         if(this == &other) return;
         *this = other;
    }

	Poly(char *s) {
		*this = Poly();
		do {
			int f = 1, coef = 0, expo = 0;

			// read sign
			if(*s == '+') {
				s++;
			}
			if(*s == '-') {
				f = -1;
				s++;
			}

			// read coef
			if(*s == 'x') coef = 1;
			else coef = strtol(s, &s, 10);

			// read expo
			if(*s == 'x') {
				s++;
				if(*s == '^') {
					s++;
					expo = strtol(s, &s, 10);
				}
				else {
					expo = 1;
				}
			}
			else {
				expo = 0;
			}

			if(coef == 0) continue;
			add_term(f*coef, expo);
			printf("coef: %d\n", coef);
			printf("expo: %d\n", expo);
		} while(*s == '+' || *s == '-');
		if(*s != '\0') {
			*this = Poly();
			throw -1;
		}
        sorted = false;
	}

	void sort_elem() {
		sort(terms, terms+size, Term::cmp_expo);
		int i = 0, j = 1;
		while(i < size && j < size) {
			if(terms[i].expo == terms[j].expo) {
				terms[i].coef += terms[j].coef;
				j++;
			}
			else {
				terms[++i] = terms[j++];
			}
		}
		size = i+1;
		while(size && terms[size-1].coef == 0) size--;
		for(int i = 0; i < size; i++) {
			if(terms[i].coef == 0) {
				swap(terms[i], terms[size-1]);
				size--;
			}
		}
		build_string();
		sorted = true;
	}

	void build_string() {
		ss.str(""); ss.clear();
		if(size == 0) {
			ss << "0";
		}
		else {
			for(int i = 0; i < size; i++) {
				int f = (terms[i].coef >= 0) ? 1 : -1, pw = terms[i].expo, cf = abs(terms[i].coef);

				if(f == -1) ss << "-";
				else if(i > 0) ss << "+";

				if(cf != 1 || pw == 0) ss << cf;
				if(pw != 0) {
					ss << "x";
					if(pw != 1) ss << "^" << pw;
				}
			}
		}
	}

	string str() {
		if(sorted) return ss.str();
		sort_elem();
		return ss.str();
	}

	void add_term(int coef, int expo) {
		terms[size++] = Term(coef, expo);
		sorted = false;
	}

	void remove_term(int expo) {
		Term *term = lower_bound(terms, terms+size, Term(0, expo), Term::cmp_expo);
		if(term == terms+size) return;
		else if(term->expo != expo) return;
		term->coef = 0;
		sorted = false;
	}

	int operator[](int expo) {
		Term *term = lower_bound(terms, terms+size, Term(0, expo), Term::cmp_expo);
		if(term == terms+size) return 0;
		else if(term->expo != expo) return 0;
		return term->coef;
	}

	friend Poly operator+(const Poly &lhs, const Poly &rhs) {
		Poly res = Poly();
		for(int i = 0; i < lhs.size; i++) {
			res.terms[res.size++] = lhs.terms[i];
		}
		for(int i = 0; i < rhs.size; i++) {
			res.terms[res.size++] = rhs.terms[i];
		}
		return res;
	}

    Poly &operator=(const Poly &other) {
        if(this == &other) return *this;
        for(int i = 0; i < other.size; i++) this->terms[i] = other.terms[i];
        this->size = other.size;
        this->sorted = other.sorted;
        this->ss.str(other.ss.str());
        return *this;
    }

};
