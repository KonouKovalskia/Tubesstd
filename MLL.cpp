#include "MLL.h"

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

    int pemainHome = 0, pemainAway = 0;
    adrPemain C = P->lPemain.first;
    while (C != Nil) {
        if (C->info.tim == 'H') pemainHome++;
        else pemainAway++;
        C = C->next;
    }

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
    adrPertandingan P = L.first;
    while (P != Nil) {
        adrPemain C = P->lPemain.first;
        while (C != Nil) {
            if (toLowerStr(C->info.nama) == toLowerStr(nama) &&
                toLowerStr(namaTim(P, C->info.tim)) == toLowerStr(tim) &&
                C->info.noPunggung == noPunggung) {
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
            C = C->next;
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
