import math
from typing import List, Callable, Iterable
from functools import reduce

import numpy as np
import galois

List2D = List[List[int]]

def binom(n, k):
    return math.factorial(n) // math.factorial(k) // math.factorial(n-k)

def M(n: int, q: int) -> int:
    """
    Number of all n*n matrices over the field F_q
    """
    return q ** (n ** 2)

def J(n: int, k: int, q: int) -> int:
    """
    Number of n*n matrices of rank k over the field F_q
    """
    result = 1

    for i in range(k):
        result *= q ** n - q ** i

    result **= 2

    for i in range(k):
        result //= q ** k - q ** i

    return result

def gamma(n: int, q: int) -> int:
    """
    Number of invertible n*n matrices over the field F_q
    """
    return J(n, n, q)

def d_ij(n: int, q: int, i: int, j: int) -> int:
    if i == n and j == 1:
        return (q ** (2*n - 1) - q ** (n - 1)) // (q - 1)
    if i == 1 and j == n:
        return q ** (n - 1) * reduce(
            lambda a, b: a * b,
            (q ** n - q ** i for i in range(1, n))
        )
    if i == n - 1 and j == 1:
        return (2*q ** (2*n - 1) - q ** (2*n - 2) - 2*q ** n + 1) // (q - 1)
    if i == 1 and j == n - 1:
        return (2*q ** (2*n - 1) - q ** (2*n - 2) - 2*q ** n + 1) * \
            reduce(lambda a, b: a * b, (q ** n - q ** i for i in range(1, n - 1))) // \
            (q - 1)
    raise NotImplementedError("No known formula for such parameters")

def is_in_d_ij(n: int, q: int, i: int, j: int, arr: List2D) -> bool:
    F = galois.GF(q)

    A = F(arr)
    if np.linalg.matrix_rank(A) != j:
        return False

    A += F(np.diag([1] * i + [0] * (n - i)))
    return np.linalg.det(A) == F(0)

def calculate_objects(n: int, q: int, callback: Callable[..., bool], args: Iterable) -> int:
    """
    Count number of n*n matrices over the field F_q, such that `callback` returns true
    """
    mask_max = q ** (n ** 2)
    arr = [[0] * n for _ in range(n)]
    result = 0

    for mask in range(mask_max):
        for i in range(n):
            for j in range(n):
                arr[i][j] = mask % q
                mask //= q

        result += callback(*args, arr=arr)

    return result

