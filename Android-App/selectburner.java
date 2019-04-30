package com.example.stoveapp.AccountActivity;

import android.app.ActionBar;
import android.content.Intent;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Adapter;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;

import com.example.stoveapp.R;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.FirebaseApp;

import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class selectburner extends AppCompatActivity {
    DatabaseReference myRef3;
    String d,actmod;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_selectburner);

        final DatabaseReference ref = FirebaseDatabase.getInstance().getReference();
        final FirebaseUser user2 = FirebaseAuth.getInstance().getCurrentUser();
        DatabaseReference mostafa = ref.child("Model_no").child(user2.getUid());
        mostafa.addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                actmod = dataSnapshot.getValue(String.class);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });








        FirebaseDatabase database = FirebaseDatabase.getInstance();
        myRef3=database.getReference();

        final Spinner spinner=findViewById(R.id.num);
        String[] items=new String[]{" ","1","2","3","4","5"};
        ArrayAdapter<String> adapter=new ArrayAdapter<>(this,android.R.layout.simple_spinner_dropdown_item
                ,items);
        spinner.setAdapter(adapter);
        final TextView edit1=findViewById(R.id.burner1);
        final TextView edit2=findViewById(R.id.burner2);
        final TextView edit3=findViewById(R.id.burner3);
        final TextView edit4=findViewById(R.id.burner4);
        final TextView edit5=findViewById(R.id.burner5);

        final  Spinner spinner2=findViewById(R.id.burner1spin);
        final   Spinner spinner3=findViewById(R.id.burner2spin);
        final  Spinner spinner4=findViewById(R.id.burner3spin);
        final   Spinner spinner5=findViewById(R.id.burner4spin);
        final  Spinner spinner6=findViewById(R.id.burner5spin);
        Button btn=findViewById(R.id.ok);


        String[] sizes=new String[]{"Small","Medium","Large"};
        ArrayAdapter<String> adapter2=new ArrayAdapter<>(this,android.R.layout.simple_spinner_dropdown_item
                ,sizes);
        spinner2.setAdapter(adapter2);
        spinner3.setAdapter(adapter2);
        spinner4.setAdapter(adapter2);
        spinner5.setAdapter(adapter2);
        spinner6.setAdapter(adapter2);


        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                 d= (String) adapterView.getSelectedItem();
                Log.d("selburner",d);
                switch (d)
                {
                    case "1":
                    {
                        edit1.setVisibility(View.VISIBLE);
                        spinner2.setVisibility(View.VISIBLE);
                        edit2.setVisibility(View.GONE);
                        spinner3.setVisibility(View.GONE);
                        edit3.setVisibility(View.GONE);
                        spinner4.setVisibility(View.GONE);
                        edit4.setVisibility(View.GONE);
                        spinner5.setVisibility(View.GONE);
                        edit5.setVisibility(View.GONE);
                        spinner6.setVisibility(View.GONE);


                        break;
                    }
                    case "2":

                    {   edit1.setVisibility(View.VISIBLE);
                        spinner2.setVisibility(View.VISIBLE);
                        edit2.setVisibility(View.VISIBLE);
                        spinner3.setVisibility(View.VISIBLE);
                        edit3.setVisibility(View.GONE);
                        spinner4.setVisibility(View.GONE);
                        edit4.setVisibility(View.GONE);
                        spinner5.setVisibility(View.GONE);
                        edit5.setVisibility(View.GONE);
                        spinner6.setVisibility(View.GONE);

                        break;
                    }

                    case "3":
                    {
                        edit1.setVisibility(View.VISIBLE);
                        spinner2.setVisibility(View.VISIBLE);
                        edit2.setVisibility(View.VISIBLE);
                        spinner3.setVisibility(View.VISIBLE);
                        edit3.setVisibility(View.VISIBLE);
                        spinner4.setVisibility(View.VISIBLE);
                        edit4.setVisibility(View.GONE);
                        spinner5.setVisibility(View.GONE);
                        edit5.setVisibility(View.GONE);
                        spinner6.setVisibility(View.GONE);


                        break;

                    }
                    case "4":
                    {
                        edit1.setVisibility(View.VISIBLE);
                        spinner2.setVisibility(View.VISIBLE);
                        edit2.setVisibility(View.VISIBLE);
                        spinner3.setVisibility(View.VISIBLE);
                        edit3.setVisibility(View.VISIBLE);
                        spinner4.setVisibility(View.VISIBLE);
                        edit4.setVisibility(View.VISIBLE);
                        spinner5.setVisibility(View.VISIBLE);
                        edit5.setVisibility(View.GONE);
                        spinner6.setVisibility(View.GONE);
                        break;


                    }

                    case "5":
                    {
                        edit1.setVisibility(View.VISIBLE);
                        spinner2.setVisibility(View.VISIBLE);
                        edit2.setVisibility(View.VISIBLE);
                        spinner3.setVisibility(View.VISIBLE);
                        edit3.setVisibility(View.VISIBLE);
                        spinner4.setVisibility(View.VISIBLE);
                        edit4.setVisibility(View.VISIBLE);
                        spinner5.setVisibility(View.VISIBLE);
                        edit5.setVisibility(View.VISIBLE);
                        spinner6.setVisibility(View.VISIBLE);
                        break;



                    }

                    default:
                    {


                    }
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });

        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                int i=Integer.parseInt(d);
                myRef3.child(actmod).child("config").child("num_burners").setValue(i);
                Log.d("nb",d);
                for(int j=1;j<=i;j++)
                {
                    String q1=(String)spinner2.getSelectedItem();
                    String q2=(String)spinner3.getSelectedItem();
                    String q3=(String)spinner4.getSelectedItem();
                    String q4=(String)spinner5.getSelectedItem();
                    String q5=(String)spinner6.getSelectedItem();
                    Log.d("the",q1);

                    String v="Burner"+j;
                    if(j==1)
                        myRef3.child(actmod).child("config").child("size").child(v).setValue(q1);
                    if(j==2)
                        myRef3.child(actmod).child("config").child("size").child(v).setValue(q2);
                    if(j==3)
                        myRef3.child(actmod).child("config").child("size").child(v).setValue(q3);
                    if(j==4)
                        myRef3.child(actmod).child("config").child("size").child(v).setValue(q4);
                    if(j==5)
                        myRef3.child(actmod).child("config").child("size").child(v).setValue(q5);

                    myRef3.child(actmod).child(v).child("Food_name").setValue("None");
                    myRef3.child(actmod).child(v).child("vessel_detect").setValue(0);
                    myRef3.child(actmod).child(v).child("req_to_off_knob").setValue(false);
                    myRef3.child(actmod).child(v).child("knob_status").setValue("off");
                    myRef3.child(actmod).child(v).child("cooking_type").setValue("None");
                    myRef3.child(actmod).child(v).child("retrain").setValue("None");
                    myRef3.child(actmod).child(v).child("time_selected").setValue(0.0);
                    myRef3.child(actmod).child(v).child("extra_time").setValue(0);
                    myRef3.child(actmod).child(v).child("req_to_start_stop").setValue(0);
                    myRef3.child(actmod).child(v).child("flame_detect").setValue(false);
                    myRef3.child(actmod).child(v).child("continue").setValue(0);



                    Intent intent=new Intent(selectburner.this,MainActivity.class);
                    intent.putExtra("noburners",i);
                    intent.putExtra("themodel",actmod);
                    startActivity(intent);

                }


            }
        });

    }
}
