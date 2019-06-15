package com.example.laberintosmart.activities;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Handler;
import android.os.ParcelUuid;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.example.laberintosmart.R;
import com.example.laberintosmart.adapters.BluetoothListAdapter;
import com.example.laberintosmart.models.Estado;
import com.example.laberintosmart.models.Registro;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;

public class StartActivity extends AppCompatActivity {

    private Registro registro;
    private Button finalizar;
    private TextView txtModo;
    private TextView txtEstado;
    private BluetoothAdapter btAdapter;
    private List<BluetoothDevice> pairedDevices;
    private BluetoothDevice btDevice;
    private OutputStream outputStream;
    private InputStream inStream;
    private BluetoothSocket btSocket;
    final int handlerState = 0;
    private StringBuilder recDataString = new StringBuilder();
    Handler bluetoothIn;
    BTListener btListener;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_start);

        finalizar = (Button) findViewById(R.id.start_end);
        txtModo = findViewById(R.id.txtModo);
        txtEstado = findViewById(R.id.start_state);

        bluetoothIn = Handler_Msg_Hilo_Principal();

        crearRegistro();
    }

    @Override
    protected void onStart() {
        super.onStart();

        boolean modoManual = (boolean)getIntent().getExtras().get("modoManual");

        if(modoManual)
            txtModo.setText("MANUAL");
        else
            txtModo.setText("AUTOMÁTICO");

        finalizar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                terminarRegistro();
                Intent intent = new Intent(getApplicationContext(), MainActivity.class);
                intent.putExtra("Registro", "registro");
                startActivity(intent);
            }
        });


        configBluetooth();
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    public void crearRegistro(){
        registro = new Registro();
    }

    public void terminarRegistro(){
        registro.calcularDuracion();
        registro.setEstado(Estado.TERMINATED);
        registro.guardarRegistro(getApplicationContext());
    }

    public void configBluetooth(){
        btAdapter = BluetoothAdapter.getDefaultAdapter();
        pairedDevices= new ArrayList<>(btAdapter.getBondedDevices());
        for(BluetoothDevice device : pairedDevices) {
            if(device.getName().equals(BluetoothListAdapter.btCarName)) {
                btDevice = device;
                break;
            }
        }
        if(btDevice==null)
            return;
        ParcelUuid[] uuid = btDevice.getUuids();
        try {
            btSocket = btDevice.createRfcommSocketToServiceRecord(uuid[0].getUuid());
            btSocket.connect();
            outputStream = btSocket.getOutputStream();
            inStream = btSocket.getInputStream();
            btListener = new BTListener(inStream, outputStream);
            btListener.start();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    protected void onStop() {
        super.onStop();
        Toast.makeText(StartActivity.this, "ON STOP", Toast.LENGTH_LONG).show();
        if(btSocket != null) {
            try {
                btSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    public void outterWrite(String s){
        try {
            outputStream.write(s.getBytes());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private Handler Handler_Msg_Hilo_Principal ()
    {
        return new Handler() {
            public void handleMessage(android.os.Message msg)
            {
                //si se recibio un msj del hilo secundario
                if (msg.what == handlerState)
                {
                    String readMessage = (String) msg.obj;

                    if(readMessage.equals("F")) {
                        txtEstado.setText("Resuelto");
                        outterWrite("Finalizamos recorrido");
                    }

                }
            }
        };

    }


    private class BTListener extends Thread
    {
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        //Constructor de la clase del hilo secundario
        public BTListener(InputStream is, OutputStream os)
        {
            mmInStream = is;
            mmOutStream = os;
        }

        //metodo run del hilo, que va a entrar en una espera activa para recibir los msjs del HC05
        public void run()
        {
            byte[] buffer = new byte[256];
            int bytes;

            //el hilo secundario se queda esperando mensajes del HC05
            while (true)
            {
                try
                {
                    //se leen los datos del Bluethoot
                    bytes = mmInStream.read(buffer);
                    String readMessage = new String(buffer, 0, bytes);

                    //se muestran en el layout de la activity, utilizando el handler del hilo
                    // principal antes mencionado
                    bluetoothIn.obtainMessage(handlerState, bytes, -1, readMessage).sendToTarget();
                } catch (IOException e) {
                    break;
                }
            }
        }


        //write method
        public void write(String input) {
            byte[] msgBuffer = input.getBytes();           //converts entered String into bytes
            try {
                mmOutStream.write(msgBuffer);                //write bytes over BT connection via outstream
            } catch (IOException e) {
                //if you cannot write, close the application
                finish();

            }
        }
    }

}
