package com.example.stoveapp.AccountActivity;

import android.content.Intent;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.example.stoveapp.R;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class FireDetectpage extends AppCompatActivity {

    String burnerinuse,actmod,food;

    FirebaseDatabase database = FirebaseDatabase.getInstance();
    DatabaseReference myRef5;
    int i=0;
    boolean flame=false;
    Intent f2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_fire_detectpage);

        assignvalue();
        myRef5.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                flame=dataSnapshot.child("flame_detect").getValue(boolean.class);
                if(flame==true&&i==0)
                {   i=i+1;
                    nextpage();
                }

            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });


    }

    public void assignvalue()
    {
        f2=getIntent();
        burnerinuse = f2.getStringExtra("burnerinuse");
        actmod = f2.getStringExtra("themodel");
        food=f2.getStringExtra("item");
        myRef5=database.getReference(actmod).child(burnerinuse);

    }
    public void nextpage()
    {
        Intent timeintent=new Intent(FireDetectpage.this,timeractivity.class);
        timeintent.putExtra("burnerinuse",burnerinuse);
        timeintent.putExtra("themodel",actmod);

        timeintent.putExtra("item",food);
        //myRef5.child("req_to_start_stop").setValue(1);
        startActivity(timeintent);

    }
}
