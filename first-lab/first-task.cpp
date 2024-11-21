#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>

const int rows = 5000;
const int cols = 3000;


void hadamard_mult(const std::vector<std::vector<int>>& A, 
                      const std::vector<std::vector<int>>& B, 
                      std::vector<std::vector<int>>& C, 
                      int start_row, int end_row) {
    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < cols; ++j) {
            C[i][j] = A[i][j] * B[i][j];
        }
    }
}


void execute_hadamard_with_threads(int n_threads, 
                                   const std::vector<std::vector<int>>& A, 
                                   const std::vector<std::vector<int>>& B, 
                                   std::vector<std::vector<int>>& C) {
    std::vector<std::thread> threads;
    int chunk_size = rows / n_threads;

    for (int i = 0; i < n_threads; ++i) {
        int start_row = i * chunk_size;
        int end_row = (i + 1) * chunk_size;
        if (i == n_threads - 1) {
            end_row = rows;
        }
        threads.emplace_back(hadamard_mult, std::ref(A), std::ref(B), std::ref(C), start_row, end_row);
    }

    for (auto& t : threads) {
        t.join();
    }
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 99);

    std::vector<std::vector<int>> A(rows, std::vector<int>(cols));
    std::vector<std::vector<int>> B(rows, std::vector<int>(cols));
    std::vector<std::vector<int>> C(rows, std::vector<int>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            A[i][j] = dis(gen);
            B[i][j] = dis(gen);
        }
    }

    for (int n_threads : {1, 2, 4, 8}) {
        auto start = std::chrono::high_resolution_clock::now();
        execute_hadamard_with_threads(n_threads, A, B, C);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Время выполнения для " << n_threads << " потоков: " 
                  << elapsed.count() << " секунд" << std::endl;
    }

    return 0;
}

