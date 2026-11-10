#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <utility>
using namespace std;
using namespace chrono;

const int N = 100;  
volatile int sink;  

// Задание 1
void fill(int a[]) {  
    mt19937 gen(random_device{}());
    uniform_int_distribution<> d(-99, 99);
    for (int i = 0; i < N; i++) a[i] = d(gen);
}

void print(const int a[]) {  
    for (int i = 0; i < N; i++)
        cout << a[i] << ((i + 1) % 20 ? ' ' : '\n');
    cout << '\n';
}

void copyArr(const int from[], int to[]) { 
    for (int i = 0; i < N; i++) to[i] = from[i];
}

// Задание 2
void bubble(int a[]) {  // пузырьковая
    for (int i = 0; i < N - 1; i++)
        for (int j = 0; j < N - 1 - i; j++)
            if (a[j] > a[j + 1]) swap(a[j], a[j + 1]);
}

void insertion(int a[]) {  // вставками
    for (int i = 1; i < N; i++) {
        int key = a[i], j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

void selection(int a[]) {  // выбором
    for (int i = 0; i < N - 1; i++) {
        int m = i;
        for (int j = i + 1; j < N; j++)
            if (a[j] < a[m]) m = j;
        swap(a[i], a[m]);
    }
}

void gnome(int a[]) {  // гномья
    int i = 0;
    while (i < N) {
        if (i == 0 || a[i - 1] <= a[i]) i++;
        else {
            swap(a[i], a[i - 1]);
            i--;
        }
    }
}

void quick(int a[], int l, int r) {  // быстрая
    int i = l, j = r, p = a[(l + r) / 2];
    while (i <= j) {
        while (a[i] < p) i++;
        while (a[j] > p) j--;
        if (i <= j) {
            swap(a[i], a[j]);
            i++;
            j--;
        }
    }
    if (l < j) quick(a, l, j);
    if (i < r) quick(a, i, r);
}

void quickSort(int a[]) { quick(a, 0, N - 1); } 

double timeSort(void (*sort)(int[]), const int src[]) {  
    int tmp[N];
    const int R = 1000;
    auto t1 = steady_clock::now();
    for (int r = 0; r < R; r++) {
        copyArr(src, tmp);
        sort(tmp);
        sink = tmp[0];
    }
    auto t2 = steady_clock::now();
    return duration<double, micro>(t2 - t1).count() / R;
}

// Задание 3
void minMax(const int a[], int& mn, int& mx) {  
    mn = mx = a[0];
    for (int i = 1; i < N; i++) {
        if (a[i] < mn) mn = a[i];
        if (a[i] > mx) mx = a[i];
    }
}

int countLess(const int a[], int x) {  // бинарный поиск
    int l = 0, r = N;
    while (l < r) {
        int m = (l + r) / 2;
        if (a[m] < x) l = m + 1;
        else r = m;
    }
    return l;
}

int countLessEq(const int a[], int x) { return countLess(a, x + 1); }  

int linearCountLess(const int a[], int x) {  // линейный подсчёт 
    int c = 0;
    for (int i = 0; i < N; i++) if (a[i] < x) c++;
    return c;
}

int linearCountGreater(const int a[], int x) {  // линейный подсчёт 
    int c = 0;
    for (int i = 0; i < N; i++) if (a[i] > x) c++;
    return c;
}

int binarySearch(const int a[], int x) {  // бинарный поиск
    int l = 0, r = N - 1;
    while (l <= r) {
        int m = (l + r) / 2;
        if (a[m] == x) return m;
        if (a[m] < x) l = m + 1;
        else r = m - 1;
    }
    return -1;
}

int linearSearch(const int a[], int x) {  // линейный поиск
    for (int i = 0; i < N; i++) if (a[i] == x) return i;
    return -1;
}

// Индивидуальное задание 41. проверка массива на палиндром
bool isPalindrome(const int a[]) {  
    for (int i = 0; i < N / 2; i++)
        if (a[i] != a[N - 1 - i]) return false;
    return true;
}

// MEASURE(R, код)
#define MEASURE(R, ...) [&] {                                    \
    auto t1 = steady_clock::now();                               \
    for (int _r = 0; _r < (R); _r++) { __VA_ARGS__; }            \
    auto t2 = steady_clock::now();                               \
    return duration<double, micro>(t2 - t1).count() / (R);       \
}()

int main() {
    int arr[N];  
    fill(arr);
    int choice;
    do {
        cout << "\n1 Новый массив\n2 Вывести массив\n3 Сортировки\n4 Min/Max\n"
                "5 Среднее min/max и совпадения\n6 Элементов меньше a\n"
                "7 Элементов больше b\n8 Бинарный и линейный поиск\n"
                "9 Обмен элементов\n10 Палиндром: да/нет\n0 Выход\nВыбор: ";
        cin >> choice;

        int sorted[N];  // отсортированная копия для заданий 3-7
        copyArr(arr, sorted);
        quickSort(sorted);

        if (choice == 1) {
            // Задание 1
            fill(arr);
            print(arr);
        } else if (choice == 2) {
            print(arr);
        } else if (choice == 3) {
            // Задание 2
            cout << "Пузырьковая: " << timeSort(bubble, arr) << " мкс\n";
            cout << "Вставками:   " << timeSort(insertion, arr) << " мкс\n";
            cout << "Выбором:     " << timeSort(selection, arr) << " мкс\n";
            cout << "Гномья:      " << timeSort(gnome, arr) << " мкс\n";
            cout << "Быстрая:     " << timeSort(quickSort, arr) << " мкс\n";
        } else if (choice == 4) {
            // Задание 3
            int mn, mx;
            minMax(arr, mn, mx);
            double t1 = MEASURE(100000, { int a, b; minMax(arr, a, b); sink = a + b; });
            cout << "Неотсортированный: min=" << mn << " max=" << mx << ", " << t1 << " мкс\n";
            double t2 = MEASURE(100000, { sink = sorted[0] + sorted[N - 1]; });
            cout << "Отсортированный:   min=" << sorted[0] << " max=" << sorted[N - 1]
                 << ", " << t2 << " мкс\n";
        } else if (choice == 5) {
            // Задание 4
            int mn, mx;
            minMax(arr, mn, mx);
            int avg = (int)lround((mn + mx) / 2.0);
            cout << "Среднее min и max (округлено): " << avg << '\n';
            int cnt = 0;
            cout << "Индексы:";
            for (int i = 0; i < N; i++)
                if (arr[i] == avg) { cout << ' ' << i; cnt++; }
            cout << "\nКоличество: " << cnt << '\n';
            double t1 = MEASURE(100000, { int c = 0; for (int i = 0; i < N; i++) if (arr[i] == avg) c++; sink = c; });
            double t2 = MEASURE(100000, { sink = countLessEq(sorted, avg) - countLess(sorted, avg); });
            cout << "Время в неотсортированном: " << t1 << " мкс, в отсортированном: " << t2 << " мкс\n";
        } else if (choice == 6) {
            // Задание 5
            int a;
            cout << "a = "; cin >> a;
            cout << "Меньше a (бинарный): " << countLess(sorted, a)
                 << ", (линейный): " << linearCountLess(sorted, a) << '\n';
            double t1 = MEASURE(100000, sink = countLess(sorted, a));
            double t2 = MEASURE(100000, sink = linearCountLess(sorted, a));
            cout << "Время: бинарный " << t1 << " мкс, линейный " << t2 << " мкс\n";
        } else if (choice == 7) {
            // Задание 6
            int b;
            cout << "b = "; cin >> b;
            cout << "Больше b (бинарный): " << N - countLessEq(sorted, b)
                 << ", (линейный): " << linearCountGreater(sorted, b) << '\n';
            double t1 = MEASURE(100000, sink = N - countLessEq(sorted, b));
            double t2 = MEASURE(100000, sink = linearCountGreater(sorted, b));
            cout << "Время: бинарный " << t1 << " мкс, линейный " << t2 << " мкс\n";
        } else if (choice == 8) {
            // Задание 7
            int x;
            cout << "Число: "; cin >> x;
            int ib = binarySearch(sorted, x);
            cout << (ib >= 0 ? "Найдено, индекс в отсортированном: " + to_string(ib) : "Не найдено") << '\n';
            double t1 = MEASURE(100000, sink = binarySearch(sorted, x));
            double t2 = MEASURE(100000, sink = linearSearch(sorted, x));
            cout << "Время: бинарный " << t1 << " мкс, линейный " << t2 << " мкс\n";
        } else if (choice == 9) {
            // Задание 8
            int i, j;
            cout << "Индексы i и j (0.." << N - 1 << "): "; cin >> i >> j;
            if (i < 0 || i >= N || j < 0 || j >= N) {
                cout << "Неверный индекс\n";
            } else {
                swap(arr[i], arr[j]);
                cout << "Обмен выполнен: arr[" << i << "]=" << arr[i] << ", arr[" << j << "]=" << arr[j] << '\n';
                int tmp[N];
                copyArr(arr, tmp);
                double t = MEASURE(100000, { swap(tmp[i], tmp[j]); sink = tmp[0]; });
                cout << "Время одного обмена: " << t << " мкс\n";
            }
        } else if (choice == 10) {
            // Индивидуальное задание 41
            cout << (isPalindrome(arr) ? "Да" : "Нет") << '\n';
        }
    } while (choice != 0);
}