#include "MLL.h"

string toLowerStr(string s) {
    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] - 'A' + 'a';
        }
    }
    return s;
}

string toUpperStr(string s) {
    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] >= 'a' && s[i] <= 'z') {
            s[i] = s[i] - 'a' + 'A';
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

adrPertandingan createParent(infoPertandingan data) {
    adrPertandingan P = new elmPertandingan;
    P->info = data;
    P->next = Nil;
    P->prev = Nil;
    createListPemain(P->lPemain);
    return P;
}

HasilInsert insertParent(ListPertandingan &L, adrPertandingan P) {
    if (findParent(L, P->info.idMatch) != Nil) {
        return ID_KEMBAR;
    }
    if (toLowerStr(P->info.timHome) == toLowerStr(P->info.timAway)) {
        return TIM_SAMA;
    }
    if (isEmpty(L)) {
        L.first = P;
        L.last = P;
    } else {
        P->prev = L.last;
        L.last->next = P;
        L.last = P;
    }
    return INSERT_OK;
}

void dealokasiPemain(adrPemain C) {
    adrKejadian K = C->lKejadian.first;
    while (K != Nil) {
        adrKejadian hapus = K;
        K = K->next;
        delete hapus;
    }
    delete C;
}

void deleteParent(ListPertandingan &L, adrPertandingan P) {
    if (isEmpty(L) || P == Nil) return;

    if (P == L.first && P == L.last) {
        L.first = Nil;
        L.last = Nil;
    } else if (P == L.first) {
        L.first = P->next;
        L.first->prev = Nil;
    } else if (P == L.last) {
        L.last = P->prev;
        L.last->next = Nil;
    } else {
        P->prev->next = P->next;
        P->next->prev = P->prev;
    }
    P->next = Nil;
    P->prev = Nil;

    adrPemain C = P->lPemain.first;
    while (C != Nil) {
        adrPemain hapus = C;
        C = C->next;
        dealokasiPemain(hapus);
    }
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
    cout << "      Stadion : " << P->info.stadion << endl;
    cout << "      Pemain  : " << hitungJumlahPemain(P) << " orang tercatat" << endl;
}

void printSkorPertandingan(adrPertandingan P, string label) {
    if (P == Nil) return;
    int skorHome, skorAway;
    hitungSkor(P, skorHome, skorAway);
    cout << label << P->info.timHome << " " << skorHome << " - " << skorAway << " " << P->info.timAway << endl;
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

void printParentTim(ListPertandingan L, string tim) {
    string t = toLowerStr(tim);
    int jumlah = 0;
    adrPertandingan P = L.first;
    while (P != Nil) {
        if (toLowerStr(P->info.timHome) == t || toLowerStr(P->info.timAway) == t) {
            jumlah++;
            cout << jumlah << ". ";
            printSatuParent(P);
        }
        P = P->next;
    }
    if (jumlah == 0) {
        cout << "Tim " << tim << " tidak ditemukan di pertandingan manapun." << endl;
    }
}

adrPemain createChild(infoPemain data) {
    adrPemain C = new elmPemain;
    C->info = data;
    C->next = Nil;
    C->prev = Nil;
    createListKejadian(C->lKejadian);
    return C;
}

HasilInsert insertChild(adrPertandingan P, adrPemain C) {
    if (findChild(P, C->info.tim, C->info.noPunggung) != Nil) {
        return NOMOR_KEMBAR;
    }
    if (isEmptyPemain(P->lPemain)) {
        P->lPemain.first = C;
        P->lPemain.last = C;
    } else {
        C->prev = P->lPemain.last;
        P->lPemain.last->next = C;
        P->lPemain.last = C;
    }
    return INSERT_OK;
}

int deleteChild(adrPertandingan P, adrPemain C) {
    if (P == Nil || C == Nil || isEmptyPemain(P->lPemain)) return -1;

    adrPemain Q = P->lPemain.first;
    while (Q != Nil && Q != C) {
        Q = Q->next;
    }
    if (Q == Nil) return -1;

    ListPemain &LP = P->lPemain;
    if (C == LP.first && C == LP.last) {
        LP.first = Nil;
        LP.last = Nil;
    } else if (C == LP.first) {
        LP.first = C->next;
        LP.first->prev = Nil;
    } else if (C == LP.last) {
        LP.last = C->prev;
        LP.last->next = Nil;
    } else {
        C->prev->next = C->next;
        C->next->prev = C->prev;
    }
    C->next = Nil;
    C->prev = Nil;
    dealokasiPemain(C);
    return rapikanAssist(P);
}

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

adrPemain findChildNama(adrPertandingan P, string nama) {
    if (P == Nil) return Nil;
    string n = toLowerStr(nama);
    adrPemain C = P->lPemain.first;
    while (C != Nil) {
        if (toLowerStr(C->info.nama) == n) {
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
             << ", Merah: "   << hitungJenisKejadian(C, KARTU_MERAH);
        if (menitKeluar(C) != -1) {
            cout << " | KELUAR menit " << menitKeluar(C);
        }
        cout << endl;
        C = C->next;
        no++;
    }
}

adrKejadian createKejadian(infoKejadian data) {
    adrKejadian K = new elmKejadian;
    K->info = data;
    K->next = Nil;
    K->prev = Nil;
    return K;
}

void insertKejadian(adrPemain C, adrKejadian K) {
    if (C == Nil || K == Nil) return;
    ListKejadian &LK = C->lKejadian;

    if (isEmptyKejadian(LK)) {
        LK.first = K;
        LK.last = K;
    } else if (K->info.menit < LK.first->info.menit) {
        K->next = LK.first;
        LK.first->prev = K;
        LK.first = K;
    } else {
        adrKejadian Prec = LK.first;
        while (Prec->next != Nil && Prec->next->info.menit <= K->info.menit) {
            Prec = Prec->next;
        }
        K->next = Prec->next;
        K->prev = Prec;
        if (Prec->next != Nil) {
            Prec->next->prev = K;
        } else {
            LK.last = K;
        }
        Prec->next = K;
    }
}

void lepasKejadian(adrPemain C, adrKejadian K) {
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
}

int deleteKejadian(adrPertandingan P, adrPemain C, adrKejadian K) {
    if (P == Nil || C == Nil || K == Nil) return -1;

    adrKejadian Q = C->lKejadian.first;
    while (Q != Nil && Q != K) {
        Q = Q->next;
    }
    if (Q == Nil) return -1;

    lepasKejadian(C, K);
    delete K;
    return rapikanAssist(P);
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

adrKejadian findKejadianKe(adrPemain C, int urutan) {
    if (C == Nil || urutan < 1) return Nil;
    adrKejadian K = C->lKejadian.first;
    int i = 1;
    while (K != Nil && i < urutan) {
        K = K->next;
        i++;
    }
    return K;
}

void printKejadian(adrPemain C) {
    if (C == Nil) return;
    if (isEmptyKejadian(C->lKejadian)) {
        cout << "        (belum ada kejadian)" << endl;
        return;
    }
    adrKejadian K = C->lKejadian.first;
    int no = 1;
    int kuning = 0;
    while (K != Nil) {
        cout << "        " << no << ". " << K->info.menit << "' " << K->info.jenis;
        if (K->info.jenis == KARTU_KUNING) {
            kuning++;
            if (kuning == 2) cout << " (kedua, pemain keluar)";
        }
        cout << endl;
        K = K->next;
        no++;
    }
}

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
                     << " (" << C->info.posisi << ") - " << namaTim(P, C->info.tim);
                if (menitKeluar(C) != -1) {
                    cout << " [KELUAR menit " << menitKeluar(C) << "]";
                }
                cout << endl;
                printKejadian(C);
                C = C->next;
            }
        }
        P = P->next;
    }
    cout << "=============================================================" << endl;
}

void printRelasiParentChild(ListPertandingan L) {
    if (isEmpty(L)) {
        cout << "Multi Linked List kosong, belum ada data pertandingan." << endl;
        return;
    }
    cout << "========== RELASI PARENT - CHILD (PERTANDINGAN -> PEMAIN) ==========" << endl;
    cout << "Total pertandingan (Parent): " << hitungJumlahParent(L) << endl;
    adrPertandingan P = L.first;
    while (P != Nil) {
        cout << "--------------------------------------------------------------------" << endl;
        printSatuParent(P);
        printChild(P);
        P = P->next;
    }
    cout << "====================================================================" << endl;
}

int hitungJumlahParent(ListPertandingan L) {
    int jumlah = 0;
    adrPertandingan P = L.first;
    while (P != Nil) {
        jumlah++;
        P = P->next;
    }
    return jumlah;
}

int hitungJumlahPemainTim(adrPertandingan P, char tim) {
    int jumlah = 0;
    if (P == Nil) return 0;
    adrPemain C = P->lPemain.first;
    while (C != Nil) {
        if (C->info.tim == tim) jumlah++;
        C = C->next;
    }
    return jumlah;
}

int hitungJumlahPemain(adrPertandingan P) {
    if (P == Nil) return 0;
    return hitungJumlahPemainTim(P, 'H') + hitungJumlahPemainTim(P, 'A');
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

int menitKartuMerah(adrPemain C) {
    if (C == Nil) return -1;
    adrKejadian K = C->lKejadian.first;
    while (K != Nil) {
        if (K->info.jenis == KARTU_MERAH) return K->info.menit;
        K = K->next;
    }
    return -1;
}

int menitKeluar(adrPemain C) {
    if (C == Nil) return -1;
    int merah = menitKartuMerah(C);
    int kuning = 0;
    int kuningKedua = -1;
    adrKejadian K = C->lKejadian.first;
    while (K != Nil) {
        if (K->info.jenis == KARTU_KUNING) {
            kuning++;
            if (kuning == 2) kuningKedua = K->info.menit;
        }
        K = K->next;
    }
    if (merah == -1) return kuningKedua;
    if (kuningKedua == -1 || merah <= kuningKedua) return merah;
    return kuningKedua;
}

int hitungKejadianTimMenit(adrPertandingan P, char tim, int menit, string jenis) {
    if (P == Nil) return 0;
    int jumlah = 0;
    adrPemain Q = P->lPemain.first;
    while (Q != Nil) {
        if (Q->info.tim == tim) {
            adrKejadian K = Q->lKejadian.first;
            while (K != Nil) {
                if (K->info.jenis == jenis && K->info.menit == menit) {
                    jumlah++;
                }
                K = K->next;
            }
        }
        Q = Q->next;
    }
    return jumlah;
}

bool adaGolRekan(adrPertandingan P, adrPemain C, int menit) {
    adrPemain Q = P->lPemain.first;
    while (Q != Nil) {
        if (Q != C && Q->info.tim == C->info.tim) {
            adrKejadian K = Q->lKejadian.first;
            while (K != Nil) {
                if (K->info.jenis == GOL && K->info.menit == menit) return true;
                K = K->next;
            }
        }
        Q = Q->next;
    }
    return false;
}

int rapikanAssist(adrPertandingan P) {
    int jumlah = 0;
    adrPemain C = P->lPemain.first;
    while (C != Nil) {
        adrKejadian K = C->lKejadian.first;
        while (K != Nil) {
            adrKejadian berikut = K->next;
            if (K->info.jenis == ASSIST) {
                bool tanpaGol = !adaGolRekan(P, C, K->info.menit);
                bool berlebih = hitungKejadianTimMenit(P, C->info.tim, K->info.menit, ASSIST)
                              > hitungKejadianTimMenit(P, C->info.tim, K->info.menit, GOL);
                if (tanpaGol || berlebih) {
                    lepasKejadian(C, K);
                    delete K;
                    jumlah++;
                }
            }
            K = berikut;
        }
        C = C->next;
    }
    return jumlah;
}

HasilKejadian tambahKejadian(adrPertandingan P, adrPemain C, infoKejadian data) {
    if (data.menit < 1 || data.menit > 120) {
        return MENIT_TIDAK_VALID;
    }
    if (findKejadian(C, data.menit, data.jenis) != Nil) {
        return KEJADIAN_KEMBAR;
    }
    int keluar = menitKeluar(C);
    if (keluar != -1 && data.menit > keluar) {
        return PEMAIN_SUDAH_KELUAR;
    }
    if (data.jenis == KARTU_MERAH && hitungJenisKejadian(C, KARTU_MERAH) >= 1) {
        return MERAH_GANDA;
    }
    if (data.jenis == KARTU_KUNING && hitungJenisKejadian(C, KARTU_KUNING) >= 2) {
        return KUNING_PENUH;
    }
    if (data.jenis == ASSIST) {
        if (!adaGolRekan(P, C, data.menit)) {
            return ASSIST_TANPA_GOL;
        }
        if (hitungKejadianTimMenit(P, C->info.tim, data.menit, ASSIST)
            >= hitungKejadianTimMenit(P, C->info.tim, data.menit, GOL)) {
            return ASSIST_SUDAH_ADA;
        }
    }

    adrKejadian baru = createKejadian(data);
    insertKejadian(C, baru);

    keluar = menitKeluar(C);
    if (keluar != -1) {
        adrKejadian K = C->lKejadian.first;
        while (K != Nil) {
            if (K->info.menit > keluar) {
                lepasKejadian(C, baru);
                delete baru;
                return KARTU_BENTROK;
            }
            K = K->next;
        }
    }
    return KEJADIAN_OK;
}

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

void printDaftarKeluar(adrPertandingan P) {
    bool ada = false;
    adrPemain C = P->lPemain.first;
    while (C != Nil) {
        int keluar = menitKeluar(C);
        if (keluar != -1) {
            string alasan = "dua kartu kuning";
            if (menitKartuMerah(C) == keluar) alasan = "kartu merah";
            cout << "    - " << C->info.nama << " (" << namaTim(P, C->info.tim) << ") "
                 << keluar << "' (" << alasan << ")" << endl;
            ada = true;
        }
        C = C->next;
    }
    if (!ada) cout << "    (tidak ada)" << endl;
}

void printLaporanPertandingan(adrPertandingan P) {
    if (P == Nil) return;
    int skorHome, skorAway;
    hitungSkor(P, skorHome, skorAway);

    int pemainHome = hitungJumlahPemainTim(P, 'H');
    int pemainAway = hitungJumlahPemainTim(P, 'A');

    cout << "+======================================================+" << endl;
    cout << "|                 LAPORAN PERTANDINGAN                 |" << endl;
    cout << "+======================================================+" << endl;
    cout << "ID      : " << P->info.idMatch << endl;
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
    cout << "  Pemain keluar:" << endl;
    printDaftarKeluar(P);
    cout << "------------------------------------------------------" << endl;
    cout << "Pemain tercatat: " << pemainHome << " (" << P->info.timHome << "), "
         << pemainAway << " (" << P->info.timAway << ")" << endl;
    cout << "======================================================" << endl;
}

void printTopScorer(adrPertandingan P) {
    if (P == Nil) return;
    int maks = 0;
    adrPemain C = P->lPemain.first;
    while (C != Nil) {
        if (hitungJenisKejadian(C, GOL) > maks) {
            maks = hitungJenisKejadian(C, GOL);
        }
        C = C->next;
    }
    if (maks == 0) {
        cout << "Belum ada gol di pertandingan ini." << endl;
        return;
    }
    cout << "Top scorer (" << maks << " gol):" << endl;
    C = P->lPemain.first;
    while (C != Nil) {
        if (hitungJenisKejadian(C, GOL) == maks) {
            cout << "  " << C->info.nama << " (#" << C->info.noPunggung << ", "
                 << namaTim(P, C->info.tim) << ")" << endl;
        }
        C = C->next;
    }
}

struct elmLini {
    string nama;
    int noPunggung;
    char tim;
    infoKejadian info;
    elmLini *next;
};

int urutanJenis(string jenis) {
    if (jenis == GOL || jenis == GOL_BUNUH_DIRI) return 0;
    if (jenis == ASSIST) return 1;
    if (jenis == KARTU_KUNING) return 2;
    return 3;
}

bool urutSebelumAtauSama(infoKejadian a, infoKejadian b) {
    if (a.menit != b.menit) return a.menit < b.menit;
    return urutanJenis(a.jenis) <= urutanJenis(b.jenis);
}

void printTimeline(adrPertandingan P) {
    if (P == Nil) return;
    elmLini *first = Nil;
    adrPemain C = P->lPemain.first;
    while (C != Nil) {
        adrKejadian K = C->lKejadian.first;
        while (K != Nil) {
            elmLini *n = new elmLini;
            n->nama = C->info.nama;
            n->noPunggung = C->info.noPunggung;
            n->tim = C->info.tim;
            n->info = K->info;
            n->next = Nil;
            if (first == Nil || !urutSebelumAtauSama(first->info, n->info)) {
                n->next = first;
                first = n;
            } else {
                elmLini *q = first;
                while (q->next != Nil && urutSebelumAtauSama(q->next->info, n->info)) {
                    q = q->next;
                }
                n->next = q->next;
                q->next = n;
            }
            K = K->next;
        }
        C = C->next;
    }
    if (first == Nil) {
        cout << "Belum ada kejadian di pertandingan ini." << endl;
        return;
    }
    elmLini *q = first;
    while (q != Nil) {
        cout << "  " << q->info.menit << "'  " << q->info.jenis << "  " << q->nama
             << " (#" << q->noPunggung << ", " << namaTim(P, q->tim) << ")" << endl;
        elmLini *hapus = q;
        q = q->next;
        delete hapus;
    }
}

void cariPemainSemuaPertandingan(ListPertandingan L, string nama, string tim, int noPunggung) {
    if (isEmpty(L)) {
        cout << "Belum ada data pertandingan." << endl;
        return;
    }
    int totalMain = 0, totalGol = 0, totalAssist = 0, totalKuning = 0, totalMerah = 0, totalKeluar = 0;

    cout << "Rekap pemain \"" << nama << "\" (#" << noPunggung << ", " << tim << "):" << endl;
    string tLower = toLowerStr(tim);
    string nLower = toLowerStr(nama);
    adrPertandingan P = L.first;
    while (P != Nil) {
        char kodeTim = 0;
        if (toLowerStr(P->info.timHome) == tLower) kodeTim = 'H';
        else if (toLowerStr(P->info.timAway) == tLower) kodeTim = 'A';

        adrPemain C = (kodeTim != 0) ? findChild(P, kodeTim, noPunggung) : Nil;
        if (C != Nil && toLowerStr(C->info.nama) == nLower) {
            int gol = hitungJenisKejadian(C, GOL);
            int assist = hitungJenisKejadian(C, ASSIST);
            int kuning = hitungJenisKejadian(C, KARTU_KUNING);
            int merah = hitungJenisKejadian(C, KARTU_MERAH);
            cout << "  [" << P->info.idMatch << "] " << P->info.timHome << " vs " << P->info.timAway
                 << " (#" << C->info.noPunggung << ")"
                 << " | Gol: " << gol << ", Assist: " << assist
                 << ", Kuning: " << kuning << ", Merah: " << merah;
            if (menitKeluar(C) != -1) {
                cout << " | KELUAR menit " << menitKeluar(C);
                totalKeluar++;
            }
            cout << endl;
            totalMain++;
            totalGol += gol;
            totalAssist += assist;
            totalKuning += kuning;
            totalMerah += merah;
        }
        P = P->next;
    }

    if (totalMain == 0) {
        cout << "  Pemain tidak ditemukan di tim tersebut pada pertandingan mana pun." << endl;
    } else {
        cout << "  TOTAL: " << totalMain << " pertandingan, " << totalGol << " gol, "
             << totalAssist << " assist, " << totalKuning << " kartu kuning, "
             << totalMerah << " kartu merah, keluar " << totalKeluar << " kali" << endl;
    }
}

void hapusSemua(ListPertandingan &L) {
    while (!isEmpty(L)) {
        deleteParent(L, L.first);
    }
}

string potong(string s) {
    int awal = 0;
    int akhir = (int)s.length() - 1;
    while (awal <= akhir && (s[awal] == ' ' || s[awal] == '\t' || s[awal] == '\r')) awal++;
    while (akhir >= awal && (s[akhir] == ' ' || s[akhir] == '\t' || s[akhir] == '\r')) akhir--;
    return s.substr(awal, akhir - awal + 1);
}

string inputString(string prompt) {
    string s;
    cout << prompt;
    if (!getline(cin, s)) {
        return "";
    }
    return potong(s);
}

string inputWajib(string prompt) {
    while (true) {
        string s = inputString(prompt);
        if (!cin) return "";
        if (s != "") return s;
        cout << "Input tidak boleh kosong." << endl;
    }
}

bool keAngka(string s, int &hasil) {
    int awal = 0;
    int akhir = (int)s.length() - 1;
    if (awal > akhir) return false;

    bool negatif = false;
    if (s[awal] == '-') {
        negatif = true;
        awal++;
    }
    if (awal > akhir || akhir - awal + 1 > 9) return false;

    hasil = 0;
    for (int i = awal; i <= akhir; i++) {
        if (s[i] < '0' || s[i] > '9') return false;
        hasil = hasil * 10 + (s[i] - '0');
    }
    if (negatif) hasil = -hasil;
    return true;
}

int inputInt(string prompt) {
    while (true) {
        string s = inputString(prompt);
        if (!cin) return -1;
        int x;
        if (keAngka(s, x)) return x;
        cout << "Input harus berupa angka bulat." << endl;
    }
}

bool posisiValid(string s) {
    return s == "GK" || s == "DF" || s == "MF" || s == "FW";
}

char inputTim() {
    while (true) {
        string s = toUpperStr(inputString("Tim (H = home / A = away): "));
        if (!cin) return 'H';
        if (s == "H" || s == "A") return s[0];
        cout << "Pilih H atau A." << endl;
    }
}

string inputJenisKejadian() {
    cout << "Jenis kejadian:" << endl;
    cout << "  1. Gol" << endl;
    cout << "  2. Assist" << endl;
    cout << "  3. Kartu kuning" << endl;
    cout << "  4. Kartu merah" << endl;
    cout << "  5. Gol bunuh diri" << endl;
    while (true) {
        int pilih = inputInt("Pilih (1-5): ");
        if (!cin) return GOL;
        switch (pilih) {
            case 1: return GOL;
            case 2: return ASSIST;
            case 3: return KARTU_KUNING;
            case 4: return KARTU_MERAH;
            case 5: return GOL_BUNUH_DIRI;
        }
        cout << "Pilihan tidak valid." << endl;
    }
}

bool konfirmasi(string prompt) {
    string s = toUpperStr(inputString(prompt + " (y/n): "));
    return s == "Y";
}

adrPertandingan pilihPertandingan(ListPertandingan L) {
    if (isEmpty(L)) {
        cout << "Belum ada data pertandingan." << endl;
        return Nil;
    }
    string id = toUpperStr(inputWajib("ID pertandingan: "));
    adrPertandingan P = findParent(L, id);
    if (P == Nil) {
        cout << "Pertandingan dengan ID " << id << " tidak ditemukan." << endl;
    }
    return P;
}

adrPemain pilihPemain(adrPertandingan P, bool tampilkanDaftar) {
    if (isEmptyPemain(P->lPemain)) {
        cout << "Pertandingan ini belum memiliki pemain." << endl;
        return Nil;
    }
    if (tampilkanDaftar) printChild(P);
    char tim = inputTim();
    int no = inputInt("No punggung: ");
    adrPemain C = findChild(P, tim, no);
    if (C == Nil) {
        cout << "Pemain [" << tim << "] #" << no << " tidak ditemukan pada pertandingan ini." << endl;
    }
    return C;
}

adrPemain tambahPemainContoh(adrPertandingan P, char tim, int no, string nama, string posisi) {
    infoPemain d;
    d.tim = tim;
    d.noPunggung = no;
    d.nama = nama;
    d.posisi = posisi;
    adrPemain C = createChild(d);
    insertChild(P, C);
    return C;
}

void tambahKejadianContoh(adrPertandingan P, adrPemain C, int menit, string jenis) {
    infoKejadian d;
    d.menit = menit;
    d.jenis = jenis;
    tambahKejadian(P, C, d);
}

adrPertandingan tambahPertandinganContoh(ListPertandingan &L, string id, string home, string away, string stadion) {
    infoPertandingan d;
    d.idMatch = id;
    d.timHome = home;
    d.timAway = away;
    d.stadion = stadion;
    adrPertandingan P = createParent(d);
    insertParent(L, P);
    return P;
}

void isiDataDefault(ListPertandingan &L) {
    adrPertandingan P;

    // M01: Manchester United vs Tottenham Hotspur
    P = tambahPertandinganContoh(L, "M01", "Manchester United", "Tottenham Hotspur", "Old Trafford");
    tambahPemainContoh(P, 'H', 1,  "Senne Lammens", "GK");
    tambahPemainContoh(P, 'H', 2,  "Diogo Dalot", "DF");
    tambahPemainContoh(P, 'H', 4,  "Matthijs de Ligt", "DF");
    tambahPemainContoh(P, 'H', 6,  "Lisandro Martinez", "DF");
    tambahPemainContoh(P, 'H', 13, "Patrick Dorgu", "DF");
    tambahPemainContoh(P, 'H', 8,  "Bruno Fernandes", "MF");
    tambahPemainContoh(P, 'H', 25, "Manuel Ugarte", "MF");
    tambahPemainContoh(P, 'H', 37, "Kobbie Mainoo", "MF");
    tambahPemainContoh(P, 'H', 10, "Matheus Cunha", "FW");
    tambahPemainContoh(P, 'H', 19, "Bryan Mbeumo", "FW");
    tambahPemainContoh(P, 'H', 30, "Benjamin Sesko", "FW");

    tambahPemainContoh(P, 'A', 1,  "Guglielmo Vicario", "GK");
    tambahPemainContoh(P, 'A', 23, "Pedro Porro", "DF");
    tambahPemainContoh(P, 'A', 17, "Cristian Romero", "DF");
    tambahPemainContoh(P, 'A', 37, "Micky van de Ven", "DF");
    tambahPemainContoh(P, 'A', 13, "Destiny Udogie", "DF");
    tambahPemainContoh(P, 'A', 6,  "Joao Palhinha", "MF");
    tambahPemainContoh(P, 'A', 10, "James Maddison", "MF");
    tambahPemainContoh(P, 'A', 21, "Dejan Kulusevski", "MF");
    tambahPemainContoh(P, 'A', 20, "Mohammed Kudus", "FW");
    tambahPemainContoh(P, 'A', 19, "Dominic Solanke", "FW");
    tambahPemainContoh(P, 'A', 11, "Mathys Tel", "FW");

    // M02: Manchester City vs Liverpool
    P = tambahPertandinganContoh(L, "M02", "Manchester City", "Liverpool", "Etihad Stadium");
    tambahPemainContoh(P, 'H', 25, "Gianluigi Donnarumma", "GK");
    tambahPemainContoh(P, 'H', 27, "Matheus Nunes", "DF");
    tambahPemainContoh(P, 'H', 3,  "Ruben Dias", "DF");
    tambahPemainContoh(P, 'H', 24, "Josko Gvardiol", "DF");
    tambahPemainContoh(P, 'H', 21, "Rayan Ait-Nouri", "DF");
    tambahPemainContoh(P, 'H', 16, "Rodri", "MF");
    tambahPemainContoh(P, 'H', 4,  "Tijjani Reijnders", "MF");
    tambahPemainContoh(P, 'H', 20, "Bernardo Silva", "MF");
    tambahPemainContoh(P, 'H', 11, "Jeremy Doku", "FW");
    tambahPemainContoh(P, 'H', 9,  "Erling Haaland", "FW");
    tambahPemainContoh(P, 'H', 7,  "Omar Marmoush", "FW");

    tambahPemainContoh(P, 'A', 1,  "Alisson Becker", "GK");
    tambahPemainContoh(P, 'A', 30, "Jeremie Frimpong", "DF");
    tambahPemainContoh(P, 'A', 5,  "Ibrahima Konate", "DF");
    tambahPemainContoh(P, 'A', 4,  "Virgil van Dijk", "DF");
    tambahPemainContoh(P, 'A', 6,  "Milos Kerkez", "DF");
    tambahPemainContoh(P, 'A', 38, "Ryan Gravenberch", "MF");
    tambahPemainContoh(P, 'A', 10, "Alexis Mac Allister", "MF");
    tambahPemainContoh(P, 'A', 8,  "Dominik Szoboszlai", "MF");
    tambahPemainContoh(P, 'A', 11, "Mohamed Salah", "FW");
    tambahPemainContoh(P, 'A', 22, "Hugo Ekitike", "FW");
    tambahPemainContoh(P, 'A', 18, "Cody Gakpo", "FW");

    // M03: Arsenal vs Chelsea
    P = tambahPertandinganContoh(L, "M03", "Arsenal", "Chelsea", "Emirates Stadium");
    tambahPemainContoh(P, 'H', 1,  "David Raya", "GK");
    tambahPemainContoh(P, 'H', 4,  "Ben White", "DF");
    tambahPemainContoh(P, 'H', 2,  "William Saliba", "DF");
    tambahPemainContoh(P, 'H', 6,  "Gabriel Magalhaes", "DF");
    tambahPemainContoh(P, 'H', 12, "Jurrien Timber", "DF");
    tambahPemainContoh(P, 'H', 41, "Declan Rice", "MF");
    tambahPemainContoh(P, 'H', 8,  "Martin Odegaard", "MF");
    tambahPemainContoh(P, 'H', 23, "Mikel Merino", "MF");
    tambahPemainContoh(P, 'H', 7,  "Bukayo Saka", "FW");
    tambahPemainContoh(P, 'H', 14, "Viktor Gyokeres", "FW");
    tambahPemainContoh(P, 'H', 11, "Gabriel Martinelli", "FW");

    tambahPemainContoh(P, 'A', 1,  "Robert Sanchez", "GK");
    tambahPemainContoh(P, 'A', 24, "Reece James", "DF");
    tambahPemainContoh(P, 'A', 29, "Wesley Fofana", "DF");
    tambahPemainContoh(P, 'A', 6,  "Levi Colwill", "DF");
    tambahPemainContoh(P, 'A', 3,  "Marc Cucurella", "DF");
    tambahPemainContoh(P, 'A', 25, "Moises Caicedo", "MF");
    tambahPemainContoh(P, 'A', 8,  "Enzo Fernandez", "MF");
    tambahPemainContoh(P, 'A', 10, "Cole Palmer", "MF");
    tambahPemainContoh(P, 'A', 7,  "Pedro Neto", "FW");
    tambahPemainContoh(P, 'A', 20, "Joao Pedro", "FW");
    tambahPemainContoh(P, 'A', 15, "Nicolas Jackson", "FW");
}

void tampilHasilKejadian(HasilKejadian h, adrPertandingan P, adrPemain C, infoKejadian d) {
    if (h == KEJADIAN_OK) {
        cout << "Kejadian " << d.jenis << " menit " << d.menit << "' berhasil ditambahkan." << endl;
        if (d.jenis == KARTU_KUNING && hitungJenisKejadian(C, KARTU_KUNING) == 2) {
            cout << "Kartu kuning kedua! " << C->info.nama << " keluar pada menit "
                 << menitKeluar(C) << "'." << endl;
        }
        printSkorPertandingan(P, "Skor saat ini: ");
    } else if (h == MENIT_TIDAK_VALID) {
        cout << "Ditolak: menit harus di antara 1 sampai 120." << endl;
    } else if (h == KEJADIAN_KEMBAR) {
        cout << "Ditolak: kejadian yang sama pada menit yang sama sudah tercatat untuk pemain ini." << endl;
    } else if (h == PEMAIN_SUDAH_KELUAR) {
        cout << "Ditolak: " << C->info.nama << " sudah keluar pada menit " << menitKeluar(C) << "'." << endl;
    } else if (h == MERAH_GANDA) {
        cout << "Ditolak: " << C->info.nama << " sudah memiliki kartu merah." << endl;
    } else if (h == KUNING_PENUH) {
        cout << "Ditolak: " << C->info.nama << " sudah memiliki 2 kartu kuning." << endl;
    } else if (h == ASSIST_TANPA_GOL) {
        cout << "Ditolak: assist butuh gol rekan satu tim di menit yang sama. Input gol lebih dulu." << endl;
    } else if (h == ASSIST_SUDAH_ADA) {
        cout << "Ditolak: gol pada menit ini sudah memiliki assist." << endl;
    } else {
        cout << "Ditolak: kartu ini membuat pemain keluar sebelum kejadian yang sudah tercatat. "
             << "Hapus kejadian setelah menit keluar lebih dulu." << endl;
    }
}

void isiSkuadOtomatis(adrPertandingan P) {
    char kodeTim[2] = {'H', 'A'};
    for (int t = 0; t < 2; t++) {
        string namaDasar = namaTim(P, kodeTim[t]);
        for (int no = 1; no <= 11; no++) {
            string pos = "FW";
            if (no == 1) pos = "GK";
            else if (no <= 5) pos = "DF";
            else if (no <= 8) pos = "MF";
            tambahPemainContoh(P, kodeTim[t], no, namaDasar + intToStr(no), pos);
        }
    }
}

void menuTambahPertandingan(ListPertandingan &L) {
    infoPertandingan d;
    d.idMatch = toUpperStr(inputWajib("ID pertandingan (contoh: M04): "));
    d.timHome = inputWajib("Tim home: ");
    d.timAway = inputWajib("Tim away: ");
    d.stadion = inputWajib("Stadion: ");
    if (!cin) return;

    adrPertandingan P = createParent(d);
    HasilInsert h = insertParent(L, P);
    if (h == INSERT_OK) {
        cout << "Pertandingan " << d.idMatch << " berhasil ditambahkan." << endl;
        if (konfirmasi("Isi skuad otomatis (11 pemain per tim)?")) {
            isiSkuadOtomatis(P);
            cout << "22 pemain otomatis ditambahkan (" << d.timHome << "1..11 & "
                 << d.timAway << "1..11)." << endl;
        }
        return;
    }
    delete P;
    if (h == ID_KEMBAR) {
        cout << "Gagal: ID " << d.idMatch << " sudah digunakan." << endl;
    } else {
        cout << "Gagal: tim home dan tim away tidak boleh sama." << endl;
    }
}

void menuCariPertandingan(ListPertandingan L) {
    if (isEmpty(L)) {
        cout << "Belum ada data pertandingan." << endl;
        return;
    }
    cout << "Cari berdasarkan:" << endl;
    cout << "  1. ID pertandingan" << endl;
    cout << "  2. Nama tim" << endl;
    int pilih = inputInt("Pilih (1-2): ");
    if (pilih == 1) {
        adrPertandingan P = pilihPertandingan(L);
        if (P != Nil) {
            cout << "Ditemukan:" << endl;
            printSatuParent(P);
        }
    } else if (pilih == 2) {
        printParentTim(L, inputWajib("Nama tim: "));
    } else {
        cout << "Pilihan tidak valid." << endl;
    }
}

void menuHapusPertandingan(ListPertandingan &L) {
    adrPertandingan P = pilihPertandingan(L);
    if (P == Nil) return;

    printSatuParent(P);
    int jumlahPemain = hitungJumlahPemain(P);
    if (!konfirmasi("Hapus pertandingan ini beserta " + intToStr(jumlahPemain) + " pemain & seluruh kejadiannya?")) {
        cout << "Penghapusan dibatalkan." << endl;
        return;
    }
    string id = P->info.idMatch;
    deleteParent(L, P);
    cout << "Pertandingan " << id << " berhasil dihapus." << endl;
}

void menuTambahPemain(ListPertandingan &L) {
    adrPertandingan P = pilihPertandingan(L);
    if (P == Nil) return;

    infoPemain d;
    cout << "Home = " << P->info.timHome << ", Away = " << P->info.timAway << endl;
    d.tim = inputTim();
    d.noPunggung = inputInt("No punggung (1-99): ");
    if (d.noPunggung < 1 || d.noPunggung > 99) {
        cout << "Gagal: nomor punggung harus 1 - 99." << endl;
        return;
    }
    d.nama = inputWajib("Nama pemain: ");
    d.posisi = toUpperStr(inputWajib("Posisi (GK/DF/MF/FW): "));
    if (!cin) return;
    if (!posisiValid(d.posisi)) {
        cout << "Gagal: posisi harus GK, DF, MF, atau FW." << endl;
        return;
    }

    adrPemain C = createChild(d);
    if (insertChild(P, C) == INSERT_OK) {
        cout << d.nama << " berhasil ditambahkan ke " << namaTim(P, d.tim) << "." << endl;
    } else {
        delete C;
        cout << "Gagal: nomor #" << d.noPunggung << " di tim " << namaTim(P, d.tim) << " sudah terdaftar." << endl;
    }
}

void menuTampilPemain(ListPertandingan L) {
    adrPertandingan P = pilihPertandingan(L);
    if (P != Nil) printChild(P);
}

void menuCariPemain(ListPertandingan L) {
    adrPertandingan P = pilihPertandingan(L);
    if (P == Nil) return;
    if (isEmptyPemain(P->lPemain)) {
        cout << "Pertandingan ini belum memiliki pemain." << endl;
        return;
    }
    cout << "Cari berdasarkan:" << endl;
    cout << "  1. Tim dan nomor punggung" << endl;
    cout << "  2. Nama pemain" << endl;
    int pilih = inputInt("Pilih (1-2): ");
    adrPemain C = Nil;
    if (pilih == 1) {
        C = pilihPemain(P, false);
    } else if (pilih == 2) {
        C = findChildNama(P, inputWajib("Nama pemain: "));
        if (C == Nil) cout << "Pemain tidak ditemukan pada pertandingan ini." << endl;
    } else {
        cout << "Pilihan tidak valid." << endl;
    }
    if (C == Nil) return;

    cout << "Ditemukan: [" << C->info.tim << "] #" << C->info.noPunggung << " " << C->info.nama
         << " (" << C->info.posisi << ") - " << namaTim(P, C->info.tim) << endl;
    cout << "  Kejadian:" << endl;
    printKejadian(C);
}

void menuHapusPemain(ListPertandingan &L) {
    adrPertandingan P = pilihPertandingan(L);
    if (P == Nil) return;
    adrPemain C = pilihPemain(P, true);
    if (C == Nil) return;

    string nama = C->info.nama;
    if (!konfirmasi("Hapus " + nama + " beserta seluruh kejadiannya?")) {
        cout << "Penghapusan dibatalkan." << endl;
        return;
    }
    int assistTerhapus = deleteChild(P, C);
    cout << nama << " berhasil dihapus dari pertandingan " << P->info.idMatch << "." << endl;
    if (assistTerhapus > 0) {
        cout << assistTerhapus << " assist yang tidak lagi didukung gol ikut dihapus." << endl;
    }
    printSkorPertandingan(P, "Skor terbaru: ");
}

void menuTambahKejadian(ListPertandingan &L) {
    adrPertandingan P = pilihPertandingan(L);
    if (P == Nil) return;
    adrPemain C = pilihPemain(P, true);
    if (C == Nil) return;

    infoKejadian d;
    d.menit = inputInt("Menit (1-120): ");
    d.jenis = inputJenisKejadian();
    if (!cin) return;

    HasilKejadian h = tambahKejadian(P, C, d);
    tampilHasilKejadian(h, P, C, d);
}

void menuHapusKejadian(ListPertandingan &L) {
    adrPertandingan P = pilihPertandingan(L);
    if (P == Nil) return;
    adrPemain C = pilihPemain(P, true);
    if (C == Nil) return;

    cout << "Kejadian milik " << C->info.nama << ":" << endl;
    printKejadian(C);
    if (isEmptyKejadian(C->lKejadian)) return;

    int urutan = inputInt("Nomor urut kejadian yang dihapus: ");
    adrKejadian K = findKejadianKe(C, urutan);
    if (K == Nil) {
        cout << "Nomor urut tidak valid." << endl;
        return;
    }
    int assistTerhapus = deleteKejadian(P, C, K);
    cout << "Kejadian berhasil dihapus." << endl;
    if (assistTerhapus > 0) {
        cout << assistTerhapus << " assist yang tidak lagi didukung gol ikut dihapus." << endl;
    }
    printSkorPertandingan(P, "Skor terbaru: ");
}

void menuRelasiParentChild(ListPertandingan L) {
    if (isEmpty(L)) {
        cout << "Belum ada data pertandingan." << endl;
        return;
    }
    cout << "Tampilkan relasi Parent - Child:" << endl;
    cout << "  1. Semua pertandingan dan pemainnya" << endl;
    cout << "  2. Satu pertandingan tertentu (by ID)" << endl;
    int pilih = inputInt("Pilih (1-2): ");
    if (pilih == 1) {
        printRelasiParentChild(L);
    } else if (pilih == 2) {
        adrPertandingan P = pilihPertandingan(L);
        if (P != Nil) {
            printSatuParent(P);
            printChild(P);
        }
    } else {
        cout << "Pilihan tidak valid." << endl;
    }
}

void menuLaporan(ListPertandingan L) {
    adrPertandingan P = pilihPertandingan(L);
    if (P != Nil) printLaporanPertandingan(P);
}

void menuTimeline(ListPertandingan L) {
    adrPertandingan P = pilihPertandingan(L);
    if (P != Nil) printTimeline(P);
}

void menuTopScorer(ListPertandingan L) {
    adrPertandingan P = pilihPertandingan(L);
    if (P != Nil) printTopScorer(P);
}

void menuRekapPemain(ListPertandingan L) {
    string nama = inputWajib("Nama pemain: ");
    string tim = inputWajib("Nama tim yang dibela: ");
    int noPunggung = inputInt("No punggung (1-99): ");
    if (noPunggung < 1 || noPunggung > 99) {
        cout << "Gagal: nomor punggung harus 1 - 99." << endl;
        return;
    }
    cariPemainSemuaPertandingan(L, nama, tim, noPunggung);
}

void jedaSingkat(int skala) {
    volatile long sink = 0;
    for (int i = 0; i < skala; i++) {
        for (int j = 0; j < 2000000; j++) {
            sink += j;
        }
    }
}

void tampilMenu() {
    cout << endl;
    cout << "+=============================================+" << endl;
    cout << "|            FOOTBALL MATCH CENTER            |" << endl;
    cout << "+=============================================+" << endl;
    cout << "| -- Pertandingan (Parent) --                 |" << endl;
    cout << "|   1. Tambah pertandingan                    |" << endl;
    cout << "|   2. Tampilkan daftar pertandingan          |" << endl;
    cout << "|   3. Cari pertandingan (ID / nama tim)      |" << endl;
    cout << "|   4. Hapus pertandingan                     |" << endl;
    cout << "| -- Pemain (Child) --                        |" << endl;
    cout << "|   5. Tambah pemain ke pertandingan          |" << endl;
    cout << "|   6. Tampilkan pemain pada pertandingan     |" << endl;
    cout << "|   7. Cari pemain pada pertandingan          |" << endl;
    cout << "|   8. Hapus pemain dari pertandingan         |" << endl;
    cout << "| -- Kejadian (Grandchild) --                 |" << endl;
    cout << "|   9. Tambah kejadian (gol/assist/kartu)     |" << endl;
    cout << "|  10. Hapus kejadian                         |" << endl;
    cout << "| -- Relasi & Laporan --                      |" << endl;
    cout << "|  11. Show relasi Parent - Child             |" << endl;
    cout << "|  12. Tampilkan seluruh data (printAll)      |" << endl;
    cout << "|  13. Laporan pertandingan (skor otomatis)   |" << endl;
    cout << "|  14. Timeline pertandingan                  |" << endl;
    cout << "|  15. Top scorer pertandingan                |" << endl;
    cout << "|  16. Rekap pemain di semua pertandingan     |" << endl;
    cout << "|   0. Keluar                                 |" << endl;
    cout << "+=============================================+" << endl;
}

void animasiPembuka() {
    cout << endl;
    cout << "+=============================================+" << endl;
    jedaSingkat(3);
    cout << "|       FOOTBALL MATCH CENTER - MLL C++       |" << endl;
    jedaSingkat(3);
    cout << "|         Pertandingan > Pemain > Event       |" << endl;
    jedaSingkat(3);
    cout << "+=============================================+" << endl;
    jedaSingkat(4);
}
