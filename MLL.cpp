#include <iostream>
using namespace std;
#include "mll.h"

// UTILITAS
string toLowerStr(string s) {
    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] - 'A' + 'a';
        }
    }
    return s;
}

string intToStr(int x) {
    if (x == 0) return "0";
    bool negatif = x < 0;
    if (negatif) x = -x;
    string hasil = "";
    while (x > 0) {
        hasil = char('0' + x % 10) + hasil;
        x = x / 10;
    }
    if (negatif) hasil = "-" + hasil;
    return hasil;
}

// LIST
void createList(ListPertandingan &L) {
    L.first = Nil;
    L.last = Nil;
}

void createListPemain(ListPemain &LP) {
    LP.first = Nil;
    LP.last = Nil;
}

void createListKejadian(ListKejadian &LK) {
    LK.first = Nil;
    LK.last = Nil;
}

bool isEmpty(ListPertandingan L) {
    return L.first == Nil;
}

bool isEmptyPemain(ListPemain LP) {
    return LP.first == Nil;
}

bool isEmptyKejadian(ListKejadian LK) {
    return LK.first == Nil;
}

// OPERASI PARENT : PERTANDINGAN
adrPertandingan createParent(infoPertandingan data) {
    adrPertandingan P = new elmPertandingan;
    P->info = data;
    P->next = Nil;
    P->prev = Nil;
    createListPemain(P->lPemain);   // pertandingan baru belum punya pemain
    return P;
}

// Insert last pada Double Linked List
void insertParent(ListPertandingan &L, adrPertandingan P) {
    if (P == Nil) return;

    if (isEmpty(L)) {
        L.first = P;
        L.last = P;
    } else {
        P->prev = L.last;
        L.last->next = P;
        L.last = P;
    }
}

// Menghapus semua kejadian milik seorang pemain, lalu pemain itu sendiri
void dealokasiPemain(adrPemain C) {
    adrKejadian K = C->lKejadian.first;
    while (K != Nil) {
        adrKejadian hapus = K;
        K = K->next;
        delete hapus;
    }
    delete C;
}

// Mengeluarkan P dari list, lalu menghapus P beserta seluruh pemain & kejadiannya
void deleteParent(ListPertandingan &L, adrPertandingan P) {
    if (isEmpty(L) || P == Nil) return;

    // 1. Lepaskan P dari list (4 kemungkinan posisi)
    if (P == L.first && P == L.last) {          // satu-satunya elemen
        L.first = Nil;
        L.last = Nil;
    } else if (P == L.first) {                  // elemen pertama
        L.first = P->next;
        L.first->prev = Nil;
    } else if (P == L.last) {                   // elemen terakhir
        L.last = P->prev;
        L.last->next = Nil;
    } else {                                    // elemen di tengah
        P->prev->next = P->next;
        P->next->prev = P->prev;
    }
    P->next = Nil;
    P->prev = Nil;

    // 2. Hapus seluruh anak (pemain) beserta kejadiannya milik P
    adrPemain C = P->lPemain.first;
    while (C != Nil) {
        adrPemain hapus = C;
        C = C->next;
        dealokasiPemain(hapus);
    }

    // 3. Hapus P
    delete P;
}

adrPertandingan findParent(ListPertandingan L, string idMatch) {
    adrPertandingan P = L.first;
    while (P != Nil) {
        if (toLowerStr(P->info.idMatch) == toLowerStr(idMatch)) {
            return P;
        }
        P = P->next;
    }
    return Nil;
}

void printSatuParent(adrPertandingan P) {
    if (P == Nil) return;
    int skorHome, skorAway;
    hitungSkor(P, skorHome, skorAway);
    cout << "[" << P->info.idMatch << "] "
         << P->info.timHome << " " << skorHome << " - " << skorAway << " " << P->info.timAway << endl;
    cout << "      Tanggal : " << P->info.tanggal << endl;
    cout << "      Stadion : " << P->info.stadion << endl;
    cout << "      Pemain  : " << hitungJumlahPemain(P) << " orang tercatat" << endl;
}

void printParent(ListPertandingan L) {
    if (isEmpty(L)) {
        cout << "Belum ada data pertandingan." << endl;
        return;
    }
    cout << "========== DAFTAR PERTANDINGAN ==========" << endl;
    adrPertandingan P = L.first;
    int no = 1;
    while (P != Nil) {
        cout << no << ". ";
        printSatuParent(P);
        P = P->next;
        no++;
    }
}

// OPERASI CHILD : PEMAIN
adrPemain createChild(infoPemain data) {
    adrPemain C = new elmPemain;
    C->info = data;
    C->next = Nil;
    C->prev = Nil;
    createListKejadian(C->lKejadian);   // pemain baru belum punya kejadian
    return C;
}

// Insert last pada list pemain (Double Linked List) milik pertandingan P
void insertChild(adrPertandingan P, adrPemain C) {
    if (P == Nil || C == Nil) return;

    if (isEmptyPemain(P->lPemain)) {
        P->lPemain.first = C;
        P->lPemain.last = C;
    } else {
        C->prev = P->lPemain.last;
        P->lPemain.last->next = C;
        P->lPemain.last = C;
    }
}

// Mengeluarkan C dari list pemain milik P, lalu menghapus C beserta kejadiannya
void deleteChild(adrPertandingan P, adrPemain C) {
    if (P == Nil || C == Nil || isEmptyPemain(P->lPemain)) return;

    // pastikan C memang anggota list pemain milik P
    adrPemain Q = P->lPemain.first;
    while (Q != Nil && Q != C) {
        Q = Q->next;
    }
    if (Q == Nil) return;                       // C bukan milik P, jangan ubah apa pun

    ListPemain &LP = P->lPemain;
    if (C == LP.first && C == LP.last) {        // satu-satunya pemain
        LP.first = Nil;
        LP.last = Nil;
    } else if (C == LP.first) {                 // pemain pertama
        LP.first = C->next;
        LP.first->prev = Nil;
    } else if (C == LP.last) {                  // pemain terakhir
        LP.last = C->prev;
        LP.last->next = Nil;
    } else {                                    // pemain di tengah
        C->prev->next = C->next;
        C->next->prev = C->prev;
    }
    C->next = Nil;
    C->prev = Nil;
    dealokasiPemain(C);
}

// Kunci pemain dalam satu pertandingan: tim (H/A) + nomor punggung
adrPemain findChild(adrPertandingan P, char tim, int noPunggung) {
    if (P == Nil) return Nil;
    adrPemain C = P->lPemain.first;
    while (C != Nil) {
        if (C->info.tim == tim && C->info.noPunggung == noPunggung) {
            return C;
        }
        C = C->next;
    }
    return Nil;
}

string namaTim(adrPertandingan P, char tim) {
    if (tim == 'H') return P->info.timHome;
    return P->info.timAway;
}

void printChild(adrPertandingan P) {
    if (P == Nil) return;
    cout << "Pemain pada [" << P->info.idMatch << "] "
         << P->info.timHome << " vs " << P->info.timAway << ":" << endl;

    if (isEmptyPemain(P->lPemain)) {
        cout << "  (belum ada pemain yang tercatat)" << endl;
        return;
    }
    adrPemain C = P->lPemain.first;
    int no = 1;
    while (C != Nil) {
        cout << "  " << no << ". [" << C->info.tim << "] #" << C->info.noPunggung << " "
             << C->info.nama << " (" << C->info.posisi << ") - " << namaTim(P, C->info.tim)
             << " | Gol: "    << hitungJenisKejadian(C, GOL)
             << ", Assist: "  << hitungJenisKejadian(C, ASSIST)
             << ", Kuning: "  << hitungJenisKejadian(C, KARTU_KUNING)
             << ", Merah: "   << hitungJenisKejadian(C, KARTU_MERAH) << endl;
        C = C->next;
        no++;
    }
}

// OPERASI LIST KEJADIAN (CUCU)
adrKejadian createKejadian(infoKejadian data) {
    adrKejadian K = new elmKejadian;
    K->info = data;
    K->next = Nil;
    K->prev = Nil;
    return K;
}

// Insert terurut berdasarkan menit (ascending) pada Double Linked List kejadian
void insertKejadian(adrPemain C, adrKejadian K) {
    if (C == Nil || K == Nil) return;
    ListKejadian &LK = C->lKejadian;

    if (isEmptyKejadian(LK)) {                          // list kosong
        LK.first = K;
        LK.last = K;
    } else if (K->info.menit < LK.first->info.menit) {  // insert first
        K->next = LK.first;
        LK.first->prev = K;
        LK.first = K;
    } else {
        // cari Prec: elemen terakhir yang menitnya <= menit K
        adrKejadian Prec = LK.first;
        while (Prec->next != Nil && Prec->next->info.menit <= K->info.menit) {
            Prec = Prec->next;
        }
        // insert after Prec
        K->next = Prec->next;
        K->prev = Prec;
        if (Prec->next != Nil) {
            Prec->next->prev = K;
        } else {
            LK.last = K;                                // K menjadi elemen terakhir
        }
        Prec->next = K;
    }
}

void deleteKejadian(adrPemain C, adrKejadian K) {
    if (C == Nil || K == Nil || isEmptyKejadian(C->lKejadian)) return;

    adrKejadian Q = C->lKejadian.first;
    while (Q != Nil && Q != K) {
        Q = Q->next;
    }
    if (Q == Nil) return;                       // K bukan milik C

    ListKejadian &LK = C->lKejadian;
    if (K == LK.first && K == LK.last) {
        LK.first = Nil;
        LK.last = Nil;
    } else if (K == LK.first) {
        LK.first = K->next;
        LK.first->prev = Nil;
    } else if (K == LK.last) {
        LK.last = K->prev;
        LK.last->next = Nil;
    } else {
        K->prev->next = K->next;
        K->next->prev = K->prev;
    }
    K->next = Nil;
    K->prev = Nil;
    delete K;
}

adrKejadian findKejadian(adrPemain C, int menit, string jenis) {
    if (C == Nil) return Nil;
    adrKejadian K = C->lKejadian.first;
    while (K != Nil) {
        if (K->info.menit == menit && K->info.jenis == jenis) {
            return K;
        }
        K = K->next;
    }
    return Nil;
}

void printKejadian(adrPemain C) {
    if (C == Nil) return;
    if (isEmptyKejadian(C->lKejadian)) {
        cout << "        (belum ada kejadian)" << endl;
        return;
    }
    adrKejadian K = C->lKejadian.first;
    while (K != Nil) {
        cout << "        - " << K->info.menit << "' " << K->info.jenis << endl;
        K = K->next;
    }
}

// TAMPILKAN SEMUA
void printAll(ListPertandingan L) {
    if (isEmpty(L)) {
        cout << "Multi Linked List kosong, belum ada data pertandingan." << endl;
        return;
    }
    adrPertandingan P = L.first;
    while (P != Nil) {
        cout << "=============================================================" << endl;
        printSatuParent(P);
        cout << "-------------------------------------------------------------" << endl;

        if (isEmptyPemain(P->lPemain)) {
            cout << "  (belum ada pemain yang tercatat pada pertandingan ini)" << endl;
        } else {
            adrPemain C = P->lPemain.first;
            while (C != Nil) {
                cout << "  [" << C->info.tim << "] #" << C->info.noPunggung << " " << C->info.nama
                     << " (" << C->info.posisi << ") - " << namaTim(P, C->info.tim) << endl;
                printKejadian(C);
                C = C->next;
            }
        }
        P = P->next;
    }
    cout << "=============================================================" << endl;
}

// FITUR TAMBAHAN
int hitungJumlahPemain(adrPertandingan P) {
    int jumlah = 0;
    if (P == Nil) return 0;
    adrPemain C = P->lPemain.first;
    while (C != Nil) {
        jumlah++;
        C = C->next;
    }
    return jumlah;
}

int hitungJenisKejadian(adrPemain C, string jenis) {
    int jumlah = 0;
    if (C == Nil) return 0;
    adrKejadian K = C->lKejadian.first;
    while (K != Nil) {
        if (K->info.jenis == jenis) jumlah++;
        K = K->next;
    }
    return jumlah;
}

// Skor TIDAK disimpan, tetapi dihitung dengan traversal pemain & kejadian.
// Gol biasa menambah skor tim pemain; gol bunuh diri menambah skor tim lawan.
void hitungSkor(adrPertandingan P, int &skorHome, int &skorAway) {
    skorHome = 0;
    skorAway = 0;
    if (P == Nil) return;

    adrPemain C = P->lPemain.first;
    while (C != Nil) {
        int gol = hitungJenisKejadian(C, GOL);
        int golBunuhDiri = hitungJenisKejadian(C, GOL_BUNUH_DIRI);
        if (C->info.tim == 'H') {
            skorHome += gol;
            skorAway += golBunuhDiri;
        } else {
            skorAway += gol;
            skorHome += golBunuhDiri;
        }
        C = C->next;
    }
}

// Mengembalikan menit kartu merah pemain, atau -1 jika belum pernah kartu merah
int menitKartuMerah(adrPemain C) {
    if (C == Nil) return -1;
    adrKejadian K = C->lKejadian.first;
    while (K != Nil) {
        if (K->info.jenis == KARTU_MERAH) return K->info.menit;
        K = K->next;
    }
    return -1;
}

/*
    Menambah kejadian dengan aturan pertandingan:
    1. Menit harus 1 - 120
    2. Pemain yang sudah kartu merah tidak boleh punya kejadian setelah menit kartu merah
    3. Pemain tidak boleh punya 2 kartu merah atau lebih dari 2 kartu kuning
    4. Kartu kuning kedua otomatis menghasilkan kartu merah
*/
bool tambahKejadian(adrPemain C, infoKejadian data, string &pesan) {
    if (C == Nil) {
        pesan = "Pemain tidak valid.";
        return false;
    }
    if (data.menit < 1 || data.menit > 120) {
        pesan = "Ditolak: menit harus di antara 1 sampai 120.";
        return false;
    }

    int merah = menitKartuMerah(C);
    if (merah != -1 && data.menit > merah) {
        pesan = "Ditolak: " + C->info.nama + " sudah diusir (kartu merah) pada menit " + intToStr(merah) + "'.";
        return false;
    }
    if (data.jenis == KARTU_MERAH && merah != -1) {
        pesan = "Ditolak: " + C->info.nama + " sudah memiliki kartu merah.";
        return false;
    }
    if (data.jenis == KARTU_KUNING && hitungJenisKejadian(C, KARTU_KUNING) >= 2) {
        pesan = "Ditolak: " + C->info.nama + " sudah memiliki 2 kartu kuning.";
        return false;
    }

    insertKejadian(C, createKejadian(data));
    pesan = "Kejadian " + data.jenis + " menit " + intToStr(data.menit) + "' berhasil ditambahkan.";

    // Aturan kartu kuning kedua -> kartu merah otomatis
    if (data.jenis == KARTU_KUNING && hitungJenisKejadian(C, KARTU_KUNING) == 2 && menitKartuMerah(C) == -1) {
        // kartu merah diberikan pada menit kartu kuning yang lebih akhir
        int menitKuningKedua = 0;
        adrKejadian K = C->lKejadian.first;
        while (K != Nil) {
            if (K->info.jenis == KARTU_KUNING) menitKuningKedua = K->info.menit;
            K = K->next;
        }
        infoKejadian kartuMerah;
        kartuMerah.menit = menitKuningKedua;
        kartuMerah.jenis = KARTU_MERAH;
        insertKejadian(C, createKejadian(kartuMerah));
        pesan += "\nKartu kuning kedua! " + C->info.nama + " otomatis mendapat KARTU MERAH pada menit "
                 + intToStr(menitKuningKedua) + "'.";
    }
    return true;
}

// Bagian dari laporan: daftar gol untuk satu tim
void printGolTim(adrPertandingan P, char tim) {
    bool ada = false;
    adrPemain C = P->lPemain.first;
    while (C != Nil) {
        adrKejadian K = C->lKejadian.first;
        while (K != Nil) {
            if (K->info.jenis == GOL && C->info.tim == tim) {
                cout << "    - " << C->info.nama << " " << K->info.menit << "'" << endl;
                ada = true;
            } else if (K->info.jenis == GOL_BUNUH_DIRI && C->info.tim != tim) {
                cout << "    - " << C->info.nama << " " << K->info.menit << "' (gol bunuh diri)" << endl;
                ada = true;
            }
            K = K->next;
        }
        C = C->next;
    }
    if (!ada) cout << "    (tidak ada)" << endl;
}

// Bagian dari laporan: daftar kejadian dengan jenis tertentu (assist / kartu)
void printDaftarJenis(adrPertandingan P, string jenis) {
    bool ada = false;
    adrPemain C = P->lPemain.first;
    while (C != Nil) {
        adrKejadian K = C->lKejadian.first;
        while (K != Nil) {
            if (K->info.jenis == jenis) {
                cout << "    - " << C->info.nama << " (" << namaTim(P, C->info.tim) << ") "
                     << K->info.menit << "'" << endl;
                ada = true;
            }
            K = K->next;
        }
        C = C->next;
    }
    if (!ada) cout << "    (tidak ada)" << endl;
}

void printLaporanPertandingan(adrPertandingan P) {
    if (P == Nil) return;
    int skorHome, skorAway;
    hitungSkor(P, skorHome, skorAway);

    int pemainHome = 0, pemainAway = 0;
    adrPemain C = P->lPemain.first;
    while (C != Nil) {
        if (C->info.tim == 'H') pemainHome++;
        else pemainAway++;
        C = C->next;
    }

    cout << "================ LAPORAN PERTANDINGAN ================" << endl;
    cout << "ID      : " << P->info.idMatch << endl;
    cout << "Tanggal : " << P->info.tanggal << endl;
    cout << "Stadion : " << P->info.stadion << endl;
    cout << endl;
    cout << "   " << P->info.timHome << "  " << skorHome << " - " << skorAway << "  " << P->info.timAway << endl;
    cout << endl;

    if (skorHome > skorAway)      cout << "Hasil   : " << P->info.timHome << " menang" << endl;
    else if (skorAway > skorHome) cout << "Hasil   : " << P->info.timAway << " menang" << endl;
    else                          cout << "Hasil   : Seri" << endl;

    cout << "------------------------------------------------------" << endl;
    cout << "  Gol " << P->info.timHome << ":" << endl;
    printGolTim(P, 'H');
    cout << "  Gol " << P->info.timAway << ":" << endl;
    printGolTim(P, 'A');
    cout << "  Assist:" << endl;
    printDaftarJenis(P, ASSIST);
    cout << "  Kartu kuning:" << endl;
    printDaftarJenis(P, KARTU_KUNING);
    cout << "  Kartu merah:" << endl;
    printDaftarJenis(P, KARTU_MERAH);
    cout << "------------------------------------------------------" << endl;
    cout << "Pemain tercatat: " << pemainHome << " (" << P->info.timHome << "), "
         << pemainAway << " (" << P->info.timAway << ")" << endl;
    cout << "======================================================" << endl;
}

// Mencari pemain (berdasarkan nama) di SEMUA pertandingan dan merekap statistiknya
void cariPemainSemuaPertandingan(ListPertandingan L, string nama) {
    if (isEmpty(L)) {
        cout << "Belum ada data pertandingan." << endl;
        return;
    }
    int totalMain = 0, totalGol = 0, totalAssist = 0, totalKuning = 0, totalMerah = 0;

    cout << "Rekap pemain \"" << nama << "\":" << endl;
    adrPertandingan P = L.first;
    while (P != Nil) {
        adrPemain C = P->lPemain.first;
        while (C != Nil) {
            if (toLowerStr(C->info.nama) == toLowerStr(nama)) {
                int gol = hitungJenisKejadian(C, GOL);
                int assist = hitungJenisKejadian(C, ASSIST);
                int kuning = hitungJenisKejadian(C, KARTU_KUNING);
                int merah = hitungJenisKejadian(C, KARTU_MERAH);
                cout << "  [" << P->info.idMatch << "] " << P->info.timHome << " vs " << P->info.timAway
                     << " (membela " << namaTim(P, C->info.tim) << ", #" << C->info.noPunggung << ")"
                     << " | Gol: " << gol << ", Assist: " << assist
                     << ", Kuning: " << kuning << ", Merah: " << merah << endl;
                totalMain++;
                totalGol += gol;
                totalAssist += assist;
                totalKuning += kuning;
                totalMerah += merah;
            }
            C = C->next;
        }
        P = P->next;
    }

    if (totalMain == 0) {
        cout << "  Pemain tidak ditemukan di pertandingan mana pun." << endl;
    } else {
        cout << "  TOTAL: " << totalMain << " pertandingan, " << totalGol << " gol, "
             << totalAssist << " assist, " << totalKuning << " kartu kuning, "
             << totalMerah << " kartu merah" << endl;
    }
}

// Menghapus seluruh data (dipanggil saat program selesai agar memori bersih)
void hapusSemua(ListPertandingan &L) {
    while (!isEmpty(L)) {
        deleteParent(L, L.first);
    }
}
