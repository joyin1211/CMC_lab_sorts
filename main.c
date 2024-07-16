/*
Минхузов Дамир 105 группа
Вариант 1335
Тип чисел: int
Вид сортировки: Неубывание модулей
Методы сортировки: Метод Шелла, сортировка кучей
Будем считать число не свапов, а присвоений, т.к. Сортировка Шелла не использует первые.
Один свап - два присвоения
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>



int randSign() {
    return rand() % 2 ? 1 : -1;
}
//[mn, mx)
int randInt(int mn, int mx) {
    return mn + ((long long)rand() * (long long)rand()) % ((long long)mx - mn) + 1;
}


//Сгенерировать массив. 1 - отсортирован, 2 - обратный порядок, 3-4 - рандомный порядок
void generateArr(int type, int n, int *a) {
    if(type == 1) {
        a[0] = randInt(0, INT_MAX - n);
        for (int i = 1; i < n; i++) {
            a[i] = randSign() * randInt(abs(a[i - 1]), INT_MAX - n + i);
        }
    } else if(type == 2) {
        a[0] = randInt(n, INT_MAX);
        for (int i = 1; i < n; i++) {
            //printf("%d %d %d\n", a[i - 1], n - i, abs(a[i - 1] + 1));
            a[i] = randSign() * randInt(n - i, abs(a[i - 1] + 1));
        }
    } else if(type == 3) {
        for (int i = 0; i < n; i++) {
            a[i] = randInt(INT_MIN, INT_MAX);
        }
    } else {
        for (int i = 0; i < n; i++) {
            a[i] = INT_MIN + ((long long)randInt(INT_MIN, INT_MAX) * randInt(INT_MIN, INT_MAX)) % INT_MAX;
        }
    }
}


long long cmpCount = 0, asgnCount = 0; // Счетчик числа сравнений обменов
// Начальные значения аргументов
int amount = 100;
int mode = 1;
int sortType = 1;
int isPrint = 0;

/*
Сортировка Шелла
Материал:
https://neerc.ifmo.ru/wiki/index.php?title=%D0%A1%D0%BE%D1%80%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D0%BA%D0%B0_%D0%A8%D0%B5%D0%BB%D0%BB%D0%B0
*/

void ShellSort(int n, int* a) {
    int j, mov;
    int x;
    for(mov = n / 2 - 1; mov > 0; mov /= 2) {
        for(int i = mov; i < n; i++) {
            x = a[i];
            for(j = i; j >= mov; j -= mov) {
                cmpCount++;
                if (abs(x) < abs(a[j - mov])) {
                    asgnCount++;
                    a[j] = a[j - mov];
                } else {
                    break;
                }
                ///printf("%d %d\n", i, j); - вывод для дебага
            }
            if(j != i) {
                asgnCount++;
                a[j] = x;
            }
        }
    }
    
}


/*
Пирамидальная сортировка
Исходник:
https://habr.com/ru/companies/otus/articles/460087/
*/

//Построение кучи с корнем i

void heapify (int n, int* a, int i) {
    int larg = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    cmpCount++;
    if (l < n && abs(a[l]) > abs(a[larg])) {
        larg = l;
    }
    cmpCount++;
    if (r < n && abs(a[r]) > abs(a[larg])) {
        larg = r;
    }
    if (larg != i) {
        asgnCount += 2;
        int x = a[i];
        a[i] = a[larg];
        a[larg] = x;
        heapify(n, a, larg);
    }
}

//Основная функция сортировки

void HeapSort(int n, int* a) {
    for (int i = n / 2; i >= 0; i--) {
        heapify(n, a, i);
    }
    for (int i = n - 1; i >= 0; i--) {
        asgnCount ++;
        int x = a[i];
        a[i] = a[0];
        a[0] = x;
        heapify(i, a, 0);
    }
}

//вспомогательная функция для парсинга аргументов
/*
void toLower(char *s) {
    for(;*s;s++) {
        if(*s <= 'Z' && *s >= 'A') {
            *s = *s - 'A' + 'a';
        }
    }
}
*/
//Функции печати текста

void PrintHelp() {
    printf("\nАргументы командной строки:\n");
    printf("\n");
    printf("1) -H или --help - вызвать описание аргументов, остальные аргументы отбрасываются\n");
    printf("\n");
    printf("2) -A или --amount - задать количество элементов массива.\n    Принимает целое положительное число до 5000000. ");
    printf("По умолчанию 100.\n");
    printf("\n");
    printf("3)  -G или --generate - задать вид исходного массива.\n");
    printf("    Принимает целое число от 1 до 4. По умолчанию 1\n");
    printf("      1: элементы идут по неубыванию модулей(Элементы отсортированы)\n");
    printf("      2: отсортированный в обратную сторону - элементы идут по невозрастанию модулей (Обратный порядок)\n");
    printf("      3: случайный - случайная расстановка элементов\n");
    printf("      4: случайный - случайная расстановка элементов, отличается от 3\n");
    printf("\n");
    printf("4) -S или --sort - задать метод сортировки.\n");
    printf("   Принимает число 1 или 2. По умолчанию 1.\n");
    printf("      1: ShellSort\n");
    printf("      2: HeapSort\n");
    printf("\n");
    printf("5) -P или --print - при добавлении этого аргумента будет выведен исходный массив.\n");
    printf("\n");
    printf("\nПримеры:\n");
    printf("--sort 1 --generate 5 --amount 1000\n");
    printf("-A 50 -S 2 -G 1 -P\n");
    printf("\n");
}

void PrintHello()
{
	printf("\n     СРАВНЕНИЕ СОРТИРОВОК\n");
    printf("Сделал Минхузов Дамир, 105 группа\n");
    printf("Вариант 1335\n");
    printf("\n");
    printf("Данные (элементы массива): int\n");
    printf("Результат сортировки: Числа идут по неубыванию модулей\n");
    printf("Методы сортировки: ShellSort\n");
    printf("                   HeapSort\n");
    printf("\n");
    PrintHelp();
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    if(argc == 1) {
        PrintHello();
        return 0;
    }

    int usedArgs[4] = {0, 0, 0, 0};

    //Парсинг аргументов


    for(int i = 1; i < argc; i++) {
        if(!strcmp(argv[i], "-H") || !strcmp(argv[i], "--help")) {
            PrintHelp();
            return 0;
        }
        if(!strcmp(argv[i], "-A") || !strcmp(argv[i], "--amount")) {
            if(usedArgs[0]) {
                printf("Аргумент -A/--amount не может быть использован дважды.\n");
                return 0;
            }
            usedArgs[0] = 1;
            i++;
            if(i >= argc || sscanf(argv[i], "%d", &amount) != 1) {
                printf("Аргумент -A/--amount принимает целое положительное число до 5000000.");
                return 0;
            }
            if(amount <= 0) {
                printf("Аргумент -A/--amount принимает ПОЛОЖИТЕЛЬНОЕ число.");
                return 0;
            }
            if(amount > 5000000) {
                printf("Аргумент -A/--amount принимает число до 5000000.");
                return 0;
            }
        } else if(!strcmp(argv[i], "-G") || !strcmp(argv[i], "--generate")) {
            if(usedArgs[1]) {
                printf("Аргумент -G/--generate не может быть использован дважды.\n");
                return 0;
            }
            usedArgs[1] = 1;
            i++;
            if(i >= argc || sscanf(argv[i], "%d", &mode) != 1) {
                printf("Аргумент -G/--generate принимает целое положительное число.\n");
                return 0;
            }
            if(mode < 1 || mode > 4) {
                printf("Аргумент -G/--generate принимает целое число от 1 до 4.\n");
                return 0;
            }
        } else if(!strcmp(argv[i], "-S") || !strcmp(argv[i], "--sort")) {
            if(usedArgs[2]) {
                printf("Аргумент -S/--sort не может быть использован дважды.\n");
                return 0;
            }
            usedArgs[2] = 1;
            i++;
            if(i >= argc || sscanf(argv[i], "%d", &sortType) != 1) {
                printf("Аргумент -S/--sort принимает целое положительное число.\n");
                return 0;
            }
            if(sortType != 1 && sortType != 2) {
                printf("Аргумент -S/--sort принимает число 1 или 2.\n");
                return 0;
            }
        } else if(!strcmp(argv[i], "-P") || !strcmp(argv[i], "--print")) {
            if(usedArgs[3]) {
                printf("Аргумент -P/--print не может быть использован дважды.\n");
                return 0;
            }
            usedArgs[3] = 1;
            isPrint = 1;
        } else {
            printf("Ошибка в команде %s.\n", argv[i]);
            return 0;
        }
    }

    //Создание массива
    int *a = malloc(sizeof(int) * amount);
    if(!a) {
        printf("Ошибка выделения памяти.\n");
        return 0;
    }
    generateArr(mode, amount, a);

    if(isPrint) {
        printf("\nСгенерированный массив:\n");
        for (int i = 0; i < amount; i++) {
            printf("%d ", a[i]);
        }
        printf("\n\n");
    }

    //Сортировка
    asgnCount = 0;
    cmpCount = 0;
    (sortType == 1) ? ShellSort(amount, a) : HeapSort(amount, a);
    printf("Количество сравнений: %lld\n", cmpCount);
    printf("Количество присвоений: %lld\n", asgnCount);
    printf("\n");

    if(isPrint) {
        printf("\nОтсортированный массив:\n");
        for (int i = 0; i < amount; i++) {
            printf("%d ", a[i]);
        }
        printf("\n\n");
    }
    //some debug & test
    /*
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
    int n = 8,  *a;
    a = malloc(sizeof(int) * n);
    generateArr(4, n, a);
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
    HeapSort(n, a);
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n%d\n", RAND_MAX);
    for(int i = 0; i < n; i++) {
        printf("%d ", randInt(-7, 10));
    }*/
}
