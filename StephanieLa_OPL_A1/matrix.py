# This code was written by Demetri Cassidy :^)
def multiply_matrices(left, right):
    result = [[0,0,0],  
              [0,0,0],  
              [0,0,0]]
    for i in range(len(left)): # go through x's rows 
        for j in range(len(right[0])): # go through y's columns
            for k in range(len(right)): # go through y's rows
                result[i][j] += left[i][k] * right[k][j] # perform multiplication and add up products in current square
    return result # return new matrix

def print_matrix(m):
    for row in m:
        print(row)

x = [[1, 2, 3],
     [4, 5, 6],
     [7, 8, 9]]

y = [[4,3,7],
     [1,9,3],
     [5,2,6]]

z = multiply_matrices(x, y)

print_matrix(z)
