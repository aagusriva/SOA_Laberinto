package com.example.laberintosmart.models;

import android.content.Context;
import android.support.annotation.NonNull;
import android.util.Log;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import static android.content.Context.MODE_PRIVATE;

public class Registro {

    private Date date;
    private String dateString;
    private long duration;
    private boolean resuelto;
    private Estado estado;
    private long tiempoInicio;
    private long tiempoFin;

    public Registro (){
        date = new Date();
        dateString = dateToString();
        tiempoInicio = System.currentTimeMillis();
    }

    public Registro(long duration, boolean resuelto, Estado estado) {
        this.date = new Date();
        dateString = dateToString();
        this.duration = duration;
        this.resuelto = resuelto;
        this.estado = estado;
    }

    public Registro(String date, String estado, String duration) {
        dateString = date;
        this.duration = Long.parseLong(duration);
        switch (estado){
            case "BLOCKED": this.estado=Estado.BLOCKED;break;
            case "TERMINATED": this.estado=Estado.TERMINATED;break;
            case "SUCCESS": this.estado=Estado.SUCCESS;break;
        }
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

    public String getDateString() {
        return dateString;
    }

    public void setDateString(String dateString) {
        this.dateString = dateString;
    }

    public long calcularDuracion(){
        tiempoFin = System.currentTimeMillis();
        duration = (tiempoFin - tiempoInicio) /1000;
        return duration;
    }



    public boolean guardarRegistro(Context context){
        try {
            FileOutputStream fileOutputStream = context.openFileOutput("SmartlabRecords.txt", Context.MODE_APPEND);
            fileOutputStream.write(this.toString().getBytes());
            fileOutputStream.close();

            //PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter("SmartlabRecords.txt", true)));
            //out.println(this.toString().getBytes());
            //out.close();

            Toast.makeText(context, "Text Saved", Toast.LENGTH_SHORT).show();
            return true;
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return false;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }

    public static List<Registro> getRecords(@NonNull Context context) {
        try {
            List<Registro> ls = new ArrayList<>();
            FileInputStream fileInputStream = context.openFileInput("SmartlabRecords.txt");
            InputStreamReader inputStreamReader = new InputStreamReader(fileInputStream);

            BufferedReader bufferedReader = new BufferedReader(inputStreamReader);

            String line;
            while ((line = bufferedReader.readLine()) != null) {
                String[] lineSplitted = line.split("&");
                ls.add(new Registro(lineSplitted[0],lineSplitted[1],lineSplitted[2]));
            }
            return ls;

        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return new ArrayList<Registro>();
        } catch (IOException e) {
            e.printStackTrace();
            return new ArrayList<Registro>();
        }
    }

    public String dateToString(){
        if(this.date == null)
            return "";
        String dateFormat = this.date.toString();
        return dateFormat.split("G")[0] + dateFormat.substring(dateFormat.length()-5,dateFormat.length());
    }

    @Override
    public String toString(){
        Log.v("TAG",this.date.toString() + "&" + this.estado + "&" + this.duration);
        return this.dateString + "&" + this.estado + "&" + this.duration + "\n";
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
