# isa

Manual is written in Czech and contains some implementation information.

Simple POP3 client used for downloading unread mails, listing mail statistics and deleting mails. Supports insecure connection and SSL/TLS.




Use cases:

$ ./popcl pop3.seznam.cz -p 995 -T -a authfile123 -o zkusebnidir/ -n

Downloaded 1 new mail(s).



$ ./popcl pop3.seznam.cz -p 110 -T -a authfile123 -o zkusebnidir/ -n

Connection failed.



$ ./popcl pop3.seznam.cz -p 110 -T -a authfile123 -o 

Error at argument 7 (-o).


