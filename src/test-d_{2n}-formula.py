from commons import d_ij, d_ij_brute

cases = (
    (3, 2),
    (3, 3),
    (3, 4),
    (4, 2)
)

for n, q in cases:
    args = (n, q, 2, n)

    print(f'n: {n}, q: {q}', end=' ')
    print(f'formula: {d_ij(*args)}', end=' ')
    print(f'real: {d_ij_brute(*args)}')
