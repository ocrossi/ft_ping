## RESSOURCES
https://www.binarytides.com/socket-programming-c-linux-tutorial/
genial pour comprendre la base d une comm en c client/server tcp

https://datatracker.ietf.org/doc/html/rfc791
ip protocol doc, search for "3.1.  Internet Header Format"

# Desole, petites differences au niveau timeout (qui n'est pas set a 1sec par defaut comme dans inetutils-2)
# Le crash lors du timeout et reponse vide.
# verbose header
affichage debug des packets en cas d'erreur (par ex. ttl=1 et ip distante) 
# time total qui n'existe pas dans l'implementation officielle sans flag. 

signal a changer en sigalarm
return values 0 1 2
erreur de display bizarre quand pas de paquets recus
ping -c 1 --ttl 4 -v youtube.com
