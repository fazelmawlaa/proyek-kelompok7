#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <direct.h> 
    #include <io.h> 
#else
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h> 
#endif

// Inisial untuk menyimpan data pengguna
typedef struct {
    char username[20];
    char password[20];
} User;

// Inisial untuk menyimpan data soal
typedef struct {
    char pertanyaan[200];
    char pilihan[4][50];
    char jawabanBenar;
} Soal;

// Fungsi cek dan buat folder
void CekdanBuatFolder(const char *namaFolder) {
    // Mengecek apakah direktori sudah ada
    #ifdef _WIN32 // Untuk windows
    if (_access(namaFolder, 0) == 0) {
        printf("Direktori '%s' sudah ada.\n", namaFolder);
    } else {
        // Jika direktori tidak ada, buat direktori baru
        if (_mkdir(namaFolder) == 0) {
            printf("Direktori '%s' berhasil dibuat.\n", namaFolder);
        } else {
            perror("Gagal membuat direktori");
            exit(1);
        }
    }
    #else // Untuk linux atau mac
        if (access(namaFolder, 0) == 0) {
        printf("Direktori '%s' sudah ada.\n", namaFolder);
    } else {
        // Jika direktori tidak ada, buat direktori baru
        if (mkdir(namaFolder, 0755) == 0) {
            printf("Direktori '%s' berhasil dibuat.\n", namaFolder);
        } else {
            perror("Gagal membuat direktori");
            exit(1);
        }
    }
    #endif
    
// Membuat file login.bin jika belum ada
    FILE *file = fopen("database/login.bin", "wb"); // "wb" membuat file jika belum ada
    if (file == NULL) {
        printf("Gagal membuat file login.bin.\n");
        exit(1);
    }
    fclose(file);
}

// Fungsi untuk registrasi pengguna
void registrasiPengguna() {
    User pengguna;
    FILE *regis;

    printf("===== Registrasi Pengguna =====\n");
    printf("Masukkan nama pengguna: ");
    scanf("%s", pengguna.username);
    printf("Masukkan kata sandi: ");
    scanf("%s", pengguna.password);

    // Simpan data pengguna ke file biner
    regis = fopen("database/login.bin", "ab");
    if (regis == NULL) {
        printf("Gagal membuka file untuk registrasi.\n");
        exit(1);
    }
    fwrite(&pengguna, sizeof(User), 1, regis);
    fclose(regis);

    printf("Registrasi berhasil! Silakan login.\n");
}
