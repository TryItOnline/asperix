%%%%%%%%%%%%%%%%%%%%%%%%%
%% Compute of Fibonacci
%%%%%%%%%%%%%%%%%%%%%%%%%

%% Predicates:
%% - fibo(X,Y): 		Y is the value of X in the Fibonacci sequence
%% - tocomputefibo(X): 	the value to compute in the Fibonacci sequence

%% Command line: ./asperix Examples/fibo.aspx 0


fibo(0,0).
fibo(1,1).

tocomputefibo(10).

fibo(N+2,F1+F) :- fibo(N,F), fibo(N+1,F1), tocomputefibo(K), K > N+1.

