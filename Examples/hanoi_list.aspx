%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Towers of Hanoi (list version)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Command line: ./asperix Examples/hanoi_list.aspx 0


%------ Initial settings

number_of_moves(100).

largest_disc(4).

%------ Initial state

initial_state(towers([4,3,2,1],[],[])).

% ------ Goal state

goal(towers([],[],[4,3,2,1])).

% ------ all discs involved ------

disc(1..X) :- largest_disc(X).

% ------ legal stacks ------

legalStack([]).
legalStack([T]) :- disc(T).
legalStack([T|[T1|S]]) :- legalStack([T1|S]), disc(T), T > T1.


% ------ possible moves ------

possible_state(0,towers(S1,S2,S3)) :- initial_state(towers(S1,S2,S3)).
possible_state(I,towers(S1,S2,S3)) :- possible_move(I,T,towers(S1,S2,S3)), legalStack(S1), legalStack(S2), legalStack(S3).

% From stack one to stack two.

possible_move(I+1,towers([X|S1],S2,S3),towers(S1,[X|S2],S3)) :-  possible_state(I,towers([X|S1],S2,S3)),
        legalMoveNumber(I), legalStack([X|S2]).

% From stack one to stack three.

possible_move(I+1,towers([X|S1],S2,S3),towers(S1,S2,[X|S3])) :-  possible_state(I,towers([X|S1],S2,S3)),
	legalMoveNumber(I), legalStack([X|S3]).

% From stack two to stack one.

possible_move(I+1,towers(S1,[X|S2],S3),towers([X|S1],S2,S3)) :-  possible_state(I,towers(S1,[X|S2],S3)),
	legalMoveNumber(I),legalStack([X|S1]).

% From stack two to stack three.

possible_move(I+1,towers(S1,[X|S2],S3),towers(S1,S2,[X|S3])) :-  possible_state(I,towers(S1,[X|S2],S3)),
	legalMoveNumber(I),legalStack([X|S3]).

% From stack three to stack one.

possible_move(I+1,towers(S1,S2,[X|S3]),towers([X|S1],S2,S3)) :-  possible_state(I,towers(S1,S2,[X|S3])),
	legalMoveNumber(I),legalStack([X|S1]).

% From stack three to stack two.

possible_move(I+1,towers(S1,S2,[X|S3]),towers(S1,[X|S2],S3)) :-  possible_state(I,towers(S1,S2,[X|S3])),
	legalMoveNumber(I),legalStack([X|S2]).

%------ actual moves ------
% a solution exists if and only if there is a "possible_move"
% leading to the goal.
% in this case, starting from the goal, we proceed backward
% to the initial state to single out the full set of moves.

% Choose from the possible moves.

move(I,towers(S1,S2,S3)) :- goal(towers(S1,S2,S3)), possible_state(I,towers(S1,S2,S3)).

move(I-1,towers(S1,S2,S3)) :- move(I,towers(A1,A2,A3)),
        possible_move(I,towers(S1,S2,S3),towers(A1,A2,A3)),
        not nomove(I-1,towers(S1,S2,S3)).

nomove(I-1,towers(S1,S2,S3)) :- move(I,towers(A1,A2,A3)),
        possible_move(I,towers(S1,S2,S3),towers(A1,A2,A3)),
        not move(I-1,towers(S1,S2,S3)).

%------ precisely one move at each step ------

moveStepI(I) :- move(I,T).

:- legalMoveNumber(I), not moveStepI(I).

:- legalMoveNumber(I), move(I,T1), move(I,T2), T1!=T2.

legalMoveNumber(0).
legalMoveNumber(I+1) :- legalMoveNumber(I), number_of_moves(J), I < J.



#hide.
#show move/2.

