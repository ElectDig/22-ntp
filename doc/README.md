

![Austral Ingenieria](https://encrypted-tbn0.gstatic.com/images?q=tbn%3AANd9GcQooGo7vQn4t9-6Bt46qZF-UY4_QFpYOeh7kVWzwpr_lbLr5wka)

<!--
    Ésta es la forma de poner comentarios en MarkDown
-->

# Proyecto 22-ntp

## Secciones

- [Objetivo](#objetivo)
- [Programa](#programa)
- [Constantes](#constantes)
- [Sobre Acceso a WiFi](#wifi)


## Objetivo

  El objetivo primordial de este proyecto es demostrar la capacidad de acceder a un servidor de hora (_NTP Server_).  
  Se accede a través de él para recabar datos de fecha y hora actuales.  
  Estos servidores se encuentran distribuídos a lo largo del mundo y, en el caso de Argentina, la dirección es _ar.pool.ntp.org_.  

  Usar con cualquier placa que use los procesadores con ESP32

## Programa

Para el acceso al protocolo correspodiente, debe usarse una biblioteca, que se denomina ___NTPClient___; el acceso a Internet se realiza sobre _UDP_, por lo cual deben crearse una objeto _ntpUDP_; además, y para poder acceder al servidor, se debe crear otro objeto denominado _timeClient_.

En las líneas 50 y 51 se encuentra el respectivo código:
~~~ 
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER, TZ(TIME_ZONE) );
~~~

En resumen, se deben definir las siguientes constantes, de acuerdo a lo que se muestra en las líneas 17 a 22

~~~ 
/*
 *  Definitions included  in platformio.ini
 * 
 *      TIME_ZONE   Time zone in hours
 *      NTP_SERVER  NTP server name
 *      SERIAL_BAUD Serial port Baud rate
 */
 ~~~

Las constantes _TIME\_ZONE_ y _NTP\_SERVER_ son usadas en la creación de _timeClient_; _TIME\_ZONE_ indica la cantidad de husos horarios respecto del meridiano de Greenwich en que se encuentra el país en cuestión (Argentina está tres horas atrasado respecto de él, por lo cual corresponde -3).

A su vez, la inicialización del objeto _timeClient_ se realiza en la línea 62:

~~~ 
    timeClient.begin();
~~~

Luego de la inicialización en la función _setup()_, la acción, como siempre, se desarrolla en la función _loop()_; en efecto, cada segundo se accede al servidor NTP y se recaba la fecha, mostrando posteriormente los resultados.

Para obtener los valores actuales de fecha y hora del servidor NTP, se llama a la actualización del _cliente_ en la línea 71 mediante:

~~~ 
    timeClient.update();
~~~

Para obtener el resultado, se lee del servidor el tiempo que se denomina _epoch time_ que no es otra cosa que la cantidad de segundos transcurridos desde las 0 horas del primero de Enero de 1970 en el meridiano de Greenwich pero corregidos de acuerdo a la zona horaria previamente declarada; este valor ocupa 4 bytes codificados como numero entero y de él, mediante cálculos, se pueden obtener todos los datos necesarios para la ubicación de la fecha y hora actuales, obviamente al segundo de precisión.

Si bien estos cálculos no son difíciles de realizarlos mediante un programa en C, hay bibliotecas _standard_ de C (que provienen de Unix), que se pueden usar en Arduino y que convenientemente, solo hay que incluir _<time.h>_, como se ha realizado en el programa en la línea 36.

Sería conveniente que el lector consulte los siguientes _links_ para tener la correcta especificación de esta biblioteca así como los detalles de la función de biblioteca _strftime_:

 [time library](https://es.wikipedia.org/wiki/Time.h)
 [strftime function](http://man7.org/linux/man-pages/man3/strftime.3.html)

Lo codigos que realizan todo el trabajo se encuentran entre las lineas 73 a 75

---

## Constantes

La única biblioteca externa que se usa es la que permite el acceso al cliente NTP, que se repite aqui

~~~ ini
lib_deps =
    NTPCLient
~~~


Como se habrá observado, hay tres constantes que se definen en el archivo _platformio.ini_ para mayor comodidad de mantenimiento; de esa manera, no es necesario entrar en los archivos de código para dicho mantenimiento.

Estas constantes son comunes a ambos, por lo cual están definidas en _build_flags_;se copian más abajo

~~~ ini
build_flags =
     -DTIME_ZONE=-3
     -DNTP_SERVER=\"ar.pool.ntp.org\"
~~~

Obsérvese que, por definirse en una de ellas una constantexs _string_, debe escaparse el caracter ___"___ mediante el caracter barra invertida.

La otra constante que falta es _SERIAL\_BAUD_, lo cual lleva una disgresión: en este caso, lo importante es que esta constante, que se usa para inicializar por programa la interfase serie, esté en un todo de acuerdo con la definición de _hardware_ de la interfase que se entrega por _monitor\_speed_ en este archivo.

Ello se logra para la inicialización de _hardware_ mediante:

~~~ ini
monitor_speed = 115200
~~~

Bastaría, por lo tanto, colocar bajo _build\_flags_:

~~~ ini
build_flags =
    -DSERIAL_BAUD=115200
~~~

pero eso significaría que se debería estar atento a que los números que se colocasen en ambas líneas _fuesen exactamente iguales_.

Sería, entonces, mejor vincular automáticamente ambos lo cual se logra en la línea recabando el valor de el ítem _monitor\_speed_ de la misma sección _env_; en definitiva, podemos aquí mostrar el _env_ correspondiente a _ESP32_ que se encuentra entre las líneas 26 y 32

~~~ ini
....
monitor_speed = 115200
....
build_flags =
    -DSERIAL_BAUD=${env:nodemcu-32s.monitor_speed}
~~~

>Claro, la pregunta que queda es ¿porqué se ha elegido tal _baud\_rate_ no normalizado para este caso? La respuesta es que, no sabemos porqué, cuando _nace_ después de un _reset_ este procesador, amanece con dicho _baud\_rate_; de esta manera, con este poco común _baud\_rate_ se podrán observar todos los caracteres que, a partir de _reset_, se envían a la interfase serie.

##    wifi

>Para acceso a WiFi simplificado usado en este proyecto, ver archivo _esp32wifi.md_ en este mismo directorio.

