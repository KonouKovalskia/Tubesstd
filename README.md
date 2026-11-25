# Football Match Center

Tugas Besar Struktur Data, kelompok ONE. Program C++ untuk mencatat pertandingan sepak bola dengan Multi Linked List model List to List.

## Anggota

| Nama | NIM |
| --- | --- |
| Muhamad Alfi Syahrin | 103032400057 |
| Muhammad Faiz Ghiffari | 103032400002 |

## Struktur Data

Tiga tingkat, semuanya Double Linked List dengan `first` dan `last`.

| Tingkat | Entitas | Atribut |
| --- | --- | --- |
| Induk | Pertandingan | idMatch, timHome, timAway, stadion |
| Anak | Pemain | noPunggung, nama, posisi, tim (H/A) |
| Cucu | Kejadian | menit, jenis (gol, assist, kartu kuning, kartu merah, gol bunuh diri) |

Setiap pertandingan menyimpan list pemainnya, dan setiap pemain menyimpan list kejadiannya yang terurut menit. Skor tidak disimpan, tapi dihitung lewat traversal: gol menambah skor tim pemain, gol bunuh diri menambah skor tim lawan.

## File

| File | Isi |
| --- | --- |
| `MLL.h` | Struktur data dan prototipe |
| `MLL.cpp` | Realisasi operasi ADT |
| `main.cpp` | Menu program dan input |

## Cara Build

Dengan g++:

```
g++ -std=c++11 -o tubes main.cpp MLL.cpp
./tubes
```

Dengan Code Blocks: buat project Console application C++ kosong, lalu tambahkan ketiga file di atas ke project dan build. File `.cbp` tidak disertakan di repo.

## Menu

| No | Fitur |
| --- | --- |
| 1 | Tambah pertandingan |
| 2 | Tampilkan daftar pertandingan |
| 3 | Cari pertandingan (ID atau nama tim) |
| 4 | Hapus pertandingan beserta seluruh pemain dan kejadiannya |
| 5 | Tambah pemain |
| 6 | Tampilkan pemain pada pertandingan |
| 7 | Cari pemain (tim dan nomor punggung, atau nama) |
| 8 | Hapus pemain |
| 9 | Tambah kejadian |
| 10 | Hapus kejadian (berdasarkan nomor urut) |
| 11 | Tampilkan seluruh data (printAll) |
| 12 | Laporan pertandingan |
| 13 | Timeline pertandingan |
| 14 | Top scorer pertandingan |
| 15 | Rekap pemain di semua pertandingan |

Secara default saat program dijalankan, 3 pertandingan awal (`M01`: Manchester United vs Tottenham Hotspur, `M02`: Manchester City vs Liverpool, `M03`: Arsenal vs Chelsea) beserta starting eleven masing-masing tim (66 pemain) sudah otomatis dimuat.

## Aturan Validasi

- ID pertandingan harus unik, tim home dan away tidak boleh sama. Skuad pemain dapat diisi otomatis (11 pemain per tim) saat menambah pertandingan atau ditambah manual.
- Nomor punggung 1 sampai 99 dan unik per tim dalam satu pertandingan. Posisi hanya GK, DF, MF, FW.
- Menit kejadian 1 sampai 120. Kejadian yang sama pada menit yang sama untuk pemain yang sama ditolak.
- Assist hanya diterima kalau ada gol rekan satu tim di menit yang sama, dan setiap gol maksimal memiliki satu assist.
- Pemain maksimal 1 kartu merah dan 2 kartu kuning. Kartu kuning kedua otomatis membuat pemain keluar, setara kartu merah.
- Pemain yang sudah keluar tidak boleh punya kejadian setelah menit keluar. Kartu yang membuat pemain keluar sebelum kejadian yang sudah tercatat juga ditolak.
- Menghapus gol atau pemain otomatis menghapus assist yang tidak lagi didukung gol.
- Rekap lintas pertandingan mengenali pemain dari nama, nama tim, dan nomor punggung agar pemain bernama sama di tim yang sama tidak tergabung.
