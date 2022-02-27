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
    LL first_prime, second_prime, n, fi;
    std::vector<LL> encrypt_message_in_dec;
    std::string message_need_to_encrypt, encrypt_message = "", decrypt_message = "";
    std::pair<LL, LL> private_key, public_key;


    std::srand(std::time(NULL));


    MakeArrayWithPrimeNumbersFromElevenToN();


    first_prime = GetRandomPrimeNumber();
    second_prime = GetRandomPrimeNumber();

    while (second_prime == first_prime)
    {
        second_prime = GetRandomPrimeNumber();
    }

    n = first_prime * second_prime;
    public_key.second = n;
    private_key.second = n;

    fi = (first_prime - 1) * (second_prime - 1);


    public_key.first = GetFirstPartPublicKey(fi);
    private_key.first = GetFirstPartPrivateKey(fi, public_key.first);

    std::cout << "First_prime = " << first_prime << " Second_prime = "
        << second_prime << " FI = " << fi << std::endl;


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

    for (int i = 11; i <= N; i++)
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
    int i = 2;

    while (first_part_public_key * i % fi != 1)
    {
        i++;
    }

    return i;
}


LL GetEncryptNumber(LL number, std::pair<LL, LL> public_key)
{
    LL encryptNumber = 1, current = number - 97;

    for (LL i = 0; i < public_key.first; i++)
    {
        encryptNumber *= current;
        encryptNumber = encryptNumber % public_key.second;
    }

    return encryptNumber;
}




void EncryptMessage(std::string message, std::vector<LL>& encrypt_message_in_dec,
    std::pair<LL, LL> public_key, std::string& encrypt_message)
{
    for (char c : message)
    {
        encrypt_message_in_dec.push_back(GetEncryptNumber(c, public_key));
        encrypt_message += std::to_string(encrypt_message_in_dec.back()) + " ";
    }
}

LL GetDecryptNumber(LL number, std::pair<LL, LL> private_key)
{
    LL decryptNumber = 1, current = number;

    for (LL i = 0; i < private_key.first; i++)
    {
        decryptNumber *= current;
        decryptNumber %= private_key.second;
    }

    return decryptNumber + 97;
}

void DecryptMessage(std::vector<LL>& encrypt_message_in_dec, std::string& decrypt_message,
    std::pair<LL, LL> private_key)
{
    for (LL c : encrypt_message_in_dec)
    {
        decrypt_message += char(GetDecryptNumber(c, private_key));
    }
}