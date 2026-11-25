#ifndef MLL_H
#define MLL_H

#include <iostream>
#include <string>
using namespace std;

#define Nil NULL

const string GOL            = "GOL";
const string ASSIST         = "ASSIST";
const string KARTU_KUNING   = "KARTU KUNING";
const string KARTU_MERAH    = "KARTU MERAH";
const string GOL_BUNUH_DIRI = "GOL BUNUH DIRI";

enum HasilInsert { INSERT_OK, ID_KEMBAR, NOMOR_KEMBAR, TIM_SAMA };

enum HasilKejadian { KEJADIAN_OK, MENIT_TIDAK_VALID, KEJADIAN_KEMBAR, PEMAIN_SUDAH_KELUAR,
                     MERAH_GANDA, KUNING_PENUH, ASSIST_TANPA_GOL, ASSIST_SUDAH_ADA, KARTU_BENTROK };

struct infoKejadian {
    int menit;
    string jenis;
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

struct infoPemain {
    int noPunggung;
    string nama;
    string posisi;
    char tim;
};

typedef struct elmPemain *adrPemain;
struct elmPemain {
    infoPemain info;
    ListKejadian lKejadian;
    adrPemain next;
    adrPemain prev;
};

struct ListPemain {
    adrPemain first;
    adrPemain last;
};

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
    ListPemain lPemain;
    adrPertandingan next;
    adrPertandingan prev;
};

struct ListPertandingan {
    adrPertandingan first;
    adrPertandingan last;
};

void createList(ListPertandingan &L);
void createListPemain(ListPemain &LP);
void createListKejadian(ListKejadian &LK);
bool isEmpty(ListPertandingan L);
bool isEmptyPemain(ListPemain LP);
bool isEmptyKejadian(ListKejadian LK);

adrPertandingan createParent(infoPertandingan data);
HasilInsert insertParent(ListPertandingan &L, adrPertandingan P);
void deleteParent(ListPertandingan &L, adrPertandingan P);
adrPertandingan findParent(ListPertandingan L, string idMatch);
void printParent(ListPertandingan L);
void printParentTim(ListPertandingan L, string tim);

adrPemain createChild(infoPemain data);
HasilInsert insertChild(adrPertandingan P, adrPemain C);
int deleteChild(adrPertandingan P, adrPemain C);
adrPemain findChild(adrPertandingan P, char tim, int noPunggung);
adrPemain findChildNama(adrPertandingan P, string nama);
void printChild(adrPertandingan P);

adrKejadian createKejadian(infoKejadian data);
void insertKejadian(adrPemain C, adrKejadian K);
int deleteKejadian(adrPertandingan P, adrPemain C, adrKejadian K);
adrKejadian findKejadian(adrPemain C, int menit, string jenis);
adrKejadian findKejadianKe(adrPemain C, int urutan);
void printKejadian(adrPemain C);

void printAll(ListPertandingan L);

void printSatuParent(adrPertandingan P);
string namaTim(adrPertandingan P, char tim);
int hitungJumlahPemain(adrPertandingan P);
int hitungJenisKejadian(adrPemain C, string jenis);
void hitungSkor(adrPertandingan P, int &skorHome, int &skorAway);
int menitKartuMerah(adrPemain C);
int menitKeluar(adrPemain C);
int hitungKejadianTimMenit(adrPertandingan P, char tim, int menit, string jenis);
bool adaGolRekan(adrPertandingan P, adrPemain C, int menit);
HasilKejadian tambahKejadian(adrPertandingan P, adrPemain C, infoKejadian data);
int rapikanAssist(adrPertandingan P);
void printLaporanPertandingan(adrPertandingan P);
void printTopScorer(adrPertandingan P);
void printTimeline(adrPertandingan P);
void cariPemainSemuaPertandingan(ListPertandingan L, string nama, string tim, int noPunggung);
void hapusSemua(ListPertandingan &L);

string toLowerStr(string s);
string intToStr(int x);

#endif
