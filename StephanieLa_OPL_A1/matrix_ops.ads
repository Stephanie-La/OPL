-- code taken from Rosetta Code
-- https://rosettacode.org/wiki/Matrix_multiplication#Ada
-- this file contains the specs for the Matrix_Ops package
-- (equiv. to a Matrix_Ops.hpp file for a Matrix_Ops class in C++)
package Matrix_Ops is
   -- the line below defines matrix type as a 2D array with any dimensions that stores float values
   type Matrix is array (Natural range <>, Natural range <>) of Float;
   -- "*" is an operator that accepts 2 Matrices and returns another matrix
   function "*" (Left, Right : Matrix) return Matrix;
end Matrix_Ops;
