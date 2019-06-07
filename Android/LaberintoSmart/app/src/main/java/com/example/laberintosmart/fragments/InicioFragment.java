package com.example.laberintosmart.fragments;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.ParcelUuid;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

import com.example.laberintosmart.R;
import com.example.laberintosmart.activities.StartActivity;
import com.example.laberintosmart.adapters.BluetoothListAdapter;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.UUID;

import static android.app.Activity.RESULT_OK;
import static android.support.constraint.Constraints.TAG;

public class InicioFragment extends Fragment{

    private Button btnIniciar;
    private Button btnConnect;
    private Button btnTest;
    private BluetoothAdapter btAdapter;
    private List<BluetoothDevice> pairedDevices;
    private BluetoothDevice btDevice;
    private OutputStream outputStream;
    private InputStream inStream;
    private ListView listaDeDispositivos;
    private BluetoothListAdapter btListAdapter;
    Thread bluetoothThread;
    private boolean stopWorker;
    private int readBufferPosition;
    private byte[] readBuffer;

    public InicioFragment() {    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_home, container, false);

        //Bluetooth config and permission
        configBluetooth(view);

        return view;
    }


    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {

        //Inicio de Variables
        btnIniciar = view.findViewById(R.id.button_start);
        btnConnect = view.findViewById(R.id.button_connect);
        btnTest = view.findViewById(R.id.button_test);
        listaDeDispositivos = view.findViewById(R.id.listaDeDispositivos);
        //listaDeDispositivos.setOnItemClickListener(InicioFragment.this);

        //Fijacion de Listeners**************
        btnIniciar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Toast.makeText(InicioFragment.this.getContext(), "boton START clickeado", Toast.LENGTH_LONG).show();
                //Comprobar enlace con Arduino
                if(!comprobarEnlace()) {
                    Toast.makeText(InicioFragment.this.getContext(), "Auto no conectado", Toast.LENGTH_LONG).show();
                    return;
                }
                Intent intent = new Intent(view.getContext(), StartActivity.class);
                intent.putExtra("Prueba", "pruebaa");
                startActivity(intent);
            }
        });

        btnTest.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Toast.makeText(InicioFragment.this.getContext(), "boton TEST clickeado", Toast.LENGTH_LONG).show();
                if(!comprobarEnlace())
                    return;
                //pairedDevices = btAdapter.getBondedDevices();
            }
        });

        btnConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Toast.makeText(InicioFragment.this.getContext(), "boton Conectar clickeado", Toast.LENGTH_LONG).show();
                if(!comprobarEnlace())
                    return;

                pairedDevices= new ArrayList<>(btAdapter.getBondedDevices());
                btListAdapter = new BluetoothListAdapter(InicioFragment.this.getContext(),R.layout.list_bluetooth,pairedDevices);
                listaDeDispositivos.setAdapter(btListAdapter);

                if(pairedDevices.size() > 0)
                {
                    for(BluetoothDevice device : pairedDevices) {
                        if(device.getName().equals(BluetoothListAdapter.btCarName)) {
                            btDevice = device;
                            break;
                        }
                    }
                    if(btDevice == null) {
                        Toast.makeText(InicioFragment.this.getContext(), "Dispositivo no encontrado", Toast.LENGTH_LONG).show();
                        return;
                    }
                    Toast.makeText(InicioFragment.this.getContext(), "Dispositivo encontrado", Toast.LENGTH_LONG).show();

                    //conectarConDispositivo();
                    //beginListenForData();
                }

                //pairedDevices = new ArrayList<>();
                //if(!btAdapter.isDiscovering()){
                //    btAdapter.startDiscovery();
                //    IntentFilter discoverDevicesIntent = new IntentFilter(BluetoothDevice.ACTION_FOUND);
                //    getActivity().registerReceiver(mBroadcastReceiver3, discoverDevicesIntent);
                //} else btAdapter.cancelDiscovery();
            }
        });
        //*************************************
    }

    private void conectarConDispositivo() {
        if(btDevice==null)
            return;
        ParcelUuid[] uuid = btDevice.getUuids();
        try {
            BluetoothSocket btSocket = btDevice.createRfcommSocketToServiceRecord(uuid[0].getUuid());
            btSocket.connect();
            outputStream = btSocket.getOutputStream();
            inStream = btSocket.getInputStream();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }



    public boolean comprobarEnlace(){
        if(btAdapter.isEnabled())
            return true;
        else
            Toast.makeText(InicioFragment.this.getContext(), "Bluetooth no conectado", Toast.LENGTH_LONG).show();
        return false;
    }

    public void write(String s) throws IOException {
        outputStream.write(s.getBytes());
    }

    public void configBluetooth(View v){
        btAdapter = BluetoothAdapter.getDefaultAdapter();
        //Obtengo el adaptador
        if (!btAdapter.isEnabled()) {
            Intent turnOn = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(turnOn, 0);

        } else {
            Toast.makeText(InicioFragment.this.getContext(), "Already on", Toast.LENGTH_LONG).show();
        }
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        // Numero de request que enviamos nosotros
        if (requestCode == 0) {
            if (resultCode == RESULT_OK)
                Toast.makeText(InicioFragment.this.getContext(), "Turned on",Toast.LENGTH_LONG).show();
            else
                Toast.makeText(InicioFragment.this.getContext(), "Error al encender Bluetooth",Toast.LENGTH_LONG).show();
        }
    }


    void beginListenForData() {
        final Handler handler = new Handler();
        final byte delimiter = 10; //This is the ASCII code for a newline character

        stopWorker = false;
        readBufferPosition = 0;
        readBuffer = new byte[1024];
        bluetoothThread = new Thread(new Runnable()
        {
            public void run()
            {
                while(!Thread.currentThread().isInterrupted() && !stopWorker)
                {
                    try
                    {
                        int bytesAvailable = inStream.available();
                        if(bytesAvailable > 0)
                        {
                            byte[] packetBytes = new byte[bytesAvailable];
                            inStream.read(packetBytes);
                            for(int i=0;i<bytesAvailable;i++)
                            {
                                byte b = packetBytes[i];
                                if(b == delimiter)
                                {
                                    byte[] encodedBytes = new byte[readBufferPosition];
                                    System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length);
                                    final String data = new String(encodedBytes, "US-ASCII");
                                    readBufferPosition = 0;

                                    handler.post(new Runnable()
                                    {
                                        public void run()
                                        {
                                            //MANEJO DE LOS DATOS DE ENTRADA
                                        }
                                    });
                                }
                                else
                                {
                                    readBuffer[readBufferPosition++] = b;
                                }
                            }
                        }
                    }
                    catch (IOException ex)
                    {
                        stopWorker = true;
                    }
                }
            }
        });

        bluetoothThread.start();
    }

    void sendData(String msg) throws IOException {
        msg += "\n";
        outputStream.write(msg.getBytes());
    }
}
