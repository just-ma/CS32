Description of Data Structures
First, there is an integer that keeps track of the depth of the current scope. There is a structure called info that includes two integers for line number and depth and a string for the string value.  The bulk of the data structure is a hash table that’s composed on an array of 7499 vectors of info’s. There is also a vector of integers that keeps record of all positions of variables within scope (and larger scopes) at the moment.


Description of Algorithms
enterScope pushes -1 into the vector to mark the beginning of a scope and increases the depth by 1.

exitScope pops the back off the vector until it reaches -1 which marks the beginning of the scope. At the same time, it removes these values from the hash table. The vector is how it keeps track of which values were in the scope. If the vector reaches length 0, that means it was trying to exit a scope it never entered and thus returns false. At the end, depth decreases by 1.

declare puts the id through a hash function which turns it into a number between 0 and 7499 which corresponds with a bucket. It inserts the value into that bucket if it’s either empty or matches the string value. If the bucket was already taken, the function repeatedly moves one bucket over until it finds a valid bucket and pushes an info containing the line number, depth, and string value to the end of the vector. 

find puts the value through the hash function and goes to the corresponding bucket. It checks if the bucket has a matching string value. If not, it repeatedly goes one bucket over until it finds the value and looks at the back of the vector. If it encounters an empty bucket throughout this algorithm, it returns -1. 


Time Complexities
enterScope() - O(1)
exitScope() - O(N)
declare() - usually O(L) with L = length of id; but during linear probing, it’s O(L+N)
find() -usually O(L) with L = length of id; but during linear probing, it’s O(L+N)


Pseudocode
enterScope() 
Mark beginning of scope through vector
Increment depth

exitScope()
Remove values from hash table in the current scope
Remove all values from vector in the current scope
If loop does not encounter a marking of the beginning of the scope
	Return false
Decrease depth by 1
Return true

declare()
Run string value through hash function which turns string into any integer between 0 and 7499
Use that integer to access a bucket in the hash table
If the bucket is empty
	Insert value
	Return true
If the string value of that bucket does not match 
	Until the value matches or the bucket is empty
	Go to the next bucket
	Loop around if at edge of array
If the depth of the last value inserted is less than the current depth
	Return false
Insert value
Return true

find()
Check if the string value is nothing
Run string value through hash function which turns string into any integer between 0 and 7499
Use that integer to access a bucket in the hash table
If the bucket is empty
	Return -1
If the string value of that bucket does not match 
	Until the value matches or the bucket is empty
	Go to the next bucket
	Loop around if at edge of array
	If the bucket is empty
		Return -1
Return the line number of the info at the back of the vector


Errors
The only error I am aware of is when all 7499 slots of the hash table are filled. It lacks a space expanding algorithm.