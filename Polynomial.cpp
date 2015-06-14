/*
Assignment_5 - Polynomial
Queens College, CS780 Advanced OOP in C++
Prepared for Dr. Waxman, June 12, 2015
by Alex White
*/

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class Polynomial;

class Term{

public:

	/* default constructor */
	Term() :
		coefficient{ NULL },
		exponent{ NULL },
		next{ nullptr }
	{}

	/* two-argument constructor */
	Term(int c, int e) :
		coefficient{ c },
		exponent{ e },
		next{ nullptr }
	{}

	/* copy constructor */
	Term(const Term& t) :

		coefficient{ t.coefficient },
		exponent{ t.exponent },
		next{ nullptr }
	{}

	/* overloaded assignment */
	Term& operator=(const Term input){

		if (this == &input) return *this;

		coefficient = input.coefficient;
		exponent = input.exponent;
		next = {nullptr};

		return *this;
	}

	/* for insertion in to polynomial */
	void add_to(const Term input)
	{
		coefficient += input.coefficient;
	}

	/* overloaded + operator */
	Term operator+(const Term input){

		Term output(input.coefficient, input.exponent);
		output.coefficient += coefficient;
		return output;
	}

	/* overloaded * operator */
	Term operator*(const Term input){

		Term output(input.coefficient, input.exponent);
		output.coefficient *= coefficient;
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

/* for nicely printing the terms of the polynomial */
ostream& operator<< (ostream& os, Term t){

	switch (t.coefficient)
	{
		case 0: 
			
			os << 0;
			return os;

		case 1:

			switch (t.exponent)
			{
				case 0: 
					os << 1;
					return os;

				case 1: 
					os << "x";
					return os;

				default:
					os << "x^" << t.exponent;
					return os;
			}

		default:
			
			switch (t.exponent)
			{
			case 0:
				os << t.coefficient;
				return os;

			case 1:
				os << t.coefficient << "x";
				return os;

			default:
				os << t.coefficient << "x^" << t.exponent;
				return os;
			}
	}
}

class Polynomial{

public:

	/* Default Constructor */
	Polynomial(){

		head = new Term();
		(*head).next = new Term(0,0);
		size = 1;
	}

	/* Conversion Constructor */
	Polynomial(string input)
	{
		head = new Term();
		(*head).next = new Term(0,0);
		size = 1;

		stringstream stream(input);

		int c, e;
		while (stream) {
			stream >> c;
			stream >> e;
			if (!stream) break;
			insert(Term(c, e));
		}
	}

	/* Copy Constructor */
	Polynomial(const Polynomial& source) 
	{	
		head = new Term();
		(*head).next = new Term(0,0);
		size = 1;

		Term* current_source = source.head;

		while (current_source->next)
		{
			insert(Term(current_source->next->coefficient, current_source->next->exponent));
			current_source = current_source->next;
		}
	}

	/* Destructor */
	~Polynomial(){
		
		Term *to_delete, *next;
		to_delete = head;

		while (to_delete){

			next = to_delete->next;
			delete to_delete;
			to_delete = next;
		}
	} 

	/* Overloaded Assignment */
	Polynomial& operator= (Polynomial source){

		if (this == &source) return *this;
		head->next = new Term(0,0);

		Term* current_source = source.head;

		while (current_source->next)
		{
			insert(Term(current_source->next->coefficient, current_source->next->exponent));
			current_source = current_source->next;
		}

		return *this;
	}

	/* Check if the polynomial is empty/zero */
	bool is_empty(){ return (head->next->coefficient == 0); }

	/* Insertion: adds terms to a polynomial, updates size */
	void insert(const Term t){

		if (t.coefficient == 0) return;

		Term* newNode = new Term(t);		
		Term* current = head;

		/* search for the place to insert */
		while (current->next){

			/* if next is lower, insert before */
			if (current->next->exponent < newNode->exponent) 
			{
				newNode->next = current->next;
				current->next = newNode;
				size++;
				return;
			}

			/* combine like terms */
			if (current->next->exponent == newNode->exponent)
			{
				/* if they zero each other out, remove the term */
				if (current->next->coefficient + newNode->coefficient == 0)
				{
					current->next = current->next->next;
					size--;
					return;
				}

				current->next->add_to(*newNode);
				return;
			} 
			
			/* if next is greater, keep moving */
			if (current->next->exponent > newNode->exponent) current = current->next;
		}

		/* tack term onto the end, if all else fails */
		if (!current->next)
		{
			current->next = newNode;
			size++;
		}
	}

	/* Addition: adds two polynomials together */
	Polynomial operator+(const Polynomial p) {

		/* copy construct the rhs polynomial */
		Polynomial output(p);

		/* if one operand is zero, return the other */
		if (output.is_empty()) return *this;
		if (is_empty()) return p;

		Term* to_insert = head;

		/* insert lhs polynomial into rhs one 
		   remember that insertion adds like
		   terms automatically */
		while (to_insert->next)
		{
			to_insert = to_insert->next;
			output.insert(Term(to_insert->coefficient, to_insert->exponent));
		}

		return output;
	}

	/* Subtraction: subtracts one polynomial from another 
	   Basically inverted operator+ */
	Polynomial operator-(const Polynomial p) {

		Polynomial output(*this);

		if (output.is_empty()) return p;

		Term* to_insert = p.head;

		while (to_insert->next)
		{
			to_insert = to_insert->next;
			output.insert(Term((-1)*to_insert->coefficient, to_insert->exponent));
		}

		return output;
	}

	/* Multiplication: multiplies two polynomials 
	   For each term in second polynomial, copies
	   the first polynomial and multiplies each of 
	   its terms by the second's term and inserts
	   the results into the output polynomial.
	*/
	Polynomial operator*(const Polynomial p){
		 
		Polynomial output;
		Term* currentA = head;

		/* for each term in A */
		while (currentA->next)
		{
			/* for each term in B, multiply against
			   A's term and insert into output*/
			Term* currentB = p.head;
			while (currentB->next)
			{
				output.insert(*currentA->next * *currentB->next);
				currentB = currentB->next;
			}
			
			currentA = currentA->next;
		}

		return output;
	}

	friend ostream& operator<< (ostream& os, Polynomial p);

private:

	Term* head;
	int size;
};

/* Outputs a nice version of a polynomial */
ostream& operator<< (ostream& os, Polynomial p){

	if (p.head->next)
	{
		Term* spot = p.head->next;

		os << *spot;
		while (spot->next)
		{	
			/* remove any zeros except 
			   if entire polynomial is zero */
			if (p.size > 1 && spot->next->coefficient == 0)
			{
				cout << "SOMETHING" << endl;
				return os;
			}

			/* if term is negative use 
			   built-in minus sign */
			spot = spot->next;
			if (spot->coefficient < 0){

				os << " " << *spot;
				continue;
			}

			/* else add a plus sign */
			os << " + " << *spot;
		}
	}
	
	return os;
}

int main(int argc, char* argv[]){

	Polynomial pair[2];

	string infilename = argv[0];
	string outfilename = argv[1];
	string input_form;

	ofstream myfile;
	myfile.open(outfilename);
	
	int count = 0;
	ifstream infile(infilename);
	while (!infile.eof())
	{
		cout << "\nPROCESSING POLYNOMIAL PAIR " << count << "...\n" << endl;
		myfile << "\nPOLYNOMIAL PAIR " << count++ << "\n" << endl;

		for (int member = 0; member < 2; member++)
		{
			getline(infile, input_form);
			myfile << "Polynomial " << member+1 << " Input Form: \n" << input_form << endl;

			pair[member] = Polynomial(input_form);

			myfile << "Polynomial " << member+1  << " Canonical Form :" << endl;
			myfile << pair[member] << endl;
		}

		myfile << "Addition : " << endl;
		myfile << pair[0] + pair[1] << endl;
		myfile << "Subtraction : " << endl;
		myfile << pair[0] - pair[1] << endl;
		myfile << "Multiplication : " << endl;
		myfile << pair[0] * pair[1] << endl;
		myfile << "= = = = = = = = = = = = = = = =" << endl;

		cout << "...Done" << endl;
	}
	
	cout << endl << "Please see output file \"" << outfilename << "\" for results\n" << endl;
	myfile.close();
	system("pause");
	return 0;
}
