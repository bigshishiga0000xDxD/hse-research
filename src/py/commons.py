from typing import List, Callable, Iterable
from functools import reduce

import numpy as np
import galois


def prod(elems: Iterable):
    return reduce(lambda x, y: x * y, elems)


def J(n: int, m: int, q: int, k: int) -> int:
    """
    Number of n*m matrices of rank k over the field F_q
    """
    result = 1

    for i in range(k):
        result *= q ** n - q ** i

    for i in range(k):
        result *= q ** m - q ** i

    for i in range(k):
        result //= q ** k - q ** i

    return result


def gamma(n: int, q: int) -> int:
    """
    Number of invertible n*n matrices over the field F_q
    """
    return J(n, n, q, n)


def d_ij(n: int, q: int, i: int, j: int) -> int:
    if i + j < n:
        return J(n, n, q, j)
    if i == n and j == 1:
        return (q ** (2*n-1) - q ** (n-1)) // (q - 1)
    if i == 1 and j == n:
        return q ** (n-1) * prod((q ** n - q ** i for i in range(1, n)))
    if i == 2 and j == n:
        return q ** (n-1) * (q ** n + q ** (n-1) - q ** (n-2) - q - 1) * \
              prod((q ** n - q ** i for i in range(2, n)))
    if i + j == n:
        return (prod((q ** n - q ** i for i in range(j))) ** 2 -
                prod((q ** n - q ** i for i in range(n - j, n))) ** 2) // \
                prod(q ** j - q ** i for i in range(j))

    raise NotImplementedError("No known formula for such parameters")


def calculate_objects(n: int, m: int, q: int, callback: Callable[..., bool],
                      args: Iterable) -> int:
    """
    Count number of n*m matrices over the field F_q, such that `callback`
    returns true

    `callback` should take `arr` as an argument
    """
    F = galois.GF(q)
    mask_max = q ** (n * m)
    arr = [[0] * m for _ in range(n)]
    result = 0

    for mask in range(mask_max):
        for i in range(n):
            for j in range(m):
                arr[i][j] = mask % q
                mask //= q

        result += bool(callback(*args, arr=F(arr)))

    return result


def get_objects(n: int, m: int, q: int, callback: Callable[..., bool],
                args: Iterable) -> List[np.array]:
    """
    Return all of n*m matrices over the field F_q, such that `callback`
    returns true

    `callback` should take `arr` as an argument
    """
    F = galois.GF(q)
    mask_max = q ** (n * m)
    arr = [[0] * m for _ in range(n)]
    result = []

    for mask in range(mask_max):
        for i in range(n):
            for j in range(m):
                arr[i][j] = mask % q
                mask //= q

        if callback(*args, arr=F(arr)):
            result.append(F(arr))

    return result


def is_in_d_ij(n: int, q: int, i: int, j: int,
               arr: Iterable[Iterable]) -> bool:
    F = galois.GF(q)

    A = F(arr)
    if np.linalg.matrix_rank(A) != j:
        return False

    A += F(np.diag([1] * i + [0] * (n - i)))
    return np.linalg.det(A) == F(0)


def d_ij_brute(n: int, q: int, i: int, j: int) -> int:
    return calculate_objects(n, n, q, is_in_d_ij, (n, q, i, j))


def get_by_mask(n: int, m: int, q: int, mask: int) -> np.array:
    F = galois.GF(q)
    arr = [[0] * m for _ in range(n)]

    for i in range(n):
        for j in range(m):
            arr[i][j] = mask % q
            mask //= q

    return F(arr)
