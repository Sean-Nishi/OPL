//Modified and Documented code from http://rosettacode.org/wiki/Combinations#C.23
using System;
using System.Collections.Generic;
 
public class Program
{
    public static void Combinations(int k, int n)
    {
            int[] result = new int[k]; //create an array to contain 1 combination at a time of length k
            Stack<int> stack = new Stack<int>();
            stack.Push(0);
 
            while (stack.Count > 0)
           {
                int index = stack.Count - 1; //stack will have max of k-1 and we need to subtract one to use this as an index for an array.
                int value = stack.Pop(); //takes the value from the stack to use for the next combination
 
                while (value < n) 
               {
                    result[index++] = value++; //value starts at 1 after value++ is evaluated
                    stack.Push(value); //save the evaluated value++ from above to the stack to be used for the next combination 
                                      //e.g. 1++ will save 2  for the two combos
 
                    if (index == k) //Once the array is full, print the output. 
                    {
                        Console.WriteLine(string.Join(",", result)+ "\n"); //output result of array
                        break;
                    }
                }
            }
    }
 
    static void Main(string[] args)
    {
        if(args.Length < 2) {
            Console.WriteLine("please run using: mono Program.exe k n.");
            Console.WriteLine("k  and n are integer values");
            return;
        }
        int k = Convert.ToInt32(args[0]);
        int n = Convert.ToInt32(args[1]);
        Combinations(k,n);
    }
}