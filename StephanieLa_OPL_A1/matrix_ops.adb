-- code taken from Rosetta Code
-- https://rosettacode.org/wiki/Matrix_multiplication#Ada
-- this file contains the body for the Matrix_Ops package
-- (equiv. to a Matrix_Ops.cpp file for a Matrix_Ops class in C++)
package body Matrix_Ops is
   ---------
   -- "*" --
   ---------
   function "*" (Left, Right : Matrix) return Matrix is
      -- create matrix for the product with all initial values == 0.0
      -- dimensions based on # of columns in left matrix and # of rows in right matrix
      Temp : Matrix(Left'Range(1), Right'Range(2)) := (others =>(others => 0.0));
   begin
      if Left'Length(2) /= Right'Length(1) then -- check if matrix dimensions are compatable
         raise Constraint_Error; -- value goes outside the range
      end if; -- end statement
 
      for I in Left'range(1) loop -- go through columns of left matrix
         for J in Right'range(2) loop -- go through the rows of right matrix
            for K in Left'range(2) loop -- go through the rows of left matrix
               Temp(I,J) := Temp(I,J) + Left(I, K)*Right(K, J); -- multiply the left and right matrices and add temp matrix and assign to temp
            end loop;
         end loop;
      end loop;
      return Temp; -- return product matrix in temp
   end "*"; -- return the function
end Matrix_Ops;
