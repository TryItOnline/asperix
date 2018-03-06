%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% A "generic" system to compute recursive arithmetic functions
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Command line: ./asperix Examples/generic.aspx 0


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% definition of fibonacci 
value(fibo(0),0).
value(fibo(1),1).
value(fibo(N+2),R+R1) :- value(fibo(N),R), N1=N+1, value(fibo(N1),R1), not ok(fibo(N1)).

% the values to compute for Fibonacci
tocompute(fibo(20)).
tocompute(fibo(10)).
% and the non maximum values to compute
nmaxtocompute(fibo(N1)):- tocompute(fibo(N1)), tocompute(fibo(N2)), N2>N1.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% definition of exponentiation
value(expo(X,0), 1) :- tocompute(expo(X,N)).
value(expo(X, N+1), R*X) :- value(expo(X, N), R), not ok(expo(X,N)).

% the values to compute for exponentiation
tocompute(expo(2,10)).
tocompute(expo(2,15)).
tocompute(expo(4,10)).
% and the non maximum values to compute
nmaxtocompute(expo(X,N1)):- tocompute(expo(X,N1)), tocompute(expo(X,N2)), N2>N1.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% the limit of computation for each recursive fonction Phi
maxtocompute(Phi) :- tocompute(Phi), not nmaxtocompute(Phi).

% to detect when all necessary computations are done for one function Phi
ok(Phi) :- maxtocompute(Phi), value(Phi, R).

% to collect the results
result(C,R) :- tocompute(C), value(C,R).


#hide.
%#show value/2.
#show result/2.
