from commons import calculate_objects, d_ij, is_in_d_ij

cases = (
    (2, 2),
    (2, 3),
    (2, 4),
    (2, 5),
    (3, 2),
    (3, 3),
    (3, 4),
    (4, 2)
)

for n, q in cases:
    args = (n, q, 1, n)

    print(f'n: {n}, q: {q}', end=' ')
    print(f'formula: {d_ij(*args)}', end=' ')
    print(f'real: {calculate_objects(n, n, q, is_in_d_ij, args)}')
