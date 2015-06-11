#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class Term{

public:

	Term() :
		coefficient{ NULL },
		exponent{ NULL },
		next{ nullptr }
	{}

	Term(int c, int e) :
		coefficient{ c },
		exponent{ e },
		next{ nullptr }
	{}

	void invert()
	{
		coefficient *= -1;
	}

	Term& operator=(const Term input){

		if (this == &input) return *this;

		coefficient = input.coefficient;
		exponent = input.exponent;
		next = input.next;

		return *this;
	}

	Term operator+(const Term input)
	{
		Term output(input.coefficient, input.exponent);
		output.coefficient += coefficient;
		return output;
	}

	Term operator*(const Term input)
	{
		Term output(input.coefficient, input.exponent);
		output.coefficient *= (*this).coefficient;
		output.exponent += exponent;
		return output;
	}

	friend class Polynomial;
	friend ostream& operator<< (ostream& os, Polynomial p);
	friend ostream& operator<< (ostream& os, Term t);

private:

	int coefficient, exponent;
	Term* next;
};

ostream& operator<< (ostream& os, Term t){

	switch (t.coefficient)
	{
		case 0: 
			
			return os;

case_1:
		case 1:

			switch (t.exponent)
			{
				case 0: 
					cout << 1;
					return os;

				case 1: 
					cout << "x";
					return os;

				default:
					cout << "x^" << t.exponent;
					return os;
			}

		default:
			
			cout << t.coefficient;
			goto case_1;
			return os;
	}
}

class Polynomial{

public:

	Polynomial(){

		head = new Term();
		(*head).next = NULL;
		size = 0;
	}

	Polynomial(string input)
	{
		head = new Term();
		(*head).next = NULL;
		size = 0;
		
		cout << "Converstion Construction Activated on " << input << endl;

		stringstream stream(input);

		int c, e;
		while (stream) {
			stream >> c;
			if (!stream) break;
			stream >> e;
			cout << "c:" << c << " e:" << e << endl;
			insert(c, e);
		}
	}

/*	Polynomial(const Polynomial& source) 
	{


	}*/

/*	~Polynomial(){

		Term* current;
		Term* temp;
		current = head;

		while (current->next){

			temp = current->next;
			current->next = current->next->next;
			delete temp;
		}
	}*/

	bool is_empty(){ return !head->next; }

	/* for addition: finds the correct place to insert a term*/
	void insert(int c, int e){

		if (c == 0) return;

		Term* newNode = new Term(c, e);
		Term* current = head;

		/* search for right exponent */
		while (current->next){

			if (current->next->exponent < e)
			{
				newNode->next = current->next;
				current->next = newNode;
				return;
			}

			if (current->next->exponent == e)
			{
				if (current->next->coefficient + newNode->coefficient == 0)
				{
					current->next = current->next->next;
					return;
				}

				current = current->next;
				*current = *current + *newNode;
				return;
			} 
			
			if (current->next->exponent > e) current = current->next;			
		}

		if (!current->next)
		{
			current->next = newNode;
			size++;
		}
	}

	friend ostream& operator<< (ostream& os, Polynomial p);

private:

	Term* head;
	int size;

};

ostream& operator<< (ostream& os, Polynomial p){

	if (p.head->next)
	{
		Term* spot = p.head->next;

		cout << *spot;
		while (spot->next)
		{		
			spot = spot->next;
			cout << " + " << *spot;
		}
	}
	
	return os;
}

int main(){

/*	Polynomial* test = new Polynomial();
	(*test).insert(4, 3);
	(*test).insert(3, 3);
	(*test).insert(4, 4);
	(*test).insert(1, 2);
	(*test).insert(0, 1);
	(*test).insert(1, 0);
	(*test).insert(10, 11);
	cout << *test << endl;*/

	Polynomial* test = new Polynomial("1 2 1 3 3 2 5 8 1 7 6 15 4 9 98 1");
	cout << *test << endl;
	system("pause");
}