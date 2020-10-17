# Information security Lab #1

This is an example of the [Advanced Encryption Standard (AES)](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf) and [Kalyna](https://eprint.iacr.org/2015/650) (Ukrainian encryption standard) ciphers implementation. 
Core functions, which are the base for these two encryption standards (such as sub_bytes or mix_columns) are contained in the helper.cpp file, along with Block and Byte structures. 
Byte - a wrapper for uint8_t type, which implements Galois field operations (addition, multiplication, inverse, and pow).
Block - wrapper for key/block, mainly used to represent key/block structure in a form of a matrix.
Specific functions for AES are implemented in the AES class, the same is true for Kalyna.  Both cipher implementations support all possible block/key size combinations.
### Usage examples 
**AES:**
```c++
string str_key = "Thats my Kung Fu";
string str_data = "Two One Nine Two";
string init_vector_str = "This is an initt";

vector<uint8_t> key(str_key.begin(), str_key.end());
vector<uint8_t> data(str_data.begin(), str_data.end());
vector<uint8_t> init_vector(init_vector_str.begin(), init_vector_str.end());

AES aes = AES(key, init_vector);
// Here one of the 5 modes can be used: ECB, CBC, CFB, OFB, CTR.
vector<uint8_t> encrypted = kalyna.encrypt(data, AES::Mode::ECB);
for (int i = 0; i < encrypted.size(); i++) {
    cout << encrypted[i];
}
cout << endl;
// Here one of the 5 modes can be used: ECB, CBC, CFB, OFB, CTR.
vector<uint8_t> decrypted = kalyna.decrypt(encrypted, AES::Mode::ECB);
for (int i = 0; i < decrypted.size(); i++) {
    cout << decrypted[i];
}
cout << endl;
```
**Expected console output:**
```
┤╫⌠╚pá0ßà?║■╪└╡j
Two One Nine Two
```
**Kalyna**
```c++
string str_key = "Thats my Kung Fu";
string str_data = "Two One Nine Two";
vector<uint8_t> key(str_key.begin(), str_key.end());
vector<uint8_t> data(str_data.begin(), str_data.end());
Kalyna kalyna = Kalyna(key, 128, 128); // you need to specify key_size and block_size explicitly for Kalyna
vector<uint8_t> encrypted = kalyna.encrypt(data);
for (int i = 0; i < encrypted.size(); i++) {
    cout << encrypted[i];
}
cout << endl;
vector<uint8_t> decrypted = kalyna.decrypt(encrypted);
for (int i = 0; i < decrypted.size(); i++) {
    cout << decrypted[i];
}
cout << endl;
```
**Expected console output:**
```
Ñíq╢^x[1═òáO╝Aä%
Two One Nine Two
```
<br />

Also, there is a small subset of tests available in the *tests* folder (*test1.cpp*).

## Performance tests

| Cipher type  | 1Gb file Encrypt + Decrypt| 
| ------------- | ------------- |
| AES 128  |  239s  |
| AES 192  |  294s  |
| AES 256  |  354s  |
| Kalyna 128/128  |  958s  |
| Kalyna 128/256  |  971s |
| Kalyna 256/256  |  1795s  |
| Kalyna 256/512  |  1799s  |
| Kalyna 512/512  |  2548s  |

# Information security Lab #2

Example of stream ciphers implementation: RC4, Salsa20 and
5 modes of AES (ECB, CBC, CFB, OFB and CTR).
Core functions and structures are contained in helper.cpp, specific implementations
for RC4 and Salsa20 can be found in the Lab2 folder, AES modes are implemented in the
AES class from Lab1 as methods of the AES class.

### Ciphers usage examples (AES modes examples can be found in the Lab1 section)

**RC4**
```c++
string key_str = "12345678901234561234567890123456";
string data_str = "Absolutely random sentence";

vector<uint8_t> key(begin(key_str), end(key_str));
vector<uint8_t> data(begin(data_str), end(data_str));

RC4 rc4 = RC4(key);
vector<uint8_t> encrypted = rc4.encrypt(data);
for (int i = 0; i < encrypted.size(); i++) {
    cout << encrypted[i];
}
cout << endl;
vector<uint8_t> decrypted = rc4.decrypt(encrypted);
for (int i = 0; i < decrypted.size(); i++) {
    cout << decrypted[i];
}
cout << endl;
```
**Expected console output:**
```
a╥α▐HÉ╕∞╕?ûΘÜ╗û╚*Hü¬¬Φ7╗
Absolutely random sentence
```

**Salsa20**
```c++
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
```
**Expected console output:**
```
₧┐VαG°δäB
x░~\½ä▓²VΩE∙·ÅM
Absolutely random sentence
```
<br />

Also, there is a small subset of tests available in the *tests* folder (*test2.cpp*).
## Performance tests

| Cipher/mode type  | 1Gb file Encrypt + Decrypt| 
| ------------- | ------------- |
| RC4  |  56s  |
| Salsa20 | 484 s |
| AES 128 ECB | 234 s |
| AES 128 CBC | 365 s |
| AES 128 CFB | 889 s |
| AES 128 OFB | 225 s |
| AES 128 CTR | 225 s |

