package com.example.laberintosmart.activities;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.design.widget.BottomNavigationView;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v7.app.AppCompatActivity;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.example.laberintosmart.R;
import com.example.laberintosmart.fragments.InformationFragment;
import com.example.laberintosmart.fragments.InicioFragment;
import com.example.laberintosmart.fragments.RegisterFragment;

public class MainActivity extends AppCompatActivity implements BottomNavigationView.OnNavigationItemSelectedListener {

    private BottomNavigationView navView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        navView = findViewById(R.id.nav_view);
        navView.setOnNavigationItemSelectedListener(this);

        //Carga como default el fragmento Inicio
        switchFragment(new InicioFragment());
    }


    public boolean switchFragment(Fragment fragment ) {
        if (fragment != null) {
            FragmentManager manager = getSupportFragmentManager();
            manager.beginTransaction().replace(R.id.fragment_container, fragment).commit();
            return true;
        }
        return false;
    }


    @Override
    public boolean onNavigationItemSelected(@NonNull MenuItem menuItem) {
        Fragment fragment = null;

        switch (menuItem.getItemId()) {
            case R.id.navigation_home:
                fragment = new InicioFragment();
                break;
            case R.id.navigation_register:
                fragment = new RegisterFragment();
                break;
            case R.id.navigation_information:
                fragment = new InformationFragment();
                break;
        }
        return switchFragment(fragment);
    }
}
