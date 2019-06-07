package com.example.laberintosmart.fragments;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;
import android.widget.TextView;

import com.example.laberintosmart.R;
import com.example.laberintosmart.adapters.RegisterAdapter;
import com.example.laberintosmart.models.Registro;

import java.util.List;

public class RegisterFragment extends Fragment {

    ListView registrosVista;
    List<Registro> registros;
    RegisterAdapter registerAdapter;
    TextView noRegisters;

    public RegisterFragment() {
        // Required empty public constructor
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_register, container, false);

        noRegisters = view.findViewById(R.id.text_msg);

        //Código
        registrosVista = view.findViewById(R.id.listaDeRegistros);
        registros = Registro.getRecords(RegisterFragment.this.getContext());
        registerAdapter = new RegisterAdapter(getContext(), R.layout.list_register,registros);
        if(registros.size() == 0)
            noRegisters.setVisibility(View.VISIBLE);
        else
            noRegisters.setVisibility(View.INVISIBLE);

        registrosVista.setAdapter(registerAdapter);
        return view;
    }
}
