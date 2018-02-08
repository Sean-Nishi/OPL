#Python attempt at hw
#input: 2 ints, k and n
#output: prints the k combinations from 1 to n

#included libraries
import sys
import array

#------------------------------------------
def combinations(arr, n, k):
    #create temp array of length k to hold values to be printed. Initialized to 0
    temp = [0] * int(k)

    #start and end markers
    #don't want off-by-one error
	#These variables are unneccessary but help clarify
    start = 0
    end = n-1
    index = 0

    hard_work(arr, temp, start, end, index, k)

#------------------

#input: array, storage array, start of array, end of array, index in array, array length
#output: prints completed functions
def hard_work(arr, temp, start, end, index, k):
    #if we finished a combination, print it
    #aka. if the index is at the end of the temp array
    if(index == k):
        i = 0
	    #print each value in the temp array
        while(i < k):
            print(temp[i], end = '')
            i = i+1
        print('\n')
        return

	#should be using an iterator in a for loop here. However, I am more comfortable with a while loop
    x = start
    while(x <= end and (end-x+1 >= k-index)):
		#assign value to the array
        temp[index] = arr[x]
	    #for recursion we need increment the start of the array
	    #also need to increment the index
        hard_work(arr, temp, x+1, end, index+1, k)
        x = x+1

#-----------------------------------------
#THIS IS NEVER CALLED! THIS IS A TEST FUNCTION
#combination function taking n and assuming k = 2
#input: k, n
#output: 
def comb(k, n):
    #k and n val check
    print('k = ', int(k), '\nn = ', int(n))
    i = 1
    j = i+1

    #2 loops to get 2 values to print
    #increments i and j
    while(i < n):
        while (j <= n):
	    #print values i and j
            print(int(i), int(j), '\n')
            j = j+1
        i = i+1
        j = i+1
        
#-----------------------------------------

#main()
#check for correct input
if len(sys.argv) != 3:
    sys.stderr.write("Format: %s k n\n" % sys.argv[0])
    sys.exit(1)

#check that n >= k
if int(sys.argv[1]) > int(sys.argv[2]):
    sys.stderr.write("ERROR: n must be equal to or greater than k\n")
    sys.exit(1)

#assignment
k = int(sys.argv[1])
n = int(sys.argv[2])

#create an array and initialize all values from 1 to n
i = 1
arr = []
while (i <= n):
    arr.append(int(i))
    i = i+1

#call fcn
combinations(arr, n, k)

#test function
#comb(k, n)


