package com.example.stoveapp.AccountActivity;

import android.content.Intent;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.Toast;

import com.example.stoveapp.R;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.FirebaseApp;

import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;


public class SignupActivity extends AppCompatActivity {

    private EditText inputEmail, inputPassword;     //hit option + enter if you on mac , for windows hit ctrl + enter
    private Button btnSignIn, btnSignUp, btnResetPassword;
    private ProgressBar progressBar;
    private FirebaseAuth auth;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_signup);
        //Get Firebase auth instance
        auth = FirebaseAuth.getInstance();

        btnSignIn = (Button) findViewById(R.id.sign_in_button);
        btnSignUp = (Button) findViewById(R.id.sign_up_button);
        inputEmail = (EditText) findViewById(R.id.email);
        inputPassword = (EditText) findViewById(R.id.password);
        progressBar = (ProgressBar) findViewById(R.id.progressBar);
        btnResetPassword = (Button) findViewById(R.id.btn_reset_password);

        btnResetPassword.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(new Intent(SignupActivity.this, ResetPasswordActivity.class));
            }
        });

        btnSignIn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });

        btnSignUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                String email = inputEmail.getText().toString().trim();
                String password = inputPassword.getText().toString().trim();

                FirebaseApp.initializeApp(getApplicationContext());



                if (TextUtils.isEmpty(email)) {
                    Toast.makeText(getApplicationContext(), "Enter email address!", Toast.LENGTH_SHORT).show();
                    return;
                }

                if (TextUtils.isEmpty(password)) {
                    Toast.makeText(getApplicationContext(), "Enter password!", Toast.LENGTH_SHORT).show();
                    return;
                }

                if (password.length() < 6) {
                    Toast.makeText(getApplicationContext(), "Password too short, enter minimum 6 characters!", Toast.LENGTH_SHORT).show();
                    return;
                }

                progressBar.setVisibility(View.VISIBLE);
                //create user
                auth.createUserWithEmailAndPassword(email, password)
                        .addOnCompleteListener(SignupActivity.this, new OnCompleteListener<AuthResult>() {
                            @Override
                            public void onComplete(@NonNull Task<AuthResult> task) {
                                Toast.makeText(SignupActivity.this, "createUserWithEmail:onComplete:" + task.isSuccessful(), Toast.LENGTH_SHORT).show();
                                progressBar.setVisibility(View.GONE);
                                // If sign in fails, display a message to the user. If sign in succeeds
                                // the auth state listener will be notified and logic to handle the
                                // signed in user can be handled in the listener.
                                if (!task.isSuccessful()) {
                                    Toast.makeText(SignupActivity.this, "Authentication failed." + task.getException(),
                                            Toast.LENGTH_SHORT).show();
                                } else {
                                    EditText mod=(EditText) findViewById(R.id.model);
                                    String actmod=mod.getText().toString();
                                    FirebaseDatabase database = FirebaseDatabase.getInstance();
                                    final FirebaseUser user2= FirebaseAuth.getInstance().getCurrentUser();
                                    final DatabaseReference myRef3=database.getReference();
                                    final DatabaseReference myRef4=database.getReference("Model_no").child(user2.getUid());



                                    myRef4.setValue(actmod);
                                    myRef3.child(actmod).child("config").child("num_burners").setValue(0);
                                    myRef3.child(actmod).child("model").setValue(actmod);
                                    myRef3.child(actmod).child("Food_profile").child("Rice").child("num_steps").setValue(1);
                                    myRef3.child(actmod).child("Food_profile").child("Rice").child("min_time").setValue(2);
                                    myRef3.child(actmod).child("Food_profile").child("Rice").child("steps").child("mode1").child("ON").setValue(0.3);
                                    myRef3.child(actmod).child("Food_profile").child("Rice").child("steps").child("mode2").child("ON-OFF").setValue(0.3);
                                    myRef3.child(actmod).child("Food_profile").child("Rice").child("steps").child("mode3").child("ON-SIM").setValue(0.6);
                                    myRef3.child(actmod).child("Food_profile").child("Rice").child("steps").child("mode4").child("SIM").setValue(0.3);
                                    myRef3.child(actmod).child("Food_profile").child("Tea").child("num_steps").setValue(1);
                                    myRef3.child(actmod).child("Food_profile").child("Tea").child("min_time").setValue(2);
                                    myRef3.child(actmod).child("Food_profile").child("Tea").child("steps").child("mode1").child("ON").setValue(0.5);
                                    myRef3.child(actmod).child("Food_profile").child("Tea").child("steps").child("mode2").child("ON-OFF").setValue(0.6);
                                    myRef3.child(actmod).child("Food_profile").child("Tea").child("steps").child("mode3").child("ON-SIM").setValue(0.3);
                                    myRef3.child(actmod).child("Food_profile").child("Tea").child("steps").child("mode4").child("SIM").setValue(0.3);
                                    myRef3.child(actmod).child("Food_profile").child("Maggi").child("num_steps").setValue(1);
                                    myRef3.child(actmod).child("Food_profile").child("Maggi").child("min_time").setValue(1);
                                    myRef3.child(actmod).child("Food_profile").child("Maggi").child("steps").child("mode1").child("ON").setValue(0.3);
                                    myRef3.child(actmod).child("Food_profile").child("Maggi").child("steps").child("mode2").child("ON-OFF").setValue(0.4);
                                    myRef3.child(actmod).child("Food_profile").child("Maggi").child("steps").child("mode3").child("ON-SIM").setValue(0.6);
                                    myRef3.child(actmod).child("Food_profile").child("Maggi").child("steps").child("mode4").child("SIM").setValue(0.3);
                                    Log.d("ma", "onCreate: ");


                                    Intent intent=new Intent(SignupActivity.this,selectburner.class);
                                    Intent intent2=new Intent(SignupActivity.this,settingspage.class);
                                    intent.putExtra("themodel",actmod);
                                    intent2.putExtra("themodel2",actmod);

                                    startActivity(intent);

                                }
                            }
                        });

            }
        });
    }

    @Override
    protected void onResume() {
        super.onResume();
        progressBar.setVisibility(View.GONE);
    }
}
