# Informe Proyecto II - CI5437

Elaborado por:
Yerimar Manzo - 14-10611
Jorge Correia - 14-10254

## Árboles de Juego

El proyecto II se trata sobre el desarrollo de los algoritmos _negamax_, _negamax con poda $\alpha$-$\beta$_, _scout_ y _negascout_. Estos algoritmos permiten ubicar soluciones óptimas para árboles de juego. Para este proyecto se realizó el estudio sobre una versión modificada del juego Othello. 

### Descripción breve de Othello/Reversi

Othello o Reversi es un juego para dos jugadores, con el objetivo de que el jugador que tenga más fichas en el tablero al terminar el juego gana. El tablero tiene tamaño 8x8, en el cual se distribuyen fichas de color negro y blanco. Los pasos del juego y movimientos válidos consisten en que cada jugador debe colocar su ficha contigua a la de su oponente sea en posición horizontal, vertical o diagonal. Una vez el jugador ubica su ficha, si existe una ficha de su color en el extremo opuesto de la horizontal, vertical o diagonal donde puso su ficha entonces todas las fichas del oponente pasan a ser suyas. 

Este proyecto emplea una variación del juego Othello con un tablero 6x6, donde se representa el tablero con la variación principal, de la cual de acuerdo a Feinstein (1993), si ambos jugadores juegan de forma eficiente el juego termina con una victoria del jugador blanco.

### Implementación

Se completó el juego con la verificación en las diagonales, así como el código que realiza _outflank_ en las mismas. Para los algoritmos se siguió el pseudocódigo dado en clases.

### Experimentación

Se ejecutaron los cuatro algoritmos por 20 minutos o hasta que fallaran por falta de recursos.

#### Características de Hardware
- Ubuntu 22.04 LTS, Intel Core i7-9750H @ 2.60GHz, 16GB de RAM
- Windows Subsystem for Linux con Ubuntu 22.04, Intel Core i5-11400F @ 2.60GHz, 16GB de RAM
### Resultados

Para cada algoritmo, se registró:
 - Estado de la variación principal
 - Color que está jugando
 - Valor del estado
 - Nodos expandidos
 - Nodos generados
 - Tiempo
 - Cantidad de nodos generados por segundo

#### Negamax

|PV|Color|Valor|Expandidos|Generados|Tiempo|Generados/s
|---|---|---|---|---|---|---|
|34|White|-4|0|1|1.00001e-06|999992|
|33|Black|-4|1|2|2.00002e-06|999992|
|32|White|-4|4|5|2.00002e-06|2.49998e+06|
|31|Black|-4|4|6|1.99996e-06|3.00006e+06|
|30|White|-4|11|13|3.00002e-06|4.3333e+06|
|29|Black|-4|12|14|3.99997e-06|3.50002e+06|
|28|White|-4|84|91|2.1e-05|4.33334e+06|
|27|Black|-4|162|177|3.4e-05|5.20589e+06|
|26|White|-4|934|1049|0.000219|4.78995e+06|
|25|Black|-4|4030|4498|0.000914|4.92123e+06|
|24|White|-4|10475|11978|0.002461|4.86713e+06|
|23|Black|-4|66687|76826|0.016049|4.78696e+06|
|22|White|-4|377424|428402|0.084275|5.08338e+06|
|21|Black|-4|3015556|3478735|0.681018|5.10814e+06|
|20|White|-4|11321961|13078933|2.55426|5.12044e+06|
|19|Black|-4|78915639|90647895|17.5217|5.17345e+06|
|18|White|-4|767689687|876269598|171.228|5.11757e+06|

El algoritmo _negamax_ es el más ineficiente, debido a que genera una gran cantidad de nodos, por lo que debe realizar más operaciones sobre todos ellos. Además, llegó hasta el PV 18, cuando otros algoritmos llegan más lejos.

#### Negamax con poda $\alpha$-$\beta$

|PV|Color|Valor|Expandidos|Generados|Tiempo|Generados/s
|---|---|---|---|---|---|---|
|34|White|-4|0|1|1.00001e-06|999992|
|33|Black|-4|1|2|2.00002e-06|999992|
|32|White|-4|4|5|2.00002e-06|2.49998e+06|
|31|Black|-4|4|6|1.00001e-06|5.99995e+06|
|30|White|-4|11|13|3.99991e-06|3.25007e+06|
|29|Black|-4|12|14|3.99991e-06|3.50008e+06|
|28|White|-4|23|27|6.99994e-06|3.85718e+06|
|27|Black|-4|74|82|2e-05|4.09999e+06|
|26|White|-4|210|238|6.4e-05|3.71875e+06|
|25|Black|-4|894|1003|0.000281|3.5694e+06|
|24|White|-4|1338|1502|0.00038|3.95263e+06|
|23|Black|-4|3578|4068|0.001109|3.66817e+06|
|22|White|-4|8095|9130|0.002271|4.02026e+06|
|21|Black|-4|87283|98755|0.024661|4.0045e+06|
|20|White|-4|112358|127644|0.032289|3.95317e+06|
|19|Black|-4|234268|267604|0.065736|4.07089e+06|
|18|White|-4|1124390|1259430|0.31087|4.05131e+06|
|17|Black|-4|1811274|2031924|0.504272|4.02942e+06|
|16|White|-4|26557311|29501798|6.92155|4.26231e+06|
|15|Black|-4|39439080|43574643|10.5374|4.13524e+06|
|14|White|-4|97517568|107642871|26.1461|4.11697e+06|
|13|Black|-4|378528257|415909956|97.7141|4.2564e+06|
|12|White|-4|2661793153|2931981147|673.959|4.35039e+06|

_Negamax con poda_ lógicamente tiene un mejor desempeño que el algoritmo _Negamax_, ya que explora menos nodos, lo cual permite que avance más en el PV. Se observa que llega al PV 12. En el caso del PV 13 hay 415.909.956 nodos generados, mientras que el _Negamax_ logró 876.269.598 para el PV 18, pero esto en un menor tiempo fue logrado por _negamax con poda_. La eficiencia es notoria.

#### Scout

|PV|Color|Valor|Expandidos|Generados|Tiempo|Generados/s|
|---|---|---|---|---|---|---|
|34|White|4|0|1|0|inf|
|33|Black|-4|1|2|0|inf|
|32|White|4|4|3|0|inf|
|31|Black|-4|4|4|0|inf|
|30|White|4|15|11|0|inf|
|29|Black|-4|16|12|0|inf|
|28|White|4|27|13|0|inf|
|27|Black|-4|73|14|0|inf|
|26|White|4|343|52|0|inf|
|25|Black|-4|1548|192|0.003054|62868.4|
|24|White|4|2302|249|0.003878|64208.4|
|23|Black|-4|3650|306|0.007611|40205|
|22|White|4|11601|727|0.016657|43645.3|
|21|Black|-4|48013|1909|0.028588|66776.3|
|20|White|4|78294|2250|0.035582|63234.2|
|19|Black|-4|178362|2692|0.077457|34754.8|
|18|White|4|573183|6294|0.248947|25282.5|
|17|Black|-4|1049780|7155|0.457953|15623.9|
|16|White|4|7198650|43606|2.92437|14911.3|
|15|Black|-4|27913174|91903|11.3831|8073.63|
|14|White|4|67478633|136919|26.7748|5113.72|
|13|Black|-4|252021206|295252|94.1932|3134.54|
|12|White|4|645683085|888835|245.833|3615.61|

#### Negascout

|PV|Color|Valor|Expandidos|Generados|Tiempo|Generados/s
|---|---|---|---|---|---|---|
|34|White|-4|0|1|1.00001e-06|999992|
|33|Black|-4|1|2|2.00002e-06|999992|
|32|White|-4|4|5|2.99991e-06|1.66672e+06|
|31|Black|-4|4|6|2.00002e-06|2.99998e+06|
|30|White|-4|15|20|6.99994e-06|2.85717e+06|
|29|Black|-4|16|21|6.00005e-06|3.49997e+06|
|28|White|-4|27|34|1.10001e-05|3.09089e+06|
|27|Black|-4|73|84|2.7e-05|3.11111e+06|
|26|White|-4|341|398|0.000125|3.184e+06|
|25|Black|-4|1481|1668|0.000515|3.23883e+06|
|24|White|-4|2176|2465|0.000878|2.80752e+06|
|23|Black|-4|3451|3898|0.001307|2.9824e+06|
|22|White|-4|10604|12067|0.004385|2.75188e+06|
|21|Black|-4|43134|48674|0.0188|2.58904e+06|
|20|White|-4|72267|81826|0.029573|2.76692e+06|
|19|Black|-4|161808|184361|0.067463|2.73277e+06|
|18|White|-4|537218|606378|0.221732|2.73473e+06|
|17|Black|-4|1003078|1134797|0.40091|2.83055e+06|
|16|White|-4|6523174|7223328|2.53376|2.85084e+06|
|15|Black|-4|23385092|25833440|9.04221|2.85698e+06|
|14|White|-4|56338645|62053925|21.8351|2.84194e+06|
|13|Black|-4|221377402|242589301|82.4484|2.94232e+06|
|12|White|-4|565087412|623019805|214.172|2.90896e+06

Los algoritmos Scout y Negascout fueron los que se desempeñaron mejor, llegando hasta el PV 12, así como generando una cantidad similar de nodos.

### Conclusiones

Podemos concluir que los mejores algoritmos para este juego son Scout y Negascout. Debido a la verificación realizada, Scout es muy bueno, de forma de que se use como función de heurística para no expandir nodos innecesarios (pruning).

Negascout aprovecha también las ventajas de Scout para explorar. Sin embargo, se requiere un orden para ello.

El más eficiente en cantidad de nodos generados fue Scout. En cantidad de nodos expandidos, fue Negascout.