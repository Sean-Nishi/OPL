README file

Sean Nishi
Homework 1
Languages: Ada, Prolog, C#, Python, Haskell
2/7/2018

Partner: Matt Bonanno

Helpful Resources:
http://rosettacode.org/wiki/Combinations#Prolog
https://www.programiz.com/python-programming/iterator
https://github.com/triska/clpfd
http://www.swi-prolog.org/pldoc/man?section=clpfd


Python: This language was much easier to program in than the rest of the languages because the syntax resembles psuedo code. I was unable to successfully implement an iterator inside a for loop so I used a while loop on line 40 of hw1.py. However after looking at some online python code an iterator can be declared on a list. After that was can use the next() function to manually iterate through all items in the list. This is easy to use because iter(ex_list) and if ex_list = [1, 2, 3] then the iterator will contain 1, then 2 after we use next(), then 3 for the final value.
	When run with k = 10, n = 15 the program finishes in about a second on my tablet and really slows down when testing k = 10, n = 20. The simplicity of this language really help me outline the procedure necessary to finish the program.

Prolog: 
	Tried to run my program by opening prolog, typing combinations(L, 3, 5), writeln(L), fail.
	By using SWI-Prolog I had access to the Constraint Logic Programming over Finite Domains (clpfd) library. One of this library's uses if for solving combination problems like scheduling, planning, and allocation tasks (swi-prolog.org). This language is almost as easy to read as python but I am still struggling to compile the program. Prolog also has a lbrary called lazy_lists that can be used. Prolog was interesting because there seem to be no arrays. Instead we use something similar to linked lists which are manipulated in a recursive way.


