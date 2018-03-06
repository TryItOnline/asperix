%%%%%%%%%%%%%%%%%%%%
%% Example Program
%%%%%%%%%%%%%%%%%%%%

%% Command line: ./asperix Examples/P2.aspx 0


p(1..400).

a :- not b.
b :- not a.

:- a.

pa(X) :- a, p(X).
pb(X) :- b, p(X).

aa(X,Y) :- pa(X), pa(Y).
bb(X,Y) :- pb(X), pb(Y).

#hide aa/2.
#hide bb/2.
#hide pa/1.
#hide pb/1.
