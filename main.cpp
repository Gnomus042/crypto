#include "AES.h"
#include "Kalyna.h"
#include <string>
#include <iostream>

using namespace std;

string str_key = "Thats my Kung Fu";
string str_data = "Two One Nine Two";


int main() {
    // vector<uint8_t> key{0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF};
    vector<uint8_t> key(str_key.begin(), str_key.end());
    vector<uint8_t> data(str_data.begin(), str_data.end());
    //Kalyna kalyna = Kalyna(key, 128, 128);
    AES aes = AES(key);
    vector<uint8_t> encrypted = aes.encrypt(data);
    for (int i = 0; i < encrypted.size(); i++) {
        cout << hex << (int)encrypted[i]<<" ";
    }
    cout << endl;
    vector<uint8_t> decrypted = aes.decrypt(encrypted);
    for (int i = 0; i < decrypted.size(); i++) {
        cout << decrypted[i];
    }
    cout << endl;
    return 0;
}