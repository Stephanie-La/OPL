(* code taken from Rosetta Code *)
(* https://rosettacode.org/wiki/Matrix_multiplication#OCaml *)
let matrix_multiply x y = (* function to matrix multiply arrays x and y *)
  let x0 = Array.length x (* x0 = length of x *)
  and y0 = Array.length y in (* y0 = length of y *)
  let y1 = if y0 = 0 then 0 else Array.length y.(0) in (* y1 = height of y *)
  let z = Array.make_matrix x0 y1 0 in (* z = matrix of length x & height y1. all elements initially set to 0 *)
  for i = 0 to x0-1 do (* for loop for x&z's length *)
    for j = 0 to y1-1 do (* for loop for y&z's height *)
      for k = 0 to y0-1 do (* for loop for y length *)
        z.(i).(j) <- z.(i).(j) + x.(i).(k) * y.(k).(j) (*sets each value in z matrix to 0+((x matrix value)(y matrix value)) following standard matrix multiplication formula *)
      done
    done
  done;
  z (* Return new product matrix *)
