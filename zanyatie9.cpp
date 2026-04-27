#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <locale>

using namespace std;

const int CHILDREN_COUNT = 20;
const int TOP_GIRLS_COUNT = 5;

enum Gender
{
    Boy = 0,
    Girl = 1
};

struct Date
{
    int day;
    int month;
    int year;
};

struct Child
{
    char surname[30];
    char name[30];
    Gender gender;
    int height;
    Date birthDate;
};

void printHeader()
{
    cout << left << setw(15) << "Фамилия"
        << setw(15) << "Имя"
        << setw(12) << "Пол"
        << setw(8) << "Рост"
        << "Дата рождения" << endl;

    cout << string(65, '-') << endl;
}

const char* genderToString(Gender gender)
{
    return gender == Boy ? "Мальчик" : "Девочка";
}

void printChild(const Child& child)
{
    cout << left << setw(15) << child.surname
        << setw(15) << child.name
        << setw(12) << genderToString(child.gender)
        << setw(8) << child.height
        << setfill('0')
        << setw(2) << child.birthDate.day << "."
        << setw(2) << child.birthDate.month << "."
        << child.birthDate.year
        << setfill(' ')
        << endl;
}

void printChildren(const Child children[], int size)
{
    printHeader();

    for (int i = 0; i < size; i++)
    {
        printChild(children[i]);
    }

    cout << endl;
}

void readGenderFromTextFile(Child children[], int size, const char* fileName)
{
    ifstream fin(fileName);

    if (!fin.is_open())
    {
        cout << "Ошибка открытия текстового файла." << endl;
        return;
    }

    char surname[30];
    int genderValue;

    while (fin >> surname >> genderValue)
    {
        for (int i = 0; i < size; i++)
        {
            if (strcmp(children[i].surname, surname) == 0)
            {
                children[i].gender = static_cast<Gender>(genderValue);
            }
        }
    }

    fin.close();
}

double getAverageBoysHeight(const Child children[], int size)
{
    int sum = 0;
    int count = 0;

    for (int i = 0; i < size; i++)
    {
        if (children[i].gender == Boy)
        {
            sum += children[i].height;
            count++;
        }
    }

    if (count == 0)
    {
        return 0;
    }

    return static_cast<double>(sum) / count;
}

void getFiveTallestGirls(const Child children[], int size, Child tallestGirls[])
{
    Child girls[CHILDREN_COUNT];
    int girlsCount = 0;

    for (int i = 0; i < size; i++)
    {
        if (children[i].gender == Girl)
        {
            girls[girlsCount] = children[i];
            girlsCount++;
        }
    }

    sort(girls, girls + girlsCount, [](const Child& a, const Child& b)
        {
            return a.height > b.height;
        });

    for (int i = 0; i < TOP_GIRLS_COUNT; i++)
    {
        tallestGirls[i] = girls[i];
    }
}

void sortBySurname(Child children[], int size)
{
    sort(children, children + size, [](const Child& a, const Child& b)
        {
            return strcmp(a.surname, b.surname) < 0;
        });
}

void printChildBySurname(const Child children[], int size, const char* surname)
{
    bool found = false;

    for (int i = 0; i < size; i++)
    {
        if (strcmp(children[i].surname, surname) == 0)
        {
            printHeader();
            printChild(children[i]);
            found = true;
        }
    }

    if (!found)
    {
        cout << "Ученик с фамилией " << surname << " не найден." << endl;
    }

    cout << endl;
}

bool changeChildData(Child children[], int size, const char* surname, const Child& newChild)
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(children[i].surname, surname) == 0)
        {
            children[i] = newChild;
            return true;
        }
    }

    return false;
}

int getCurrentMonth()
{
    time_t now = time(nullptr);
    tm localTime;

    localtime_s(&localTime, &now);

    return localTime.tm_mon + 1;
}

void printChildrenWithBirthdayInCurrentMonth(const Child children[], int size)
{
    int currentMonth = getCurrentMonth();
    bool found = false;

    printHeader();

    for (int i = 0; i < size; i++)
    {
        if (children[i].birthDate.month == currentMonth)
        {
            printChild(children[i]);
            found = true;
        }
    }

    if (!found)
    {
        cout << "В текущем месяце дней рождения нет." << endl;
    }

    cout << endl;
}

void writeToBinaryFile(const Child children[], int size, const char* fileName)
{
    ofstream fout(fileName, ios::binary);

    if (!fout.is_open())
    {
        cout << "Ошибка открытия бинарного файла для записи." << endl;
        return;
    }

    fout.write(reinterpret_cast<const char*>(children), sizeof(Child) * size);
    fout.close();
}

int readFromBinaryFile(Child children[], int maxSize, const char* fileName)
{
    ifstream fin(fileName, ios::binary);

    if (!fin.is_open())
    {
        cout << "Ошибка открытия бинарного файла для чтения." << endl;
        return 0;
    }

    int count = 0;

    while (count < maxSize && fin.read(reinterpret_cast<char*>(&children[count]), sizeof(Child)))
    {
        count++;
    }

    fin.close();

    return count;
}

int main()
{
    setlocale(LC_ALL, "");

    Child children[CHILDREN_COUNT] =
    {
        {"Ivanov", "Artem", Boy, 142, {15, 1, 2012}},
        {"Petrova", "Anna", Girl, 151, {4, 4, 2011}},
        {"Sidorov", "Maxim", Boy, 148, {21, 9, 2012}},
        {"Kuznetsova", "Maria", Girl, 160, {11, 6, 2011}},
        {"Smirnov", "Daniil", Boy, 139, {9, 12, 2013}},
        {"Vasileva", "Elena", Girl, 155, {26, 4, 2012}},
        {"Popov", "Ilya", Boy, 150, {3, 3, 2011}},
        {"Sokolova", "Daria", Girl, 158, {18, 10, 2011}},
        {"Morozov", "Nikita", Boy, 145, {30, 7, 2012}},
        {"Novikova", "Sofia", Girl, 162, {7, 11, 2010}},
        {"Fedorov", "Kirill", Boy, 153, {13, 4, 2011}},
        {"Mikhailova", "Polina", Girl, 149, {25, 2, 2013}},
        {"Alekseev", "Roman", Boy, 147, {5, 8, 2012}},
        {"Lebedeva", "Alina", Girl, 157, {17, 5, 2011}},
        {"Egorov", "Timur", Boy, 154, {22, 4, 2011}},
        {"Pavlova", "Ksenia", Girl, 164, {1, 1, 2010}},
        {"Kozlov", "Matvey", Boy, 141, {12, 6, 2013}},
        {"Stepanova", "Victoria", Girl, 159, {29, 4, 2011}},
        {"Orlov", "Gleb", Boy, 146, {8, 10, 2012}},
        {"Nikolaeva", "Ekaterina", Girl, 156, {20, 9, 2011}}
    };

    cout << "Исходный список учеников:" << endl;
    printChildren(children, CHILDREN_COUNT);

    readGenderFromTextFile(children, CHILDREN_COUNT, "gender.txt");

    cout << "Список после чтения пола из текстового файла:" << endl;
    printChildren(children, CHILDREN_COUNT);

    cout << "Средний рост мальчиков: "
        << fixed << setprecision(2)
        << getAverageBoysHeight(children, CHILDREN_COUNT)
        << " см" << endl << endl;

    Child tallestGirls[TOP_GIRLS_COUNT];

    getFiveTallestGirls(children, CHILDREN_COUNT, tallestGirls);

    cout << "Пять самых высоких девочек:" << endl;
    printChildren(tallestGirls, TOP_GIRLS_COUNT);

    sortBySurname(children, CHILDREN_COUNT);

    cout << "Список учеников после сортировки по фамилии:" << endl;
    printChildren(children, CHILDREN_COUNT);

    cout << "Поиск ученика по фамилии Ivanov:" << endl;
    printChildBySurname(children, CHILDREN_COUNT, "Ivanov");

    Child changedChild = { "Ivanov", "Artem", Boy, 150, {15, 1, 2012} };

    if (changeChildData(children, CHILDREN_COUNT, "Ivanov", changedChild))
    {
        cout << "Данные ученика Ivanov изменены." << endl << endl;
    }

    cout << "Данные ученика Ivanov после изменения:" << endl;
    printChildBySurname(children, CHILDREN_COUNT, "Ivanov");

    cout << "Ученики, у которых день рождения в текущем месяце:" << endl;
    printChildrenWithBirthdayInCurrentMonth(children, CHILDREN_COUNT);

    writeToBinaryFile(children, CHILDREN_COUNT, "children.dat");

    Child childrenFromFile[CHILDREN_COUNT];
    int readCount = readFromBinaryFile(childrenFromFile, CHILDREN_COUNT, "children.dat");

    cout << "Данные, считанные из бинарного файла:" << endl;
    printChildren(childrenFromFile, readCount);

    return 0;
}