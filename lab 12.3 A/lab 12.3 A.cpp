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

    // ����� ��� ���������� ����� ������������
    string return_spec() {
        switch (spec) {
        case CS: return "����'�����i �����";
        case IT: return "I����������";
        case ME: return "���������� �� ������i��";
        case PhIT: return "�i���� �� i����������";
        case TR: return "������� ��������";
        default: return "�������";
        }
    }

    // ����� ��� ���������� ���������
    void fill_struct(int i) {
        cout << "������� �" << i + 1 << endl;
        cout << "��i�����: ";
        cin.ignore();
        cin.getline(lname, 50);

        cout << "����: ";
        cin >> kurs;

        int sp;
        cout << "����i����i��� (1-5): ";
        cin >> sp;
        spec = static_cast<Spec>(sp);

        cout << "��i��� � �i����: ";
        cin >> physics;

        cout << "��i��� � ����������: ";
        cin >> math;

        cout << "��i��� � �����������: ";
        cin >> informatik;
        cout << endl;
    }
};
#pragma pack(pop)

// ���������� ������ �������� � ����� � ����
void Create(Student* arr, int n, const char* filename) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "������� �������� ����� ��� ������" << endl;
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        arr[i].fill_struct(i);
        file.write(reinterpret_cast<char*>(&arr[i]), sizeof(Student));
    }
    file.close();
}

// ������� ������� ���������� ������� � ����
int lenOfLongestLastName(const char* filename, int n) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "������� �������� ����� ��� �������" << endl;
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

// �������� ������� ��������
void Print(const char* filename, int n) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "������� �������� ����� ��� �������" << endl;
        exit(1);
    }

    Student s;
    int width = lenOfLongestLastName(filename, n);
    width = max(width, 8);

    // ���� ������� ��� �������
    for (int i = 0; i < width + 100; i++) cout << "=";
    cout << endl;

    // ��������� �������
    cout << left
        << setw(5) << "�"
        << setw(width + 2) << "�������"
        << setw(7) << "����"
        << setw(25) << "������������"
        << setw(10) << "Գ����"
        << setw(12) << "����������"
        << "�����������"
        << endl;

    // ���� ��� �������
    for (int i = 0; i < width + 100; i++) cout << "-";
    cout << endl;

    // ��������� �����
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

    // ���� ������ ��� �������
    for (int i = 0; i < width + 100; i++) cout << "-";
    cout << endl;

    file.close();
}

// ������������� ����� � ����
void Sort(const char* filename, int n) {
    fstream file(filename, ios::in | ios::out | ios::binary);
    if (!file) {
        cerr << "������� �������� �����" << endl;
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
    cout << "���� �����������!" << endl;
}

// �������� �����
int BinarySearch(const char* filename, int n, const string& prizv, Spec spec, int math) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "������� �������� �����" << endl;
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
    cout << "ʳ������ ��������: ";
    cin >> amSt;

    char filename[100];
    cout << "������ ����� �����: ";
    cin >> filename;

    Student* s = new Student[amSt];

    int menuItem;
    do {
        cout << endl << endl;
        cout << "������� ��:" << endl;
        cout << " [1] - �������� �����" << endl;
        cout << " [2] - ���� �����" << endl;
        cout << " [3] - ������� �������������" << endl;
        cout << " [4] - ������� �����" << endl;
        cout << " [0] - �����" << endl;
        cout << "��� ����: ";
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

            cout << "������ �������: ";
            cin >> prizv;

            cout << "������ ������������ (1-5): ";
            cin >> sp;

            cout << "������ ������ � ����������: ";
            cin >> math;

            int found = BinarySearch(filename, amSt, prizv, static_cast<Spec>(sp), math);

            if (found != -1)
                cout << "�������� �������� � �������: " << found + 1 << endl;
            else
                cout << "�������� �� ��������" << endl;
            break;
        }
        case 0:
            break;
        }
    } while (menuItem != 0);

    delete[] s;
    return 0;
}