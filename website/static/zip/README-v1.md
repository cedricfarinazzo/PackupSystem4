## Packup System 4 v1
---

### AUTHORS

PROMO 2022 (S4)

- MAUBANC Rémi
- RIDEAU Cyril
- CASIER Frédéric
- FARINAZZO Cédric

### The Project

A project to create a backup system with some cipher and compression algorithm

### Requirements
CPU, RAM, motherboard and power supply


### Build

#### Requirements
- gcc or clang
- gtk3
- gmplib
- criterion (unit test)

```
make
```

# Usage

- gui
```
./packup
```

- rotn
```
./packup rotn <text> <key:int>
```

- vigenere
```
./packup vigenere <text> <key:string>
```

- rsa
```
./packup rsa <text> <p:int> <q:int>
```
p and q are primes numbers and p < q

- aes
```
./packup aes <text> <key:string>
```
key is a 16 charactère string

- huffman
```
./packup huffman <text>
```
- filesystem
```
./packup filesystem <path_to_something>
```
