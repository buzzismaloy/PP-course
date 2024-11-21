#include <mpi.h>
#include <iostream>
#include <cstdlib> // для rand() и srand()
#include <ctime>   // для time()

// Пароль, который должен быть угадан
const int CORRECT_PASSWORD = 5;

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        // Процесс Жеглова
        srand(time(0));  // Инициализация генератора случайных чисел

        for (int i = 0; i < 10; ++i) {
            int password_guess = rand() % 10;  // Случайный пароль от 0 до 9
            std::cout << "Жеглов: Я передаю пароль " << password_guess << std::endl;

            // Отправляем пароль жулику
            MPI_Send(&password_guess, 1, MPI_INT, 1, password_guess, MPI_COMM_WORLD);

            // Получаем ответ от жулика
            char response[20];
            MPI_Recv(response, 20, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Жулик: " << response << std::endl;
        }

    } else if (rank == 1) {
        // Процесс Жулика

        for (int i = 0; i < 10; ++i) {
            int received_password;
            
            // Получаем пароль от Жеглова
            MPI_Recv(&received_password, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // Проверка пароля
            if (received_password == CORRECT_PASSWORD) {
                const char* response = "I will think";
                MPI_Send(response, 20, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            } else {
                const char* response = "I don't understand";
                MPI_Send(response, 20, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            }
        }
    }

    MPI_Finalize();
    return 0;
}

