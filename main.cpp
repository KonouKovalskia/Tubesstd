#include <iostream>
using namespace std;
#include "mll.h"

// FUNGSI BANTU INPUT
string inputString(string prompt) {
    string s;
    cout << prompt;
    if (!getline(cin, s)) {
        s = "0";        // input berakhir (misal Ctrl+Z / Ctrl+D): anggap memilih 0
    }
    return s;
}

// Input yang tidak boleh kosong
string inputWajib(string prompt) {
    while (true) {
        string s = inputString(prompt);
        if (s != "") return s;
        cout << "Input tidak boleh kosong." << endl;
    }
}

// Mengubah string menjadi angka bulat; mengembalikan false jika bukan angka
bool keAngka(string s, int &hasil) {
    int awal = 0;
    int akhir = (int)s.length() - 1;
    while (awal <= akhir && s[awal] == ' ') awal++;
    while (akhir >= awal && s[akhir] == ' ') akhir--;
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

// Meminta ID lalu mencari pertandingan; menangani list kosong & tidak ditemukan
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

// Meminta tim + nomor punggung lalu mencari pemain pada pertandingan P
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

// DATA CONTOH
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

void tambahKejadianContoh(adrPemain C, int menit, string jenis) {
    infoKejadian d;
    d.menit = menit;
    d.jenis = jenis;
    string pesan;
    tambahKejadian(C, d, pesan);
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

    // Pertandingan 1 : 2 - 1
    P = tambahPertandinganContoh(L, "M01", "Garuda FC", "Rajawali United", "12-08-2026", "Stadion Si Jalak Harupat");
    C = tambahPemainContoh(P, 'H', 10, "Andi Saputra", "FW");
    tambahKejadianContoh(C, 23, GOL);
    tambahKejadianContoh(C, 67, GOL);
    C = tambahPemainContoh(P, 'H', 8, "Rizky Pratama", "MF");
    tambahKejadianContoh(C, 23, ASSIST);
    tambahKejadianContoh(C, 55, KARTU_KUNING);
    C = tambahPemainContoh(P, 'H', 1, "Fajar Nugraha", "GK");
    C = tambahPemainContoh(P, 'A', 9, "Dimas Ardiansyah", "FW");
    tambahKejadianContoh(C, 81, GOL);
    C = tambahPemainContoh(P, 'A', 5, "Yoga Permana", "DF");
    tambahKejadianContoh(C, 30, KARTU_KUNING);

    // Pertandingan 2 : 2 - 1 (ada gol bunuh diri & kartu kuning kedua)
    P = tambahPertandinganContoh(L, "M02", "Banteng Muda", "Garuda FC", "19-08-2026", "Stadion Manahan");
    C = tambahPemainContoh(P, 'H', 11, "Bayu Setiawan", "FW");
    tambahKejadianContoh(C, 15, GOL);
    C = tambahPemainContoh(P, 'H', 4, "Hendra Wijaya", "DF");
    tambahKejadianContoh(C, 20, KARTU_KUNING);
    tambahKejadianContoh(C, 62, KARTU_KUNING);     // otomatis kartu merah menit 62
    C = tambahPemainContoh(P, 'A', 10, "Andi Saputra", "FW");
    tambahKejadianContoh(C, 44, GOL);
    C = tambahPemainContoh(P, 'A', 3, "Galih Ramadhan", "DF");
    tambahKejadianContoh(C, 88, GOL_BUNUH_DIRI);

    // Pertandingan 3 : belum dimainkan, belum ada pemain
    tambahPertandinganContoh(L, "M03", "Harimau Sumatera", "Rajawali United", "26-08-2026", "Stadion Teladan");

    cout << "Data contoh berhasil dimasukkan: 3 pertandingan, 9 pemain." << endl;
}

// MENU-MENU
void menuTambahPertandingan(ListPertandingan &L) {
    infoPertandingan d;
    d.idMatch = toUpperStr(inputWajib("ID pertandingan (contoh: M04): "));
    if (findParent(L, d.idMatch) != Nil) {
        cout << "Gagal: ID " << d.idMatch << " sudah digunakan." << endl;
        return;
    }
    d.timHome = inputWajib("Tim home: ");
    d.timAway = inputWajib("Tim away: ");
    if (toLowerStr(d.timHome) == toLowerStr(d.timAway)) {
        cout << "Gagal: tim home dan tim away tidak boleh sama." << endl;
        return;
    }
    d.tanggal = inputWajib("Tanggal (dd-mm-yyyy): ");
    d.stadion = inputWajib("Stadion: ");

    insertParent(L, createParent(d));
    cout << "Pertandingan " << d.idMatch << " berhasil ditambahkan." << endl;
}

void menuCariPertandingan(ListPertandingan L) {
    adrPertandingan P = pilihPertandingan(L);
    if (P != Nil) {
        cout << "Ditemukan:" << endl;
        printSatuParent(P);
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
    if (findChild(P, d.tim, d.noPunggung) != Nil) {
        cout << "Gagal: nomor #" << d.noPunggung << " di tim " << namaTim(P, d.tim) << " sudah terdaftar." << endl;
        return;
    }
    d.nama = inputWajib("Nama pemain: ");
    d.posisi = toUpperStr(inputWajib("Posisi (GK/DF/MF/FW): "));

    insertChild(P, createChild(d));
    cout << d.nama << " berhasil ditambahkan ke " << namaTim(P, d.tim) << "." << endl;
}

void menuTampilPemain(ListPertandingan L) {
    adrPertandingan P = pilihPertandingan(L);
    if (P != Nil) printChild(P);
}

void menuCariPemain(ListPertandingan L) {
    adrPertandingan P = pilihPertandingan(L);
    if (P == Nil) return;
    adrPemain C = pilihPemain(P, false);
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
    deleteChild(P, C);
    cout << nama << " berhasil dihapus dari pertandingan " << P->info.idMatch << "." << endl;

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

    string pesan;
    tambahKejadian(C, d, pesan);
    cout << pesan << endl;

    int h, a;
    hitungSkor(P, h, a);
    cout << "Skor saat ini: " << P->info.timHome << " " << h << " - " << a << " " << P->info.timAway << endl;
}

void menuHapusKejadian(ListPertandingan &L) {
    adrPertandingan P = pilihPertandingan(L);
    if (P == Nil) return;
    adrPemain C = pilihPemain(P, true);
    if (C == Nil) return;

    cout << "Kejadian milik " << C->info.nama << ":" << endl;
    printKejadian(C);
    if (isEmptyKejadian(C->lKejadian)) return;

    int menit = inputInt("Menit kejadian yang dihapus: ");
    string jenis = inputJenisKejadian();
    adrKejadian K = findKejadian(C, menit, jenis);
    if (K == Nil) {
        cout << "Kejadian " << jenis << " pada menit " << menit << "' tidak ditemukan." << endl;
        return;
    }
    deleteKejadian(C, K);
    cout << "Kejadian berhasil dihapus." << endl;

    int h, a;
    hitungSkor(P, h, a);
    cout << "Skor terbaru: " << P->info.timHome << " " << h << " - " << a << " " << P->info.timAway << endl;
}

void menuLaporan(ListPertandingan L) {
    adrPertandingan P = pilihPertandingan(L);
    if (P != Nil) printLaporanPertandingan(P);
}

void menuRekapPemain(ListPertandingan L) {
    string nama = inputWajib("Nama pemain: ");
    cariPemainSemuaPertandingan(L, nama);
}

// MAIN
int main() {
    ListPertandingan L;
    createList(L);

    int pilihan = -1;
    while (pilihan != 0) {
        cout << endl;
        cout << "=========== MATCH STATS SEPAK BOLA ===========" << endl;
        cout << " -- Pertandingan (Parent) --" << endl;
        cout << "  1. Tambah pertandingan" << endl;
        cout << "  2. Tampilkan daftar pertandingan" << endl;
        cout << "  3. Cari pertandingan" << endl;
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
        cout << " 13. Rekap pemain di semua pertandingan" << endl;
        cout << " 14. Isi data contoh" << endl;
        cout << "  0. Keluar" << endl;
        cout << "==============================================" << endl;
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
            case 13: menuRekapPemain(L); break;
            case 14: isiDataContoh(L); break;
            case 0:  cout << "Terima kasih!" << endl; break;
            default: cout << "Menu tidak tersedia." << endl;
        }
        if (pilihan != 0) jeda();
    }

    hapusSemua(L);
    return 0;
}
