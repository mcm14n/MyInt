// Michael Mullings
// COP 3330-0001
// Assignment 5 - The MyInt class
/* myint.cpp is the implementation file that defines the MyInt class 
and its features. 
*/ 

#include "myint.h"

// Private Functions

// Manages Dynamic Memory Allocation
void MyInt::Reallocate()
{ // Resizes the array so that no more than 5 unused slots are allocated
   max = current+5;
   int* temp = new int[max]; // temporary array

   for (int i = 0; i<current; i++)
      { // copies old array into temporary array
        temp[i] = boundless[i];
      }
   delete [] boundless; // deletes old array
   boundless = temp; // sets the old array to new array
}


// Returns the product of a MyInt object and an int
MyInt MyInt::Multiply(const int& num) const
{
    // Creates a temporary object
    MyInt t = *this; // holds the value of current object
    int over = 0; // int to hold the carry over integer
	
    // loop to mutliply each integer in MyInt array with an int
    for (int i = t.current-1; i >= 0; i--)
        { // assigns product + carry over to end of array
            t.boundless[i] = t.boundless[i]*num+over;
            over = 0; // resets carry over
            if (t.boundless[i] > 9) // a double digit?
                {  // sets carry over to carry over variable
                    over = t.boundless[i]/10;
 			// sets int in array to single digit
                    t.boundless[i] = t.boundless[i]%10;
                    if (i == 0) // is at the start of array?
                        {
                            t.current++; // add digit
			    // is array big enough?
                            if (t.current == t.max) 
                                { // resizes array
                                    t.Reallocate();
                                }
				// moves digits in array one space over
                            for (int c = t.current; c-2 >= 0 ; c--)
                                {
                                    t.boundless[c-1] = t.boundless[c-2];
                                }
				// carry over is assigned to start of 
				// array
                            t.boundless[i] = over;
                        }
                }
        }
    return t; // returns product of Myint and int
}

MyInt MyInt::Divide(const MyInt& big, char oper) const
{
    // creates to temporary objects
    // result hold the quotient, diff the remainder
    MyInt result, diff; 
    int i; // tracks index in "big" array 
    int num = 0; // the number to assign to quotient
 
	// any invalid division of integers, returns 0
    if ((*this) == 0 || big == 0 || (*this) > big)
        return result;
	// dividing by 1, returns the "big" array
    if ((*this) == 1)
        { 
            if (oper == '%') // do you want the remainder?
                return diff; // remainder
            return big; // quotient
        }

     // loop to get an MyInt from "big" that is larger 
     // than the caller
    for (i = 0; i < big.current; i++)
        {
            diff.boundless[i] = big.boundless[i]; // assigns integers to diff array
            if (diff >= (*this)) // is diff big enough?
                {
                    break;
                }
            diff.current++; // increases number of characters
            if (diff.current == diff.max) // is diff big enough?
                {
                    diff.Reallocate(); // resizes diff
                }

        }
    if (i < big.current) i++; // moves index of "big" to next digit

	// loop to find quotient
    while (i <= big.current)
        {   
            while ((*this)*num <= diff) 
            { // how many times does caller go into diff
                num++;
                if ((*this)*num > diff) // does loop go to far?
                    { 
                        num = num-1;
                        break;
                    }
            }

        // assigns num to quotient
        result.boundless[result.current-1] = num;
	// if there are digits left in "big",
         // increase number of digits in quotient
        if (i != big.current) result.current++;

        if (result.current == result.max) // is result big enough?
            {
                result.Reallocate(); // resize result
            }

        diff = diff-((*this)*num); // assigns new divisible to diff 
     
        num = 0; // reset num
        if (diff < (*this)) // is diff smaller?
            { // grab next digit from "big"
                if (diff == 0) // diff is 0, so next digit to add to quotient is 0
                    {
                        result.boundless[current-1] = 0;
                        result.current++;
                        diff = diff+big.boundless[i++];
                    }
                if (diff < (*this)) // is diff smaller?
                    {
                        if (i == big.current) break; // no more digits, so end loop
			// add extra "big" digit to diff array
                        diff.boundless[diff.current++] = big.boundless[i++];
                    }
                if (diff.current == diff.max) // is the array big enough?
                    {
                        diff.Reallocate(); // resizes array
                    }
            }
        }

        if (oper == '%') // do you want the remainder?
            {
                return diff; // returns remainder
            }

    return result; // returns the quotient
}


// Overloaded Operators

// Insertion operator 
ostream& operator<<(ostream& output, const MyInt& integer)
{ // loop to print each element in array
    for (int i = 0; i<integer.current; i++)
        {
            if (integer.boundless[i] >= 0 && integer.boundless[i] <= 9)
                {
                    output << integer.boundless[i];
                }
            else // not a digit, terminate loop
                break;
        }

    return output; // return ostream object
}

// Extraction operator
istream& operator>>(istream& input, MyInt& integer)
{
    char c; // character to hold user input

    delete [] integer.boundless; // delete old array 

    // set object to a blank MyInt
    integer.current = 0; 
    integer.max = integer.current + 5;
    integer.boundless = new int[integer.max];

    do // do while loop to get user input
        {
            if (integer.current == 0) // is first character
                { // ignore leading white space and '0's
                    while(input.peek() == ' ' || input.peek()== '0')
                        {
                            input.ignore();
                        }
                }
            c = input.get(); // get character from input stream
            if (c >= '0' && c <= '9') // is a valid character?
                { // convert character to int and load into array
                    integer.boundless[integer.current++] = c - '0';
                    if (integer.current == integer.max)
                        { // if not enough space, resize
                            integer.Reallocate();
                        }
                }
            else if (c == '\n') // end the loop at a newline
                break;
            else
                { // put invalid characters back on the input stream
                    input.putback(c);
                    break; // end loop
                }
	// while c is proper user input
        }while(c >= '0' && c <= '9');

    return input; // return istream object
}

// less than operator
bool operator< (const MyInt& x, const MyInt& y)
{ 
    bool f = false; // bool to return true and false
    
    // length of each object 	
    int alpha = x.current; // for x
    int beta = y.current; // for y

    if (alpha < beta) // does x have less digits
        {
            f = true; // x has less digits
        }
    else if (alpha == beta)
        { // same number of digits
            for (int i = 0; i < x.current; i++)
                {  // search through array for first distinct digit
		   // for x and y
                    if (x.boundless[i] < y.boundless[i])
                        { // x is smaller
                            f = true;
                            break;
                        }
                    else if (x.boundless[i] > y.boundless[i])
                        { // y is bigger
                            break; // end loop
                        }
                }
        }

    return f; // return bool
}

// greater operator
bool operator> (const MyInt& x, const MyInt& y)
{ 
    return (y < x); // y is smaller
}

// less than or equal to operator
bool operator<= (const MyInt& x, const MyInt& y)
{
    return !(x > y); // not greater than
}

// greater than or equal to 
bool operator>= (const MyInt& x, const MyInt& y)
{ 
    return !(x < y); // return not less than
}

// equals operator
bool operator== (const MyInt& x, const MyInt& y)
{
    return !(x < y || y < x); // not less than or greater than
}

// not equal operator
bool operator!= (const MyInt& x, const MyInt& y)
{
    return !(x == y); // not equal
}

// Arithmetic operators

// Addition operator
MyInt operator+(const MyInt& x, const MyInt& y)
{
    int over = 0; // carry over variable
    MyInt big,small,sum; // temporary objects to hold values
    // Which is the bigger number?
    if (x > y)
        {  // x is bigger
            big = x;
            small = x;
            sum = y;
        }
    else
        { // y is bigger
            big = y;
            small = y;
            sum = x;
        }
    for (int i = small.current-1, j = sum.current-1; i >= 0; i--, j--)
            { // fill array with integers from smaller MyInt
                if (j >= 0)
                    small.boundless[i] = sum.boundless[j];
                else // pad array with zeros for easy addition
                    small.boundless[i] = 0;
            }
    sum = big; // MyInt sum now equals the bigger number
    // to accomodate size of the sum of the addition
    for (int i = big.current-1; i >= 0; i--)
        { // loop will add each integers in each array plus possible 
          // carry over
          sum.boundless[i] = big.boundless[i] + small.boundless[i]+over;
          over = 0;
          if (sum.boundless[i] > 9) // there is a carry over
            { // set over to 1; 
                over = 1;
		// set to integer to single digit
                sum.boundless[i] = sum.boundless[i] - 10;
                if (i == 0) // there is one extra digit to add to array
                    {// increase current
                        sum.current++;
                        if (sum.current == sum.max) // enough room?
                            { // resize sum array
                                sum.Reallocate();
                            }
                        for (int c = sum.current; c-2 >= 0 ; c--)
                            { // shift all digits over one space
                                sum.boundless[c-1] = sum.boundless[c-2];
                            } 
                        sum.boundless[i] = over; // carry over
                    }  // is put into first array slot
            }
        }
    return sum; // return the sum of the addition
}

MyInt operator* (const MyInt& x, const MyInt& y)
{ // temp objects to store values
    MyInt big,small,num,product;

    if (x <= 0 || y <= 0) // multiplying by 0 or negative
        return product; // return 0
    // find the bigger number
    if (x > y)
        {
            big = x;
            small = y;
        }
    else
        {
            big = y;
            small = x;
        }
     
    for (int i = small.current-1, j = 0; i >=0 ; i--, j++)
        { // calls the Multiply function to get the product 
	  // of the MyInt object and the integer in small's array
            num = big.Multiply(small.boundless[i]);
            if (j > 0)  
                { // a zero is insert at the end of the object
		  // after first round of multiplication
                    for (int c = 0; c < j; c++)
                        { // 
                            num.boundless[num.current++] = 0;
                            if (num.current == num.max)
                                { // Memory management
                                    num.Reallocate();
                                }
                        }
                }
            product = product+num; // add each product for each 
        }  // row of multiplication
    return product; // return product of two objects
}

MyInt operator- (const MyInt& x, const MyInt& y)
{ // temporary objects to hold values
    MyInt diff, big, small;
    int d, index;

    if (x < y || x == y) // is x smaller or the same as y
        return diff; // return 0;
    // accommodate necessary size for subtraction 
    diff = x;
    big = x;
    small = x;
    for (int i = x.current-1,j = y.current-1; i >= 0; i--, j--)
        { // small gets the value of y 
            if (j >= 0)
                small.boundless[i] = y.boundless[j];
            else // then padded with zeros for easy subtraction
                small.boundless[i] = 0;
        }

    for(int i = small.current-1, j = big.current-1; i >= 0; i--, j--)
        { // loop for subtraction
            if (big.boundless[j] < small.boundless[i])
                { // the integer in big is smaller, so add 10 to it
                  // and perform subtraction
                    d = ((big.boundless[j]+10)-small.boundless[i]);
                    if (j != 0) // if not first digit 
			 // subtract 1 to account for carry over
                        big.boundless[j-1] = (big.boundless[j-1])-1;
                }
            else // subtract integers
                d = big.boundless[j] - small.boundless[i];
            diff.boundless[j] = d; // put integer in diff array
        }
    if (diff.boundless[0] == 0)
        { // if first digit is zero
            for(int i = 0; i<diff.current; i++)
                { // find first nonzero
                    if (diff.boundless[i] != 0)
                        {
                            index = i;
                            break;
                        }
                }
            for (int j = 0, k = index; k < diff.current; j++, k++)
                { // shift valid number to start of array
                    diff.boundless[j] = diff.boundless[k];
                }
            // get the right amount of digits
            diff.current = diff.current - index;
            diff.Reallocate(); // resize array
        }

    return diff; // return difference
}

// Overloaded division operator
MyInt operator/ (const MyInt& x, const MyInt& y)
{
    // temp object for division
    MyInt quote = y;
    // calls divide function and returns quotient
    return quote.Divide(x);
}

// Overloaded modulus operator          
MyInt operator% (const MyInt& x, const MyInt& y)
{ // temp object for division           
    MyInt quote = y;                
     // calls divide function and return remainder
    return quote.Divide(x,'%');
}


// Member functions

// conversion constructor for int, default constructor
MyInt::MyInt(int primitive)
{   // int for data manipulation of parameter
    int digit = primitive;
    
    // sets new object's members to blank object
    current = 0; 
    max = 0;
    boundless = 0;

    // is parameter 0 or negative?
    if (primitive <= 0)
    {  // sets object to 0
        current = 1;
        max = current + 5;
        boundless = new int[max];

        for (int i = 0; i < current; i++)
            boundless[i] = 0;
    }
    else
        {
         while (digit != 0) // while there are still digits
            {
                digit = digit/10; // seperate digits
                current++; // number of digits
            }
        max = current + 5; // set max
        boundless = new int[max]; // creates new array
	
	// assigns each digit to array
        for (int i = 0; i < current; i++)
            {
                boundless[i] = primitive%10;
                primitive = primitive/10;
            }
	 // reverse array to proper order
        int i = 0;
        int h = current-1;
        do{
            int temp = boundless[i];
            boundless[i] = boundless[h];
            boundless[h]= temp;
            i++;
            h--;
         }while(i!=h&&i<h);
    }

}

// c-string conversion constructor
MyInt::MyInt(const char* num)
{ 
    int index = 0; // variable to store first valid digit 
    for (unsigned int i = 0; i < strlen(num); i++)
        { // loop to find first valid digit
            if (num[i] > '0' && num[i] <= '9')
                {
                    index = i;
                    break;
                }
        }
    
    // set object to the necessary number of digits	
    current  = strlen(num)-index;
    max = current + 5; // set max
    boundless = new int[max]; // creates new array 
     
     // copies valid data to new object
    for (int i = 0; i < current; i++, index++)
        { 
            if (num[index] < '0' || num[index] > '9')
                {
                    current = 1;
                    Reallocate();
                    boundless[0] = 0;
                    break;
                }
            boundless[i] = num[index] - '0'; // converts char to int
        }
}

// Copy Constructor
MyInt::MyInt(const MyInt& data)
{   // copies all primitive data from parameter object
    current = data.current;
    max =  data.max;
    boundless = new int[max]; // creates new array 

    for (int i = 0; i<current; i++)
        { // copies data from parameter's array to this object's array
            boundless[i] = data.boundless[i];
        }
}

// Destructor
MyInt::~MyInt()
{// deletes dynamic array of current object
    delete [] boundless;
}

// Overloaded Assignment Operator
MyInt MyInt::operator=(const MyInt& data)
{ 
    if (this != &data) // are the two objects the same?
        {
            delete [] boundless; // delete old array 

            current = data.current; // set current to current of parameter 
            max = data.max; // set max to max of parameter

            boundless = new int[max]; // creates dynamic array
            for (int i = 0; i < current; i++)
                { // copies data from parameter's array to this object's array
                    boundless[i] = data.boundless[i];
                }
        }
    return *this; // returns copied object
}

// pre-increment operator
MyInt& MyInt::operator++()
{ // increments caller by 1
    *this = *this+1;
	// returns caller
    return *this;
}

// post-incremet operator
MyInt MyInt::operator++(int)
{ // temp object to hold value of caller
    MyInt t = *this;
    *this = *this+1; // adds 1 to caller
    return t; // returns temp object
}



