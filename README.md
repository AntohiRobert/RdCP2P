sudo apt-get install libsqlite3-dev
g++ servfin.c -o s -l sqlite3
g++ serverfile.c -o sf
g++ clientfin.c -o c
g++ clientfile.c -o cf

./s
./c <serv_ip> <serv_port> <free_local_port>
//servport=2728 usually

commands: 
share file.txt ---------------------shares file.txt
search len l ---------------------- searches files with len l
search like fi% -------------------- searches for files that begin with fi
get file.txt ----------------------- tries to get file.txt from network
