package com.example.laberintosmart.models;

import java.util.Date;

public class Registro {

    private Date time;
    private long duration;
    private boolean resuelto;
    private Estado estado;
    private long tiempoInicio;
    private long tiempoFin;

    public Registro (){
        time = new Date();
        tiempoInicio = System.currentTimeMillis();
    }

    public Date getTime() {
        return time;
    }

    public void setTime(Date time) {
        this.time = time;
    }

    public long getDuration() {
        return duration;
    }

    public void setDuration(long duration) {
        this.duration = duration;
    }

    public boolean isResuelto() {
        return resuelto;
    }

    public void setResuelto(boolean resuelto) {
        this.resuelto = resuelto;
    }

    public Estado getEstado() {
        return estado;
    }

    public void setEstado(Estado estado) {
        this.estado = estado;
    }

    public long calcularDuracion(){
        tiempoFin = System.currentTimeMillis();
        duration = (tiempoFin - tiempoInicio) /1000;
        return duration;
    }

    public boolean guardarRegistro(){
        return true;
    }
}
