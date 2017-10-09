package com.example.dbz.smarthomecontroller;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class Vilagitas extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_vilagitas);

        Button btn_belteri_be = (Button) findViewById(R.id.btn_belteri_be);
        btn_belteri_be.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //btn_belteri_be
            }
        });

        Button btn_belteri_ki = (Button) findViewById(R.id.btn_belteri_ki);
        btn_belteri_be.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //btn_belteri_ki
            }
        });

        Button btn_kulteri_be = (Button) findViewById(R.id.btn_kulteri_be);
        btn_belteri_be.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //btn_kulteri_be
            }
        });

        Button btn_kulteri_ki = (Button) findViewById(R.id.btn_kulteri_ki);
        btn_belteri_be.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //btn_kulteri_ki
            }
        });
    }
}
