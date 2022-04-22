# isa

Use cases:

$ ./popcl pop3.seznam.cz -p 995 -T -a authfile123 -o zkusebnidir/ -n
Downloaded 1 new mail(s).

$ ./popcl pop3.seznam.cz -p 110 -T -a authfile123 -o zkusebnidir/ -n
Connection failed.

$ ./popcl pop3.seznam.cz -p 110 -T -a authfile123 -o 
Error at argument 7 (-o).

Na serveru merlin dojde k SIGSEGV, podezrivam radek SSL\_CTX  *ctx = SSL_CTX_new(SSLv23_client_method());

