#include <iostream>
#include <cstring>
#include <queue>
#include <stack>

using namespace std;

struct Menu {
    char kode[10];
    char nama[50];
    int harga;
    char kategori[30];
};

struct Transaksi {
    char namaPelanggan[50];
    char kodeMenu[10];
    char namaMenu[50];
    int jumlah;
    int totalHarga;
};

queue<Transaksi> antrianPesanan;
stack<Transaksi> riwayatPesananStack;

void tambahMenu() {
    Menu m;
    cout << "\nTambah Menu Baru\n";
    cout << "Kode Menu     : "; cin >> m.kode;
    cin.ignore();
    cout << "Nama Menu     : "; cin.getline(m.nama, 50);
    cout << "Harga Menu    : "; cin >> m.harga;
    cin.ignore();
    cout << "Kategori      : "; cin.getline(m.kategori, 30);

    FILE* file = fopen("menu.dat", "ab");
    if (file) {
        fwrite(&m, sizeof(Menu), 1, file);
        fclose(file);
        cout << "Menu berhasil ditambahkan!\n";
    } else {
        cout << "Gagal menyimpan menu.\n";
    }
}

void tampilMenu() {
    Menu m;
    FILE* file = fopen("menu.dat", "rb");
    if (!file) {
        cout << "File menu tidak ditemukan.\n";
        return;
    }

    cout << "\nDaftar Menu\n";
    cout << "Kode\tNama\t\tHarga\tKategori\n";
    cout << "------------------------------------------\n";
    while (fread(&m, sizeof(Menu), 1, file)) {
        cout << m.kode << "\t" << m.nama << "\t\t" << m.harga << "\t" << m.kategori << endl;
    }

    fclose(file);
}

void editMenu() {
    char kode[10];
    cout << "Masukkan kode menu yang akan diedit: ";
    cin >> kode;

    FILE* file = fopen("menu.dat", "rb+");
    if (!file) {
        cout << "File menu tidak ditemukan.\n";
        return;
    }

    Menu m;
    bool found = false;

    while (fread(&m, sizeof(Menu), 1, file)) {
        if (strcmp(m.kode, kode) == 0) {
            cout << "Menu ditemukan. Masukkan data baru:\n";
            cout << "Nama Menu     : "; cin.ignore(); cin.getline(m.nama, 50);
            cout << "Harga Menu    : "; cin >> m.harga;
            cin.ignore();
            cout << "Kategori      : "; cin.getline(m.kategori, 30);

            fseek(file, -sizeof(Menu), SEEK_CUR);
            fwrite(&m, sizeof(Menu), 1, file);
            found = true;
            cout << "Menu berhasil diperbarui!\n";
            break;
        }
    }

    if (!found) {
        cout << "Menu dengan kode tersebut tidak ditemukan.\n";
    }

    fclose(file);
}

void pesanMenu() {
    char kode[10];
    int jumlah;
    char namaPelanggan[50];
    cout << "Nama Pelanggan : "; cin.ignore(); cin.getline(namaPelanggan, 50);
    cout << "Kode Menu      : "; cin >> kode;

    FILE* file = fopen("menu.dat", "rb");
    if (!file) {
        cout << "File menu tidak ditemukan.\n";
        return;
    }

    Menu m;
    bool found = false;
    while (fread(&m, sizeof(Menu), 1, file)) {
        if (strcmp(m.kode, kode) == 0) {
            found = true;
            break;
        }
    }
    fclose(file);

    if (!found) {
        cout << "Menu tidak ditemukan.\n";
        return;
    }

    cout << "Jumlah Pesanan : "; cin >> jumlah;

    Transaksi t;
    strcpy(t.namaPelanggan, namaPelanggan);
    strcpy(t.kodeMenu, m.kode);
    strcpy(t.namaMenu, m.nama);
    t.jumlah = jumlah;
    t.totalHarga = m.harga * jumlah;

    antrianPesanan.push(t);        
    riwayatPesananStack.push(t);   

    cout << "Pesanan berhasil dicatat dan masuk ke antrian!\n";
}

void tampilAntrianPesanan() {
    if (antrianPesanan.empty()) {
        cout << "Tidak ada pesanan dalam antrian.\n";
        return;
    }

    queue<Transaksi> temp = antrianPesanan;
    cout << "\nAntrian Pesanan:\n";
    while (!temp.empty()) {
        Transaksi t = temp.front();
        cout << "Pelanggan: " << t.namaPelanggan
             << " | Menu: " << t.namaMenu
             << " | Jumlah: " << t.jumlah
             << " | Total: Rp " << t.totalHarga << endl;
        temp.pop();
    }
}

void prosesPesanan() {
    if (antrianPesanan.empty()) {
        cout << "Tidak ada pesanan yang dapat diproses.\n";
        return;
    }

    Transaksi t = antrianPesanan.front();
    antrianPesanan.pop();

    cout << "\nPesanan Diproses:\n";
    cout << "Pelanggan : " << t.namaPelanggan << endl;
    cout << "Menu      : " << t.namaMenu << endl;
    cout << "Jumlah    : " << t.jumlah << endl;
    cout << "Total     : Rp " << t.totalHarga << endl;
}

void tampilRiwayatPesanan() {
    if (riwayatPesananStack.empty()) {
        cout << "Belum ada pesanan.\n";
        return;
    }

    stack<Transaksi> temp = riwayatPesananStack;
    cout << "\nRiwayat Pesanan (Terbaru di atas):\n";
    while (!temp.empty()) {
        Transaksi t = temp.top();
        cout << "Pelanggan: " << t.namaPelanggan
             << " | Menu: " << t.namaMenu
             << " | Jumlah: " << t.jumlah
             << " | Total: Rp " << t.totalHarga << endl;
        temp.pop();
    }
}

void menuUtama() {
    int pilihan;
    do {
        cout << "\nSISTEM MANAJEMEN RESTORAN\n";
        cout << "1. Tambah Menu\n";
        cout << "2. Tampilkan Menu\n";
        cout << "3. Edit Menu\n";
        cout << "4. Tambah Pesanan\n";
        cout << "5. Tampilkan Antrian Pesanan\n";
        cout << "6. Proses Pesanan\n";
        cout << "7. Tampilkan Riwayat Pesanan\n";
        cout << "0. Keluar\n";
        cout << "Pilih: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: tambahMenu(); break;
            case 2: tampilMenu(); break;
            case 3: editMenu(); break;
            case 4: pesanMenu(); break;
            case 5: tampilAntrianPesanan(); break;
            case 6: prosesPesanan(); break;
            case 7: tampilRiwayatPesanan(); break;
            case 0: cout << "Terima kasih!\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }
    } while (pilihan != 0);
}

int main() {
    menuUtama();
    return 0;
}