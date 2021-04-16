#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <direct.h> 
#include <time.h>
#include <math.h>

#define MAX 30
#define ENTRY_MAX 1024

typedef struct database{
    char nik[MAX];
    char nama[MAX];
    char tempat_lahir[MAX];
    char tanggal_lahir[MAX];
    int umur;
    char jen_kel[MAX];
    char gol_dar[MAX];
    char status[MAX];
    char pekerjaan[MAX];
} data ;

typedef struct date_sys{
    int day;
    int month;
    int year;
} date ;

char filename[128];

//Deklarasi Fungsi
void red();
void yellow();
void blue();
void reset();
void title();

date initDate();

int menuSelect();
int countAge(data id[], date today, int index);
int countEntry(data id[]);

void readData (data id[], int index_num);
void saveData(data id[], date today);
void editData(data id[], date today);
void insertData(data id[]);
void deleteData(data id[], date today);
void showData(data id[], date today);
void showSingleData(data id[], date today, int i);
void searchData(data id[], date today);
void sortSelection(data id[]);
void sortData(data id[], int sort_by);


//Implementasi Fungsi

int main() { 
    //Deklarasi 
    int option = 0;
    data id[ENTRY_MAX];
    date today;

    //Inialisasi
    for (int i = 0; i < ENTRY_MAX; i++){
        id[i].nama[0] = 0;
    }
    yellow();
    title();
    today = initDate(); 
    while (option != 9){
        //system("cls");
        blue();
        option = menuSelect();
        reset();
        puts("");
        switch(option){
            case 1:
                insertData(id);
                break;
            case 2:
                readData(id, countEntry(id));
                break;
            case 3:
                showData(id, today);
                break;
            case 4:
                editData(id, today);
                break;
            case 5:
                deleteData(id, today);
                break;
            case 6:
                searchData(id, today);
                break;
            case 7:
                sortSelection(id);
                break;
            case 8:
                saveData(id, today);
                break;
            case 9:
                printf("Shutting Down...");
                break;
            default:
                puts("nani?");
                break;
        }
    }
    
}

/**/

//aesthetics
void red(){
    //printf("\033[1;31m");
    
}

void yellow(){
    //printf("\033[1;33m");
}

void blue(){
    //printf("\033[1;34m");
}

void reset(){
    //printf("\033[0m");
}


void title(){
    puts(" ___        _        _                   ___              _         _       _   ");
    puts("| . \\ ___ _| |_ ___ | |_  ___  ___ ___  | . \\ ___ ._ _  _| | _ _  _| | _ _ | |__");
    puts("| | |<_> | | | <_> || . \\<_> |<_-</ ._> |  _// ._>| ' |/ . || | |/ . || | || / /");
    puts("|___/<___| |_| <___||___/<___|/__/\\___. |_|  \\___.|_|_|\\___|`___|\\___|`___||_\\_\\\n");
}

//logic
int menuSelect(){
    int option;
    int valid = 0;

    puts("\nMenu :");
    puts("1. Input Data Manual");
    puts("2. Input Data dari File Eksternal");
    puts("3. Tampilkan Database");
    puts("4. Edit Data");
    puts("5. Hapus Data"); 
    puts("6. Temukan Data");
    puts("7. Urutkan Data");
    puts("8. Simpan Data ke File Eksternal");
    puts("9. Keluar");
    
    while (valid == 0){
        printf(">>> ");
        scanf("%d", &option);
        if (option >= 1 && option <= 9){
            valid = 1;
        }
        else{
            puts("Pilihan belum tersedia, silahkan coba lagi");
        }
    }

    return option;
}

date initDate(){
    date today;

    time_t now;
	time(&now);
	struct tm *local = localtime(&now);

    today.day = local->tm_mday;        	// get day of month (1 to 31)
    today.month = local->tm_mon + 1;   	// get month of year (0 to 11)
    today.year = local->tm_year + 1900;	// get year 
    
    printf("Tanggal hari ini : %02d/%02d/%d\n", today.day, today.month, today.year);
    
    return today;
}

int countAge(data id[], date today, int i){
    int count;
    int age;
    date dob;       //date of birth

    count = countEntry(id);

    dob.day = ((id[i].tanggal_lahir[0] - 48) * 10) + (id[i].tanggal_lahir[1] - 48); 
    dob.month = ((id[i].tanggal_lahir[2] - 48) * 10) + (id[i].tanggal_lahir[3] - 48); 
    dob.year = ((id[i].tanggal_lahir[4] - 48) * 1000) + ((id[i].tanggal_lahir[5] - 48) * 100) + ((id[i].tanggal_lahir[6] - 48) * 10) + (id[i].tanggal_lahir[7] - 48); 
    age = today.year - dob.year;

    if (dob.month == today.month){
        if (dob.day > today.day){
            age--;
        }
    } 
    else if (dob.month > today.month){
        age--;
    }
    id[i].umur = age;
    return age;
}

int countEntry(data id[]){
    int count = 0;

    for (int i = 0; i < ENTRY_MAX; i++){
        if (id[i].nama[0] == 0){
            break;
        }
        count++;
    }

    return count;
}

void readData(data id[], int index_num){
    FILE *fp;
    char str[1024];
    int i = 0;
    int valid = 0;
    char dump[64];

    while (valid == 0){
        printf("Masukka nama file (tanpa extension): ");
        scanf("%s", filename);
        strcat(filename, ".csv");
        fp = fopen(filename,"r");
        if (fp != NULL){
            valid = 1;
            printf("\nFile %s berhasil diload\n", filename);
        }
        else{
            puts("File tidak ditemukan, silahkan coba lagi");
        }
    }
    
    while(!feof(fp)){
        
        fgets(str, 1024, fp);
        if (i != 0){
            strcpy(dump, strtok(str, ";"));
            strcpy(id[i-1+index_num].nik, strtok(NULL, ";"));
            strcpy(id[i-1+index_num].nama, strtok(NULL, ";"));
            strcpy(id[i-1+index_num].tempat_lahir, strtok(NULL, ";"));
            strcpy(id[i-1+index_num].tanggal_lahir, strtok(NULL, ";"));
            strcpy(dump, strtok(NULL, ";"));
            strcpy(id[i-1+index_num].jen_kel, strtok(NULL, ";"));
            strcpy(id[i-1+index_num].gol_dar, strtok(NULL, ";"));
            strcpy(id[i-1+index_num].status, strtok(NULL, ";"));
            strcpy(id[i-1+index_num].pekerjaan, strtok(NULL, "\n"));
        }
        i++;
    }

    fclose(fp);
}

void saveData(data id[], date today){
    FILE *fp;
    int index;
    char choice;
    int valid = 0;
    char date[8];
    char day[2];
    char month[2];
    char year[4];

    puts("Overwrite File Input Terakhir (Y/N)?");
    while (valid == 0){
        printf(">>> ");
        scanf(" %c", &choice);
        if (choice == 'N' || choice == 'n'){
            printf("Masukkan nama File (tanpa extension): ");
            scanf("%s", filename);
            
            itoa(today.day, day, 10);
            itoa(today.month, month, 10);
            itoa(today.year, year, 10);
            if (today.day <= 9){
                date[0] = '0';
                date[1] = day[0];
            }
            else{
                date[0] = day[0];
                date[1] = day[1];
            }

            if (today.month <= 9){
                date[2] = '0';
                date[3] = today.month + 48;
            }
            else{
                date[2] = month[0];
                date[3] = month[1];
            }
            date[4] = year[0];
            date[5] = year[1];
            date[6] = year[2];
            date[7] = year[3];
            date[8] = '\0';
            strcat(filename, date);
            strcat(filename, ".csv");
            valid = 1;
        }
        else if (choice == 'Y' || choice == 'y'){
            valid = 1;
        }
        else{
            puts("Pilihan tidak tersedia, silahkan coba lagi\n");
        }
    }
    
    fp = fopen(filename, "w+");
    index = countEntry(id);

    for (int i = 0; i < index+1; i++){
        if (i == 0){
            fprintf(fp, "No;NIK;NamaLengkap;TempatLahir;TanggalLahir;Umur;JenisKelamin;GolonganDarah;Status;Pekerjaan\n");
        }
        else{
            fprintf(fp, "%d;%s;%s;%s;%s;%d;%s;%s;%s;%s",        i,  
                                                                id[i-1].nik,  
                                                                id[i-1].nama,
                                                                id[i-1].tempat_lahir,
                                                                id[i-1].tanggal_lahir,
                                                                id[i-1].umur,
                                                                id[i-1].jen_kel,
                                                                id[i-1].gol_dar,
                                                                id[i-1].status,
                                                                id[i-1].pekerjaan);
        }

        if (i < index && i != 0){
            fprintf(fp, "\n");
        }
    } 
    fclose(fp);
    printf("\nData berhasil tersimpan di csv dengan nama %s\n", filename);
}

void editData(data id[], date today){
    int index;
    char placeholder[MAX];
    showData(id, today);

    printf("Pilih indeks yang ingin diedit : ");
    scanf("%d", &index);
    index--;

    puts("\nSilahkan isi dengan '-' apabila tidak ingin melakukan perubahan");
    printf("NIK: ");
    scanf("%s", placeholder);
    if (placeholder[0] != '-'){
        strcpy(id[index].nik, placeholder);
    }

    printf("Nama: ");
    scanf(" %[^\n]%*c", placeholder);
    if (placeholder[0] != '-'){
        strcpy(id[index].nama, placeholder);
    }

    printf("Tempat Lahir: ");
    scanf(" %[^\n]%*c", placeholder);
    if (placeholder[0] != '-'){
        strcpy(id[index].tempat_lahir, placeholder);
    }
    
    printf("Tanggal Lahir: ");
    scanf("%s", placeholder);
    if (placeholder[0] != '-'){
        strcpy(id[index].tanggal_lahir, placeholder);
    }
    
    printf("Jenis Kelamin: ");
    scanf("%s", placeholder);
    if (placeholder[0] != '-'){
        strcpy(id[index].jen_kel, placeholder);
    }
    
    printf("Golongan Darah: ");
    scanf(" %[^\n]%*c", placeholder);
    if (placeholder[0] != '-'){
        strcpy(id[index].gol_dar, placeholder);
    }
    
    printf("Status: ");
    scanf(" %[^\n]%*c", placeholder);
    if (placeholder[0] != '-'){
        strcpy(id[index].status, placeholder);
    }
    
    printf("Pekerjaan: ");
    scanf(" %[^\n]%*c", placeholder);
    if (placeholder[0] != '-'){
        strcpy(id[index].pekerjaan, placeholder);
    }
    
    puts("Entry berhasil diperbaharui\n");
}

void insertData(data id[]){
    int ins = countEntry(id);
    int valid = 0;
    char choice;

    while (valid == 0){
        printf("NIK: ");
        scanf("%s", id[ins].nik);
        printf("Nama: ");
        scanf(" %[^\n]%*c", id[ins].nama);
        printf("Tempat Lahir: ");
        scanf(" %[^\n]%*c", id[ins].tempat_lahir);
        printf("Tanggal Lahir: ");
        scanf("%s", id[ins].tanggal_lahir);
        printf("Jenis Kelamin: ");
        scanf("%s", id[ins].jen_kel);
        printf("Golongan Darah: ");
        scanf(" %[^\n]%*c", id[ins].gol_dar);
        printf("Status: ");
        scanf(" %[^\n]%*c", id[ins].status);
        printf("Pekerjaan: ");
        scanf(" %[^\n]%*c", id[ins].pekerjaan);

        printf("\nMasukkan Data lagi (Y/N) ?\n>>> ");
        scanf(" %c", &choice);
        if (choice == 'N' || choice == 'n'){
            valid = 1;
            puts("\nData berhasil disimpan, silahkan menjalankan menu 3 untuk melihat perubahan");
        }
        ins++;
    }
    
}

void deleteData(data id[], date today){
    int index;
    int index_count;
    index_count = countEntry(id);

    showData(id, today);
    printf("Indeks yang ingin didelete : ");
    scanf("%d", &index);
    index--;
    printf("Data dengan nama %s berhasil dihapus\n", id[index].nama);
    for (int i = index ; i < index_count; i++){
        id[i] = id[i+1];
    }

    id[index_count].nama[0] = 0;
}

void showData(data id[], date today){
    int indexLength;
    int space_length;
    indexLength = countEntry(id);
    yellow();
    puts("Tabel Data Penduduk Pada Sistem");
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------------------");
    puts("| No\t | NIK        | Nama Lengkap         | Tempat Lahir         | Tanggal Lahir | Umur | Jenis Kelamin | Golongan Darah | Status      | Pekerjaan       |");
    //               10 fix      max 20                 max 20                   13            4     ------a------      max14          max 11(11/5)  max 15
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------------------");
    for (int i = 0; i < indexLength; i++){
        //No dan NIK
        printf("| %d\t | %s ", i+1, id[i].nik);

        //Nama
        printf("| %s", id[i].nama);
        space_length = strlen(id[i].nama);
        for (int j = 0; j < 21 - space_length; j++){    
            printf(" ");
        }

        //Tempat Lahir
        printf("| %s", id[i].tempat_lahir);
        space_length = strlen(id[i].tempat_lahir);
        for (int j = 0; j < 21 - space_length; j++){
            printf(" ");
        }

        //Tanggal Lahir, Umur, Jenis Kelamin
        printf("| %s      | %d   |       %s       ", id[i].tanggal_lahir, countAge(id, today, i), id[i].jen_kel);

        //Golongan Darah
        printf("| %s", id[i].gol_dar);
        space_length = strlen(id[i].gol_dar);
        for (int j = 0; j < 15 - space_length; j++){
            printf(" ");
        }

        //Status
        printf("| %s", id[i].status);
        space_length = strlen(id[i].status);
        for (int j = 0; j < 12 - space_length; j++){
            printf(" ");
        }
        
        //Pekerjaan
        printf("| %s", id[i].pekerjaan);
        space_length = strlen(id[i].pekerjaan);
        for (int j = 0; j < 16 - space_length; j++){
            printf(" ");
        }

        //New Line
        printf("|\n");
    }
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------------------");
    reset();
}

void showSingleData(data id[], date today, int i){
    int indexLength;
    int space_length;
    indexLength = countEntry(id); 
    //No dan NIK
    printf("| %d\t | %s ", i+1, id[i].nik);

    //Nama
    printf("| %s", id[i].nama);
    space_length = strlen(id[i].nama);
    for (int j = 0; j < 21 - space_length; j++){    
        printf(" ");
    }

    //Tempat Lahir
    printf("| %s", id[i].tempat_lahir);
    space_length = strlen(id[i].tempat_lahir);
    for (int j = 0; j < 21 - space_length; j++){
        printf(" ");
    }

    //Tanggal Lahir, Umur, Jenis Kelamin
    printf("| %s      | %d   |       %s       ", id[i].tanggal_lahir, countAge(id, today, i), id[i].jen_kel);

    //Golongan Darah
    printf("| %s", id[i].gol_dar);
    space_length = strlen(id[i].gol_dar);
    for (int j = 0; j < 15 - space_length; j++){
        printf(" ");
    }

    //Status
    printf("| %s", id[i].status);
    space_length = strlen(id[i].status);
    for (int j = 0; j < 12 - space_length; j++){
        printf(" ");
    }
    
    //Pekerjaan
    printf("| %s", id[i].pekerjaan);
    space_length = strlen(id[i].pekerjaan);
    for (int j = 0; j < 16 - space_length; j++){
        printf(" ");
    }
    //New Line
    printf("|\n");
    
    }

void searchData(data id[], date today){
    char key[15];
    char *result = NULL;
    int compare = 1;
    int mode;
    char age[2];
    int index;
    int isFound = 0;
    index = countEntry(id);

    puts("Mode Pencarian: ");
    puts("1. Pencarian di bagian NIK, Nama, Tempat Lahir, Tanggal Lahir, dan Pekerjaan");
    puts("2. Pencarian di bagian Umur, Jenis Kelamin, Golongan Darah, dan Status");
    puts("\n*ket: Opsi 1 pencarian di substring, Opsi 2 pecarian eksak");
    printf(">>> ");
    scanf("%d", &mode);

    printf("\nKeyword: ");
    scanf(" %[^\n]%*c", key);

    //Header tabel
    printf("\nTabel data dimana keyword %s ditemukan", key);
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------------------");
    puts("| No\t | NIK        | Nama Lengkap         | Tempat Lahir         | Tanggal Lahir | Umur | Jenis Kelamin | Golongan Darah | Status      | Pekerjaan       |");
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------------------");
 
    switch(mode){
        case 1:
            for (int i = 0; i < 9; i++){
                switch(i){
                    case 0:
                        for (int j = 0; j < index; j++){
                            result = strstr(id[j].nik, key);
                            if (result != NULL){
                                showSingleData(id, today, j);
                                result = NULL;
                                isFound = 1;
                            }
                        }
                        result = NULL;
                        break;
                    case 1:
                        for (int j = 0; j < index; j++){
                            result = strstr(id[j].nama, key);
                            if (result != NULL){
                                showSingleData(id, today, j);
                                result = NULL;
                                isFound = 1;
                            }
                        }
                        result = NULL;
                        break;
                    case 2:
                        for (int j = 0; j < index; j++){
                            result = strstr(id[j].tempat_lahir, key);
                            if (result != NULL){
                                showSingleData(id, today, j);
                                result = NULL;
                                isFound = 1;
                            }
                        }
                        result = NULL;
                        break;
                    case 3:
                        for (int j = 0; j < index; j++){
                            result = strstr(id[j].tanggal_lahir, key);
                            if (result != NULL){
                                showSingleData(id, today, j);
                                result = NULL;
                                isFound = 1;
                            }
                        }
                        result = NULL;
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                    case 6:
                        break;
                    case 7:
                        break;
                    case 8:
                        for (int j = 0; j < index; j++){
                            result = strstr(id[j].pekerjaan, key);
                            if (result != NULL){
                                showSingleData(id, today, j);
                                result = NULL;
                                isFound = 1;
                            }
                        }
                        result = NULL;
                        break;
                    default:
                        break;
                }
            } 
            break;
        case 2:
            for (int i = 0; i < 9; i++){
                switch(i){
                    case 0:
                        break;
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                    //umur
                        for (int j = 0; j < index; j++){
                            if (id[j].umur == atoi(key)){
                                showSingleData(id, today, j);
                                isFound = 1;
                            }
                        }
                        break;
                    case 5:
                    //jenkel
                        for (int j = 0; j < index; j++){
                            compare = strcmp(id[j].jen_kel, key);
                            if (compare == 0){
                                showSingleData(id, today, j);
                                compare = 1;
                                isFound = 1;
                            }
                        }
                        break;
                    case 6:
                    //goldar
                        for (int j = 0; j < index; j++){
                            compare = strcmp(id[j].gol_dar, key);
                            if (compare == 0){
                                showSingleData(id, today, j);
                                compare = 1;
                                isFound = 1;
                            }
                        }
                        break;
                    case 7:
                        for (int j = 0; j < index; j++){
                            compare = strcmp(id[j].status, key);
                            if (compare == 0){
                                showSingleData(id, today, j);
                                compare = 1;
                                isFound = 1;
                            }
                        }
                        result = NULL;
                        break;
                    case 8:
                        break;
                    default:
                        break;
                }
            } 
            break;
        default:
            puts("tidak tersedia");
            break;
    }

    if (isFound == 0){
            puts("========================================================================= NOT FOUND =========================================================================");
    }
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------------------");       
}

void sortSelection(data id[]){
    int option;
    int valid = 0;

    puts("Pengurutan berdasarkan: ");
    puts("1. NIK");
    puts("2. Nama Lengkap");
    puts("3. Tempat Lahir");
    puts("4. Tanggal Lahir");
    puts("5. Umur");
    puts("6. Jenis Kelamin");
    puts("7. Golongan Darah");
    puts("8. Status");
    puts("9. Pekerjaan");
    
    while (valid == 0){
        printf(">>> ");
        scanf("%d", &option);
        if (option >= 1 && option <= 9){
            valid = 1;
        }
    }
    
    sortData(id, option);

    printf("\nData berhasil diurutkan berdasarkan parameter ke-%d\n", option);
}

void sortData(data id[], int sort_by){
    //tes sort nama
    int size = countEntry(id);
    int comp;
    int isUmur = 0;

    for (int step = 0; step < size - 1; ++step) {
        for (int j = 0; j < size - step - 1; ++j) {
            switch(sort_by){
                case 1:
                    comp = strcmp(id[j].nik, id[j+1].nik);
                    break;
                case 2:
                    comp = strcmp(id[j].nama, id[j+1].nama);
                    break;
                case 3:
                    comp = strcmp(id[j].tempat_lahir, id[j+1].tempat_lahir);
                    break;
                case 4:
                    comp = strcmp(id[j].tanggal_lahir, id[j+1].tanggal_lahir);
                    break;
                case 5:
                    isUmur = 1;
                    if (id[j].umur > id[j+1].umur) {
                        //swap
                        data temp = id[j];
                        id[j] = id[j+1];
                        id[j+1]= temp;
                    }
                    break;
                case 6:
                    comp = strcmp(id[j].jen_kel, id[j+1].jen_kel);
                    break;
                case 7:
                    comp = strcmp(id[j].gol_dar, id[j+1].gol_dar);
                    break;
                case 8:
                    comp = strcmp(id[j].status, id[j+1].status);
                    break;
                case 9:
                    comp = strcmp(id[j].pekerjaan, id[j+1].pekerjaan);
                    break;
                default:
                    puts("invalid");
            }
            
            if (comp > 0 && isUmur == 0){
                //swap
                data temp = id[j];
                id[j] = id[j+1];
                id[j+1]= temp;
            }
        }
    }
}
