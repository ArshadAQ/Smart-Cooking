package com.example.stoveapp.AccountActivity;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;

import com.example.stoveapp.R;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

import java.util.ArrayList;


public class cookextra extends AppCompatActivity {
    ArrayList<Integer> time = new ArrayList<Integer>();
    Button ontimeok;
    String actmod,burnerinuse,food;
    int selecttime;
    FirebaseDatabase database = FirebaseDatabase.getInstance();
    DatabaseReference myRef1,mostafa,myRef2,mTimerRef4;
    Intent fl;

    @Override

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_cookextra);

        assignvalue();

        ontimeok=(Button) findViewById(R.id.oknext);
        myRef1=database.getReference(actmod).child(burnerinuse).child("extra_time");
       // myRef2=database.getReference(actmod).child(burnerinuse).child("req_to_start_stop");
        mTimerRef4= FirebaseDatabase.getInstance().getReference(actmod).child(burnerinuse).child("continue");
        final Spinner spinner4=findViewById(R.id.extrausertime1);
        for (int k = 0; k <= 60; k++) {
            time.add(k);
        }
        ArrayAdapter<Integer> adapter4= new ArrayAdapter<Integer>(this, android.R.layout.simple_spinner_dropdown_item, time);
        spinner4.setAdapter(adapter4);

        spinner4.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                selecttime=(Integer)adapterView.getSelectedItem();

            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });

        ontimeok.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                myRef1.setValue(selecttime);
               // myRef2.setValue(1);
                mTimerRef4.setValue(1);
                Intent f1=new Intent(cookextra.this,timeractivity.class);
                f1.putExtra("themodel",actmod);
                f1.putExtra("burnerinuse",burnerinuse);
                f1.putExtra("item",food);
                startActivity(f1);

            }
        });



    }

    public void assignvalue()
    {
        fl=getIntent();
       actmod=fl.getStringExtra("themodel");
        burnerinuse=fl.getStringExtra("burnerinuse");
        food=fl.getStringExtra("item");
    }
}
