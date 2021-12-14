#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <readline/readline.h>

#define clear() printf("\033[H\033[J")
#define MAXKOMUT 80 // Desteklenen max komut karakter sayisi.
#define MAXKELIME 10 // Desteklenen max komut kelime sayisi.
void exit_from_console(){//Kabuktan cikis fonksiyonu.
    printf("exit\n");
    exit(EXIT_SUCCESS);
}
void printDirectory(){//Calistirilan kabugun dosya yolu.
    char pwd[1024];
    getcwd(pwd, sizeof(pwd));
    printf("\n%s ",pwd);
}
void forkOlusturma(char** parsed)// Cocuk olusturma
{
    pid_t pid = fork();
    int cpid;
    int epid;
    if (pid == -1) {//Fork islemi basarisiz.
        printf("\nFork basarisiz..Cocuk olustuluramadi.");
        return;
    } else if (pid == 0) {//Cocuk proses olusma durumu.
        if (execvp(parsed[0], parsed) < 0) {//execvp kullanimi.
            printf("\nCocuk proses olusturuldu..Bir komut atanmadi.\n");
            cpid=getpid();
            printf("\nCocuk pid->%d\n",cpid);
        }
        exit(0);
    } else {//Ebeveyn proses bekleme durumu
        epid=getpid();
         printf("\nEbeveyn pid->%d",epid);
        wait(NULL); 
        return;
    }
    
}
void showid(char** parsed)//proses id gosteren fonksiyon.(5 adet fork fonksiyonu cagirilir.)
{
    pid_t pid = fork();
    int ppid; 
  
    if (pid == -1) {
        return;
    } else if (pid == 0) {
        if (execvp(parsed[0], parsed) < 0) {
            ppid=getpid();//olusan prosesin id atamasi.
            printf("\n%d",ppid);
        }
        exit(0);
    } else {
        
        wait(NULL); 
        return;
    }
    
}
int komutlar(char** parsed)//Kabukta girilen komutun kontrolu ve fonksiyonlarin cagirilmasi.
{
    int KomutSayisi = 3, i, switchDegisken = 0;
        char* KomutListesi[KomutSayisi];
       
  
        KomutListesi[0] = "showpid";
        KomutListesi[1] = "cd";
        KomutListesi[2] = "fork()";
  
         for (i = 0; i < KomutSayisi; i++) {//Girilen komut ile komut listesini karsilastirarak ona uygun switchDegisken degerini dondurur.
            if (strcmp(parsed[0], KomutListesi[i]) == 0) {
                switchDegisken = i + 1;
                break;
            }
            else
            {
                switchDegisken=4;
            }
            
        }
  
        switch (switchDegisken) {
        case 1:
            for(int i=0;i<5;i++)
            {
                showid(parsed);
            }
        case 2:
            chdir(parsed[1]);
            return 1;

        case 3:
           forkOlusturma(parsed);
           
            return 1;
        case 4:
           printf("Hatali Komut Girdisi");
           
            return 1;
        default:
            break;
        }
  
        return 0;
}
void boslukAyiklama(char* str,char ** parsed)//Girilen komutlarin bosluklara gore kelime dizilerine aktarilmasi.
{
    if(strcmp(str,"exit") == 0){
        exit_from_console();
    }else
    {

        int i;
  
        for (i = 0; i < MAXKELIME; i++) 
        {
            parsed[i] = strsep(&str, " ");//strsep fonksiyonu ile komut girdisini bosluklara ayiripip diziye aktarilmasi.

            if (parsed[i] == NULL)//parcalanmis fonksiyondan gelen deger bos ise fonksiyon sonlanir.
              break;
            if (strlen(parsed[i]) == 0)//parcalanmis diziden gelen degerin uzunlugu sifira esit ise i degerini bir azaltir.
                i--;
        }
     
        komutlar(parsed);
 
    }
}
void check_commands(char *input,char** parsed){//bos komut kontrolu yapan fonksiyon.
    if(strlen(input)!=0) boslukAyiklama(input,parsed);
    else{
        printf("Bos komut!");
    }
}
void take_input(char* inpt,char** parsed){//input degerini alan fonksiyon
    printDirectory();
    char* prompt = "\033[1;31m ~~ SAU > \033[0m";
    inpt = readline(prompt);//girilen komutun okuma islemi.
    check_commands(inpt,parsed);
}

int main(int argc, char* argv[]){
    char komutGirdisi[MAXKOMUT];
    char* parseKelime[MAXKELIME];
    while(1){
        take_input(komutGirdisi,parseKelime);
    }
    return 0;
}