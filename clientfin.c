/* cliTCP.c - Exemplu de client TCP 
   Trimite un nume la server; primeste de la server "Hello nume".
         
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009

*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>
#include <bits/stdc++.h>

using namespace std;

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

int main(int argc, char *argv[])
{

    if (argc != 4)
    {
        printf("[client] Sintaxa: %s <adresa_server> <port> <portPropriu>\n", argv[0]);
        return -1;
    }
    int pid=fork();
    if (pid==0)
    {
    char comm[100];
    bzero(comm,100);
    strcat(comm, "./sf ");
    strcat(comm, argv[3]);
    system(comm);
    return 0;
    }
    int sd;                    // descriptorul de socket
    struct sockaddr_in server; // structura folosita pentru conectare
    char msg[100];             // mesajul trimis

    /* exista toate argumentele in linia de comanda? */

    /* stabilim portul */
    port = atoi(argv[2]);

    /* cream socketul */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[client] Eroare la socket().\n");
        return errno;
    }

    /* umplem structura folosita pentru realizarea conexiunii cu serverul */
    /* familia socket-ului */
    server.sin_family = AF_INET;
    /* adresa IP a serverului */
    server.sin_addr.s_addr = inet_addr(argv[1]);
    /* portul de conectare */
    server.sin_port = htons(port);

    /* ne conectam la server */
    if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[client]Eroare la connect().\n");
        return errno;
    }

    while (1)
    {
        /* citirea mesajului */
        bzero(msg, 100);
        printf("[client]Introduceti o comanda spre server: ");
        fflush(stdout);
        read(0, msg, 95);
        strcat(msg, argv[3]);
        /*
        if (msg[0] == '0')
        {
            printf("[client]Deconectare...\n");
            break;
        }
        */
        /* trimiterea mesajului la server */
        if (write(sd, msg, 100) <= 0)
        {
            perror("[client]Eroare la write() spre server.\n");
            return errno;
        }

        /* citirea raspunsului dat de server 
     (apel blocant pina cind serverul raspunde) */
        if (read(sd, msg, 100) < 0)
        {
            perror("[client]Eroare la read() de la server.\n");
            return errno;
        }
        stringstream ss;
        ss<<msg;
        string s;
        ss>>s;
        if (s=="Got")
        {
            string nume, addr,port;
            ss>>nume>>addr>>port;
            char comm[100];
            bzero(comm,100);
            strcat(comm, "./cf ");
            strcat(comm, addr.c_str());
            strcat(comm, " ");
            strcat(comm, port.c_str());
            strcat(comm, " ");
            strcat(comm, nume.c_str());
            int pid2=fork();
            if (pid2==0)
            {
                system(comm);
            }
        }
        /* afisam mesajul primit */
        printf("[client]Mesajul primit este: %s\n", msg);
    }
    /* inchidem conexiunea, am terminat */
    close(sd);
    kill(pid, SIGKILL);
}