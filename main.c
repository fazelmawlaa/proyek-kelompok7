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
    printf("Masukkan nama pengguna : ");
    scanf("%s", pengguna.username);
    printf("Masukkan kata sandi    : ");
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

// Fungsi untuk login pengguna
int login(const char *username, const char *password) {
    User pengguna;
    FILE *login;

    // Baca data pengguna dari file biner
    login = fopen("database/login.bin", "rb");
    if (login == NULL) {
        printf("File login tidak ditemukan! Silakan registrasi terlebih dahulu.\n");
        exit(1);
    }

    while (fread(&pengguna, sizeof(User), 1, login)) {
        if (strcmp(pengguna.username, username) == 0 && strcmp(pengguna.password, password) == 0) {
            fclose(login);
            return 1; // Login berhasil
        }
    }
    fclose(login);
    return 0; // Login gagal
}


// Fungsi untuk menjalankan sistem (program) kuis
void jalankanKuis(Soal *kuis, int jumlahSoal, int nilaiHadiah) {
    float uang = 0;
    char jawaban;
    char konfirmasi;
    char status = 'L' ;

    for (int i = 0; i < jumlahSoal; i++) {
        printf("%d. %s\n", i + 1, kuis[i].pertanyaan);
        printf("A. %s\nB. %s\nC. %s\nD. %s\n", kuis[i].pilihan[0], kuis[i].pilihan[1], kuis[i].pilihan[2], kuis[i].pilihan[3]);
        printf("Jawaban Anda: ");
        scanf(" %c", &jawaban);

        if (jawaban == kuis[i].jawabanBenar || jawaban == kuis[i].jawabanBenar + 32) {
            uang += nilaiHadiah;
            printf("Jawaban benar! Uang Anda sekarang: Rp %.3f\n\n", uang);

            // Jika ini bukan pertanyaan terakhir, tanyakan konfirmasi
            if (i < jumlahSoal - 1) {
                printf("Apakah Anda ingin melanjutkan ke pertanyaan berikutnya? (Y/N): ");
                scanf(" %c", &konfirmasi);

 // Jika ini bukan pertanyaan terakhir, tanyakan konfirmasi
            if (i < jumlahSoal - 1) {
                 while (status == 'L') {
                printf("Apakah Anda ingin melanjutkan ke pertanyaan berikutnya? (Y/N): ");
                scanf(" %c", &konfirmasi);

               
                // Tambahkan baris kosong sebelum pertanyaan berikutnya
                if (konfirmasi == 'Y' || konfirmasi == 'y') {
                    status = 'T';
                    printf("\n");
                } else if (konfirmasi == 'N' || konfirmasi == 'n') {
                    printf("\nPermainan selesai. Uang yang Anda dapatkan: Rp %.3f\n", uang);
                    return;
                } else {
                 printf("Inputtan tidak Valid. Coba lagi\n\n");
                     }
                }
            }
            }
        } else {
            printf("\nJawaban salah! Permainan selesai. Uang anda telah hangus.\n");
            return;
        }
    }
    printf("\nSelamat! Anda berhasil menyelesaikan kuis dan mendapatkan uang senilai: Rp %.3f\n", uang);
}


// Fungsi untuk memulai kuis
void mulaiKuis() {
int diff;
    int nilaiHadiah;
    printf("\n===============================\n");
    printf("    PILIH TINGKAT KESULITAN \n");
    printf("===============================\n");
    printf("1. Easy (Rp 500.000)\n");
    printf("2. Normal (Rp 800.000)\n");
    printf("3. Hard (Rp 1.000.000)\n");
    printf("4. Very hard (2.000.000)\n");

    // Validasi input tingkat kesulitan
    while (1) {
        printf("Silahkan pilih tingkat kesulitan (1-4): ");
        scanf("%d", &diff);
        if (diff >= 1 && diff <= 4) break;
        printf("Input tidak valid, coba lagi.\n");
        getchar();
    } 
   // Pertanyaan berdasarkan tingkat kesulitan
    if (diff == 1) {
        nilaiHadiah = 100;
        printf("\n===============================\n");
        printf("          Game Dimulai\n");
        printf("===============================\n");
       Soal kuisEasy[] = {
            {"Gunung tertinggi di dunia adalah?", {"Gunung Everest", "Gunung Kilimanjaro", "Gunung Elbrus", "Gunung Denali"}, 'A'},
            {"2 x 3 + 4 x 5 = ?", {"26", "28", "20", "30"}, 'A'},
            {"Palung terdalam di dunia adalah?", {"Palung Weber", "Palung Tonga", "Palung Filipina", "Palung Mariana"}, 'D'},
            {"Nama samudra terkecil di dunia adalah?", {"Samudra Pasifik", "Samudra Hindia", "Samudra Arktik", "Samudra Atlantik"}, 'C'},
            {"Manusia tercepat di dunia adalah?", {"Usain Bolt", "Mo Farah", "Michael Phelps", "Carl Lewis"}, 'A'}
        };
        jalankanKuis(kuisEasy, sizeof(kuisEasy) / sizeof(kuisEasy[0]), nilaiHadiah);

    } else if (diff == 2) {
        nilaiHadiah = 160;
        printf("\n===============================\n");
        printf("          Game Dimulai\n");
        printf("===============================\n");
        Soal kuisMedium[] = {
            {"Siapa penemu teori relativitas?", {"Isaac Newton", "Albert Einstein", "Galileo Galilei", "Marie Curie"}, 'B'},
            {"Pulau terbesar di dunia adalah?", {"Greenland", "Kalimantan", "Madagaskar", "Australia"}, 'A'},
            {"10 tahun sama dengan satu...?", {"Abad", "Windu", "Dasawarsa", "Triwindu"}, 'C'},
            {"Berapakah 4/2 + 8/4?", {"4", "3", "2", "5"}, 'A'},
            {"Siapa presiden yang mampu menekan nilai dollar dari Rp 16.500 menjadi 6.500?", {"Soekarno", "Megawati", "Donald Trump", "BJ Habibie"}, 'D'}
        };
        jalankanKuis(kuisMedium, sizeof(kuisMedium) / sizeof(kuisMedium[0]), nilaiHadiah);
    }
    else if (diff == 3) {
        nilaiHadiah = 200;
        printf("\n===============================\n");
        printf("          Game Dimulai\n");
        printf("===============================\n");
        Soal kuisHard[] = {
           {"Zat yang digunakan pada AC untuk mendinginkan udara adalah?", {"Kondensor", "Kapiler", "Freon", "Evaporator"}, 'C'},
           {"The Harbour City (Kota Dermaga) adalah julukan untuk kota ?", {"Perth", "Melbourne", "Hobart", "Sidney"}, 'D'},
           {"Ibukota Kanada adalah?", {"Toronto", "Vancouver", "Ottawa", "Montreal"}, 'C'},
           {"Siapa yang berhak mengeluarkan Perppu di Indonesia?", {"Presiden" , "Mahkamah Konstitusi", "DPR", "KPK"}, 'A'},
           {"Berapakah 2/9 x 27/8 ?", {"4/3", "3/4", "3", "4"}, 'B'}
        };
        jalankanKuis(kuisHard, sizeof(kuisHard) / sizeof(kuisHard[0]), nilaiHadiah);

    } else if (diff == 4) {
        nilaiHadiah = 400;
        printf("\n===============================\n");
        printf("          Game Dimulai\n");
        printf("===============================\n");;
        Soal kuisVeryHard[] = {
            {"Pemenang FIFA World Cup tahun 2010 berasal dari negara?", {"Spanyol", "Brazil", "Jerman", "Indonesia"}, 'A'},
            {"Kerbau kecil yang terdapat di pedalaman Sulawesi disebut?", {"Surti", "Murrah", "Rawa", "Anoa"}, 'D'},
            {"Tongkat Biliard biasa disebut dengan?", {"Glove", "Trip Tool", "Kiu", "Cue"}, 'D'},
            {"Berapakah 1/4 + 1/5 + 1/6 ?", {"3/15", "3/120", "37/60", "42/60"}, 'C'},
            {"Alat untuk mengukur penguapan udara?", {"Barometer", "Atmometer", "Hygrometer", "Mikrometer"}, 'B'}
        };
        jalankanKuis(kuisVeryHard, sizeof(kuisVeryHard) / sizeof(kuisVeryHard[0]), nilaiHadiah);
    }
}

// Fungsi untuk menampilkan peraturan
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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        CekdanBuatFolder("database");
        printf("================================================\n");
        printf("    Selamat datang di Game Brain Billionare!\n");
        printf("================================================\n");
        printf("Cara memulai permainan:\n");
        printf("- Registrasi: ./namaProgram register\n");
        printf("- Login: ./namaProgram login username password\n");
        return 1;
    }

    if (strcmp(argv[1], "register") == 0) {
        registrasiPengguna();
        return 0;
    }

    if (argc == 4 && strcmp(argv[1], "login") == 0) {
        const char *username = argv[2];
        const char *password = argv[3];

        if (login(username, password)) {
            printf("Login berhasil! Selamat datang %s!\n", username);
            rules();
        
        } else {
            printf("Login gagal! Nama pengguna atau kata sandi salah.\n");
            printf("Cara penggunaan: ./namaProgram login username password\n");
            return 1;
        }
    }
    return 1;
}
