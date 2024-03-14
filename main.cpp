#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <omp.h>

void writeMatrix(int** matrix, int rows, const char* filename) {
    std::ofstream file(filename);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            file << matrix[i][j];
            if (j < rows - 1) {
                file << ",";
            }
        }
        if (i < 3) {
            file << std::endl;
        }
    }
    file.close();
}
void writeMatrixTime(int** matrix, int rows, int cols, const char* filename) {
    std::ofstream file(filename);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            file << matrix[i][j];
            if (j < rows - 1) {
                file << ",";
            }
        }
        if (i < 3) {
            file << std::endl;
        }
    }
    file.close();
}

void writeTime(int** res_time, int rows, const char* filename) {
    std::ofstream file(filename);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            file << res_time[i][j];
            if (j < rows - 1) {
                file << ",";
            }
        }
        if (i < rows - 1) {
            file << std::endl;
        }
    }
    file.close();
}

int** readMatrix(const char* file_name) {
    std::ifstream file(file_name);
    int rows = 0;

    std::string line;
    while (std::getline(file, line)) {
        rows++;
    }

    int** matrix = new int* [rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[rows];
    }

    file.clear();
    file.seekg(0, std::ios::beg);

    int row_index = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        int col_index = 0;

        while (std::getline(ss, cell, ',')) {
            matrix[row_index][col_index] = std::stoi(cell);
            col_index++;
        }

        row_index++;
    }
    return matrix;
}
void clearMemory(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}
int** mulMatrix(int** matrix, int rows) {
    int i, j, count;
    int** matrix_new = new int* [rows];
    for (int i = 0; i < rows; i++) {
        matrix_new[i] = new int[rows];
    }

#pragma omp parallel for shared(matrix, matrix_new) private(i, count, j)
    for (i = 0; i < rows; i++) {
        for (count = 0; count < rows; count++) {
            int sum = 0;
            for (j = 0; j < rows; j++) {
                sum += (matrix[i][j] * matrix[j][count]);
            }
            matrix_new[i][count] = sum;
        }
    }

    return matrix_new;
}
int fixTime(int** matrix, int rows, const char* file_name_res) {
    int start = clock();
    int** matrix_new = mulMatrix(matrix, rows);
    int end = clock();
    writeMatrix(matrix_new, rows, file_name_res);
    int result = end - start;
    return result;
}
int main()
{
    int** matrix = readMatrix("C:\\Users\\Professional\\.vscode\\lab2_parallel_prog\\lab2_parallel_prog-1\\matrix_500.csv");
    int rows = 500;
    int count_op = 10;
    int** matrix_time = new int* [3];
    for (int i = 0; i < 3; i++) {
        matrix_time[i] = new int[count_op];
    }
    std::cout << std::endl << std::endl << "the process of multiplying matrices of size " << rows << "x" << rows << "..." << std::endl << std::endl;
    for (int i = 0; i < count_op; i++) {
        int time_res = fixTime(matrix, rows, "C:\\Users\\Professional\\.vscode\\lab2_parallel_prog\\lab2_parallel_prog-1\\matrix_res_500.csv");
        matrix_time[0][i] = time_res;
    }
    std::cout << "the process of adding result of multiplying matrices - size " << rows << "x" << rows << " to the file..." << std::endl << std::endl;
    clearMemory(matrix, rows);

    matrix = readMatrix("C:\\Users\\Professional\\.vscode\\lab2_parallel_prog\\lab2_parallel_prog-1\\matrix_1000.csv");
    rows = 1000;
    std::cout << std::endl << std::endl << "the process of multiplying matrices of size " << rows << "x" << rows << "..." << std::endl << std::endl;
    for (int i = 0; i < count_op; i++) {
        int time_res = fixTime(matrix, rows, "C:\\Users\\Professional\\.vscode\\lab2_parallel_prog\\lab2_parallel_prog-1\\matrix_res_1000.csv");
        matrix_time[1][i] = time_res;
    }
    std::cout << "the process of adding result of multiplying matrices - size " << rows << "x" << rows << " to the file..." << std::endl << std::endl;
    clearMemory(matrix, rows);


    matrix = readMatrix("C:\\Users\\Professional\\.vscode\\lab2_parallel_prog\\lab2_parallel_prog-1\\matrix_1500.csv");
    rows = 1500;
    std::cout << std::endl << std::endl << "the process of multiplying matrices of size " << rows << "x" << rows << "..." << std::endl << std::endl;
    for (int i = 0; i < count_op; i++) {
        int time_res = fixTime(matrix, rows, "C:\\Users\\Professional\\.vscode\\lab2_parallel_prog\\lab2_parallel_prog-1\\matrix_res_1500.csv");
        matrix_time[2][i] = time_res;
    }
    std::cout << "the process of adding result of multiplying matrices - size " << rows << "x" << rows << " to the file..." << std::endl << std::endl;
    std::cout << "the process of adding time for matrix to the file..." << std::endl << std::endl;
    writeMatrixTime(matrix_time, 3, count_op, "C:\\Users\\Professional\\.vscode\\lab2_parallel_prog\\lab2_parallel_prog-1\\matrix_time.csv");
    clearMemory(matrix, rows);
    clearMemory(matrix_time, 3);
    std::cout << "end of the program..." << std::endl << std::endl;
    return 0;
}