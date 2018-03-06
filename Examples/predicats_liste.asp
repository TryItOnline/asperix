#include "lists"

newList1(X) :- #append([a,b,c],[d,e],X).
newList2(X) :- #delNth([a,b,c],1,X).
headElement(X) :- #head([a,b,c],X).
newList3(X) :- #insLast([a,b,c],d,X).
newList4(X) :- #insNth([a,b,c],d,4,X).
lastElement(X) :- #last([a,b,c],X).
size(X) :- #length([a,b,c],X).
yes :- #member(c,[a,b,c]).
reversedList(X) :- #reverse([a,b,c],X).
yes2 :- #subList([c],[a,b,c]).
tailList(X) :- #tail([a,b,c],X).
