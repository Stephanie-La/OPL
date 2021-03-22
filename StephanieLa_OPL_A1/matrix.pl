% code taken from Rosetta Code 
% https://rosettacode.org/wiki/Matrix_multiplication#Prolog
% SWI-Prolog has transpose/2 in its clpfd library
:- use_module(library(clpfd)).
 
% N is the dot product of lists V1 and V2.
% * maplist passes V1, V2, and P to product(), which multiplies V1*V2 and stores
%       the products in P
% * ssumlist returns true if N is the sum of all number in P
dot(V1, V2, N) :- maplist(product,V1,V2,P), sumlist(P,N).
% * product multiplies N1 * N2 and returns N3
product(N1,N2,N3) :- N3 is N1*N2.
 
% Matrix multiplication with matrices represented
% as lists of lists. M3 is the product of M1 and M2

% * mmult first transposes M2, which results with MT
% * the function then transitions to maplist(), which passes MT, M1, and M3
%       into mm_helper, which handles matrix multiplication and returns the
%       product matrix in M3
mmult(M1, M2, M3) :- transpose(M2,MT), maplist(mm_helper(MT), M1, M3).
% * mm_helper passes I1(M1), M2, and M3 into dot(), which finds the dot product
%        of M1 * M2 and returns M3
mm_helper(M2, I1, M3) :- maplist(dot(I1), M2, M3).t(dot(I1), M2, M3).