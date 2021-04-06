#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

using namespace std;

/**
* Перечисляемые индексы полей в массиве
*/
enum DATA_INDEX {
    LOGIN,
    EMAIL,
    PHONE_NUMBER,
};

struct user {
    int id;
    string login;
    string password;
    string email;
    string phoneNumber;
    bool adminRights;
};

int menu(bool userAuth, bool authAdmin, string menuNotAuthData[], int lengthMenuNotAuthData, 
    string menuAuthData[], int lengthMenuAuthData, string menuAdminhData[], 
    int lengthMenuAdminData, user authData[128], int userID);
void trim(string& word);
bool checkField(string word);
bool dataExist(user authData[128], int index, const string& dataUser, int countUser);
bool passwordExist(user authData[128], const string& dataUser, int userID);
bool breakOrContinueForError();
bool emailValidator(string email);
bool phoneNumberValidator(string phoneNumber);
void registerUser(user authData[128], bool& userAuth, int& userID, int& countUser, int*& ptrCountDeleteUser);
void authorization(user authData[128], bool& userAuth, bool& authAdmin, int& userID, int& countUser, int counterMax);
void logoutUser(bool& user);
void editMyPass(user authData[128], int userID, int counterMax);
void editPass(user authData[128], int countUser, int counterMax);
void forgotLogin(user authData[128], int countUser);
bool deleteUser(user authData[128], const string& login, int countUser, int*& ptrCountDeleteUser);
void printAuthData(user authData[128], int countUser, int* ptrCountDeleteUser);
void editMyLogin(user authData[128], int userID, int countUser);
void editMyEmail(user authData[128], int userID, int countUser);
void editMyPhoneNumber(user authData[128], int userID, int countUser);

int main() {
    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned int>(time(nullptr)));

    // проверки на аторизованность пользователя и хранение его ID
    bool userAuth = false;
    bool authAdmin = false;
    int userID = 0;

    // количество зарегистрированных пользователей не считает администратора и начинается с 1
    // нужно всегда учитывать при любом переборе authData
    int countUser = 5;

    // база данных
    user authData[128]; 

    // Следующие данные нужны только для теста, удалить перед релизной сборкой
    user newUser0 = { 0, "adminadmin", "adminadmin", "admin@admin.com", "9998887766", true };
    authData[0] = newUser0;    
    user newUser1 = { 1, "useruser1", "useruser1", "user1@user.com", "1279478218", false };
    authData[1] = newUser1;
    user newUser2 = { 2,"useruser2", "useruser2", "user2@user.com", "2279478218", false };
    authData[2] = newUser2;
    user newUser3 = { 3,"useruser3", "useruser3", "user3@user.com", "3279478218", false };
    authData[3] = newUser3;
    user newUser4 = { 4,"useruser4", "useruser4", "user4@user.com", "4279478218", false };
    authData[4] = newUser4;
    user newUser5 = { 5,"useruser5", "useruser5", "user5@user.com", "5279478218", false };
    authData[5] = newUser5;

    // меню неавторизованного пользователя
    string menuNotAuthData[] = { "Регистрация пользователя\t\t", "Авторизация пользователя\t\t", "Восстановить логин\t\t\t", "Редактировать пароль\t\t", "Завершить программу\t\t" };
    int lengthMenuNotAuthData = sizeof(menuNotAuthData) / sizeof(menuNotAuthData[0]);

    // меню аторизованного пользователя
    string menuAuthData[] = { "Смена пароля\t\t\t", "Смена логина\t\t\t", "Смена e-mail\t\t\t", "Смена телефона\t\t\t", "Удалить пользователя\t\t", "Логаут\t\t\t\t", "Завершить программу\t\t" };
    int lengthMenuAuthData = sizeof(menuAuthData) / sizeof(menuAuthData[0]);

    // меню администратора
    string menuAdminData[] = { "Удалить пользователя\t\t", "Показать базу данных\t\t", "Логаут\t\t\t\t", "Завершить программу\t\t" };
    int lengthMenuAdminData = sizeof(menuAdminData) / sizeof(menuAdminData[0]);


    // количество попыток вода данных
    int counterMax = 3;

    // указатель на счетчик пустых ячеек после удаления пользователя
    int* ptrCountDeleteUser = nullptr;

    bool valid;

    // выбор разделов меню
    do {
        int input = menu(userAuth, authAdmin, menuNotAuthData, lengthMenuNotAuthData, menuAuthData, lengthMenuAuthData, menuAdminData, lengthMenuAdminData, authData, userID);
        if (authAdmin) {
            string login;
            switch (input) {
            case 1:
                while (true) {
                    cout << "# Введите логин пользователя которого вы хотите удалить\n# ";
                    cin >> login;
                    if (!deleteUser(authData, login, countUser, ptrCountDeleteUser)) {
                        valid = breakOrContinueForError();
                        if (!valid) { return 0; }
                        continue;
                    }
                    break;
                }
            case 2:
                printAuthData(authData, countUser, ptrCountDeleteUser);
                break;
            case 3:
                logoutUser(authAdmin);
                break;
            case 4:
                cout << "\nВсего хорошего!\n";
                return 0;
            default:
                cout << "\nОтсутствующий пункт меню!\n";
            }
            cout << endl;
        }
        else if (userAuth) {
            switch (input) {
            case 1:
                editMyPass(authData, userID, counterMax);
                break;
            case 2:
                editMyLogin(authData, userID, countUser);
                break;
            case 3:
                editMyEmail(authData, userID, countUser);
                break;
            case 4:
                editMyPhoneNumber(authData, userID, countUser);
                break;
            case 5:
                deleteUser(authData, authData[userID].login, countUser, ptrCountDeleteUser);
                break;
            case 6:
                logoutUser(userAuth);
                break;
            case 7:
                cout << "\nВсего хорошего!\n";
                return 0;
            default:
                cout << "\nОтсутствующий пункт меню!\n";
            }
            cout << endl;
        }
        else {
            switch (input) {
            case 1:
                registerUser(authData, userAuth, userID, countUser, ptrCountDeleteUser);
                break;
            case 2:
                authorization(authData, userAuth, authAdmin, userID, countUser, counterMax);
                break;
            case 3:
                forgotLogin(authData, countUser);
                break;
            case 4:
                editPass(authData, countUser, counterMax);
                break;
            case 5:
                cout << "# Всего хорошего!\n";
                return 0;
            default:
                cout << "# Отсутствующий пункт меню!\n";
            }
            cout << endl;
        }
    } while (true);
}

/**
 * Функция вывода меню
 *
 * @return Индекс действия
 */
int menu(bool userAuth, bool authAdmin, string menuNotAuthData[], int lengthMenuNotAuthData, string menuAuthData[],
    int lengthMenuAuthData, string menuAdminhData[], int lengthMenuAdminData,
    user authData[128], int userID) {
    while (true) {
        int maxLengthMenu = 0;
        int input;
        cout << "# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #\n";
        cout << "# - - - - - - - -  Меню - - - - - - - - # ";
        cout << (userAuth ? "Добро пожаловать,\t\t\t#\n" : "Выполните вход.\t\t\t#\n");
        if (authAdmin) {
            for (int i = 0; i < lengthMenuAdminData; i++) {
                if (i == 0) {
                    cout << "# " << (i + 1) << ". " << menuAdminhData[i] << "# ";
                    cout << authData[userID].login << "!\t\t\t\t#\n";
                    continue;
                }
                if (i == 1) {
                    cout << "# " << (i + 1) << ". " << menuAdminhData[i] << "# ";
                    cout << "userID: " << userID << "\t\t\t\t#\n";
                    continue;
                }
                if (i == 2) {
                    cout << "# " << (i + 1) << ". " << menuAdminhData[i] << "# ";
                    cout << "e-mail: " << authData[userID].email << "\t\t\t#\n";
                    continue;
                }
                if (i == 3) {
                    cout << "# " << (i + 1) << ". " << menuAdminhData[i] << "# ";
                    cout << "тел.: " << authData[userID].phoneNumber << "\t\t\t#\n";
                    continue;
                }
                cout << "# " << (i + 1) << ". " << menuAdminhData[i] << "#\t\t\t\t\t#\n";
            }
            maxLengthMenu = lengthMenuAdminData;
        }
        else if (userAuth) {
            for (int i = 0; i < lengthMenuAuthData; i++) {
                if (i == 0) {
                    cout << "# " << (i + 1) << ". " << menuAuthData[i] << "# ";
                    cout << authData[userID].login << "!\t\t\t\t#\n";
                    continue;
                }
                if (i == 1) {
                    cout << "# " << (i + 1) << ". " << menuAuthData[i] << "# ";
                    cout << "userID: " << userID << "\t\t\t\t#\n";
                    continue;
                }
                if (i == 2) {
                    cout << "# " << (i + 1) << ". " << menuAuthData[i] << "# ";
                    cout << "e-mail: " << authData[userID].email << "\t\t\t#\n";
                    continue;
                }
                if (i == 3) {
                    cout << "# " << (i + 1) << ". " << menuAuthData[i] << "# ";
                    cout << "тел.: " << authData[userID].phoneNumber << "\t\t\t#\n";
                    continue;
                }
                cout << "# " << (i + 1) << ". " << menuAuthData[i] << "#\t\t\t\t\t#\n";
            }
            maxLengthMenu = lengthMenuAuthData;
        }
        else {
            for (int i = 0; i < lengthMenuNotAuthData; i++) {
                cout << "# " << (i + 1) << ". " << menuNotAuthData[i] << "#\t\t\t\t\t#\n";
            }
            maxLengthMenu = lengthMenuNotAuthData;
        }

        cout << "# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #\n\n";
        cout << "# Выберите пункт меню: ";
        cin >> input;
        for (int i = 1; i <= maxLengthMenu; i++) {
            if (i == input) {
                return input;
            }
            else if (input > maxLengthMenu) {
                cout << "# Некорректный ввод!\n\n";
                break;
            }
            else {
                continue;
            }
        }
    }
}

/**
 * Удаляет из логина пробелы с начала и с конца
 *
 * @param word Введенный логин
 */
void trim(string& word) {
    int startI = 0;
    int finishI = word.length();
    for (int i = 0; i < word.length(); i++) {
        if (word[i] == ' ') {
            startI++;
        }
        if (word[word.length() - i] == ' ') {
            finishI--;
        }
        if (word[i] != ' ' && word[word.length() - i] != ' ') {
            break;
        }
    }
    word = word.substr(static_cast<unsigned int>(startI), static_cast<unsigned int>(finishI - startI));
}

/**
 * Проверяет данные на длину ввода данных от 8 до 256
 *
 * @param word Вводимое слово
 * @return да/нет
 */
bool checkField(string word) {
    trim(word);
    return (word.length() < 8) || (word.length() > 256);
}

/**
 * Проверяет есть ли уже такие строковые данные в базе
 *
 * Эту функцию нужно переписать и избавиться от enum в программе, так как перешли на хранение данных пользователей в формате struct
 *
 * @param authData Данные о ригистрации пользователя
 * @param loginUser Вводимый логин пользователя
 * @param countUser Количество пользователей
 * @return есть такой логин в базе или нет
 */
bool dataExist(user authData[128], int index, const string& dataUser, int countUser) {
    bool temp = false;
    for (int i = 0; i <= countUser; i++) {
        switch (index) {
        case 0:
            if (authData[i].login == dataUser) {
                temp = true;
            }
            break;
        case 1:
            if (authData[i].email == dataUser) {
                temp = true;
            }
            break;
        case 2:
            if (authData[i].phoneNumber == dataUser) {
                temp = true;
            }
            break;
        default:
            break;
        }
    }
    return temp;
}

/**
 * Проверяет пароль на совпадение с БД
 *
 * @param authData Данные о ригистрации пользователя
 * @param dataUser Пароль вводимый пользователем
 * @param userID ID пользователя
 * @return есть такой логин в базе или нет
 */
bool passwordExist(user authData[128], const string& dataUser, int userID) {
    bool temp = false;
    if (authData[userID].password == dataUser) {
        temp = true;
    }
    return temp;
}

/**
* Запрашивает у пользователя повтора попытки или завершения операции
*/
bool breakOrContinueForError() {
    cout << "# 0 - выход, >1 - продолжить\n# ";
    int input;
    cin >> input;
    return input != 0;
}

/**
* Прохождение валидации e-mail
*/
bool emailValidator(string email) {
    bool valid = true;
    int all_presents = 0;
    string suitable_symbols = "-_.@"; // допустимые символы помимо латинских букв и цифр
    for (int i = 0; email[i] != '\0'; i++) {
        if (!isalpha(email[i]) && !isdigit(email[i]) && suitable_symbols.find(email[i]) == string::npos) { return false; } // наличие недопустимого символа
        if (i > 0 && email[i] == '.' && email[i - 1] == '.') { valid = false; break; } // две точки подряд
        if (email.length() < 7) { valid = false; break; } // проверка на длину (мин. 7 символов)
        else if (i > 0 && email[i] == '@' && !all_presents) { all_presents = 1; } // наличие @
        else if (email[i] == '@' && all_presents) { valid = false; break; } // наличие двух @ в строчке
        else if ((isalpha(email[i]) || isdigit(email[i])) && all_presents == 1) { all_presents = 2; } // наличие буквы или цифры после @
        else if (email[i] == '.' && all_presents == 2) { all_presents = 3; } // наличие точки после символа( который после @ )
        else if (isalpha(email[i]) && all_presents == 3 && email[i + 1] == '\0') { all_presents = 4; } // наличие буквы в конце строчки
    }
    if (valid && all_presents == 4) { return valid; }
    return false;
}

/**
* Прохождение валидации номера телефона
*/
bool phoneNumberValidator(string phoneNumber) {
    string numberStr;

    // убираем из номера телефона + или +7 в начале
    for (int i = 0; i < phoneNumber[i] != '\0'; i++) {
        if (phoneNumber[i] == '+' && i == 0 || phoneNumber[i] == '7' &&
            i == 0 || phoneNumber[0] == '+' && phoneNumber[1] == '7' && i == 1)
        {
            continue;
        }
        else {
            numberStr += phoneNumber[i];
        }
    }
    return numberStr.length() == 10;
}

/**
 * Регистрация пользователя
 *
 * @param authData Данные о ригистрации пользователя
 * @param userAuth Флаг авторизации пользователя
 * @param userID ID пользователя
 * @param countUser Количество пользователей
 */
void registerUser(user authData[128], bool& userAuth, int& userID, int& countUser, int*& ptrCountDeleteUser) {
    string loginUser;
    string passUser;
    string email;
    string phoneNumber;
    int boolAdminInt;
    bool boolAdminStr;
    bool valid;

    cout << "# - - Регистрация - -" << '\n';
    while (true) {
        cout << "# Введите логин" << "\n# ";
        cin >> loginUser;
        if (checkField(loginUser)) {
            cout << "# Неверная длина логина. Введите логин длиной от 8 до 256 символов!\n\n";
            valid = breakOrContinueForError();
            if (!valid) { return; }
            continue;
        }
        if (dataExist(authData, LOGIN, loginUser, countUser)) {
            cout << "# Логин занят. Введите другой логин.\n\n";
            valid = breakOrContinueForError();
            if (!valid) { return; }
            continue;
        }
        break;
    }

    while (true) {
        cout << "# Введите e-mail" << "\n# ";
        cin >> email;
        if (!emailValidator(email)) {
            cout << "# Введенный адрес недействительный или не содержит все необходимые данные\n\n";
            valid = breakOrContinueForError();
            if (!valid) { return; }
            continue;
        }
        if (dataExist(authData, EMAIL, email, countUser)) {
            cout << "# e-mail занят. Введите другой e-mail.\n\n";
            valid = breakOrContinueForError();
            if (!valid) { return; }
            continue;
        }
        break;
    }

    while (true) {
        cout << "# Введите телефон" << "\n# +7 ";
        cin >> phoneNumber;
        if (!phoneNumberValidator(phoneNumber)) {
            cout << "# Введенный номер телефона недействительный или не содержит все необходимые данные\n\n";
            valid = breakOrContinueForError();
            if (!valid) { return; }
            continue;
        }
        if (dataExist(authData, PHONE_NUMBER, phoneNumber, countUser)) {
            cout << "# Такой телефон уже зарегистрирован. Введите другой номер телефона.\n\n";
            valid = breakOrContinueForError();
            if (!valid) { return; }
            continue;
        }
        break;
    }

    while (true) {
        cout << "# Введите пароль" << "\n# ";
        cin >> passUser;
        if (checkField(passUser)) {
            cout << "# Неверная длина пароля. Введите пароль длиной от 8 до 256 символов!\n\n";
            valid = breakOrContinueForError();
            if (!valid) { return; }
            continue;
        }
        break;
    }

    while (true) {
        cout << "# Хотите стать администратором? 0 - нет, >1 - да" << "\n# ";
        cin >> boolAdminInt;
        if (boolAdminInt) {
            boolAdminStr = true;
        }
        else {
            boolAdminStr = false;
        }
        break;
    }

    // если есть свободные ячеки в БД то записываем данные нового пользователя в них
    if (ptrCountDeleteUser == nullptr) {
        userID = ++countUser;
    }
    else {
        userID = ptrCountDeleteUser[1];
        if (ptrCountDeleteUser[0] == 1) {
            delete[]ptrCountDeleteUser;
            ptrCountDeleteUser = nullptr;
        }
        else {
            // создаем временный указатель на счетчик удаленных пользователей откуда удаляем занятую ячеку памяти
            auto* ptrTemp = new int[ptrCountDeleteUser[0]];
            ptrTemp[0] = --ptrCountDeleteUser[0];
            for (int k = 1; k <= ptrTemp[0]; k++) {
                ptrTemp[k] = ptrCountDeleteUser[k + 1];
            }
            delete[]ptrCountDeleteUser;   // освобождаем память со старыми данными свободых ячеек в БД
            ptrCountDeleteUser = ptrTemp; // теперь в указатели хранится массив с нашими новыми пустыми ячеками
            // ptrTemp = nullptr;         // выходит из области видимости и уничтожается, не обязательно обнулять
        }
    }

    user newUser = { userID , loginUser , passUser , email , phoneNumber , boolAdminStr };
    authData[userID] = newUser;
}

/**
 * Авторизация пользователя
 *
 * @param authData
 * @param userAuth
 * @param authAdmin
 * @param userID
 * @param countUser
 * @param counterMax
 */
void authorization(user authData[128], bool& userAuth, bool& authAdmin, int& userID, int& countUser, int counterMax) {
    string loginUser;
    string passUser;
    bool flag = false;
    bool valid;
    cout << "# - - Авторизация - -" << '\n';
    while (true) {
        int counter = 0;
        while (!flag) {
            cout << "# Введите логин" << "\n# ";
            cin >> loginUser;
            if (checkField(loginUser)) {
                cout << "# Неверная длина логина. Введите логин длиной от 8 до 256 символов!\n\n";
                valid = breakOrContinueForError();
                if (!valid) { return; }
                continue;
            }
            for (int i = 0; i <= countUser; i++) {
                if (authData[i].login == loginUser) {
                    flag = true;
                    userID = i;
                    break;
                }
            }
            if (!flag) {
                cout << "# Логин отсутствует.\n\n";
                counter++;
                if (counter >= counterMax) {
                    cout << "# Превышено количество попыток ввода логина\n\n";
                    return;
                }
                valid = breakOrContinueForError();
                if (!valid) { return; }
            }
        }

        flag = false;
        counter = 0;
        while (!flag) {
            cout << "# Введите пароль" << "\n# ";
            cin >> passUser;
            if (checkField(passUser)) {
                cout << "# Неверная длина пароля. Введите пароль длиной от 8 до 256 символов!\n\n";
                valid = breakOrContinueForError();
                if (!valid) { return; }
                continue;
            }
            flag = passwordExist(authData, passUser, userID);
            if (!flag) {
                cout << "# Неверный пароль" << "\n# ";
                counter++;
                if (counter >= counterMax) {
                    cout << "# Превышено количество попыток ввода пароля\n\n";
                    return;
                }
                valid = breakOrContinueForError();
                if (!valid) { return; }
            }
        }

        if (authData[userID].adminRights == true) {
            authAdmin = true;
        }
        else {
            userAuth = true;
        }
        break;
    }
}

/**
 * Логаут пользователя
 *
 * @param user Вод пользователя (админ или юзер)
 */
void logoutUser(bool& user) {
    user = false;
}

/**
 * Функция редактирования пароля из меню авторизованного пользователя
 *
 * @param authData
 * @param userID
 * @param counterMax
 */
void editMyPass(user authData[128], int userID, int counterMax) {
    string passUser;
    int counter = 0;
    bool flag = false;
    bool valid;
    while (!flag) {
        cout << "# Введите старый пароль\n # ";
        cin >> passUser;
        if (checkField(passUser)) {
            cout << "# Неверная длина пароля. Введите пароль длиной от 8 до 256 символов!\n\n";
            valid = breakOrContinueForError();
            if (!valid) { return; }
            continue;
        }
        flag = passwordExist(authData, passUser, userID);
        if (!flag) {
            cout << "# Неверный пароль" << "\n# ";
            counter++;
            if (counter >= counterMax) {
                cout << "# Превышено количество попыток ввода пароля\n\n";
                break;
            }
            valid = breakOrContinueForError();
            if (!valid) { return; }
        }
    }
    if (counter >= counterMax) {
        flag = false;
    }
    while (flag) {
        cout << "Введите новый пароль" << "\n# ";
        cin >> passUser;
        if (checkField(passUser)) {
            cout << "# Неверная длина пароля. Введите пароль длиной от 8 до 256 символов!\n\n";
            valid = breakOrContinueForError();
            if (!valid) { return; }
            continue;
        }
        authData[userID].password = passUser;
        break;
    }
}

/**
 * Функция редактирования пароля из меню неавторизованного пользователя
 *
 * @param authData
 * @param countUser
 * @param counterMax
 */
void editPass(user authData[128], int countUser, int counterMax) {
    string passUser;
    int counter = 0;
    bool flag = false;
    string loginUser;
    bool valid;
    while (!flag) {
        cout << "Введите логин" << "\n# ";
        cin >> loginUser;
        if (checkField(loginUser)) {
            cout << "# Неверная длина логина. Введите логин длиной от 8 до 256 символов!\n\n";
            valid = breakOrContinueForError();
            if (!valid) { return; }
            continue;
        }
        for (int i = 0; i <= countUser; i++) {
            if (authData[i].login == loginUser) {
                flag = true;
                editMyPass(authData, i, counterMax);
                break;
            }
        }
        if (!flag) {
            cout << "# Логин отсутствует.\n";
            valid = breakOrContinueForError();
            if (!valid) { return; }
        }
        counter++;
        if (counter >= counterMax) {
            cout << "# Превышено количество попыток ввода логина\n\n";
            return;
        }
    }
}

/**
 * Функция восстановления логина по ID
 *
 * @param authData
 * @param countUser
 */
void forgotLogin(user authData[128], int countUser) {
    int input;
    bool valid;
    while (true) {
        cout << "# Для восстановление логина введите ваш ID\n# ";
        cin >> input;
        for (int i = 0; i <= countUser; i++) {
            if (i == input) {
                cout << "# Ваш логин: " << authData[input].login << '\n';
                return;
            }
        }
        cout << "# ID не найден!\n";
        valid = breakOrContinueForError();
        if (!valid) { return; }
    }
}

/**
* Удаление пользователя
*/
bool deleteUser(user authData[128], const string& login, int countUser, int*& ptrCountDeleteUser) {
    for (int i = 0; i <= countUser; i++) {
        if (authData[i].login == login) {
            authData[i].login = "0";
            authData[i].password = "0";
            authData[i].email = "0";
            authData[i].phoneNumber = "0";
            authData[i].adminRights = false;

            if (ptrCountDeleteUser == nullptr) {
                ptrCountDeleteUser = new int[2];
                ptrCountDeleteUser[0] = 1;
                ptrCountDeleteUser[1] = i;
            }
            else {
                // создаем временный указатель на счетчик удаленных пользователей куда добавляем нового удаленного
                auto* ptrTemp = new int[ptrCountDeleteUser[0] + 1];
                ptrTemp[0] = ++ptrCountDeleteUser[0];
                for (int k = 1; k <= ptrCountDeleteUser[0]; k++) {
                    ptrTemp[k] = ptrCountDeleteUser[k];
                }
                ptrTemp[ptrCountDeleteUser[0]] = i;
                delete[]ptrCountDeleteUser;   // освобождаем память со старыми данными свободых ячеек в БД
                ptrCountDeleteUser = ptrTemp; // теперь в указатели хранится массив с нашими новыми пустыми ячеками
                // ptrTemp = nullptr;         // выходит из области видимости и уничтожается, не обязательно обнулять
            }
            cout << "# Пользователь удален!\n\n";
            return true;
        }
    }
    cout << "# Логин не найден\n\n";
    return false;
}

/**
* Печать базы данных (показывает данные всех пользователей)
*/
void printAuthData(user authData[128], int countUser, int* ptrCountDeleteUser) {
    cout << "# -- База данных --" << '\n';
    cout << "# Всего зарегиcтрированно пользователей: " << (ptrCountDeleteUser == nullptr ? countUser : (countUser - ptrCountDeleteUser[0])) << "\n\n";
    cout << "# userID" << "" <<
        "# login" << "\t\t" <<
        "# e-mail" << "\t" <<
        "# password" << "\t" <<
        "# phone number" << "\t" <<
        "# access level" << "\n";
    for (int i = 0; i <= countUser; i++) {
        cout << i << "\t" <<
            authData[i].login << "\t" <<
            authData[i].email << "\t" <<
            authData[i].password << "\t" <<
            authData[i].phoneNumber << "\t" <<
            (authData[i].adminRights == true ? "администратор" : "пользователь");
        cout << endl;
    }
}


/**
* Редактор логина из меню
*/
void editMyLogin(user authData[128], int userID, int countUser) {
    string passUser;
    string loginUser;
    bool valid;
    while (true) {
        cout << "# Введите новый логин" << "\n# ";
        cin >> loginUser;
        if (checkField(loginUser)) {
            cout << "# Неверная длина логина. Введите логин длиной от 8 до 256 символов!\n\n";
            valid = breakOrContinueForError();
            if (!valid) { return; }
            continue;
        }
        if (dataExist(authData, LOGIN, loginUser, countUser)) {
            cout << "# Логин занят. Введите другой логин.\n\n";
            valid = breakOrContinueForError();
            if (!valid) { return; }
            continue;
        }
        break;
    }
    authData[userID].login = loginUser;
}

/**
* Редактор почты из меню
*/
void editMyEmail(user authData[128], int userID, int countUser) {
    string passUser;
    string email;
    bool valid;
    while (true) {
        cout << "# Введите новый e-mail" << "\n# ";
        cin >> email;
        if (!emailValidator(email)) {
            cout << "# Введенный адрес недействительный или не содержит все необходимые данные\n\n";
            valid = breakOrContinueForError();
            if (!valid) { return; }
            continue;
        }
        if (dataExist(authData, EMAIL, email, countUser)) {
            cout << "# e-mail занят. Введите другой e-mail.\n\n";
            valid = breakOrContinueForError();
            if (!valid) { return; }
            continue;
        }
        break;
    }
    authData[userID].email = email;
}

/**
* Редактор телефона из меню
*/
void editMyPhoneNumber(user authData[128], int userID, int countUser) {
    string passUser;
    string phoneNumber;
    bool valid;
    while (true) {
        cout << "# Введите новый телефон" << "\n# +7 ";
        cin >> phoneNumber;
        if (!phoneNumberValidator(phoneNumber)) {
            cout << "# Введенный номер телефона недействительный или не содержит все необходимые данные\n\n";
            valid = breakOrContinueForError();
            if (!valid) { return; }
            continue;
        }
        if (dataExist(authData, PHONE_NUMBER, phoneNumber, countUser)) {
            cout << "# Такой телефон уже зарегистрирован. Введите другой номер телефона.\n\n";
            valid = breakOrContinueForError();
            if (!valid) { return; }
            continue;
        }
        break;
    }
    authData[userID].phoneNumber = phoneNumber;
}