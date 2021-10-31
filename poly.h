#include <bits/stdc++.h>
using namespace std;

class Poly {
	class Term {
	public:
		int coef, expo;
		Term() : coef(0), expo(0) {}

		Term(int coef, int expo) : coef(coef), expo(expo) {}

		static Term ZERO() {
			static const Term zero = Term(0, 0);
			return zero;
		}

		static bool cmp_expo(const Term &lhs, const Term &rhs) {
			return lhs.expo > rhs.expo;
		}
		
		
		friend bool operator<(const Term &lhs, const Term &rhs) {
			if(lhs.expo != rhs.expo) return lhs.expo < rhs.expo;
			return lhs.coef < rhs.coef;
		}

		friend bool operator>(const Term &lhs, const Term &rhs) {
			if(lhs.expo != rhs.expo) return lhs.expo > rhs.expo;
			return lhs.coef > rhs.coef;
		}

		friend bool operator==(const Term &lhs, const Term &rhs) {
			return lhs.coef == rhs.coef && lhs.expo == rhs.expo;
		}

		friend Term operator*(Term &lhs, Term &rhs) {
			Term res = Term(lhs.coef*rhs.coef, lhs.expo+rhs.expo);
			return res;
		}
		
		friend Term operator/(Term &lhs, Term &rhs) {
			Term res = Term(lhs.coef/rhs.coef, lhs.expo-rhs.expo);
			return res;
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

	Poly &operator=(const Poly &other) {
		if(this == &other) return *this;
		for(int i = 0; i < other.size; i++) this->terms[i] = other.terms[i];
		this->size = other.size;
		this->sorted = other.sorted;
		this->ss.str(other.ss.str());
		return *this;
	}

	void sort_elem() {
		if(sorted) return;
		sort(terms, terms+size, Term::cmp_expo);
		int i = 0, j = 0;
		while(j < size) {
			terms[i] = terms[j++];
			while(j < size && terms[i].expo == terms[j].expo) {
				terms[i].coef += terms[j].coef;
				j++;
			}
			if(terms[i].coef != 0) i++;
		}
		size = i;
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

	void add_term(Term term) {
		terms[size++] = term;
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

	friend Poly operator+(Poly &lhs, Poly &rhs) {
		lhs.sort_elem(); rhs.sort_elem();
		Poly res = Poly();
		res.size = lhs.size+rhs.size;
		res.sorted = false;
		memcpy(res.terms, lhs.terms, lhs.size*sizeof(Term));
		memcpy(res.terms+lhs.size, rhs.terms, rhs.size*sizeof(Term));
		return res;
	}

	friend Poly operator-(Poly &lhs, Poly &rhs) {
		lhs.sort_elem(); rhs.sort_elem();
		Poly res = Poly();
		res.size = lhs.size+rhs.size;
		res.sorted = false;
		memcpy(res.terms, lhs.terms, lhs.size*sizeof(Term));
		memcpy(res.terms+lhs.size, rhs.terms, rhs.size*sizeof(Term));
		for(int i = lhs.size; i < res.size; i++) res.terms[i].coef *= -1;
		return res;
	}

	friend Poly operator*(Poly &lhs, Poly &rhs) {
		lhs.sort_elem(); rhs.sort_elem();
		Poly res = Poly();
		res.sorted = false;
		for(int i = 0; i < lhs.size; i++) {
			for(int j = 0; j < rhs.size; j++) {
				res.add_term(lhs.terms[i] * rhs.terms[j]);
			}
		}
		return res;
	}

	friend pair<Poly, Poly> operator/(Poly &lhs, Poly &rhs) {
		lhs.sort_elem(); rhs.sort_elem();
		Poly qut = Poly(), rmn = lhs, tmp;
		while(1) {
			rmn.sort_elem();
			if(rmn.size == 0) break; // rmn == 0
			if(rmn.terms[0].expo < rhs.terms[0].expo) break;
			Term q = rmn.terms[0] / rhs.terms[0];
			if(q == Term::ZERO()) {
				if(rmn.terms[0].coef < 0) {
					if(rhs.terms[0].coef > 0) {
						qut.add_term(-1, 0);
						rmn = rmn+rhs;
					}
					else {
						qut.add_term(1, 0);
						rmn = rmn-rhs;
					}
				}
				break;
			}
			tmp = Poly();
			tmp.add_term(q);
			qut = qut+tmp;
			tmp = tmp*rhs;
			rmn = rmn-tmp;
		}
		return {qut, rmn};
	}
};
