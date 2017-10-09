package com.example.dbz.smarthomecontroller;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button button_vilagitas = (Button) findViewById(R.id.button_vilagitas);
        button_vilagitas.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this, Vilagitas.class);
                startActivity(intent);
            }
        });

        Button button_biztonsag = (Button) findViewById(R.id.button_biztonsag);
        button_biztonsag.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this, Biztonsag.class);
                startActivity(intent);
            }
        });

        Button button_termosztat = (Button) findViewById(R.id.button_termosztat);
        button_termosztat.setOnClickListener((new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this, Termosztat.class);
                startActivity(intent);
            }
        }));
    }
}
