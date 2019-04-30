package com.example.stoveapp.AccountActivity;


import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.example.stoveapp.R;
import com.google.firebase.FirebaseApp;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

public class control extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_control);
        FirebaseApp.initializeApp(getApplicationContext());
        FirebaseDatabase database = FirebaseDatabase.getInstance();
        final FirebaseUser user2= FirebaseAuth.getInstance().getCurrentUser();
        final DatabaseReference myRef3=database.getReference("User");



        final DatabaseReference myRef = database.getReference().child("User").child(user2.getUid()).child("item");
        final DatabaseReference myRef1 = database.getReference("Time");
        final DatabaseReference myRef2 = database.getReference("stove_user");


        Button Rice = findViewById(R.id.rice);
        Button pot = findViewById(R.id.pot);

        Button Tea = findViewById(R.id.mag);
        final int[] item_no = {0};
        final int[] time = {0};
        Button time1 = findViewById(R.id.t1);
        Button time2 = findViewById(R.id.t2);
        Button time3 = findViewById(R.id.t3);
        Button ok_btn = findViewById(R.id.ok_btn);

        Rice.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                item_no[0] = 1;
            }
        });

        pot.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                item_no[0] = 2;
            }

        });

        Tea.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                item_no[0] = 3;
            }
        });

        time1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                time[0] = 5;
            }
        });

        time2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                time[0] = 10;
            }
        });

        time3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                time[0] = 15;
            }
        });


        ok_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                myRef.setValue(item_no[0]);
                myRef1.setValue(time[0]);
                myRef2.setValue( user2.getEmail());
                Intent i =new Intent(getApplicationContext(),StatusActivity.class);
                startActivity(i);
                finish();

            }
        });




    }


}

