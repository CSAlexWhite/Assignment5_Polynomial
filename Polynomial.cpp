#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
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
		case 0: return os;

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

/*	~Polynomial(){

		Term *to_delete, *next;
		to_delete = head;

		while (to_delete){

			next = to_delete->next;
			delete to_delete;
			to_delete = next;
		}
	}*/

	bool is_empty(){ return !head->next; }

	/* for addition: finds the correct place to insert a term*/
	void insert(int c, int e){

		Term* newNode = new Term(c, e);
		Term* current = head;

		/* search for right exponent */
		while (current->next){

			if (current->next->exponent > e) current = current->next;

			if (current->next->exponent == e)
			{
				current = current->next;
				*current = *current + *newNode;
				return;
			}

			if (current->next->exponent < e)
			{
				newNode->next = current->next;
				current->next = newNode;
				return;
			}
		}

		if (!current->next)
		{
			current->next = newNode;
			size++;
		}
	}

	void delete_node(Term* spot){

		Term* temp = spot->next;
		spot->next = spot->next->next;
		delete temp; return;
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

class Something{

private:

	int degree;
	string canonical_form;
	int* coefficients;
	int* powers;

public:

	Something() :
		degree(0),
		canonical_form(NULL),
		coefficients{ new int[0] }, 
		powers{ new int[0] }
	{};

	Something(string equation) :
		canonical_form(equation)
	{
		cout << "Converstion Construction Activated on " << equation << endl;

		int str_length = equation.length()+1;						 
		char* equation_string = new char[str_length];
		strcpy_s(equation_string, str_length, equation.c_str());

		coefficients = new int[str_length / 2];
		powers = new int[str_length / 2];

		int tick = 0, c = 0, p = 0;
		for (int i = 0; i < str_length; i++){

			if (equation_string[i] == ' '){
				continue;
			}

			if (tick % 2 == 0){

				coefficients[c++] = static_cast<int>(equation_string[i]) - 48;
			}

			if (tick % 2 == 1){

				powers[p++] = static_cast<int>(equation_string[i]) - 48;
				
			}

			tick++;
		}

		for (int i = 0; i < sizeof(coefficients); i++){

			cout << coefficients[i] << "x^" << powers[i] << " ";
		}
	};

	Something(const Something& source) :
		canonical_form(source.canonical_form),
		coefficients(source.coefficients),
		powers(source.powers)
	{


	};

	~Something(){

		/*delete[] coefficients;
		delete[] powers;*/
	}

	Something operator+(const Something& source){


	}

	Something operator-(const Something& source){


	}

	Something operator*(const Something& source){


	}

	Something operator=(const Something& source){


	}

	friend ostream& operator<< (ostream& os, Something p);
};

int main(){

/*	string filename, first_line, second_line;

	/*cout << "Please enter filename: ";
	cin >> filename;#1#

	filename = "Text.txt";

	ifstream infile(filename);

	while (!infile.eof()){

		getline(infile, first_line);
		getline(infile, second_line);

		cout << first_line << endl;
		cout << second_line << endl;
		cout << "++++++++++++++++" << endl;

		Something test1 = Something(first_line);
		cout << test1 << endl;
		Something test2 = Something(second_line);
		cout << test2 << endl;
	}

	cout << "DONE!";*/

/*	Term test1(1, 1);
	Term test2(1, 1);
	cout << test1 << endl;
	cout << test2 << endl;
	cout << test1 + test2 << endl;
	cout << test1 * test2 << endl;*/

	Polynomial* test = new Polynomial();
	(*test).insert(4, 3);
	(*test).insert(3, 3);
	(*test).insert(4, 4);
	(*test).insert(1, 2);
	(*test).insert(0, 1);
	(*test).insert(1, 0);
	(*test).insert(10, 11);
	cout << *test << endl;
	system("pause");
}