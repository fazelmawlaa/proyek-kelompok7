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

void rules() {
    int rule;

    printf("\n===================================================\n");
    printf("      SELAMAT DATANG DI GAME BRAIN BILLIONARE       \n");
    printf("===================================================\n");

    printf("Peraturan permainan: \n");
    printf("1. Terdapat 5 soal yang jumlah hadiahnya berbeda tergantung tingkat kesulitan.\n");
    printf("2. Tingkat kesulitan dan prizepool:\n");
    printf("   Easy\t\tRp 500.000\n");
    printf("   Normal\tRp 800.000\n");
    printf("   Hard\t\tRp 1.000.000\n");
    printf("   Very Hard\tRp 2.000.000\n");
    printf("3. Game akan berakhir jika soal dijawab salah dan uang yang anda kumpulkan akan hangus.\n");
    printf("\nApakah Anda ingin memulai permainan?\n");
    printf("1. GAZZZ\n2. SKIP DULU DEH\n\nJawaban Anda: ");
    scanf("%d", &rule);

    if (rule == 1) {
        mulaiKuis();
    } else {
        printf("Baik, terima kasih sudah berpartisipasi\n");
    }
}
