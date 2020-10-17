#include "Lab1/AES.h"
#include "Lab2/RC4.h"
#include "Lab2/Salsa20.h"
#include <string>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string read_file(string path) {
    ifstream in(path.c_str(), ios_base::binary);
    in.seekg(0, ios::end);
    size_t length = in.tellg();
    in.seekg(0, ios::beg);

    string s = string(length, ' ');
    in.read(s.data(), length);
    return s;
}

int main() {
    string key_str = "12345678901234561234567890123456";
    string data_str = "Absolutely random sentence";

    vector<uint8_t> key(begin(key_str), end(key_str));
    vector<uint8_t> data(begin(data_str), end(data_str));

    Salsa20 salsa = Salsa20(key);
    vector<uint8_t> encrypted = salsa.crypt(data);
    for (int i = 0; i < encrypted.size(); i++) {
        cout << encrypted[i];
    }
    cout << endl;
    vector<uint8_t> decrypted = salsa.crypt(encrypted);
    for (int i = 0; i < decrypted.size(); i++) {
        cout << decrypted[i];
    }
    cout << endl;
}