package com.example.laberintosmart.fragments;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.Toast;

import com.example.laberintosmart.R;

public class InicioFragment extends Fragment {

    private Button btnIniciar;
    private Button btnFinish;
    private Button btnTest;

    public InicioFragment() {
        // Required empty public constructor
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_home, container, false);

        return view;
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {

        //Inicio de Variables
        btnIniciar = getView().findViewById(R.id.button_start);
        btnFinish = getView().findViewById(R.id.button_finish);
        btnTest = getView().findViewById(R.id.button_test);

        //Fijacion de Listeners
        btnIniciar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Toast.makeText(InicioFragment.this.getContext(), "boton START clickeado", Toast.LENGTH_LONG).show();
            }
        });

        btnFinish.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Toast.makeText(InicioFragment.this.getContext(), "boton TERMINAR clickeado", Toast.LENGTH_LONG).show();
            }
        });

        btnTest.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Toast.makeText(InicioFragment.this.getContext(), "boton TEST clickeado", Toast.LENGTH_LONG).show();
            }
        });
    }
}
