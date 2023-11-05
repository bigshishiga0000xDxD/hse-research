template <int N, int M, int Q>
struct Matrix {
    int arr[N][M];

    Matrix() {
        static_assert(M >= N && (N == 2 || N == 3));
        static_assert(Q == 2 || Q == 3);
        memset(arr, 0, sizeof arr);
    }

    Matrix(int mask) {
        /**
         * Конструктор от q-ичной маски.
         * переводим число в q-ичную систему исчисления
         * и записываем цифры по элементам матрицы.
         * Существует биекция между числами от 0 до q^(n^2)-1
         * и матрицами
        */
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                arr[i][j] = mask % Q;
                mask /= Q;
            }
        }
    }

    Matrix& operator+=(const Matrix& other) {
        /**
         * обычное суммирование матриц (над полем простого порядка)
        */
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                arr[i][j] += other.arr[i][j];
                if (arr[i][j] >= Q) {
                    arr[i][j] -= Q;
                }
            }
        }
        return *this;
    }

    bool is_singular() {
        if constexpr (N == M) {
            return det() == 0;
        } else {
            return matrix_rank() < N;
        }
    }

    int det() {
        /**
         * явная формула для определителя в случае n = 3
        */
        static_assert(N == 3 && M == 3);

        int result = 0;
        result += arr[0][0] * arr[1][1] * arr[2][2];
        result += arr[0][1] * arr[1][2] * arr[2][0];
        result += arr[0][2] * arr[1][0] * arr[2][1];
        result -= arr[0][2] * arr[1][1] * arr[2][0];
        result -= arr[0][0] * arr[1][2] * arr[2][1];
        result -= arr[0][1] * arr[1][0] * arr[2][2];
        result %= Q;
        result += Q;
        result %= Q;
        return result;
    }

    bool is_rank_at_least(int x) {
        /**
         * возвращает true, если ранг матрицы хотя бы x и false иначе
        */

        if (x == 0) {
            return true;
        } else if (x == 1) {    // rk >= 1, если в матрице есть ненулевой элемент
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < M; ++j) {
                    if (arr[i][j] != 0) {
                        return true;
                    }
                }
            }
            return false;
        } else if (x == 2) {    // rk >= 2, если в матрице есть ненулевой 2x2 минор
            for (int i1 = 0; i1 < N; ++i1) {
                for (int i2 = i1 + 1; i2 < N; ++i2) {
                    for (int j1 = 0; j1 < M; ++j1) {
                        for (int j2 = j1 + 1; j2 < M; ++j2) {
                            int minor = arr[i1][j1] * arr[i2][j2] - arr[i1][j2] * arr[i2][j1];
                            if (minor != 0) {
                                return true;
                            }
                        }
                    }
                }
            }
            return false;
        } else if (x == 3) {    // rk >= 3, если матрица невырождена
            if constexpr (N == 2) {
                return false;
            } else {
                return det() != 0;
            }
        } else {                // rk >= 4 в нашем случае не бывает
            return false;
        }
    }

    int matrix_rank() {
        /**
         * считает ранг матрицы
        */
        for (int k = 0; ; ++k) {
            // ранг равен k, если он хотя бы k и неверно, что он хотя бы k+1
            if (is_rank_at_least(k) && !is_rank_at_least(k + 1)) {
                return k;
            }
        }
    }
};
