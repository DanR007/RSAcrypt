#include <iostream>
#include <math.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <filesystem>
#include <fstream>

//край для поиска простых чисел
//N >= 13
#define N 8000
#define LL long long int
#define PR_LL %lld

void MakeArrayWithPrimeNumbersFromElevenToN();
LL GetRandomPrimeNumber();
LL GetRandomPrimeInRange(LL left_border, LL right_border);

void EncryptMessage(std::string message, std::vector<LL>& encrypt_message_in_dec,
    std::pair<LL, LL> public_key, std::string& encrypt_message);
LL GetEncryptNumber(LL number, std::pair<LL, LL> public_key);

void DecryptMessage(std::vector<LL>& encrypt_message_in_dec, std::string& decrypt_message,
    std::pair<LL, LL> private_key);
LL GetDecryptNumber(LL number, std::pair<LL, LL> private_key);


LL GetFirstPartPublicKey(LL fi);
LL GetFirstPartPrivateKey(LL fi, LL first_part_public_key);

//массив для простых чисел
std::vector<LL> g_array_prime_numbers;

//файлы для кодирования сообщений
std::ifstream g_file_message(std::filesystem::current_path().string() + "/message.txt");
std::ofstream g_file_encrypt_message(std::filesystem::current_path().string() + "/encrypt_message.txt");
std::ifstream g_file_need_encrypt_message(std::filesystem::current_path().string() + "/encrypt_message.txt");

int main()
{
    //шифрование RSA
    //first_prime (p) second_prime (q)
    //n = q * p или first_prime * second_prime
    //fi это функция Эйлера fi = (first_prime - 1) * (second_prime - 1)
    //
    LL first_prime, second_prime, n, fi;
    std::vector<LL> encrypt_message_in_dec;
    std::string message_need_to_encrypt, encrypt_message = "", decrypt_message = "";
    std::pair<LL, LL> private_key, public_key;

    //постоянно рандомные числа
    std::srand(std::time(NULL));

    //заполняем глобальный массив простыми числами
    MakeArrayWithPrimeNumbersFromElevenToN();

    //получаю рандомные простые числа из тех, которые есть в массиве
    first_prime = GetRandomPrimeNumber();
    second_prime = GetRandomPrimeNumber();

    while (second_prime == first_prime)
    {
        second_prime = GetRandomPrimeNumber();
    }

    //составляем модуль произведений и вторые части ключей
    n = first_prime * second_prime;
    public_key.second = n;
    private_key.second = n;

    //вычисляем функцию Эйлера
    fi = (first_prime - 1) * (second_prime - 1);

    //вычисляем первую часть открытого ключа
    public_key.first = GetFirstPartPublicKey(fi);
    private_key.first = GetFirstPartPrivateKey(fi, public_key.first);
    //вывод полученных простых чисел и числа Эйлера для проверки
    std::cout << "First_prime = " << first_prime << " Second_prime = "
        << second_prime << " FI = " << fi << std::endl;

    //считываем значения с файла
    std::getline(g_file_message, message_need_to_encrypt);

    std::cout << "Your message is: " << std::endl;
    std::cout << message_need_to_encrypt << std::endl;

    std::cout << std::endl << std::endl;

    printf("Open Key is: (%lld, %lld)\n", public_key.first, public_key.second);
    printf("Secret Key is: (%lld, %lld)\n", private_key.first, private_key.second);

    EncryptMessage(message_need_to_encrypt, encrypt_message_in_dec, public_key, encrypt_message);
    printf("Encrypt message is: \n");
    std::cout << encrypt_message << std::endl;

    g_file_encrypt_message << encrypt_message;

    g_file_need_encrypt_message >> encrypt_message;

    encrypt_message_in_dec.clear();
    size_t pos = encrypt_message.find(" ");
    while (pos != std::string::npos)
    {
        encrypt_message_in_dec.push_back((LL)std::stoi(encrypt_message.substr(0, pos)));
        encrypt_message.erase(0, pos + 1);
        pos = encrypt_message.find(" ");
    }


    DecryptMessage(encrypt_message_in_dec, decrypt_message, private_key);
    printf("Decrypt message is: \n");
    std::cout << decrypt_message << std::endl;
}





void MakeArrayWithPrimeNumbersFromElevenToN()
{
    std::vector <bool> additional_array;
    //работаем по принципу решета Эратосфена
    additional_array.resize(N + 1);
    additional_array[0] = additional_array[1] = false;

    for (LL i = 2; i <= N; i++)
    {
        additional_array[i] = true;
    }

    for (LL i = 2; i * i <= N; i++)
    {
        if (additional_array[i])
        {
            for (LL j = i * i; j <= N; j += i)
            {
                additional_array[j] = false;
            }
        }
    }
    //я решил брать крайним простым числом "11" просто так 
    //(чтобы были большие значения при маленьких значениях N)
    for (int i = 3; i <= N; i++)
    {
        if (additional_array[i])
        {
            g_array_prime_numbers.push_back(i);
        }
    }
}

LL GetRandomPrimeNumber()
{
    return g_array_prime_numbers[std::rand() % g_array_prime_numbers.size()];
}


LL GetRandomPrimeInRange(LL left_border, LL right_border)
{
    return g_array_prime_numbers[std::rand() % g_array_prime_numbers.size()];
}

LL GetFirstPartPublicKey(LL fi)
{
    //это число (e) должно быть простым, меньше значения функции Эйлера (fi),
    //а также взаимнопростым с ним (fi не должно делиться на e)
    for (LL first_part : g_array_prime_numbers)
    {
        if ((fi % first_part != 0) && (fi > first_part))
        {
            return first_part;
        }
    }

    return -1;
}

LL GetFirstPartPrivateKey(LL fi, LL first_part_public_key)
{
    //произведение этого числа (это число есть d) и первой части открытого ключа (e)
    //должно иметь остаток от деления (1) на число Эйлера (fi)
    int i = 2;

    while (first_part_public_key * i % fi != 1)
    {
        i++;
        //std::cout << i << std::endl;
    }

    return i;
}


LL GetEncryptNumber(LL number, std::pair<LL, LL> public_key)
{
    //кодирование происходит по схеме:
    //1) возводим число, которое хотим зашифровать в степень 
    //(ранее вычисленную первую часть открытого ключа) number^public_key.first
    //2) получаем остаток от деления на вторую часть открытого ключа
    //number^public_key.first % public_key.second
    LL encryptNumber = 1, current = number - 97;

    for (LL i = 0; i < public_key.first; i++)
    {
        //так как компьютер не умеет работать с такими большими числами я решил взять логику,
        //которую подсмотрел в этом репозитории: (если не заменять его код частично моим,
        // то у него всё работает)
        //https://gist.github.com/SergiyOsadchyy/d64fe7e1f9847a4b9efaea198302b850
        //как я понимаю это место проблемы всего кода, но я не знаю, как 
        //правильно работать с огромными числами
        encryptNumber *= current;
        encryptNumber = encryptNumber % public_key.second;
        //до того как я полностью скопировал оттуда код у меня была примерно такая реализация:
        /* --------------------------------------------------- */
        /* --------------------------------------------------- */
        /* --------------------------------------------------- */
        /* --------------------------------------------------- */
        //LL encryptNumber = number;

        //for (LL i = 0; i < public_key.first; i++)
        //{
        //    encryptNumber *= number;
        //    encryptNumber = encryptNumber % public_key.second;

       //}
       // 
        //return decryptNumber;
        /* --------------------------------------------------- */
        /* --------------------------------------------------- */
        /* --------------------------------------------------- */
        /* --------------------------------------------------- */

    }
    //std::cout << encryptNumber << std::endl;
    return encryptNumber;
}




void EncryptMessage(std::string message, std::vector<LL>& encrypt_message_in_dec,
    std::pair<LL, LL> public_key, std::string& encrypt_message)
{
    for (char c : message)
    {// тут я получаю значение нового символа ASCII из зашифрованного числа
        encrypt_message_in_dec.push_back(GetEncryptNumber(c, public_key));
        encrypt_message += std::to_string(encrypt_message_in_dec.back()) + " ";
    }
}

LL GetDecryptNumber(LL number, std::pair<LL, LL> private_key)
{
    //дешифрование проводится примерно также, как и шифрование:
    //1) возводим число в степень первой части закрытого ключа 
    // answer = (number^private_key.first)
    //2) получаем остаток от деления на вторую часть закрытого ключа 
    //answer % private_key.second
    LL decryptNumber = 1, current = number;

    for (LL i = 0; i < private_key.first; i++)
    {
        //так как компьютер не умеет работать с такими большими числами я решил взять логику,
        //которую подсмотрел в этом репозитории: (если не заменять его код частично моим,
        // то у него всё работает)
        //https://gist.github.com/SergiyOsadchyy/d64fe7e1f9847a4b9efaea198302b850
        //как я понимаю это место проблемы всего кода, но я не знаю, как 
        //правильно работать с огромными числами
        decryptNumber *= current;
        decryptNumber %= private_key.second;

        //до того как я полностью скопировал оттуда код у меня была примерно такая реализация:
        /* --------------------------------------------------- */
        /* --------------------------------------------------- */
        /* --------------------------------------------------- */
        /* --------------------------------------------------- */
        //LL decryptNumber = number;

        //for (LL i = 0; i < private_key.first; i++)
        //{
        //    decryptNumber *= number;
        //    decryptNumber = decryptNumber % private_key.second;

       //}

        //return decryptNumber;
        /* --------------------------------------------------- */
        /* --------------------------------------------------- */
        /* --------------------------------------------------- */
        /* --------------------------------------------------- */


    }

    return decryptNumber + 97;
}

void DecryptMessage(std::vector<LL>& encrypt_message_in_dec, std::string& decrypt_message,
    std::pair<LL, LL> private_key)
{
    for (LL c : encrypt_message_in_dec)
    {// тут я получаю значение нового символа ASCII из декодированного числа
        decrypt_message += char(GetDecryptNumber(c, private_key));
    }
}