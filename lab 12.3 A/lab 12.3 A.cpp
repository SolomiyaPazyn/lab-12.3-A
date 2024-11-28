#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <iomanip>

using namespace std;

enum Spec { CS = 1, IT, ME, PhIT, TR };

#pragma pack(push, 1)
struct Student {
    char lname[50];
    int kurs;
    Spec spec;
    int physics;
    int math;
    int informatik;

    // Метод для повернення назви спеціальності
    string return_spec() {
        switch (spec) {
        case CS: return "Комп'ютернi науки";
        case IT: return "Iнформатика";
        case ME: return "Математика та економiка";
        case PhIT: return "Фiзика та iнформатика";
        case TR: return "Трудове навчання";
        default: return "Невідомо";
        }
    }

    // Метод для заповнення структури
    void fill_struct(int i) {
        cout << "Студент №" << i + 1 << endl;
        cout << "Прiзвище: ";
        cin.ignore();
        cin.getline(lname, 50);

        cout << "Курс: ";
        cin >> kurs;

        int sp;
        cout << "Спецiальнiсть (1-5): ";
        cin >> sp;
        spec = static_cast<Spec>(sp);

        cout << "Оцiнка з фiзики: ";
        cin >> physics;

        cout << "Оцiнка з математики: ";
        cin >> math;

        cout << "Оцiнка з інформатики: ";
        cin >> informatik;
        cout << endl;
    }
};
#pragma pack(pop)

// Заповнення масиву студентів і запис у файл
void Create(Student* arr, int n, const char* filename) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Помилка відкриття файлу для запису" << endl;
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        arr[i].fill_struct(i);
        file.write(reinterpret_cast<char*>(&arr[i]), sizeof(Student));
    }
    file.close();
}

// Визначає довжину найдовшого прізвища в файлі
int lenOfLongestLastName(const char* filename, int n) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Помилка відкриття файлу для читання" << endl;
        exit(1);
    }

    Student s;
    int maxLen = -1;

    for (int i = 0; i < n; i++) {
        file.read(reinterpret_cast<char*>(&s), sizeof(Student));
        int currentLen = strlen(s.lname);
        maxLen = max(maxLen, currentLen);
    }

    file.close();
    return maxLen;
}

// Виводить таблицю студентів
void Print(const char* filename, int n) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Помилка відкриття файлу для читання" << endl;
        exit(1);
    }

    Student s;
    int width = lenOfLongestLastName(filename, n);
    width = max(width, 8);

    // Друк верхньої межі таблиці
    for (int i = 0; i < width + 100; i++) cout << "=";
    cout << endl;

    // Заголовок таблиці
    cout << left
        << setw(5) << "№"
        << setw(width + 2) << "Прізвище"
        << setw(7) << "Курс"
        << setw(25) << "Спеціальність"
        << setw(10) << "Фізика"
        << setw(12) << "Математика"
        << "Інформатика"
        << endl;

    // Друк межі таблиці
    for (int i = 0; i < width + 100; i++) cout << "-";
    cout << endl;

    // Виведення даних
    for (int i = 0; i < n; i++) {
        file.read(reinterpret_cast<char*>(&s), sizeof(Student));

        cout << left
            << setw(5) << i + 1
            << setw(width + 2) << s.lname
            << setw(7) << s.kurs
            << setw(25) << s.return_spec()
            << setw(10) << s.physics
            << setw(12) << s.math
            << s.informatik
            << endl;
    }

    // Друк нижньої межі таблиці
    for (int i = 0; i < width + 100; i++) cout << "-";
    cout << endl;

    file.close();
}

// Впорядкування даних у файлі
void Sort(const char* filename, int n) {
    fstream file(filename, ios::in | ios::out | ios::binary);
    if (!file) {
        cerr << "Помилка відкриття файлу" << endl;
        return;
    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            Student s1, s2;

            file.seekg(j * sizeof(Student));
            file.read(reinterpret_cast<char*>(&s1), sizeof(Student));

            file.seekg((j + 1) * sizeof(Student));
            file.read(reinterpret_cast<char*>(&s2), sizeof(Student));

            bool needSwap =
                (s1.spec > s2.spec) ||
                (s1.spec == s2.spec && s1.math > s2.math) ||
                (s1.spec == s2.spec && s1.math == s2.math && string(s1.lname) > string(s2.lname));

            if (needSwap) {
                file.seekp(j * sizeof(Student));
                file.write(reinterpret_cast<char*>(&s2), sizeof(Student));

                file.seekp((j + 1) * sizeof(Student));
                file.write(reinterpret_cast<char*>(&s1), sizeof(Student));
            }
        }
    }

    file.close();
    cout << "Файл відсортовано!" << endl;
}

// Бінарний пошук
int BinarySearch(const char* filename, int n, const string& prizv, Spec spec, int math) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Помилка відкриття файлу" << endl;
        return -1;
    }

    int L = 0, R = n - 1;
    while (L <= R) {
        int m = (L + R) / 2;

        Student s;
        file.seekg(m * sizeof(Student));
        file.read(reinterpret_cast<char*>(&s), sizeof(Student));

        if (s.lname == prizv && s.spec == spec && s.math == math)
            return m;

        if ((s.spec < spec) ||
            (s.spec == spec && s.math < math) ||
            (s.spec == spec && s.math == math && string(s.lname) < prizv))
            L = m + 1;
        else
            R = m - 1;
    }

    file.close();
    return -1;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int amSt;
    cout << "Кількість студентів: ";
    cin >> amSt;

    char filename[100];
    cout << "Введіть назву файлу: ";
    cin >> filename;

    Student* s = new Student[amSt];

    int menuItem;
    do {
        cout << endl << endl;
        cout << "Виберіть дію:" << endl;
        cout << " [1] - введення даних" << endl;
        cout << " [2] - вивід даних" << endl;
        cout << " [3] - фізичне впорядкування" << endl;
        cout << " [4] - бінарний пошук" << endl;
        cout << " [0] - вихід" << endl;
        cout << "Ваш вибір: ";
        cin >> menuItem;

        switch (menuItem) {
        case 1:
            Create(s, amSt, filename);
            break;
        case 2:
            Print(filename, amSt);
            break;
        case 3:
            Sort(filename, amSt);
            break;
        case 4: {
            string prizv;
            int sp, math;

            cout << "Введіть прізвище: ";
            cin >> prizv;

            cout << "Введіть спеціальність (1-5): ";
            cin >> sp;

            cout << "Введіть оцінку з математики: ";
            cin >> math;

            int found = BinarySearch(filename, amSt, prizv, static_cast<Spec>(sp), math);

            if (found != -1)
                cout << "Знайдено студента в позиції: " << found + 1 << endl;
            else
                cout << "Студента не знайдено" << endl;
            break;
        }
        case 0:
            break;
        }
    } while (menuItem != 0);

    delete[] s;
    return 0;
}