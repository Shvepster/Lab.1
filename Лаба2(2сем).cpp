#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// Функция для заполнения файла числами
void fillFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Не удалось открыть файл.\n");
        return;
    }

    int n, number;
    printf("Введите количество чисел: ");
    while (1) {
        if (scanf_s("%d", &n) != 1 || n <= 0 || n > 100 || getchar() != '\n') {
            printf("Повторите ввод:\n");
            while (getchar() != '\n');
        }
        else {
            break;
        }
    }

    printf("Введите числа:\n");
    for (int i = 0; i < n; i++) {
        while (1) {
            if (scanf_s("%d", &number) != 1 || number <= 0 || number > 100 || getchar() != '\n') {
                printf("Повторите ввод:\n");
                while (getchar() != '\n');
            }
            else {
                break;
            }
        }
        fprintf(file, "%d\n", number);
    }

    fclose(file);
}

// Функция для вывода содержимого файла на экран
void printFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Не удалось открыть файл.\n");
        return;
    }

    int number;
    printf("Содержимое файла:\n");
    while (fscanf(file, "%d", &number) != EOF) {
        printf("%d ", number);
    }
    printf("\n");

    fclose(file);
}

// Функция для нахождения максимального числа
void findMax(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Не удалось открыть файл.\n");
        return;
    }

    int number, max = -2147483648; // Минимальное значение для int
    int maxNumbers[100]; // Массив для хранения максимальных чисел
    int count = 0; // Счётчик максимальных чисел

    while (fscanf(file, "%d", &number) != EOF) {
        if (number > max) {
            max = number;
            count = 1;
            maxNumbers[0] = number;
        }
        else if (number == max) {
            maxNumbers[count++] = number;
        }
    }

    printf("Максимальное число: ");
    for (int i = 0; i < count; i++) {
        printf("%d ", maxNumbers[i]);
    }
    printf("\n");

    fclose(file);
}

// Функция для переворота числа по указанному номеру
void reverseNumberInFile(const char* filename, int index) {
    FILE* file = fopen(filename, "r+");
    if (file == NULL) {
        printf("Не удалось открыть файл.\n");
        return;
    }

    int numbers[100]; // Массив для хранения чисел из файла
    int count = 0, num;

    while (fscanf(file, "%d", &num) != EOF) {
        numbers[count++] = num; // Добавляем числа в массив
    }

    if (index < 1 || index > count) {
        printf("Некорректный номер числа.\n");
        fclose(file);
        return;
    }

    int reversed = 0, original = numbers[index - 1]; // Берём число по индексу

    while (original != 0) {
        reversed = reversed * 10 + original % 10; // Переворачиваем число
        original /= 10;
    }

    numbers[index - 1] = reversed; // Сохраняем перевёрнутое число

    rewind(file); // Перемещаем указатель в начало файла
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d\n", numbers[i]); // Записываем числа обратно в файл
    }

    fclose(file); // Закрываем файл
}

// Функция для обмена местами двух элементов в файле
void swapElementsInFile(const char* filename, int index1, int index2) {
    FILE* file = fopen(filename, "r+"); // Открываем файл для чтения и записи
    if (file == NULL) {
        printf("Не удалось открыть файл.\n");
        return;
    }

    int numbers[100]; // Массив для хранения чисел из файла
    int count = 0, num;

    while (fscanf(file, "%d", &num) != EOF) {
        numbers[count++] = num; // Добавляем числа в массив
    }

    if (index1 < 1 || index1 > count || index2 < 1 || index2 > count) {
        printf("Некорректные индексы.\n");
        fclose(file);
        return;
    }

    // Обмен местами двух чисел в массиве
    int temp = numbers[index1 - 1];
    numbers[index1 - 1] = numbers[index2 - 1];
    numbers[index2 - 1] = temp;

    rewind(file); // Перемещаем указатель в начало файла
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d\n", numbers[i]); // Записываем изменённые данные обратно в файл
    }

    fclose(file); // Закрываем файл
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "RU");
    if (argc < 2) {
        printf("Имя файла не задано.\n");
        return 1;
    }

    const char* filename = argv[1];
    int choice, index1, index2;

    while (1) {
        printf("\nВыберите действие:\n");
        printf("1. Заполнить файл числами.\n");
        printf("2. Вывести содержимое файла.\n");
        printf("3. Найти максимальное число.\n");
        printf("4. Перевернуть число в файле.\n");
        printf("5. Поменять местами элементы.\n");
        printf("6. Выход.\n");
        printf("Ваш выбор: ");

        while (1) {
            if (scanf_s("%d", &choice) != 1 || (choice < 1 || choice > 6) || getchar() != '\n') {
                printf("Повторите ввод:\n");
                while (getchar() != '\n'); // Очистка буфера.
            }
            else {
                break; // Корректный ввод.
            }
        }

        switch (choice) {
        case 1:
            fillFile(filename); // Заполняем файл числами
            break;
        case 2:
            printFile(filename); // Выводим содержимое файла
            break;
        case 3:
            findMax(filename); // Ищем максимальное число
            break;
        case 4:
            printf("Введите номер числа для переворота: ");
            while (1) {
                if (scanf_s("%d", &index1) != 1 || index1 <= 0 || index1 > 100 || getchar() != '\n') {
                    printf("Повторите ввод:\n");
                    while (getchar() != '\n');
                }
                else {
                    break;
                }
            }
            reverseNumberInFile(filename, index1); // Переворачиваем указанное число
            break;
        case 5:
            printf("Введите два индекса для обмена: ");
            while (1) {
                if (scanf_s("%d %d", &index1, &index2) != 2 || index1 <= 0 || index2 <= 0 || index1 > 100 || index2 > 100 || getchar() != '\n') {
                    printf("Повторите ввод:\n");
                    while (getchar() != '\n');
                }
                else {
                    break;
                }
            }
            swapElementsInFile(filename, index1, index2); // Меняем местами элементы
            break;
        case 6:
            return 0; // Выход из программы
        default:
            printf("Некорректный выбор.\n");
        }
    }
}