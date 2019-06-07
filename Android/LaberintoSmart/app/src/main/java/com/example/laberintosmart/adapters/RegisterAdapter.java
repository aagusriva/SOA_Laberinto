package com.example.laberintosmart.adapters;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.example.laberintosmart.R;
import com.example.laberintosmart.models.Registro;

import java.util.List;

public class RegisterAdapter extends BaseAdapter {
    private Context context;
    private int layout;
    private List<Registro> registros;
    private TextView textDate, textDuration, textState;

    public RegisterAdapter(Context context, int layout, List<Registro> registros) {
        this.context = context;
        this.layout = layout;
        this.registros = registros;
    }

    @Override
    public int getCount() {
        return registros.size();
    }

    @Override
    public Object getItem(int position) {
        return registros.get(position);
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        View view = convertView;

        LayoutInflater layoutInflater = LayoutInflater.from(this.context);
        view = layoutInflater.inflate(R.layout.list_register,parent,false);

        textDate = view.findViewById(R.id.text_date);
        textDuration = view.findViewById(R.id.text_duration);
        textState = view.findViewById(R.id.text_state);

        textDate.setText(registros.get(position).getDateString());
        textDuration.setText(Long.toString(registros.get(position).getDuration()));
        textState.setText(registros.get(position).getEstado().toString());

        return view;
    }
}
