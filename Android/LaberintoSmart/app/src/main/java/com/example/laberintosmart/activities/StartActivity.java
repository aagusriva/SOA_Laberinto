package com.example.laberintosmart.activities;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.example.laberintosmart.R;
import com.example.laberintosmart.models.Estado;
import com.example.laberintosmart.models.Registro;

public class StartActivity extends AppCompatActivity {

    private Registro registro;
    private Button finalizar;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_start);

        finalizar = (Button) findViewById(R.id.start_end);

        finalizar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                terminarRegistro();
                Intent intent = new Intent(getApplicationContext(), MainActivity.class);
                intent.putExtra("Registro", "registro");
                startActivity(intent);
            }
        });

        crearRegistro();
    }


    public void crearRegistro(){
        registro = new Registro();
    }

    public void terminarRegistro(){
        registro.calcularDuracion();
        registro.setEstado(Estado.TERMINATED);
        registro.guardarRegistro(getApplicationContext());
    }
}
