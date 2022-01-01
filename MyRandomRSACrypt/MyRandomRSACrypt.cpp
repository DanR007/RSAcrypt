#include <iostream>
#include <math.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h>

//край для поиска простых чисел
//N >= 13
#define N 13

void MakeArrayWithPrimeNumbersFromElevenToN();
long int GetRandomPrimeNumber();
long int GetRandomPrimeInRange(long int left_border, long int right_border);

void EncryptMessage(std::string message, std::string& encrypt_message, 
    std::pair<long int, long int> public_key);
long int GetEncryptNumber(long int number, std::pair<long int, long int> public_key);

void DecryptMessage(std::string encrypt_message, std::string& decrypt_message,
    std::pair<long int, long int> private_key);
long int GetDecryptNumber(long int number, std::pair<long int, long int> private_key);


long int GetFirstPartPublicKey(long int fi);
long int GetFirstPartPrivateKey(long int fi, long int first_part_public_key);

//массив для простых чисел
std::vector<long int> g_array_prime_numbers;

int main()
{
    //шифрование RSA
    //first_prime (p) second_prime (q)
    //n = q * p или first_prime * second_prime
    //fi это функция Эйлера fi = (first_prime - 1) * (second_prime - 1)
    //
    long int first_prime, second_prime, n, fi;
    std::string message_need_to_encrypt, encrypt_message = "", decrypt_message = "";
    std::pair<long int, long int> private_key, public_key;

    //постоянно рандомные числа
    std::srand(std::time(NULL));

    //заполняем глобальный массив простыми числами
    MakeArrayWithPrimeNumbersFromElevenToN();

    //получаю рандомные простые числа из тех, которые есть в массиве
    first_prime = GetRandomPrimeNumber();
    second_prime = GetRandomPrimeNumber();

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
    std::cout <<"First_prime = " << first_prime << " Second_prime = "
        << second_prime << " FI = " << fi << std::endl;
    std::cout << "Enter your message: " << std::endl;

    std::cin >> message_need_to_encrypt;

    std::cout << std::endl << std::endl;

    EncryptMessage(message_need_to_encrypt, encrypt_message, public_key);

    printf("Open Key is: (%ld, %ld)\n", public_key.first, public_key.second);
    printf("Secret Key is: (%ld, %ld)\n", private_key.first, private_key.second);

    printf("Encrypt massage is: \n");
    std::cout << encrypt_message << std::endl;

    DecryptMessage(encrypt_message, decrypt_message, private_key);
    std::cout << decrypt_message << std::endl;
}





void MakeArrayWithPrimeNumbersFromElevenToN()
{
    std::vector <bool> additional_array;
    //работаем по принципу решета Эратосфена
    additional_array.resize(N + 1);
    additional_array[0] = additional_array[1] = false;

    for (long int i = 2; i <= N; i++)
    {
        additional_array[i] = true;
    }

    for (long int i = 2; i * i <= N; i++)
    {
        if (additional_array[i])
        {
            for (long int j = i * i; j <= N; j += i)
            {
                additional_array[j] = false;
            }
        }
    }
    //я решил брать крайним простым числом "11" просто так 
    //(чтобы были большие значения при маленьких значениях N)
    for (int i = 11; i <= N; i++)
    {
        if (additional_array[i])
        {
            g_array_prime_numbers.push_back(i);
        }
    }
}

long int GetRandomPrimeNumber()
{
    return g_array_prime_numbers[std::rand() % g_array_prime_numbers.size()];
}


long int GetRandomPrimeInRange(long int left_border, long int right_border)
{
    return g_array_prime_numbers[std::rand() % g_array_prime_numbers.size()];
}

long int GetFirstPartPublicKey(long int fi)
{
    //это число (e) должно быть простым, меньше значения функции Эйлера (fi),
    //а также взаимнопростым с ним (fi не должно делиться на e)
    bool flag = true;
    long int first_part;

    while (flag)
    {
        first_part = GetRandomPrimeInRange(0, fi);
        flag = (fi % first_part == 0) || (fi < first_part);
    }

    return first_part;
}

long int GetFirstPartPrivateKey(long int fi, long int first_part_public_key)
{
    //произведение этого числа (это число есть d) и первой части открытого ключа (e)
    //должно иметь остаток от деления (1) на число Эйлера (fi)
    int i = 2;

    while (first_part_public_key * i % fi != 1)
    {
        i++;
    }

    return i;
}


long int GetEncryptNumber(long int number, std::pair<long int, long int> public_key)
{
    //кодирование происходит по схеме:
    //1) возводим число, которое хотим зашифровать в степень 
    //(ранее вычисленную первую часть открытого ключа) number^public_key.first
    //2) получаем остаток от деления на вторую часть открытого ключа
    //number^public_key.first % public_key.second
    long int encryptNumber = 1, current = number - 97;

    for (long int i = 0; i < public_key.first; i++)
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
        //long int encryptNumber = number;

        //for (long int i = 0; i < public_key.first; i++)
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




void EncryptMessage(std::string message, std::string &encrypt_message, 
    std::pair<long int, long int> public_key)
{
    for (char c : message)
    {// тут я получаю значение нового символа ASCII из зашифрованного числа
        encrypt_message += char(GetEncryptNumber((long int)c, public_key));
    }
}

long int GetDecryptNumber(long int number, std::pair<long int, long int> private_key)
{
    //дешифрование проводится примерно также, как и шифрование:
    //1) возводим число в степень первой части закрытого ключа 
    // answer = (number^private_key.first)
    //2) получаем остаток от деления на вторую часть закрытого ключа 
    //answer % private_key.second
    long int decryptNumber = 1, current = number;

    for (long int i = 0; i < private_key.first; i++)
    {
        //так как компьютер не умеет работать с такими большими числами я решил взять логику,
        //которую подсмотрел в этом репозитории: (если не заменять его код частично моим,
        // то у него всё работает)
        //https://gist.github.com/SergiyOsadchyy/d64fe7e1f9847a4b9efaea198302b850
        //как я понимаю это место проблемы всего кода, но я не знаю, как 
        //правильно работать с огромными числами
        decryptNumber *= current;
        decryptNumber = decryptNumber % private_key.second;


        //до того как я полностью скопировал оттуда код у меня была примерно такая реализация:
        /* --------------------------------------------------- */
        /* --------------------------------------------------- */
        /* --------------------------------------------------- */
        /* --------------------------------------------------- */
        //long int decryptNumber = number;

        //for (long int i = 0; i < private_key.first; i++)
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

void DecryptMessage(std::string encrypt_message, std::string& decrypt_message,
    std::pair<long int, long int> private_key)
{
    for (char c : encrypt_message)
    {// тут я получаю значение нового символа ASCII из декодированного числа
        decrypt_message += char(GetDecryptNumber((long int)c, private_key));
    }
}