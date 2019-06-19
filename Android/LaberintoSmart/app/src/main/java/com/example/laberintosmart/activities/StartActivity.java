package com.example.laberintosmart.activities;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Handler;
import android.os.ParcelUuid;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Chronometer;
import android.widget.TextView;
import android.widget.Toast;

import com.example.laberintosmart.R;
import com.example.laberintosmart.adapters.BluetoothListAdapter;
import com.example.laberintosmart.models.Directiva;
import com.example.laberintosmart.models.Estado;
import com.example.laberintosmart.models.Registro;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;

public class StartActivity extends AppCompatActivity implements SensorEventListener{

    private Registro registro;

    //Componentes UI
    private Button finalizar;
    private Button btnHome;
    private TextView txtModo;
    private TextView txtEstado;
    private boolean modoManual;
    private Chronometer myChronometer;

    //Bluetooth connection
    private BluetoothAdapter btAdapter;
    private List<BluetoothDevice> pairedDevices;
    private BluetoothDevice btDevice;
    private OutputStream outputStream;
    private InputStream inStream;
    private BluetoothSocket btSocket;

    //Bluetooth messaging
    final int handlerState = 0;
    private StringBuilder recDataString = new StringBuilder();
    Handler bluetoothIn;
    BTListener btListener;

    //Sensors
    SensorManager sensorManager;
    Sensor sensorLuz;
    Sensor sensorProx;
    Sensor sensorAcelerometro;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_start);

        //Componentes UI
        finalizar = (Button) findViewById(R.id.start_end);
        btnHome = findViewById(R.id.btn_start_home);
        btnHome.setVisibility(View.GONE);
        btnHome.setActivated(false);
        txtModo = findViewById(R.id.txtModo);
        txtEstado = findViewById(R.id.start_state);
        myChronometer = (Chronometer)findViewById(R.id.chronometer);


        //BT MSG
        bluetoothIn = Handler_Msg_Hilo_Principal();

        //Sensores
        sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        sensorLuz = sensorManager.getDefaultSensor(Sensor.TYPE_LIGHT);
        sensorProx = sensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY);
        sensorAcelerometro = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        if( sensorLuz == null || sensorProx == null || sensorAcelerometro == null)
            finish();

    }

    @Override
    protected void onStart() {
        super.onStart();

        modoManual = (boolean)getIntent().getExtras().get("modoManual");

        configBluetooth();

        setModoDeEjecucion();
        txtEstado.setText("RESOLVIENDO");

        sensorManager.registerListener(this, sensorLuz, SensorManager.SENSOR_DELAY_NORMAL);
        sensorManager.registerListener(this, sensorProx, SensorManager.SENSOR_DELAY_NORMAL);
        sensorManager.registerListener(this, sensorAcelerometro, SensorManager.SENSOR_DELAY_UI);

        myChronometer.start();

        finalizar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                terminarRegistro(Estado.TERMINATED);
                Intent intent = new Intent(getApplicationContext(), MainActivity.class);
                //intent.putExtra("Registro", "registro");
                startActivity(intent);
            }
        });

        btnHome.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getApplicationContext(), MainActivity.class);
                startActivity(intent);
            }
        });

        crearRegistro();
    }

    public void crearRegistro(){
        registro = new Registro();
    }

    public void terminarRegistro(Estado estado){
        myChronometer.stop();
        outerWrite(Directiva.FRENAR);
        registro.calcularDuracion();
        registro.setEstado(estado);
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

    public void setModoDeEjecucion(){
        if(modoManual) {
            txtModo.setText("MANUAL");
            outerWrite(Directiva.INICIAR_MANUAL);
        }
        else {
            txtModo.setText("AUTOMÁTICO");
            outerWrite(Directiva.INICIAR_AUTOMATICO);
        }
    }

    @Override
    protected void onStop() {
        super.onStop();

        sensorManager.unregisterListener(this, sensorProx);
        sensorManager.unregisterListener(this, sensorLuz);
        sensorManager.unregisterListener(this, sensorAcelerometro);

        myChronometer.setBase(SystemClock.elapsedRealtime());

        if(btSocket != null) {
            try {
                btSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void outerWrite(String s){
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

                    //SI ARDUINO NO MANDA OTRA SENAL MAS QUE ESTAS DOS SE PUEDE PONER EL CODIGO DE BOTONES APARTE
                    if(readMessage.equals(Directiva.RESUELTO)) {
                        txtEstado.setText("RESUELTO");
                        finalizar.setVisibility(View.GONE);
                        finalizar.setActivated(false);
                        btnHome.setVisibility(View.VISIBLE);
                        btnHome.setActivated(true);
                        terminarRegistro(Estado.SUCCESS);
                    }   else if(readMessage.equals(Directiva.BLOQUEADO)){
                        txtEstado.setText("BLOQUEADO");
                        finalizar.setVisibility(View.GONE);
                        finalizar.setActivated(false);
                        btnHome.setVisibility(View.VISIBLE);
                        btnHome.setActivated(true);
                        terminarRegistro(Estado.BLOCKED);
                    }
                }
            }
        };

    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        switch(event.sensor.getType()){
            case Sensor.TYPE_LIGHT: onLightChange(event);break;
            case Sensor.TYPE_PROXIMITY: onProximityChange(event);;break;
            case Sensor.TYPE_ACCELEROMETER: onAccelerometerChange(event);break;
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }

    public void onLightChange(SensorEvent event){

    }

    public void onProximityChange(SensorEvent event){
        float x = event.values[0];
        if( x < sensorProx.getMaximumRange() - 3) {
            txtEstado.setText("FRENADO");
            outerWrite(Directiva.FRENAR);
        }else{
            txtEstado.setText("RESOLVIENDO");
            outerWrite(Directiva.ARRANCAR);
        }
    }

    public void onAccelerometerChange(SensorEvent event){
        if(!modoManual)
            return;
        float x = event.values[0];
        float z = event.values[2];
        if(x<-6) {
            outerWrite(Directiva.DERECHA);
        }else if(x>6){
            outerWrite(Directiva.IZQUIERDA);
        }else if( z >= 6){
            outerWrite(Directiva.CONTINUAR);
        }
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
