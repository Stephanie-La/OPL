// code taken from Rosetta Code
// https://rosettacode.org/wiki/Matrix_multiplication#Go
package main

import "fmt" // include package containing I/O functions

type Value float64    // sets Value as float64 format
type Matrix [][]Value // sets type Matrix as an array of arrays that can contain Value (float64) numbers

func Multiply(m1, m2 Matrix) (m3 Matrix, ok bool) { // attempt to multiply 2 matrices and return resulting matrix and bool value for if the operation succeeded
	rows, cols, extra := len(m1), len(m2[0]), len(m2) // get # of rows in m1, # of columns in m2, and # of rows in m2
	if len(m1[0]) != extra {                          // check if dimensions are valid (or if # columns in m1 == # of rows in m2)
		return nil, false // return NULL and false value
	}
	m3 = make(Matrix, rows)     // set m3 as an empty matrix, set amount of rows = # of rows in m1
	for i := 0; i < rows; i++ { // go through rows in m1
		m3[i] = make([]Value, cols) // set up row (size = # of columns in m2)
		for j := 0; j < cols; j++ { // go through columns in m2
			for k := 0; k < extra; k++ { // go through each value in column
				m3[i][j] += m1[i][k] * m2[k][j] // perform multiplication
			}
		}
	}
	return m3, true // return resulting matrix and true value
}

func (m Matrix) String() string { // prints out matrix to screen
	rows := len(m)
	cols := len(m[0])
	out := "["
	for r := 0; r < rows; r++ { // go through the matrix's rows
		if r > 0 {
			out += ",\n "
		}
		out += "[ "
		for c := 0; c < cols; c++ { // go through values in each row
			if c > 0 {
				out += ", "
			}
			out += fmt.Sprintf("%7.3f", m[r][c]) // print out value in row
		}
		out += " ]"
	}
	out += "]"
	return out // display matrix on screen
}

func main() {
	A := Matrix{[]Value{1, 2, 3, 4}, // A = 2x4 matrix
		[]Value{5, 6, 7, 8}}
	B := Matrix{[]Value{1, 2, 3}, // B = 4x3 matrix
		[]Value{4, 5, 6},
		[]Value{7, 8, 9},
		[]Value{10, 11, 12}}
	P, ok := Multiply(A, B) // P = resulting matrix, ok = whether or not A and B can be multiplied
	if !ok {                // if ok == 0, A and B can't be multiplied since their dimensions are invalid
		panic("Invalid dimensions") // if !ok, throw error and panic (terminate execution)
	}
	fmt.Printf("Matrix A:\n%s\n\n", A)           // print out matrix A
	fmt.Printf("Matrix B:\n%s\n\n", B)           // print out matrix B
	fmt.Printf("Product of A and B:\n%s\n\n", P) // print out resulting matrix
}
