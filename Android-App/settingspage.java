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
import android.widget.Spinner;
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

public class settingspage extends AppCompatActivity {

    private Button btnChangePassword, btnRemoveUser,
            changePassword, remove, signOut;


    private EditText oldEmail, password, newPassword;
    private ProgressBar progressBar;
    private FirebaseAuth auth;
    String actmod2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settingspage);

        auth = FirebaseAuth.getInstance();


        //get current user
        final FirebaseUser user = FirebaseAuth.getInstance().getCurrentUser();
        FirebaseDatabase database = FirebaseDatabase.getInstance();
        final DatabaseReference myRef3=database.getReference();


        final DatabaseReference ref = FirebaseDatabase.getInstance().getReference();
        final FirebaseUser user2 = FirebaseAuth.getInstance().getCurrentUser();
        final DatabaseReference myRef4=database.getReference("Model_no").child(user2.getUid());

        final DatabaseReference mostafa = ref.child("Model_no").child(user2.getUid());




        FirebaseAuth.AuthStateListener authListener = new FirebaseAuth.AuthStateListener() {
            @Override
            public void onAuthStateChanged(@NonNull FirebaseAuth firebaseAuth) {
                FirebaseUser user = firebaseAuth.getCurrentUser();
                if (user == null) {
                    // user auth state is changed - user is null
                    // launch login activity
                    startActivity(new Intent(settingspage.this, LoginActivity.class));
                    finish();
                }
            }
        };


        btnChangePassword = (Button) findViewById(R.id.change_password_button);

        btnRemoveUser = (Button) findViewById(R.id.remove_user_button);
        Button btnburner=(Button) findViewById(R.id.burner);


        btnChangePassword.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent passintent=new Intent(settingspage.this,changepass.class);
                startActivity(passintent);
            }
        });


        btnRemoveUser.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {




                if (user != null) {
                    mostafa.addListenerForSingleValueEvent(new ValueEventListener() {
                        @Override
                        public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                            actmod2 = dataSnapshot.getValue(String.class);
                        }

                        @Override
                        public void onCancelled(@NonNull DatabaseError databaseError) {

                        }
                    });

                    myRef4.child(actmod2).removeValue();
                    myRef4.removeValue();



                    user.delete()
                            .addOnCompleteListener(new OnCompleteListener<Void>() {
                                @Override
                                public void onComplete(@NonNull Task<Void> task) {
                                    if (task.isSuccessful()) {


                                        Toast.makeText(settingspage.this, "Your profile is deleted:( Create a account now!", Toast.LENGTH_SHORT).show();
                                        startActivity(new Intent(settingspage.this, SignupActivity.class));
                                        finish();

                                    } else {
                                        Toast.makeText(settingspage.this, "Failed to delete your account!", Toast.LENGTH_SHORT).show();

                                    }
                                }
                            });
                }
            }
        });

        btnburner.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent passintent1=new Intent(settingspage.this,selectburner.class);
                startActivity(passintent1);
            }
        });



    }


}
