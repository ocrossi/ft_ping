## RESSOURCES
https://www.binarytides.com/socket-programming-c-linux-tutorial/
genial pour comprendre la base d une comm en c client/server tcp

https://datatracker.ietf.org/doc/html/rfc791
ip protocol doc, search for "3.1.  Internet Header Format"
https://datatracker.ietf.org/doc/html/rfc792
icmp protocol

trckything is in error packets encapsulaton of ip protocol, icmp error message, then our packet
and finally in some cases a multi part extension depending on the machines encountered on network

try pinging yahoo.fr with the for at home see what the hell is going on
seems to work for youtube and google
https://man7.org/linux/man-pages/man7/signal-safety.7.html

dont print stats in signal handler, just manage a flag to stop flow of exec
check if PID matches with the sent one for concurrent ping calls
