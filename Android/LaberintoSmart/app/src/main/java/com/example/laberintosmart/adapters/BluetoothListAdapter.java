package com.example.laberintosmart.adapters;

import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.example.laberintosmart.R;
import com.example.laberintosmart.models.Registro;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

public class BluetoothListAdapter extends BaseAdapter {

    public static final String btCarName = "VW_RADIO_94";

    private Context context;
    private int layout;
    private List<BluetoothDevice> btDevices;

    public BluetoothListAdapter(Context context, int layout, List<BluetoothDevice> btDevices) {
        this.context = context;
        this.layout = layout;
        this.btDevices = btDevices;
        for (int i=0; i<btDevices.size(); i++)
            if(btDevices.get(i).getName().equals(BluetoothListAdapter.btCarName) && i!=0) {
                btDevices.set(0, btDevices.get(i));
                btDevices.remove(i);
                i=90; //re villero
            }

        //this.btDevices = new ArrayList<BluetoothDevice>(btDevices);
    }

    public void add(BluetoothDevice btDevice){
        btDevices.add(btDevice);
    }

    @Override
    public int getCount() {
        return btDevices.size();
    }

    @Override
    public BluetoothDevice getItem(int position) {
        return btDevices.get(position);
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        View view = convertView;

        LayoutInflater layoutInflater = LayoutInflater.from(this.context);
        view = layoutInflater.inflate(R.layout.list_bluetooth,parent,false);

        TextView deviceName= view.findViewById(R.id.text_bt_name);

        deviceName.setText(btDevices.get(position).getName());
        if(btDevices.get(position).getName().equals(BluetoothListAdapter.btCarName))
            deviceName.setTextColor(Color.MAGENTA);
        return view;
    }


}
