%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Towers of Hanoi (improved version)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Command line: ./asperix Examples/hanoiBIS.aspx 0


%------ Initial settings

number_of_moves(80).

%largest_disc(2).
largest_disc(4).
%largest_disc(5).
%largest_disc(6).

%------ Initial state

%initial_state(towers(l(2,l(1,nil)),nil,nil)).
initial_state(towers(l(4,l(3,l(2,l(1,nil)))),nil,nil)).
%initial_state(towers(l(5,l(4,l(3,l(2,l(1,nil))))),nil,nil)).
%initial_state(towers(l(6,l(5,l(4,l(3,l(2,l(1,nil)))))),nil,nil)).

% ------ Goal state

%goal(towers(nil, nil, l(2,l(1,nil)))).
goal(towers(nil, nil, l(4,l(3,l(2,l(1,nil)))))).
%goal(towers(nil, nil, l(5,l(4,l(3,l(2,l(1,nil))))))).
%goal(towers(nil, nil, l(6,l(5,l(4,l(3,l(2,l(1,nil)))))))).

% ------ all discs involved ------

disc(1..X) :- largest_disc(X).


% ------ legal stacks ------

legalStack(nil).
legalStack(l(T,nil)) :- disc(T).
legalStack(l(T,l(T1,S))) :- legalStack(l(T1,S)), disc(T), T > T1.


% ------ possible moves ------

possible_state(0,towers(S1,S2,S3)) :- initial_state(towers(S1,S2,S3)), legalStack(S1), legalStack(S2), legalStack(S3).
possible_state(I,towers(S1,S2,S3)) :- possible_move(I,T,towers(S1,S2,S3)).

% From stack one to stack two.

possible_move(I+1,towers(l(X,S1),S2,S3),towers(S1,l(X,S2),S3)) :-  possible_state(I,towers(l(X,S1),S2,S3)),
                               legalMoveNumber(I), legalStack(l(X,S2)), not ok(I).

% From stack one to stack three.

possible_move(I+1,towers(l(X,S1),S2,S3),towers(S1,S2,l(X,S3))) :-  possible_state(I,towers(l(X,S1),S2,S3)),
                               legalMoveNumber(I), legalStack(l(X,S3)), not ok(I).

% From stack two to stack one.

possible_move(I+1,towers(S1,l(X,S2),S3),towers(l(X,S1),S2,S3)) :-  possible_state(I,towers(S1,l(X,S2),S3)),
                               legalMoveNumber(I), legalStack(l(X,S1)), not ok(I).

% From stack two to stack three.

possible_move(I+1,towers(S1,l(X,S2),S3),towers(S1,S2,l(X,S3))) :-  possible_state(I,towers(S1,l(X,S2),S3)),
                               legalMoveNumber(I), legalStack(l(X,S3)), not ok(I).

% From stack three to stack one.

possible_move(I+1,towers(S1,S2,l(X,S3)),towers(l(X,S1),S2,S3)) :-  possible_state(I,towers(S1,S2,l(X,S3))),
                               legalMoveNumber(I), legalStack(l(X,S1)), not ok(I).

% From stack three to stack two.

possible_move(I+1,towers(S1,S2,l(X,S3)),towers(S1,l(X,S2),S3)) :-  possible_state(I,towers(S1,S2,l(X,S3))),
                               legalMoveNumber(I), legalStack(l(X,S2)), not ok(I).

%------ actual moves ------
% a solution exists if and only if there is a "possible_move"
% leading to the goal.
% in this case, starting from the goal, we proceed backward
% to the initial state to single out the full set of moves.

% Choose from the possible moves.

move(I,towers(S1,S2,S3)) :- goal(towers(S1,S2,S3)), possible_state(I,towers(S1,S2,S3)).

ok(I) :- move(I,towers(S1,S2,S3)), goal(towers(S1,S2,S3)).
win :- ok(I).
:- not win.


move(I-1,towers(S1,S2,S3)) :- move(I,towers(A1,A2,A3)),
                              possible_move(I,towers(S1,S2,S3),towers(A1,A2,A3)),
                              not nomove(I-1,towers(S1,S2,S3)).

nomove(I-1,towers(S1,S2,S3)) :- move(I,towers(A1,A2,A3)),
                                possible_move(I,towers(S1,S2,S3),towers(A1,A2,A3)),
                                not move(I-1,towers(S1,S2,S3)).

%------ precisely one move at each step ------

moveStepI(I) :- move(I,T).

:- legalMoveNumber(I), ok(J), I<J, not moveStepI(I).

:- legalMoveNumber(I), move(I,T1), move(I,T2), T1!=T2.

legalMoveNumber(0).
legalMoveNumber(I+1) :- legalMoveNumber(I), number_of_moves(J), I < J.



#hide.
#show move/2.

