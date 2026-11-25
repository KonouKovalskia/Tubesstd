#include <cstdlib>
#include "MLL.h"

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
        cout << endl << "Input berakhir, program ditutup." << endl;
        exit(0);
    }
    return potong(s);
}

string inputWajib(string prompt) {
    while (true) {
        string s = inputString(prompt);
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
        int x;
        if (keAngka(s, x)) return x;
        cout << "Input harus berupa angka bulat." << endl;
    }
}

string toUpperStr(string s) {
    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] >= 'a' && s[i] <= 'z') {
            s[i] = s[i] - 'a' + 'A';
        }
    }
    return s;
}

bool posisiValid(string s) {
    return s == "GK" || s == "DF" || s == "MF" || s == "FW";
}

bool tanggalValid(string t) {
    if (t.length() != 10 || t[2] != '-' || t[5] != '-') return false;
    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && (t[i] < '0' || t[i] > '9')) return false;
    }
    int hari = (t[0] - '0') * 10 + (t[1] - '0');
    int bulan = (t[3] - '0') * 10 + (t[4] - '0');
    int tahun = (t[6] - '0') * 1000 + (t[7] - '0') * 100 + (t[8] - '0') * 10 + (t[9] - '0');
    if (tahun < 1900 || bulan < 1 || bulan > 12 || hari < 1) return false;
    int batas = 31;
    if (bulan == 4 || bulan == 6 || bulan == 9 || bulan == 11) {
        batas = 30;
    } else if (bulan == 2) {
        bool kabisat = (tahun % 4 == 0 && tahun % 100 != 0) || tahun % 400 == 0;
        batas = kabisat ? 29 : 28;
    }
    return hari <= batas;
}

char inputTim() {
    while (true) {
        string s = toUpperStr(inputString("Tim (H = home / A = away): "));
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

void jeda() {
    inputString("\nTekan Enter untuk kembali ke menu...");
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

adrPertandingan tambahPertandinganContoh(ListPertandingan &L, string id, string home, string away,
                                         string tanggal, string stadion) {
    infoPertandingan d;
    d.idMatch = id;
    d.timHome = home;
    d.timAway = away;
    d.tanggal = tanggal;
    d.stadion = stadion;
    adrPertandingan P = createParent(d);
    insertParent(L, P);
    return P;
}

void isiDataContoh(ListPertandingan &L) {
    if (findParent(L, "M01") != Nil || findParent(L, "M02") != Nil || findParent(L, "M03") != Nil) {
        cout << "Data contoh tidak dimasukkan karena ID M01/M02/M03 sudah dipakai." << endl;
        return;
    }
    adrPertandingan P;
    adrPemain C;

    P = tambahPertandinganContoh(L, "M01", "Garuda FC", "Rajawali United", "12-08-2026", "Stadion Si Jalak Harupat");
    C = tambahPemainContoh(P, 'H', 10, "Andi Saputra", "FW");
    tambahKejadianContoh(P, C, 23, GOL);
    tambahKejadianContoh(P, C, 67, GOL);
    C = tambahPemainContoh(P, 'H', 8, "Rizky Pratama", "MF");
    tambahKejadianContoh(P, C, 23, ASSIST);
    tambahKejadianContoh(P, C, 55, KARTU_KUNING);
    C = tambahPemainContoh(P, 'H', 1, "Fajar Nugraha", "GK");
    C = tambahPemainContoh(P, 'A', 9, "Dimas Ardiansyah", "FW");
    tambahKejadianContoh(P, C, 81, GOL);
    C = tambahPemainContoh(P, 'A', 5, "Yoga Permana", "DF");
    tambahKejadianContoh(P, C, 30, KARTU_KUNING);

    P = tambahPertandinganContoh(L, "M02", "Banteng Muda", "Garuda FC", "19-08-2026", "Stadion Manahan");
    C = tambahPemainContoh(P, 'H', 11, "Bayu Setiawan", "FW");
    tambahKejadianContoh(P, C, 15, GOL);
    C = tambahPemainContoh(P, 'H', 4, "Hendra Wijaya", "DF");
    tambahKejadianContoh(P, C, 15, ASSIST);
    tambahKejadianContoh(P, C, 20, KARTU_KUNING);
    tambahKejadianContoh(P, C, 62, KARTU_KUNING);
    C = tambahPemainContoh(P, 'A', 10, "Andi Saputra", "FW");
    tambahKejadianContoh(P, C, 44, GOL);
    C = tambahPemainContoh(P, 'A', 3, "Galih Ramadhan", "DF");
    tambahKejadianContoh(P, C, 88, GOL_BUNUH_DIRI);

    tambahPertandinganContoh(L, "M03", "Harimau Sumatera", "Rajawali United", "26-08-2026", "Stadion Teladan");

    cout << "Data contoh berhasil dimasukkan: 3 pertandingan, 9 pemain." << endl;
}

void tampilHasilKejadian(HasilKejadian h, adrPertandingan P, adrPemain C, infoKejadian d) {
    if (h == KEJADIAN_OK) {
        cout << "Kejadian " << d.jenis << " menit " << d.menit << "' berhasil ditambahkan." << endl;
        if (d.jenis == KARTU_KUNING && hitungJenisKejadian(C, KARTU_KUNING) == 2) {
            cout << "Kartu kuning kedua! " << C->info.nama << " keluar pada menit "
                 << menitKeluar(C) << "'." << endl;
        }
        int hm, aw;
        hitungSkor(P, hm, aw);
        cout << "Skor saat ini: " << P->info.timHome << " " << hm << " - " << aw << " " << P->info.timAway << endl;
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
    } else {
        cout << "Ditolak: kartu ini membuat pemain keluar sebelum kejadian yang sudah tercatat. "
             << "Hapus kejadian setelah menit keluar lebih dulu." << endl;
    }
}

void menuTambahPertandingan(ListPertandingan &L) {
    infoPertandingan d;
    d.idMatch = toUpperStr(inputWajib("ID pertandingan (contoh: M04): "));
    d.timHome = inputWajib("Tim home: ");
    d.timAway = inputWajib("Tim away: ");
    d.tanggal = inputWajib("Tanggal (dd-mm-yyyy): ");
    if (!tanggalValid(d.tanggal)) {
        cout << "Gagal: format tanggal harus dd-mm-yyyy dan tanggalnya harus ada." << endl;
        return;
    }
    d.stadion = inputWajib("Stadion: ");

    adrPertandingan P = createParent(d);
    HasilInsert h = insertParent(L, P);
    if (h == INSERT_OK) {
        cout << "Pertandingan " << d.idMatch << " berhasil ditambahkan." << endl;
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

    int h, a;
    hitungSkor(P, h, a);
    cout << "Skor terbaru: " << P->info.timHome << " " << h << " - " << a << " " << P->info.timAway << endl;
}

void menuTambahKejadian(ListPertandingan &L) {
    adrPertandingan P = pilihPertandingan(L);
    if (P == Nil) return;
    adrPemain C = pilihPemain(P, true);
    if (C == Nil) return;

    infoKejadian d;
    d.menit = inputInt("Menit (1-120): ");
    d.jenis = inputJenisKejadian();

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

    int h, a;
    hitungSkor(P, h, a);
    cout << "Skor terbaru: " << P->info.timHome << " " << h << " - " << a << " " << P->info.timAway << endl;
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
    cariPemainSemuaPertandingan(L, nama, tim);
}

int main() {
    ListPertandingan L;
    createList(L);

    int pilihan = -1;
    while (pilihan != 0) {
        cout << endl;
        cout << "============ FOOTBALL MATCH CENTER ============" << endl;
        cout << " -- Pertandingan (Parent) --" << endl;
        cout << "  1. Tambah pertandingan" << endl;
        cout << "  2. Tampilkan daftar pertandingan" << endl;
        cout << "  3. Cari pertandingan (ID / nama tim)" << endl;
        cout << "  4. Hapus pertandingan" << endl;
        cout << " -- Pemain (Child) --" << endl;
        cout << "  5. Tambah pemain ke pertandingan" << endl;
        cout << "  6. Tampilkan pemain pada pertandingan" << endl;
        cout << "  7. Cari pemain pada pertandingan" << endl;
        cout << "  8. Hapus pemain dari pertandingan" << endl;
        cout << " -- Kejadian (Grandchild) --" << endl;
        cout << "  9. Tambah kejadian (gol/assist/kartu)" << endl;
        cout << " 10. Hapus kejadian" << endl;
        cout << " -- Lainnya --" << endl;
        cout << " 11. Tampilkan seluruh data (printAll)" << endl;
        cout << " 12. Laporan pertandingan (skor otomatis)" << endl;
        cout << " 13. Timeline pertandingan" << endl;
        cout << " 14. Top scorer pertandingan" << endl;
        cout << " 15. Rekap pemain di semua pertandingan" << endl;
        cout << " 16. Isi data contoh" << endl;
        cout << "  0. Keluar" << endl;
        cout << "===============================================" << endl;
        pilihan = inputInt("Pilih menu: ");
        cout << endl;

        switch (pilihan) {
            case 1:  menuTambahPertandingan(L); break;
            case 2:  printParent(L); break;
            case 3:  menuCariPertandingan(L); break;
            case 4:  menuHapusPertandingan(L); break;
            case 5:  menuTambahPemain(L); break;
            case 6:  menuTampilPemain(L); break;
            case 7:  menuCariPemain(L); break;
            case 8:  menuHapusPemain(L); break;
            case 9:  menuTambahKejadian(L); break;
            case 10: menuHapusKejadian(L); break;
            case 11: printAll(L); break;
            case 12: menuLaporan(L); break;
            case 13: menuTimeline(L); break;
            case 14: menuTopScorer(L); break;
            case 15: menuRekapPemain(L); break;
            case 16: isiDataContoh(L); break;
            case 0:  cout << "Terima kasih!" << endl; break;
            default: cout << "Menu tidak tersedia." << endl;
        }
        if (pilihan != 0) jeda();
    }

    hapusSemua(L);
    return 0;
}
