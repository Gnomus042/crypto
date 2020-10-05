# Information security Lab #1

This is an example of the [Advanced Encryption Standard (AES)](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf) and [Kalyna](https://eprint.iacr.org/2015/650) (Ukrainian encryption standard) ciphers implementation. 
Core functions, which are the base for these two encryption standards (such as sub_bytes or mix_columns) are contained in the helper.cpp file, along with Block and Byte structures. 
Byte - a wrapper for uint8_t type, which implements Galois field operations (addition, multiplication, inverse, and pow).
Block - wrapper for key/block, mainly used to represent key/block structure in a form of a matrix.
Specific functions for AES are implemented in the AES class, the same is true for Kalyna.  Both cipher implementations support all possible block/key size combinations.
### Usage examples 
**AES:**
```
string str_key = "Thats my Kung Fu";
string str_data = "Two One Nine Two";
vector<uint8_t> key(str_key.begin(), str_key.end());
vector<uint8_t> data(str_data.begin(), str_data.end());
AES aes = AES(key);
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
┤╫⌠╚pá0ßà?║■╪└╡j
Two One Nine Two
```
**Kalyna**
```
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

Also, there is a small subset of tests available in the *tests* folder.

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