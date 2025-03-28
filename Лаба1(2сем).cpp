#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

void sort_file(const char* filename, int count) {
    FILE* file = fopen(filename, "rb+");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int num1, num2;

            // Считываем два соседних элемента
            fseek(file, j * sizeof(int), SEEK_SET);
            fread(&num1, sizeof(int), 1, file);
            fread(&num2, sizeof(int), 1, file);

            // Если элементы в неправильном порядке, меняем их местами
            if (num1 > num2) {
                fseek(file, j * sizeof(int), SEEK_SET);
                fwrite(&num2, sizeof(int), 1, file);
                fwrite(&num1, sizeof(int), 1, file);
            }
        }
    }

    fclose(file);
}

void swap_min_max(const char* filename) {
    FILE* file = fopen(filename, "rb+");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return;
    }

    int number, min = 11, max = -1;
    long min_pos = -1, max_pos = -1, pos = 0;

    // Определяем min и max
    while (fread(&number, sizeof(int), 1, file) == 1) {
        if (number < min) {
            min = number;
            min_pos = pos;
        }
        if (number > max) {
            max = number;
            max_pos = pos;
        }
        pos += sizeof(int);
    }

    if (min_pos != -1 && max_pos != -1) {
        fseek(file, min_pos, SEEK_SET);
        fwrite(&max, sizeof(int), 1, file);
        fseek(file, max_pos, SEEK_SET);
        fwrite(&min, sizeof(int), 1, file);
    }

    fclose(file);
}

void calculate_score(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return;
    }

    int number, min = 11, max = -1, sum = 0, count = 0;
    int min_found = 0, max_found = 0;

    // Определяем минимальный и максимальный баллы
    while (fread(&number, sizeof(int), 1, file) == 1) {
        if (number < min) min = number;
        if (number > max) max = number;
    }
    rewind(file); // Возвращаемся в начало файла

    // Считаем сумму и количество без одного min и max
    while (fread(&number, sizeof(int), 1, file) == 1) {
        if (number == min && !min_found) {
            min_found = 1;
            continue;
        }
        if (number == max && !max_found) {
            max_found = 1;
            continue;
        }
        sum += number;
        count++;
    }
    fclose(file);

    if (count > 0) {
        printf("Средний балл спортсмена: %.2f\n", (double)sum / count);
    }
    else {
        printf("Нет оценок для расчета среднего балла.\n");
    }
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "RU");

    if (argc < 2) {
        printf("Ошибка: необходимо указать имя файла в командной строке.\n");
        return EXIT_FAILURE;
    }

    const char* filename = argv[1];
    FILE* file;
    int number, n = 0, min = 11, max = -1;

    // Открываем файл для записи
    file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return EXIT_FAILURE;
    }

    // Ввод оценок
    printf("Введите оценки судей (0-10). Введите -1 для завершения ввода:\n");
    while (1) {
        if (scanf("%d", &number) != 1) {
            printf("Ошибка ввода. Повторите ввод:\n");
            while (getchar() != '\n');
            continue;
        }
        if (number == -1) break;
        if (number < 0 || number > 10) {
            printf("Ошибка: введите число от 0 до 10.\n");
            continue;
        }
        fwrite(&number, sizeof(int), 1, file);
    }
    fclose(file);

    // Подсчет количества чисел в файле
    file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return EXIT_FAILURE;
    }

    while (fread(&number, sizeof(int), 1, file) == 1) {
        n++;
    }
    fclose(file);

    // Чтение данных из файла и вывод их на экран
    file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return EXIT_FAILURE;
    }

    printf("Данные в файле после записи:\n");
    while (fread(&number, sizeof(int), 1, file) == 1) {
        printf("%d ", number);
    }
    printf("\n");
    fclose(file);

    // Вычисление среднего балла спортсмена
    calculate_score(filename);

    // Замена минимальных и максимальных элементов
    swap_min_max(filename);

    // Вывод обновленных данных из файла
    file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return EXIT_FAILURE;
    }

    printf("Файл после замены мин/макс: ");
    while (fread(&number, sizeof(int), 1, file) == 1) {
        printf("%d ", number);
    }
    printf("\n");
    fclose(file);

    // Сортировка данных в файле
    sort_file(filename, n);

    // Вывод отсортированных данных
    file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return EXIT_FAILURE;
    }

    printf("Файл после сортировки: ");
    while (fread(&number, sizeof(int), 1, file) == 1) {
        printf("%d ", number);
    }
    printf("\n");
    fclose(file);

    return EXIT_SUCCESS;
}
