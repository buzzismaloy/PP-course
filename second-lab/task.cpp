#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <omp.h>

const int rows = 5000;
const int cols = 3000;


void hadamard_mult_omp(const std::vector<std::vector<int>>& A, 
                       const std::vector<std::vector<int>>& B, 
                       std::vector<std::vector<int>>& C) {
    #pragma omp parallel for 
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            C[i][j] = A[i][j] * B[i][j];
        }
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
        omp_set_num_threads(n_threads); 

        auto start = std::chrono::high_resolution_clock::now();
        hadamard_mult_omp(A, B, C); 
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        std::cout << "Время выполнения для " << n_threads << " потоков: " 
                  << elapsed.count() << " секунд" << std::endl;
    }

    return 0;
}

