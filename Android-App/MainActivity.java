package com.example.stoveapp.AccountActivity;



import android.annotation.SuppressLint;
import android.content.Intent;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.example.stoveapp.R;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity {

    private Button btnChangePassword, btnRemoveUser, changePassword, remove, signOut,btnmonitor;
    private TextView email;
    static int nobu;

    private EditText oldEmail, password, newPassword;
    private ProgressBar progressBar;
    private FirebaseAuth auth;
    String actmod;





    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);




        Intent intent1=getIntent();
         nobu=intent1.getIntExtra("noburners",0);


//get firebase auth instance
        auth = FirebaseAuth.getInstance();
        email = (TextView) findViewById(R.id.useremail);

        //get current user
        final FirebaseUser user = FirebaseAuth.getInstance().getCurrentUser();
        setDataToView(user);

        authListener = new FirebaseAuth.AuthStateListener() {
            @Override
            public void onAuthStateChanged(@NonNull FirebaseAuth firebaseAuth) {
                FirebaseUser user = firebaseAuth.getCurrentUser();
                if (user == null) {
                    // user auth state is changed - user is null
                    // launch login activity
                    startActivity(new Intent(MainActivity.this, LoginActivity.class));
                    finish();
                }
            }
        };


        btnChangePassword = (Button) findViewById(R.id.change_password_button);

        btnRemoveUser = (Button) findViewById(R.id.remove_user_button);
        btnmonitor=(Button) findViewById(R.id.mgset);

        btnmonitor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent i1=new Intent(MainActivity.this,timeractivity.class);
                startActivity(i1);
            }
        });




        signOut = (Button) findViewById(R.id.sign_out);

        progressBar = (ProgressBar) findViewById(R.id.progressBar);

        if (progressBar != null) {
            progressBar.setVisibility(View.GONE);
        }












    }

    @SuppressLint("SetTextI18n")
    private void setDataToView(FirebaseUser user) {

        email.setText( user.getEmail());


    }

    // this listener will be called when there is change in firebase user session
    FirebaseAuth.AuthStateListener authListener = new FirebaseAuth.AuthStateListener() {
        @SuppressLint("SetTextI18n")
        @Override
        public void onAuthStateChanged(@NonNull FirebaseAuth firebaseAuth) {
            FirebaseUser user = firebaseAuth.getCurrentUser();
            if (user == null) {
                // user auth state is changed - user is null
                // launch login activity
                startActivity(new Intent(MainActivity.this, LoginActivity.class));
                finish();
            } else {
                setDataToView(user);

            }
        }


    };



    //sign out method
    public void signOut() {
        auth.signOut();


// this listener will be called when there is change in firebase user session
        FirebaseAuth.AuthStateListener authListener = new FirebaseAuth.AuthStateListener() {
            @Override
            public void onAuthStateChanged(@NonNull FirebaseAuth firebaseAuth) {
                FirebaseUser user = firebaseAuth.getCurrentUser();
                if (user == null) {
                    // user auth state is changed - user is null
                    // launch login activity
                    startActivity(new Intent(MainActivity.this, LoginActivity.class));
                    finish();
                }
            }
        };
    }

    @Override
    protected void onResume() {
        super.onResume();
        progressBar.setVisibility(View.GONE);
    }

    @Override
    public void onStart() {
        super.onStart();
        auth.addAuthStateListener(authListener);
    }

    @Override
    public void onStop() {
        super.onStop();
        if (authListener != null) {
            auth.removeAuthStateListener(authListener);
        }
    }

    public void controlfun (View v){

        final DatabaseReference ref = FirebaseDatabase.getInstance().getReference();
        final FirebaseUser user2 = FirebaseAuth.getInstance().getCurrentUser();
        final DatabaseReference mostafa = ref.child("Model_no").child(user2.getUid());
        // DatabaseReference mostafa1 = ref.child("User").child(actmod).child("config").child("num_burners");


        mostafa.addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {

                actmod = dataSnapshot.getValue(String.class);

                /*mostafa1.addListenerForSingleValueEvent(new ValueEventListener() {
                    @Override
                    public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                        nobu=dataSnapshot.getValue(Integer.class);
                        Intent intent=new Intent(MainActivity.this,cookpage.class);
                        intent.putExtra("themodel",actmod);
                        intent.putExtra("noburners",nobu);
                        startActivity(intent);
                    }

                    @Override
                    public void onCancelled(@NonNull DatabaseError databaseError) {

                    }
                });*/

                Intent intent=new Intent(MainActivity.this,cookpage.class);
                intent.putExtra("themodel",actmod);
               // intent.putExtra("noburners",nobu);
                startActivity(intent);







            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });




    }

    public void controlset (View v){
        Intent myIntent2 = new Intent(getBaseContext(),   settingspage.class);
        startActivity(myIntent2);



    }


    public void logout (View v){
        signOut();



    }
}
