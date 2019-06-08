package com.example.laberintosmart.adapters;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.os.ParcelUuid;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import com.example.laberintosmart.activities.MainActivity;
import com.example.laberintosmart.activities.StartActivity;
import com.example.laberintosmart.fragments.InicioFragment;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;


public class BluetoothClient extends Thread {
    private BluetoothAdapter adapter;
    private BluetoothDevice device;
    private BluetoothSocket socket;
    private InputStream inputStream;
    private OutputStream outputStream;
    private Context context;


    public BluetoothClient(BluetoothDevice dev, BluetoothAdapter adapter,Context context){
        this.context = context;
        this.device = dev;
        this.adapter = adapter;
        try{
            socket=device.createRfcommSocketToServiceRecord(device.getUuids()[0].getUuid());
        } catch(IOException e) {e.printStackTrace();}
    }

    @Override
    public void run(){
        try{
            socket.connect();
            outputStream = socket.getOutputStream();
            inputStream = socket.getInputStream();
            write("F");
            read();
        }catch(IOException e){
            e.printStackTrace();
        }
    }

    public void write(String s){
        try {
            if(adapter.isEnabled())
                outputStream.write(s.getBytes());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void read(){
        byte[] buffer = new byte[1024];  // buffer store para el stream

        int bytes; // bytes retornados desde read()
        String incomingMessage;

        // se queda escuchando al InputStream hasta que una excepcion ocurre
        while (true) {
            // lee del InputStream
            try {
                bytes = inputStream.read(buffer);
                incomingMessage = new String(buffer, 0, bytes);
                if(bytes > 0) {
                    Toast.makeText(context, "Mensaje: " + incomingMessage, Toast.LENGTH_LONG).show();
                }
                //Intent incomingMessageIntent = new Intent("incomingMessage");
                //incomingMessageIntent.putExtra("theMessage", incomingMessage);
                //LocalBroadcastManager.getInstance(mContext).sendBroadcast(incomingMessageIntent);

            } catch (IOException e) {
                //Log.e(TAG, "write: Error leyendo Input Stream. " + e.getMessage() );
                break;
            }
        }
    }

}
