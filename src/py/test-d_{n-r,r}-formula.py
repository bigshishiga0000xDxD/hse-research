from commons import d_ij, d_ij_brute

cases = (
    (2, 2, 1, 1),
    (2, 3, 1, 1),
    (2, 4, 1, 1),
    (2, 5, 1, 1),
    (3, 2, 1, 2),
    (3, 3, 1, 2),
    (3, 2, 2, 1),
    (3, 3, 2, 1),
    (4, 2, 1, 3),
    (4, 2, 2, 2),
    (4, 2, 3, 1),
)

for args in cases:
    n, q, i, j = args

    print(f'n: {n}, q: {q}, i: {i}, j: {j}', end=' ')
    print(f'formula: {d_ij(*args)}', end=' ')
    print(f'real: {d_ij_brute(*args)}')
