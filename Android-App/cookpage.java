package com.example.stoveapp.AccountActivity;

import android.content.Intent;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Adapter;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.example.stoveapp.R;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.text.DecimalFormat;
import java.util.ArrayList;

public class cookpage extends AppCompatActivity {

    String a, d,z,mintimestr,b1;
    Float b, c, e, x, f;
//    long f;
    int  numberbur,mintime,minimum,selecttime,flag,vesseldetect;

    String actmod,burner,item,Food,knobstatus;

    String operation1;

    ArrayList<String> stringArrayList = new ArrayList<String>();


    ArrayList<Integer> time = new ArrayList<Integer>();

    FirebaseDatabase database = FirebaseDatabase.getInstance();

    DatabaseReference myRef3, ref2;
    DatabaseReference myRef5;
    DatabaseReference myRef4;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_cookpage);

        getvalues();



        final DatabaseReference ref1 = FirebaseDatabase.getInstance().getReference();
        final FirebaseUser user2 = FirebaseAuth.getInstance().getCurrentUser();

        final DatabaseReference mostafa = ref1.child("Model_no").child(user2.getUid());




       /* mostafa.addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {

                actmod = dataSnapshot.getValue(String.class);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });*/







        final TextView expectedtime = findViewById(R.id.exptime);
        Button btnonclick=(Button) findViewById(R.id.ok_btn);


        final DatabaseReference ref = FirebaseDatabase.getInstance().getReference();

       //getnumberofburners;


        final Spinner spinner = findViewById(R.id.item);
        final Spinner spinner2 = findViewById(R.id.oper);
        final Spinner spinner3 = findViewById(R.id.burselect);

        final Spinner spinner4=findViewById(R.id.extrausertime);


        String[] items = new String[]{" ", "Rice", "Tea", "Maggi"};
        final ArrayAdapter<String> adapter = new ArrayAdapter<>(this, android.R.layout.simple_spinner_dropdown_item
                , items);
        spinner.setAdapter(adapter);

        final String[] operation = new String[]{"Cook", "Reheat"};
        ArrayAdapter<String> adapter2 = new ArrayAdapter<>(this, android.R.layout.simple_spinner_dropdown_item
                , operation);
        spinner2.setAdapter(adapter2);


        for (int i = 1; i <= 5; i++) {
            b1 = String.valueOf(i);
            stringArrayList.add(b1);
        }



        ArrayAdapter<String> adapter3 = new ArrayAdapter<>(this, android.R.layout.simple_spinner_dropdown_item
                , stringArrayList);
        spinner3.setAdapter(adapter3);



        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                d = (String) adapterView.getSelectedItem();
                switch (d) {
                    case " ": {
                        Log.d("y", "nothing");
                        break;
                    }
                    case "Rice": {
                        myRef4.addListenerForSingleValueEvent(new ValueEventListener() {
                            @Override
                            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                                f = (float)0;
                                DataSnapshot ds = dataSnapshot.child("Rice").child("steps");
                                for(DataSnapshot snapshot: ds.getChildren()){
                                    //                    temp = snapshot.getRef();
                                    //                    temp1 = temp.orderByKey().limitToFirst(1).getRef();
                                    //                     = snapshot.getChildren().getClass();
                                    for(DataSnapshot snap : snapshot.getChildren()){
                                        f += snap.getValue(Float.class);
                                    }
                                }
//                                b = dataSnapshot.child("Rice").child("steps").child("mode1").child("ON").getValue(Float.class);
//                                c = dataSnapshot.child("Rice").child("steps").child("mode2").child("ON-OFF").getValue(Float.class);
//                                e = dataSnapshot.child("Rice").child("steps").child("mode3").child("ON-SIM").getValue(Float.class);
//                                x=dataSnapshot.child("Rice").child("steps").child("mode4").child("SIM").getValue(Float.class);
//                                mintime=dataSnapshot.child("Rice").child("min_time").getValue(Integer.class);
                                Food="Rice";
//                                f = b + c + e+x;
                                expectedtime.setText(f.toString());





                            }

                            @Override
                            public void onCancelled(@NonNull DatabaseError databaseError) {

                            }
                        });

                        break;
                    }

                    case "Tea": {
                        myRef4.addListenerForSingleValueEvent(new ValueEventListener() {
                            @Override
                            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                                f = (float)0;
                                DataSnapshot ds = dataSnapshot.child("Tea").child("steps");
//                                b = dataSnapshot.child("Tea").child("steps").child("mode1").child("ON").getValue(Float.class);
//                                c = dataSnapshot.child("Tea").child("steps").child("mode2").child("ON-OFF").getValue(Float.class);
//                                e = dataSnapshot.child("Tea").child("steps").child("mode3").child("ON-SIM").getValue(Float.class);
//                                x=dataSnapshot.child("Tea").child("steps").child("mode4").child("SIM").getValue(Float.class);
//                                f = b + c + e+x;
//                                f = 0;
                                for(DataSnapshot snapshot: ds.getChildren()){
                                    //                    temp = snapshot.getRef();
                                    //                    temp1 = temp.orderByKey().limitToFirst(1).getRef();
                                    //                     = snapshot.getChildren().getClass();
                                    for(DataSnapshot snap : snapshot.getChildren()){
                                        f += snap.getValue(Float.class);
                                    }
                                }
                                Food = "Tea";

                                expectedtime.setText(f.toString());


                            }

                            @Override
                            public void onCancelled(@NonNull DatabaseError databaseError) {

                            }
                        });

                        break;
                    }

                    case "Maggi": {
                        myRef4.addListenerForSingleValueEvent(new ValueEventListener() {
                            @Override
                            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                                f = (float)0;
                                DataSnapshot ds = dataSnapshot.child("Maggi").child("steps");
                                for(DataSnapshot snapshot: ds.getChildren()){
                                    //                    temp = snapshot.getRef();
                                    //                    temp1 = temp.orderByKey().limitToFirst(1).getRef();
                                    //                     = snapshot.getChildren().getClass();
                                    for(DataSnapshot snap : snapshot.getChildren()){
                                        f += snap.getValue(Float.class);
                                    }
                                }

//                                b = dataSnapshot.child("Maggi").child("steps").child("mode1").child("ON").getValue(Float.class);
//                                c = dataSnapshot.child("Maggi").child("steps").child("mode2").child("ON-OFF").getValue(Float.class);
//                                e = dataSnapshot.child("Maggi").child("steps").child("mode3").child("ON-SIM").getValue(Float.class);
//                                x=dataSnapshot.child("Maggi").child("steps").child("mode4").child("SIM").getValue(Float.class);

//                                f = b + c + e+x;
                                Food = "Maggi";
                                expectedtime.setText(f.toString());



                            }

                            @Override
                            public void onCancelled(@NonNull DatabaseError databaseError) {

                            }
                        });

                        break;
                    }


                }







            }


            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {
            }
        });
        for (int k = 0; k <= 60; k++) {
            time.add(k);
        }
        ArrayAdapter<Integer> adapter4= new ArrayAdapter<Integer>(this, android.R.layout.simple_spinner_dropdown_item, time);
        spinner4.setAdapter(adapter4);

        spinner2.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                 String operation=(String)adapterView.getSelectedItem();
                 operation1=operation;
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });

        spinner3.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                burner="Burner"+adapterView.getSelectedItem();
                getvalbur();
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });


        spinner4.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                selecttime=(Integer)adapterView.getSelectedItem();
                if(selecttime!=0)
                {
                    flag=1;
                }
                else if(selecttime==0)
                {
                    flag=0;
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });








        btnonclick.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                myRef5.addValueEventListener(new ValueEventListener() {
                    @Override
                    public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                        vesseldetect=dataSnapshot.child(burner).child("vessel_detect").getValue(Integer.class);
                         knobstatus=dataSnapshot.child(burner).child("knob_status").getValue(String.class);

                         {

                         }

                    }

                    @Override
                    public void onCancelled(@NonNull DatabaseError databaseError) {

                    }
                });






                            if(vesseldetect==0)
                        {
                          Toast.makeText(getApplicationContext(),"Place the vessel on the selected burner",Toast.LENGTH_SHORT).show();
                         }

                         else if(vesseldetect==1)
                        {
                            if(knobstatus.equalsIgnoreCase("off"))
                               {
                                 myRef5.child(burner).child("Food_name").setValue(d);

                                   if (flag == 1)
                                   {
                                       if (selecttime >= mintime) {
                                           myRef3.child("time_selected").setValue(selecttime);
                                           myRef3.child("cooking_type").setValue(operation1);
                                           myRef3.child("req_to_start_stop").setValue(1);

                                           Intent timeintent = new Intent(cookpage.this, FireDetectpage.class);
                                           timeintent.putExtra("burnerinuse", burner);
                                           timeintent.putExtra("time", Float.valueOf(selecttime));
                                           timeintent.putExtra("themodel", actmod);
                                           timeintent.putExtra("item", Food);


                                           startActivity(timeintent);
                                       } else {
                                           Toast.makeText(getApplicationContext(), "The minimum time is " + mintime + " minutes,please select higher value", Toast.LENGTH_SHORT).show();
                                       }

                                   }
                                   else if (flag == 0)

                                   {
                                       myRef3.child("time_selected").setValue(0);
                                       myRef3.child("cooking_type").setValue(operation1);
                                       myRef3.child("req_to_start_stop").setValue(1);

                                       Intent timeintent = new Intent(cookpage.this, FireDetectpage.class);
                                       timeintent.putExtra("burnerinuse", burner);
                                       timeintent.putExtra("themodel", actmod);

                                       timeintent.putExtra("item", Food);

                                       startActivity(timeintent);
                            }



                               }

                            else
                            {
                                Toast.makeText(getApplicationContext(),"Please switch off your knob",Toast.LENGTH_SHORT).show();
                            }



                        }


                        }
        });



    }

   /* public void getnumberofburners(){
         DatabaseReference myRef6=database.getReference("User").child(actmod).child("config").child("num_burners");
         myRef6.addListenerForSingleValueEvent(new ValueEventListener() {
             @Override
             public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                 numberbur=dataSnapshot.getValue(Integer.class);
                 for (int i = 1; i <= numberbur; i++) {
                     b1= String.valueOf(i);
                     stringArrayList.add(b1);
                 }

             }

             @Override
             public void onCancelled(@NonNull DatabaseError databaseError) {

             }
         });

    }*/

   public void getvalues(){
       Intent intent1=getIntent();
       actmod = intent1.getStringExtra("themodel");
      // numberbur=intent1.getIntExtra("noburners",1);

       myRef5=database.getReference(actmod);
        myRef4 = database.getReference(actmod).child("Food_profile");





   }

   public void getvalbur()
   {

       myRef3=database.getReference(actmod).child(burner);
   }

}
