package com.example.laberintosmart.models;

public class Directiva {

    //Ordenes

    //Modo manual
    public static final String CONTINUAR = "A";
    public static final String DERECHA = "D";
    public static final String IZQUIERDA = "I";

    //FRENAR MOMENTANEAMENTE
    public static final String FRENAR = "P";

    public static final String DETENERSE = "F";
    //public static final String ARRANCAR = "A"; No se usa
    public static final String ENCENDER_LED = "B";
    public static final String APAGAR_LED = "T";
    public static final String INICIAR_MANUAL = "M";
    public static final String INICIAR_AUTOMATICO = "S";
    public static final String OPTIMIZAR_RECORRIDO = "O";
    public static final String APRENDER_RECORRIDO = "L";

    //Tipos de transmiciones
    public static final String RESUELTO = "F";
    public static final String BLOQUEADO = "P";
    public static final String NO_BLOQUEADO = "J";
}
