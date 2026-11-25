#ifndef MLL_H
#define MLL_H

#include <iostream>
using namespace std;

#define Nil NULL

// JENIS KEJADIAN
const string GOL            = "GOL";
const string ASSIST         = "ASSIST";
const string KARTU_KUNING   = "KARTU KUNING";
const string KARTU_MERAH    = "KARTU MERAH";
const string GOL_BUNUH_DIRI = "GOL BUNUH DIRI";

// CUCU : KEJADIAN
struct infoKejadian {
    int menit;      // 1 - 120
    string jenis;   // salah satu konstanta di atas
};

typedef struct elmKejadian *adrKejadian;
struct elmKejadian {
    infoKejadian info;
    adrKejadian next;
    adrKejadian prev;
};

struct ListKejadian {
    adrKejadian first;
    adrKejadian last;
};

// ANAK : PEMAIN
struct infoPemain {
    int noPunggung;
    string nama;
    string posisi;  // GK / DF / MF / FW
    char tim;       // 'H' = tim home, 'A' = tim away
};

typedef struct elmPemain *adrPemain;
struct elmPemain {
    infoPemain info;
    ListKejadian lKejadian;      // list kejadian milik pemain ini
    adrPemain next;
    adrPemain prev;
};

struct ListPemain {
    adrPemain first;
    adrPemain last;
};

// INDUK : PERTANDINGAN
struct infoPertandingan {
    string idMatch;
    string timHome;
    string timAway;
    string tanggal;
    string stadion;
};

typedef struct elmPertandingan *adrPertandingan;
struct elmPertandingan {
    infoPertandingan info;
    ListPemain lPemain;          // list pemain milik pertandingan ini
    adrPertandingan next;
    adrPertandingan prev;
};

struct ListPertandingan {
    adrPertandingan first;
    adrPertandingan last;
};

// LIST
void createList(ListPertandingan &L);
void createListPemain(ListPemain &LP);
void createListKejadian(ListKejadian &LK);
bool isEmpty(ListPertandingan L);
bool isEmptyPemain(ListPemain LP);
bool isEmptyKejadian(ListKejadian LK);

// OPERASI PARENT (WAJIB)
adrPertandingan createParent(infoPertandingan data);
void insertParent(ListPertandingan &L, adrPertandingan P);
void deleteParent(ListPertandingan &L, adrPertandingan P);
adrPertandingan findParent(ListPertandingan L, string idMatch);
void printParent(ListPertandingan L);

// OPERASI CHILD (WAJIB)
adrPemain createChild(infoPemain data);
void insertChild(adrPertandingan P, adrPemain C);
void deleteChild(adrPertandingan P, adrPemain C);
adrPemain findChild(adrPertandingan P, char tim, int noPunggung);
void printChild(adrPertandingan P);

// OPERASI KEJADIAN (CUCU)
adrKejadian createKejadian(infoKejadian data);
void insertKejadian(adrPemain C, adrKejadian K);
void deleteKejadian(adrPemain C, adrKejadian K);
adrKejadian findKejadian(adrPemain C, int menit, string jenis);
void printKejadian(adrPemain C);

// TAMPILKAN SEMUA (WAJIB)
void printAll(ListPertandingan L);

// FITUR TAMBAHAN
void printSatuParent(adrPertandingan P);
string namaTim(adrPertandingan P, char tim);
int hitungJumlahPemain(adrPertandingan P);
int hitungJenisKejadian(adrPemain C, string jenis);
void hitungSkor(adrPertandingan P, int &skorHome, int &skorAway);
int menitKartuMerah(adrPemain C);
bool tambahKejadian(adrPemain C, infoKejadian data, string &pesan);
void printLaporanPertandingan(adrPertandingan P);
void cariPemainSemuaPertandingan(ListPertandingan L, string nama);
void hapusSemua(ListPertandingan &L);

// UTILITAS
string toLowerStr(string s);
string intToStr(int x);

#endif
