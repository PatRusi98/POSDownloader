#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

int noOfDownloads = -1;
char address1[500];
char address2[500];
char name1[50] = "download1.jpg";
char name2[50] = "download2.jpg";
CURL *curl1;
CURL *curl2;
FILE *fp1;
FILE *fp2;
int result;
pthread_t thread1;
pthread_t thread2;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct downloadStruct {
    bool first;
    bool oneFile;

    pthread_mutex_t * mutex;
}D;

void* downloader1(void * data) {
    D * dataD = data;

    char dAddress[500];
    char dName[50];

    if(dataD->first){
        strcpy(dAddress, address1);
        strcpy(dName, name1);
    } else {
        strcpy(dAddress, address2);
        strcpy(dName, name2);
    }

    fp1 = fopen(dName, "wb");

    curl1 = curl_easy_init();

    curl_easy_setopt(curl1, CURLOPT_URL, dAddress);
    curl_easy_setopt(curl1, CURLOPT_WRITEDATA, fp1);
    curl_easy_setopt(curl1, CURLOPT_FAILONERROR, 1L);

    printf("Downloading file 1...\n");

    result = curl_easy_perform(curl1);

    if(result == CURLE_OK) {
        printf("File 1 downloaded successfully!\n");
    } else {
        printf("Error: %s\n", curl_easy_strerror(result));
    }

    fclose(fp1);

    curl_easy_cleanup(curl1);

    return 0;
}

void* downloader2(void * data) {
    D * dataD = data;

    char dAddress[500];
    char dName[50];

    if(dataD->first){
        strcpy(dAddress, address1);
        strcpy(dName, name1);
    } else {
        strcpy(dAddress, address2);
        strcpy(dName, name2);
    }

    fp2 = fopen(dName, "wb");

    curl2 = curl_easy_init();

    curl_easy_setopt(curl2, CURLOPT_URL, dAddress);
    curl_easy_setopt(curl2, CURLOPT_WRITEDATA, fp2);
    curl_easy_setopt(curl2, CURLOPT_FAILONERROR, 1L);

    printf("Downloading file 2...\n");

    result = curl_easy_perform(curl2);

    if(result == CURLE_OK) {
        printf("File 2 downloaded successfully!\n");
    } else {
        printf("Error: %s\n", curl_easy_strerror(result));
    }

    fclose(fp2);

    curl_easy_cleanup(curl2);

    return 0;
}

void noOfDownloaded() {
    printf("Kolko suborov chces stiahnut?\n");
    printf("1.\t jeden\n");
    printf("2.\t dva\n");

    noOfDownloads = -1;

    do
    {
        printf("Your choice: ");
        scanf("%d", &noOfDownloads);
    } while (noOfDownloads != 1 && noOfDownloads != 2);
}

void download() {
    noOfDownloaded();

    if (noOfDownloads == 1)
    {
        strcpy(address1, "");

        printf("Zadaj adresu!\n");
        printf("Your choice: ");
        scanf("%s", &address1);

        printf("Zadaj meno suboru.\n");
        printf("Your choice: ");
        scanf("%s", &name1);

        D d = {true, true, &mutex};

        pthread_create(&thread1, NULL, downloader1, &d);
        pthread_join(thread1, NULL);

        pthread_mutex_destroy(&mutex);

    }
    else
    {
        strcpy(address1, "");
        strcpy(address2, "");

        printf("Zadaj prvu adresu (vyssia priorita)\n");
        printf("Your choice: ");
        scanf("%s", &address1);

        printf("Zadaj meno prveho suboru.\n");
        printf("Your choice: ");
        scanf("%s", &name1);

        printf("Zadaj druhu adresu (nizsia priorita)\n");
        printf("Your choice: ");
        scanf("%s", &address2);

        printf("Zadaj meno druheho suboru.\n");
        printf("Your choice: ");
        scanf("%s", &name2);

        D d1;
        D d2;

        d1.first = true;
        d1.oneFile = false;
        d1.mutex = &mutex;

        d2.first = false;
        d2.oneFile = false;
        d2.mutex = &mutex;

        pthread_create(&thread1, NULL, downloader1, &d1);
        pthread_create(&thread2, NULL, downloader2, &d2);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);

        pthread_mutex_destroy(&mutex);
    }
}

int main() {
    printf("                     DOWNLOAD MANAGER\n");
    printf("------------------------------------------------------------\n");
    printf("         created by Patrik Rusinak, Kevin Laurincik\n");
    printf("------------------------------------------------------------\n");

    download();

    return 0;
}