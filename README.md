# TheGuardianJourney
## [Repositorio GitHub](https://github.com/ColoroGit/TheGuardianJourney)

Este programa fue elaborado por Tomás Concha, como solución a la problematica entregada por el profesor Manuel Moscoso, para el proyecto de la segunda unidad del curso Algoritmos y Estructiras de Datos. 

Si desea ver un explicativo audiovisual de este proyecto, puede acceder a él haciendo click [aquí](https://drive.google.com/file/d/1Irm4pL8jCJzmij1eMHkC-QGGNB2poaio/view?usp=sharing) 

La problemática en sí era aplicar los contenidos vistos en clase, que abarcan la comprensión y aplicación de Tipos de Datos Abstractos, el uso de árboles y grafos y sus algorimtos asociados (Busqueda, Inserción, Eliminación, etc.) para la implementación de soluciones, entre otros contenidos.

Para llevar acabo este proceso, el contexto de la problemática termina siendo el desarrollo de un juego en el idioma C++, que involucre el uso de los conocimientos adqueridos. 
El contexto del juego es que en este mundo devastado por la guerra existen los Guardianes; héroes y heroínas que protegen los últimos asentamientos humanos que son las villas, dispersas por el mundo. Pero para convertirse en Guardián hay que llegar a la ciudad de Tesla, y enfrentarse a Stormheart, mas el largo viaje desde que te conviertes en aprendiz hasta llegar a dar cara a Stormheart está lleno de aventuras y de aprendizaje. 
En esta última parte se enfoca el juego, en la aventura de un aprendiz, interpretado por el jugador, quien recorre el mundo para convertirse en Guardián.

El producto computacional en si se basa en la carga de dos archivos, Guardians.txt y Villages.txt, los cuales definen los personajes con los que se encontrara el jugador, y los lugares que podrá visitar. Uno, si lo desea, puede modificar la información que viene dentro de estos archivos, pero hay que tener cuidado a la hora de escribirlos, teniendo en cuenta las siguientes reglas:

Para el archivo Villages.txt:
 - La primera línea que se escribe es la ciudad en la que se encuentra el jefe final, por defecto viene escrito `Tesla`
 - Las siguientes líneas son dos columnas separadas por una `,`, y cada línea representa una conexión entre las dos villas escritas en ella. Un ejemplo es `Desert Village,Coast Village`. Esto, cuando el programa lo lea, significa que va a crear dos ciudades con los nombres entregados, y formará un camino directo entre una y otra, de tal forma que el jugador, cuando decida Viajar estando parado en la primera o segunda, podrá llegar a la segunda o primera.
 - Si se desea crear una conexión entre una villa nueva, y una que ya haya aparecido, simplemente hay que seguir el mismo formato. Siguiendo el ejemplo anterior, si ahora se quiere crear una conexión entre `Desert Village`, que ya existe, con una nueva llamada `River Village`, simplemente hay que ingresar como una única línea `Desert Village,River Village`.

Para el archivo Guardians.txt:
 - La primera línea que se escriba será el nombre del Jefe Final, por defecto viene escrito `Stormheart`
 - Luego, las líneas de abajo vienen separadas en columnas por una `,`, y cada línea es un Guardián a cargar, por ejemplo `Cyrus,84,Stormheart,Desert Village`
 - El primer texto antes de la primera coma se traduce en el nombre del Guardián; el segundo texto es su Nivel de Poder, que se recomienda sea un número entre 100 y 50; el tercer texto es el nombre de su maestro. En este punto, para que todo funcione bien, hay que asegurarse que el maestro haya sido declarado en líneas anteriores. En el caso de `Cyrus`, Stormheart debe haber sido declarado en alguna línea superior; por último, el cuarto texto indica la villa a la que el Guardián pertenece, que tambien debe existir dentro del archivo Villages.txt para que todo funcione bien.

Posterior a la carga de archivos está la creación o selección de personajes, donde se podrán ingresar el nombre y la villa a la que desea pertenecer el personaje creado, o seleccionar a uno de los personajes cargados, quien será reemplazado por Rex, un Guardián interno[^1].

Finalmente, ya con un personaje, comienza la experiencia de juego, donde despues de un pequeño texto introductorio, el personaje comienza en su villa de origen, y se le dan las siguientes acciones para elegir:

 - Entrenar con un Aprendiz: Al seleccionar esta opción, se recomendará al Guardián que tenga un menor nivel de poder, pero de todas formas se mostrarán todas sus opciones a elegir. Una vez seleccionado el oponente comenzará la batalla, que estará decidida por el lanzamiento de un dado de 6 caras. Para salir vencedor, el jugador deberá obtener un número mayor a 2. Si se gana el combate, el aprendiz ganará 1 Punto de Poder. Aún así, independiente del resultado, se creará un registro del combate realizado.
 - Entrenar con *nombre*, Maestro/a de la Villa: Al seleccionar esta opción se pasa directamente al combate contra el Guardián con un mayor Nivel de Poder dentro de la villa, y en este caso, al lanzar el dado, necesitas por lo menos un 5 para ganar. Si se gana, se obtienen 2 Puntos de Poder, y también se crea un registro del combate independiente del resultado.
 - Viajar: Al Viajar se muestra en pantalla las villas que esten directamente conectadas a la que se encuentra el jugador actualmente, y solo hay que ingresar el nombre a la que se desea ir. Cada vez que se viaja, se obtiene 1 Punto de Poder.
 - Usar Alquímia: Esta opción te permite crear un camino entre la ciudad actual y cualquier otra ciudad con la que no exista conexión directa, pero para hacerlo es necesario gastar entre 2 y 4 Puntos de Poder, lo cual se decide aleatoriamente.
 - Ver tu Recorrido: Esta opción muestra todas las villas por las que el jugador haya pasado desde que comenzó el juego.
 - Ver tu Historial de Entrenamiento: Esta opción muestra los resgistros de todos los combates realizados por el jugador.

Se hace una distinción en este menú de selección si el jugador se encuentra en la ciudad del jefe final, desapareciendo la opcion de entrenar con aprendices, ya que no deberían haber (incluso si se cargan Guardianes de tal forma que pertenezcan a la ciudad del jefe, estos no van a aparecer), pero apareciendo la opcion para enfrentarse al jefe final, pero esto solo será posible si se cumple 1 de las siguientes 2 condiciones:
 - Haber pasado por todas las villas del juego
 - Tener un Nivel de Poder mayor o igual a 90
Llegados a ese punto, se mostrará un vistazo de lo que sería el combate entre el aprendiz del jugador y el jefe final, lo cual es el final del juego. Se te da la opción de ver el recorrido y el historial de combate del jugador antes de que el juego se cierre. 

Con eso se llega al final de este README. Disfrute de la experiencia de juego y revise el archivo INSTALL para poder compilar y correr el juego en su consola de Windows. 

[^1]: En esta parte hay un Easter Egg interesante dentro del programa, si desea descubrirlo por su cuenta, deje de leer este subíndice. 
Cuando se muestra la lista de guardianes a seleccionar, no aparece el primero que se carga, quien vendría a ser el jefe del juego, 
pero si uno ingresa su nombre de todas formas para intentar seleccionarlo, este puede ser reemplazado por Rex, y ahora es él quien pasa a ser el jefe, 
mientras que el usuario jugará con quien era el jefe, pero ahora como aprendiz.
