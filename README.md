# Arduino_Basic_Programs_Stepper
Stepper motor movement, programs to test with any driver
## Descripcion
1. Potenciometro -> Velocidad
Se tiene un driver A4988, TB6600, un motor paso a paso y un potenciometro, se busca que cuando se mueva el potenciometro se pueda controlar la velocidad del motor, aumentandola o disminuyendola en funcion de cuanto gira el potenciometro, 100% sera toda la velocidad, 0% sera el minimo de la velocidad.
2. Potencimoetro -> Velocidad, Gira tantos pasos (Pasos: Puerto serie), para (Tiempo: Puerto serie), y manda una señal, la cual debe tener un tiempo.
En este programa se va a enviar por puerto serie los pasos y el tiempo, la velocidad sera configurada con un potenciometro, de forma analoga al anterior ejercicio, el motor al tener todos los datos girara los pasos que le han configurado, espera el tiempo programado y por ultimo genera una señal de salida.
3. Potencimoetro -> Velocidad, (Pasos: Puerto serie), no se mueve hasta no volver a presionar, Oprime la primera va a la posicion y segunda y se regresa.
Se programa la velocidad con un potencometro y los pasos por el puerto serie, al presionar el boton por primera vez, el motor hace un recorrido y al presionar por segunda vez el motor va a regresar a la posicion original.
## Autor
* **Miguel A. Califa U.** - [*GitHub*](https://github.com/miguel5612) - [CV](https://scienti.colciencias.gov.co/cvlac/visualizador/generarCurriculoCv.do?cod_rh=0000050477)
