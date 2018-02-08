/* This code is based off of the Ada implementation from rosettacode.org
Sean Nishi
Homework1
*/


% include library
:= use_module(library(clpfd)).

% function creates a list with k elements, members of the list are nums 1 through n (1, 2, ... , n)

combinations(L, k, n) :-
    % length returns true if n is the number of elements in list L
    length(L, k),
    L ins 1..n,
	% creates a chain from a list(L) with respect to the relation (second arg)
    chain(L, #<),
	% label() does a similar funciton to labeling(+options, +vars)
	% label() defaults to labeling the vars in the order they are linked
    label(L).

