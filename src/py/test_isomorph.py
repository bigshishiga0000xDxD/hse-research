import galois
import numpy as np
import commons

n, m, q = map(int, input().split())
print(f'testing cliques for (n, m, q) = {(n, m, q)}')

GLn = commons.get_objects(n, n, q, lambda arr: np.linalg.det(arr) != 0, args=())

it = 0
std = 0
while True:
    try:
        masks = list(map(int, input().split()))
    except EOFError:
        break
    it += 1

    clique = [commons.get_by_mask(n, m, q, mask) for mask in masks]

    isomorph = False
    for A in GLn:
        clique_shifted = (A @ matrix for matrix in clique)
        
        flag = True
        for matrix_shifted in clique_shifted:
            if matrix_shifted[0, :].any():
                flag = False
                break
        
        if flag:
            isomorph = True
            break 

    if isomorph:
        std += 1

print(f'{std}/{it} cliques are of standart form')