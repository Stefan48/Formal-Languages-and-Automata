#include <bits/stdc++.h>

// number of digits used to store big numbers
#define NRDIGITS 160
// macros for every instruction
#define NOP 0
#define INPUT 1
#define ROT 10
#define SWAP 11
#define PUSH 12
#define RROT 100
#define DUP 101
#define ADD 102
#define LBRACE 110
#define OUTPUT 111
#define MULTIPLY 112
#define EXECUTE 120
#define NEGATE 121
#define POP 122
#define RBRACE 123

using namespace std;

struct Number
{
	bool sign;
	int digits[NRDIGITS];
	
	Number()
	{
		sign = false;
		for (int i = 0; i < NRDIGITS; ++i)
		{
			digits[i] = 0;
		}
	}
	
	Number(long long int nr)
	{
		if (nr < 0)
		{
			sign = true;
			nr = -nr;
		}
		else
		{
			sign = false;
		}
		int i = NRDIGITS - 1;
		while (nr > 0)
		{
			digits[i] = nr % 10;
			i--;
			nr /= 10;
		}
		while (i >= 0)
		{
			digits[i] = 0;
			i--;
		}
	}
	
	void operator =(const Number &other)
	{ 
    	sign = other.sign;
        for (int i = 0; i < NRDIGITS; ++i)
        {
        	digits[i] = other.digits[i];
        }
    }
    
    Number operator+(const Number &other)
    {
      	Number number = AddNumbers(*this, other);
      	return number;
    }
    
    Number operator*(const Number &other)
    {
      	Number number = MultiplyNumbers(*this, other);
      	return number;
    }
    
    bool operator==(const Number &other)
    {
    	return CompareNumbers(*this, other) == 0;
    }
    
    bool operator!=(const Number &other)
    {
    	return CompareNumbers(*this, other) != 0;
    }
    
    friend ostream& operator<<(ostream& os, const Number &number);
    friend istream& operator>>(istream& in, Number &number);
    
	static int CompareNumbers(const Number &x, const Number &y)
	{
		// returns 1 if x > y, -1 if x < y, 0 if x == y
		if (!x.sign && y.sign)
		{
			return 1;
		}
		if (x.sign && !y.sign)
		{
			return -1;
		}
		for (int i = 0; i < NRDIGITS; ++i)
		{
			if (x.digits[i] > y.digits[i])
			{
				return x.sign ? -1 : 1;
			}
			else if (x.digits[i] < y.digits[i])
			{
				return x.sign ? 1 : -1;
			}
		}
		return 0;
	}	
	
	static Number AddNumbers(Number x, Number y)
	{
		Number rez;
		if (x.sign == y.sign)
		{
			rez.sign = x.sign;
			int t = 0, aux;
			for (int i = NRDIGITS - 1; i >= 0; --i)
			{
				aux = x.digits[i] + y.digits[i] + t;
				rez.digits[i] = aux % 10;
				t = aux / 10;
			}
		}
		else
		{
			Number neg;
			if (x.sign)
			{
				neg = x;
				x = y;
				y = neg;
			}
			else
			{
				neg = y;
			}
			// now x is positive and y is negative
			neg.sign = !neg.sign;
			int cmp = CompareNumbers(x, neg);
			if (cmp == 0)
			{
				return Number(0);
			}
			if (cmp == 1)
			{
				rez.sign = false;
			}
			else
			{
				rez.sign = true;
				y = x;
				x = neg;
			}
			// compute x - y
			int t = 0, aux;
			for (int i = NRDIGITS - 1; i >= 0; --i)
			{
				aux = x.digits[i] - y.digits[i];
				if (t)
				{
					aux--;
					t = 0;
				}
				if (aux < 0)
				{
					rez.digits[i] = aux + 10;
					t = 1;
				}
				else
				{
					rez.digits[i] = aux;
				}
			}
		}
		return rez;
	}
	
	static Number MultiplyNumbers(Number x, Number y)
	{
		Number rez;
		rez.sign = (x.sign == y.sign) ? false : true;
		// compute first non-zero digit's position for x and y
		int stopx = 0, stopy = 0;
		while (stopx < NRDIGITS && x.digits[stopx] == 0)
		{
			stopx++;
		}
		if (stopx == NRDIGITS)
		{
			// x is 0
			return Number(0);
		}
		stopx--;
		while (stopy < NRDIGITS && y.digits[stopy] == 0)
		{
			stopy++;
		}
		if (stopy == NRDIGITS)
		{
			// y is 0
			return Number(0);
		}
		int pos, t, aux;
		// y's digits
		for (int i = NRDIGITS - 1; i >= stopy; --i)
		{
			pos = i;
			t = 0;
			// x's digits
			for (int j = NRDIGITS - 1; j >= stopx; --j)
			{
				aux = y.digits[i] * x.digits[j] + t;
				rez.digits[pos] += aux % 10;
				t = aux / 10;
				pos--;
			}
		}
		for (int i = NRDIGITS - 1; i > 0; --i)
		{
			rez.digits[i - 1] += rez.digits[i] / 10;
			rez.digits[i] %= 10;
		}
		return rez;
	}
};

ostream& operator<<(ostream &os, const Number &number)
{
	if (number.sign)
	{
		os << '-';
	}
	int i = 0;
	while (number.digits[i] == 0 && i < NRDIGITS-1)
	{
		i++;
	}
	while (i < NRDIGITS)
	{
		os << number.digits[i];
		i++;
	}
	return os;
}

istream& operator>> (istream &in, Number &number)
{
	string str;
	in >> str;
	int len = str.length(), d = NRDIGITS - 1;
	number.sign = false;
	for (int i = len - 1; i >= 0; --i)
	{
		if (str[i] == '-')
		{
			number.sign = true;
			break;
		}
		else if (isdigit(str[i]))
		{
			number.digits[d--] = str[i] - '0';
		}
		else
		{
			number.digits[d--] = str[i] - 55;
		}
	}
	while (d >= 0)
	{
		number.digits[d--] = 0;
	}
    return in;
}

int base;
deque<Number> stk;

void PrintStack()
{
	for (auto it = stk.begin(); it != stk.end(); ++it)
	{
		cout << *it << ' ';
	}
	cout << '\n';
}

int DecodeInstruction(string code)
{
	int digit[4];
	digit[0] = 0;
	int next = 1;
	bool repeated;
	for (int i = 1; i < 4; ++i)
	{
		repeated = false;
		for (int j = 0; j < i; ++j)
		{
			if (code[i] == code[j])
			{
				repeated = true;
				digit[i] = digit[j];
				break;
			}
		}
		if (!repeated)
		{
			digit[i] = next++;
		}
	}
	int instruction = digit[3] + 10 * digit[2] + 100 * digit[1];
	return instruction;
}

int DecodeInstructionFromStack(Number numbers[4])
{
	int digit[4];
	digit[0] = 0;
	int next = 1;
	bool repeated;
	int cmp;
	for (int i = 1; i < 4; ++i)
	{
		repeated = false;
		for (int j = 0; j < i; ++j)
		{
			cmp = Number::CompareNumbers(numbers[i], numbers[j]);
			if (cmp == 0)
			{
				repeated = true;
				digit[i] = digit[j];
				break;
			}
		}
		if (!repeated)
		{
			digit[i] = next++;
		}
	}
	int instruction = digit[3] + 10 * digit[2] + 100 * digit[1];
	return instruction;
}

bool ExecuteInstruction(int instruction)
{
	// returns false if the execution completed successfully
	switch (instruction)
	{
		case INPUT:
		{
			Number number;
			cin >> number;
			for (int i = 0; i < NRDIGITS; ++i)
			{
				if (number.digits[i] >= base)
				{
					return true;
				}
			}
			stk.push_front(number);
			break;
		}
		
		case ROT:
		{
			if (stk.empty())
			{
				return true;
			}
			stk.push_back(stk.front());
			stk.pop_front();
			break;
		}
		
		case SWAP:
		{
			if (stk.size() < 2)
			{
				return true;
			}
			swap(stk[0], stk[1]);
			break;
		}
		
		case PUSH:
		{
			stk.push_front(Number(1));
			break;
		}
		
		case RROT:
		{
			if (stk.empty())
			{
				return true;
			}
			stk.push_front(stk.back());
			stk.pop_back();
			break;
		}
		
		case DUP:
		{
			if (stk.empty())
			{
				return true;
			}
			stk.push_front(stk.front());
			break;
		}
		
		case ADD:
		{
			if (stk.size() < 2)
			{
				return true;
			}
			Number x = stk.front();
			stk.pop_front();
			Number y = stk.front();
			stk.pop_front();
			Number sum = x + y;
			stk.push_front(sum);
			break;
		}
		
		case OUTPUT:
		{
			if (stk.empty())
			{
				return true;
			}
			cout << stk.front() << '\n';
			stk.pop_front();
			break;
		}
		
		case MULTIPLY:
		{
			if (stk.size() < 2)
			{
				return true;
			}
			Number x = stk.front();
			stk.pop_front();
			Number y = stk.front();
			stk.pop_front();
			Number prod = x * y;
			stk.push_front(prod);
			break;
		}
		
		case NEGATE:
		{
			if (stk.empty())
			{
				return true;
			}
			if (stk.front() != Number(0))
			{
				stk.front().sign = !stk.front().sign;
			}
			break;
		}
		
		case POP:
		{
			if (stk.empty())
			{
				return true;
			}
			stk.pop_front();
			break;
		}
		
		case EXECUTE:
		{
			if (stk.size() < 4)
			{
				return true;
			}
			Number numbers[4];
			for (int i = 0; i < 4; ++i)
			{
				numbers[i] = stk.front();
				stk.pop_front();
			}
			int instruction = DecodeInstructionFromStack(numbers);
			if (instruction == LBRACE || instruction == RBRACE)
			{
				return true;
			}
			return ExecuteInstruction(instruction);
		}
		
		
		default: break;
	}
	return false;
}



int main(int argc, char *argv[])
{
	string code;
	ifstream in(argv[1]);
	in >> code;
	in.close();
	base = argv[2] == NULL ? 10 : atoi(argv[2]);
	
	int len = code.length();	
	// check if all instructions are complete
	if (len % 4)
	{
		cerr << "Error:" << len / 4 << '\n';
		return -1;
	}
	
	vector<int> instructions;
	int instruction;
	stack<char> parantheses;
	
	// parse code and check if the parantheses are correct
	// decoded instructions are stored in the instructions vector
	for (int i = 0; i < len; i += 4)
	{
		instruction = DecodeInstruction(code.substr(i, 4));
		instructions.push_back(instruction);
		if (instruction == LBRACE)
		{
			parantheses.push('(');
		}
		else if (instruction == RBRACE)
		{
			if (parantheses.empty())
			{
				cerr << "Error:" << i / 4 << '\n';
				return -1;
			}
			else
			{
				parantheses.pop();
			}
		}
	}
	if (!parantheses.empty())
	{
		cerr << "Error:" << len / 4 << '\n';
		return -1;
	}
	
	// execute instructions
	bool err;
	stack<int> eip;
	bool skip = false;
	for (int i = 0; i < instructions.size(); ++i)
	{
		if (skip)
		{
			if (instructions[i] == LBRACE)
			{
				
				parantheses.push('(');
			}
			else if (instructions[i] == RBRACE)
			{
				parantheses.pop();
				if (parantheses.empty())
				{
					skip = false;
				}
			}
		}
		else
		{
			if (instructions[i] == LBRACE)
			{
				if (stk.empty())
				{
					cerr << "Exception:" << i << '\n';
					return -2;
				}
				Number number = stk.front();
				if (number == Number(0))
				{
					skip = true;
					parantheses.push('(');
				}
				else
				{
					eip.push(i);
				}
			}
			else if (instructions[i] == RBRACE)
			{
				// return to the matching LBRACE
				i = eip.top() - 1;
				eip.pop();
			}
			else
			{
				err = ExecuteInstruction(instructions[i]);
				if (err)
				{
					cerr << "Exception:" << i << '\n';
					return -2;
				}
			}
		}
	}
	return 0;
}
