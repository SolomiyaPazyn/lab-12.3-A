#include "pch.h"
#include "CppUnitTest.h"
#include "../lab 12.3 A.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// Створення тестових даних
			const char* filename = "test_students.dat";
			Student test_students[2] = {
				{"Ivanov", 1, CS, 85, 90, 95},
				{"Petrov", 2, IT, 70, 75, 80}
			};

			// Запис у файл
			ofstream file_out(filename, ios::binary);
			for (int i = 0; i < 2; i++) {
				file_out.write(reinterpret_cast<char*>(&test_students[i]), sizeof(Student));
			}
			file_out.close();

			// Читання з файлу
			ifstream file_in(filename, ios::binary);
			Student read_students[2];
			for (int i = 0; i < 2; i++) {
				file_in.read(reinterpret_cast<char*>(&read_students[i]), sizeof(Student));
			}
			file_in.close();

			// Перевірка, чи дані коректно записані та зчитані
			for (int i = 0; i < 2; i++) {
				Assert::AreEqual(string(test_students[i].lname), string(read_students[i].lname));
				Assert::AreEqual(test_students[i].kurs, read_students[i].kurs);
				Assert::AreEqual((int)test_students[i].spec, (int)read_students[i].spec);
				Assert::AreEqual(test_students[i].physics, read_students[i].physics);
				Assert::AreEqual(test_students[i].math, read_students[i].math);
				Assert::AreEqual(test_students[i].informatik, read_students[i].informatik);
			}
		}
	};
}
