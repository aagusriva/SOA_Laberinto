package com.example.laberintosmart.models;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class Registro {

    private Date date;
    private long duration;
    private boolean resuelto;
    private Estado estado;
    private long tiempoInicio;
    private long tiempoFin;

    public Registro (){
        date = new Date();
        tiempoInicio = System.currentTimeMillis();
    }

    public Registro(long duration, boolean resuelto, Estado estado) {
        this.date = new Date();
        this.duration = duration;
        this.resuelto = resuelto;
        this.estado = estado;
    }

    public Date getDate() {
        return date;
    }

    public void setDate(Date date) {
        this.date = date;
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

    public String dateToString(){
        if(this.date == null)
            return "";
        String dateFormat = this.date.toString();
        return dateFormat.split("G")[0] + dateFormat.substring(dateFormat.length()-5,dateFormat.length());
    }

    public static List<Registro> getExampleData(){
        List<Registro> ls = new ArrayList<Registro>();
        ls.add(new Registro(10, true, Estado.SUCCESS));
        ls.add(new Registro(15, false, Estado.TERMINATED));
        ls.add(new Registro(20, false, Estado.BLOCKED));
        ls.add(new Registro(10, true, Estado.SUCCESS));
        ls.add(new Registro(15, false, Estado.TERMINATED));
        ls.add(new Registro(20, false, Estado.BLOCKED));
        ls.add(new Registro(10, true, Estado.SUCCESS));
        ls.add(new Registro(15, false, Estado.TERMINATED));
        ls.add(new Registro(20, false, Estado.BLOCKED));
        return ls;
    }
}
