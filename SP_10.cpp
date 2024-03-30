// SP_10.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <windows.h>
#include <conio.h>
#include <iostream>
#include <string>

DWORD WINAPI ThreadFunction1(LPVOID lpParam) {
	std::cout << "Поток 1 создан.Ура\n";
	return 0;
}

DWORD WINAPI ThreadFunction2(LPVOID lpParam) {
	std::cout << "Поток 2 создан.\n";

	char input;
	std::cout << "Поток 2: Введите знак: ";
	input = _getch();
	for (int i = 0; i < 50; i++)
	{
		std::cout << input;
	}
	std::cout << "\nПоток 2: Вывод 50 копий символа: " << input << "\n";

	return 0;
}


int main()
{
	setlocale(LC_ALL, "rus");

	wchar_t lpszComLine[80];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hProcess = GetCurrentProcess();
	HANDLE hInheritProcess;
	int inputNum;
	std::cout << "Введите число\n";
	std::cout << "Number: ";
	std::cin >> inputNum;



	if (!DuplicateHandle(GetCurrentProcess(), hProcess, GetCurrentProcess(), &hInheritProcess, 0, TRUE, DUPLICATE_SAME_ACCESS))
	{
		std::cerr << "Псевдодескриптор не был скопирован.\n";
		_cputs("Нажмите любую из клавиш чтобы выйти.\n");
		_getch();
		return GetLastError();
	}
	std::cout << "Создан наследуемый дубликат псевдодескриптора текущего процесса.\n";

	std::cout << "\nВведите любой знак для начала дочернего процесса: ";
	_getch();

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	wsprintf(lpszComLine, L"C:\\Users\\st310-03\\Desktop\\Ltcgbtr\\SP_10\\x64\\Debug\\ConsoleApplication1.exe %d", (int)inputNum);

	if (!CreateProcess(NULL, lpszComLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		std::cerr << "Новый процесс не создан.\n";
		_cputs("Нажмите любую из клавиш чтобы выйти.\n");
		_getch();
		return GetLastError();
	}

	std::cout << "\nPID: " << pi.dwProcessId << std::endl;
	std::cout << "Дескриптор: " << pi.hProcess << std::endl;

	WaitForSingleObject(pi.hProcess, INFINITE);

	TerminateThread(pi.hThread, 0);
	TerminateProcess(pi.hProcess, 0);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	CloseHandle(hInheritProcess);

    HANDLE hThreadmas[2]{NULL,NULL};
    char input;

    while (true) {
        if (hThreadmas[1] != NULL) {
            WaitForSingleObject(hThreadmas[0], INFINITE);
        }
        if (hThreadmas[1] != NULL) {
            WaitForSingleObject(hThreadmas[1], INFINITE);
        }

        std::cout << "\nВыберите действие:\n";
        std::cout << "1. Создать поток 1\n";
        std::cout << "2. Создать поток 2\n";
        std::cout << "3. Завершить поток 1\n";
        std::cout << "4. Завершить поток 2\n";
        std::cout << "e. Выйти из программы\n";
        std::cout << "Ваш выбор: ";
        std::cin >> input;

        switch (input) {
        case '1':
            if (hThreadmas[0] == NULL) {
                hThreadmas[0] = CreateThread(NULL, 0, ThreadFunction1, NULL, 0, NULL);
                if (hThreadmas[0] == NULL) {
                    std::cerr << "Не удалось создать поток 1.\n";
                }
            }
            else {
                std::cerr << "Поток 1 уже создан.\n";
            }
            break;
        case '2':
            if (hThreadmas[1] == NULL) {
                hThreadmas[1] = CreateThread(NULL, 0, ThreadFunction2, NULL, 0, NULL);
                if (hThreadmas[1] == NULL) {
                    std::cerr << "Не удалось создать поток 2.\n";
                }
            }
            else {
                std::cerr << "Поток 2 уже создан.\n";
            }
            break;
        case '3':
            if (hThreadmas[0] != NULL) {
                TerminateThread(hThreadmas[0], 0);
                CloseHandle(hThreadmas[0]);
                hThreadmas[0] = NULL;
                std::cout << "Поток 1 завершен.\n";
            }
            else {
                std::cerr << "Поток 1 не был создан.\n";
            }
            break;
        case '4':
            if (hThreadmas[1] != NULL) {
                TerminateThread(hThreadmas[1], 0);
                CloseHandle(hThreadmas[1]);
                hThreadmas[1] = NULL;
                std::cout << "Поток 2 завершен.\n";
            }
            else {
                std::cerr << "Поток 2 не был создан.\n";
            }
            break;
        case 'e':
            std::cout << "Выход из программы.\n";
            if (hThreadmas[0] != NULL) {
                TerminateThread(hThreadmas[0], 0);
                CloseHandle(hThreadmas[0]);
            }
            if (hThreadmas[1] != NULL) {
                TerminateThread(hThreadmas[1], 0);
                CloseHandle(hThreadmas[1]);
            }
            return 0;
        default:
            std::cerr << "Неверный выбор\n";
            break;
        }
    }


    _cputs("Нажмите любую из клавиш чтобы выйти\n");
    _getch();

    return 0;
}


